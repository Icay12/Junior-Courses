#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//////////////////////////利用windows.h中定义的bmp文件结构
#include <windows.h>

using namespace std;
#define BITMAP_ID 0x4D42

unsigned char* bitmapImage;
unsigned char* bitmapData_gray;
double* bitmapData_yuv;
BITMAPINFOHEADER bitmapInfoHeader;
BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
int bmpWidth, bmpHeight;
int lineByte;




int LoadBitmapFile(char *filename)
{
	FILE *filePtr;	// 文件指针
	int	imageIdx = 0;		// 图像位置索引

	// 以“二进制+读”模式打开文件filename 
	filePtr = fopen(filename, "rb"); 
	if (filePtr == NULL) return NULL;
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr); 
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// 读入bitmap信息头
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr); 
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	bmpWidth = bitmapInfoHeader.biWidth;
	bmpHeight = bitmapInfoHeader.biHeight;
	//补全为4的倍数
	lineByte=(bmpWidth*3 + 3)/4*4;

	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[lineByte * bmpHeight];

	// 验证内存是否创建成功
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, lineByte * bmpHeight, filePtr); 

	// 确认读入成功
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 关闭bitmap图像文件
	fclose(filePtr); 
}

int WriteBMP(unsigned char* image,char *newfilename)
{
	FILE *fpr;
	FILE *fpw;
	int lineByte = (bmpWidth*3 + 3)/4*4;

	if(!image)
	{
		return 0;
	}

	if((fpw = fopen(newfilename,"wb")) == NULL)
	{
		return 0;
	}

	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + lineByte*bmpHeight;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	fileHead.bfOffBits = 54;

	BITMAPINFOHEADER fileHeadInfo; 
	fileHeadInfo.biBitCount=24;
	fileHeadInfo.biClrImportant=0;
	fileHeadInfo.biClrUsed=0;
	fileHeadInfo.biCompression=0;
	fileHeadInfo.biHeight=bmpHeight;
	fileHeadInfo.biPlanes=1;
	fileHeadInfo.biSize=40;
	fileHeadInfo.biSizeImage=0;
	fileHeadInfo.biWidth=bmpWidth;
	fileHeadInfo.biXPelsPerMeter=0;
	fileHeadInfo.biYPelsPerMeter=0;

	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&fileHeadInfo, sizeof(BITMAPINFOHEADER),1,fpw);
	fwrite(image, bmpHeight*lineByte, 1, fpw);

	fclose(fpw);
}

double *RGBtoYUV(const unsigned char* bmpRGB, LONG width, LONG height)
{
	int lineByte = (width*3 + 3)/4*4;
	double *bmpYUV = new double[lineByte * height];
	int i, j;

	for(i = 0; i < width; ++i)
	{
		for(j = 0; j < height; ++j)
		{
			BYTE B = *(bmpRGB+i*lineByte+j*3+0);
			BYTE G = *(bmpRGB+i*lineByte+j*3+1);
			BYTE R = *(bmpRGB+i*lineByte+j*3+2);

			*(bmpYUV+i*lineByte+j*3+0) = 0.299*R+0.587*G+0.114*B;		//y
			*(bmpYUV+i*lineByte+j*3+1) = -0.147*R-0.289*G+0.435*B;	//u
			*(bmpYUV+i*lineByte+j*3+2) = 0.615*R-0.515*G-0.100*B;		//v
		}	
	}
	return bmpYUV;
}

unsigned char *ColorToGray(double* bmpYUV, LONG width, LONG height)
{
	int lineByte = (width*3 + 3)/4*4;
	unsigned char *bmpRGB_gray = new unsigned char[lineByte * height];
	int i, j;

	for(i = 0; i < width; ++i)
	{
		for(j = 0; j < height; ++j)
		{
			*(bmpRGB_gray+i*lineByte+j*3+0) = *(bmpYUV+i*lineByte+j*3+0);
			*(bmpRGB_gray+i*lineByte+j*3+1) = *(bmpYUV+i*lineByte+j*3+0);
			*(bmpRGB_gray+i*lineByte+j*3+2) = *(bmpYUV+i*lineByte+j*3+0);
		}
	}
	return bmpRGB_gray;
}

unsigned char *ChangeLuminance(double* bmpYUV, int IBright,LONG width, LONG height)
{
	int lineByte = (bmpWidth*3 + 3)/4*4;
	unsigned char *bmpRGB_new = new unsigned char[lineByte * height];
	int i, j;

	for(i = 0; i < width; ++i)
	{
		for(j = 0; j < height; ++j)
		{
			double Y = *(bmpYUV+i*lineByte+j*3+0) + IBright;
			double U = *(bmpYUV+i*lineByte+j*3+1);
			double V = *(bmpYUV+i*lineByte+j*3+2);
			int R,G,B;

			R = Y + 1.140*V;
			G = Y - 0.394*U - 0.581*V;
			B = Y + 2.032*U;

			R = R > 255 ? 255:R;
			R = R < 0 ? 0:R;

			G = G > 255 ? 255:G;
			G = G < 0 ? 0:G;

			B = B > 255 ? 255:B;
			B = B < 0 ? 0:B;
	
			*(bmpRGB_new+i*lineByte+j*3+2) = R;		//r
			*(bmpRGB_new+i*lineByte+j*3+1) = G;	//g
			*(bmpRGB_new+i*lineByte+j*3+0) = B;		//b
		}	
	}
	return bmpRGB_new;	
}

void reArrangeGray(unsigned char*bmpRGB_gray,LONG width,LONG height)
{
	int lineByte = (width*3 + 3)/4*4;
	int i, j;
	int maxG=-1,minG=1e9;

	for(i = 0; i < width; ++i)
	{
		for(j = 0; j < height; ++j)
		{
			int t = *(bmpRGB_gray+i*lineByte+j*3+0);
			maxG=(t>maxG)?t:maxG;  //最大的灰度值
			minG=(t<minG)?t:minG;  //最小的灰度值
		}
	}
	for(i = 0; i < width; ++i)
	{
		for(j = 0; j < height; ++j)
		{
			int t = *(bmpRGB_gray+i*lineByte+j*3+0);
			t = 255*(t-minG)/(maxG-minG)+0;
			*(bmpRGB_gray+i*lineByte+j*3+0) = t;
			*(bmpRGB_gray+i*lineByte+j*3+1) = t;
			*(bmpRGB_gray+i*lineByte+j*3+2) = t;
		}
	}
}

int main(void)
{
	FILE* fpw;
	char filename[100];
    /////////////////////////读入bmp文件
    cout << "input file name:" <<endl;
    scanf("%s",filename);	
	LoadBitmapFile(filename);

	////////////////////////RGB to YUV
	bitmapData_yuv = RGBtoYUV(bitmapImage, bmpWidth, bmpHeight);

	////////////////////////Color to Gray
	bitmapData_gray = ColorToGray(bitmapData_yuv, bmpWidth, bmpHeight);

	////////////////////////Rearrange Gray intensity to [0,255]
	reArrangeGray(bitmapData_gray,bmpWidth, bmpHeight);

	////////////////////////Write a grayscale bmp
	WriteBMP(bitmapData_gray,"gray.bmp");

	////////////////////////Change the luminance value Y
	int IBright = 50;
	cout << "input lunminance value you want to add:" <<endl;
	scanf("%d",&IBright);
	bitmapImage = ChangeLuminance(bitmapData_yuv, IBright, bmpWidth, bmpHeight);

	///////////////////////Write a color bmp
	WriteBMP(bitmapImage, "LuminChange.bmp");
}


void showBmpHead(BITMAPFILEHEADER pBmpHead)
{
	cout << "filehead" << endl;
	cout << "bfSize:" << pBmpHead.bfSize << endl;
	cout << "bfReserved1:" << pBmpHead.bfReserved1 << endl;
	cout << "bfReserved2:" << pBmpHead.bfReserved2 << endl;
	cout << "bfOffBits:" << pBmpHead.bfOffBits << endl << endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead)
{
	cout << "fileinfohead" << endl;
	cout << "biSize:" << pBmpInforHead.biSize << endl;
	cout << "biWidth:" << pBmpInforHead.biWidth << endl;
	cout << "biHeight:" << pBmpInforHead.biHeight << endl;
	cout << "biPlanes:" << pBmpInforHead.biPlanes << endl;
	cout << "biBitCount:" << pBmpInforHead.biBitCount << endl;
	cout << "biCompression:" << pBmpInforHead.biCompression << endl;
	cout << "biSizeImage:" << pBmpInforHead.biSizeImage << endl;
	cout << "biXPelsPerMeter:" << pBmpInforHead.biXPelsPerMeter << endl;
	cout << "biYPelsPerMeter :" << pBmpInforHead.biYPelsPerMeter << endl;
	cout << "biClrUsed:" << pBmpInforHead.biClrUsed << endl;
	cout << "biClrImportant:" << pBmpInforHead.biClrImportant << endl;
}

