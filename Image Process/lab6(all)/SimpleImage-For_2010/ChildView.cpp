
//���������Ҫ���ļ�

#include "stdafx.h"
#include "CImage.h"
#include "ChildView.h"
#include "InfoDlg.h"
#include <math.h>
#include <malloc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*------------------------------------------------------------------------------------------------------------
1.���������ǻ���MFC��������ʾ������ͼ���Ӧ�ó����ܣ�ͬѧ�ǿ����ڴ˿�ܵĻ����������ҵ����Ĺ��ܴ��롣
2.�����д���ͼ�������΢��MFC��������ṩ��CImage�࣬�����ṩ�˶�ȡ����ʾ���ָ�ʽͼ��ĺ��������Դ��򻯴�
  ��ͼ������еĹ�������ʹ��Ұ���Ҫ������������ҵ���еĹ���ʵ���ϡ�
3.ÿ����ҵ��Ӧ�Ų˵���Tools�µ�һ���˵��ÿ���˵����Ӧһ���հ׺���������ʵ��ÿ����ҵ�Ĺ��ܴ��롣
4.�����һ��ͼ���Ժ�ͼ�������洢��CImage���͵�imgOriginal�����У����CImage��ĳ�Ա������ǿ�󣬿�����MSDN
 ����F1�򿪣��в�ѯ�й����Ĳ�����imgOriginal�����Ǳ�������CChildView��˽�б��������������µĳ�Ա�����б����á�
  �й�ͼ���һЩ��������������imgOriginal.������()�ķ������ã������һ��ͼ�������imgOriginal.Load("x.jpg")��
  �������õĲ�������MSDN������CImage��
5.�����CChildView::OnToolsMakeBW��һ���򵥵����ӣ����ǲ˵���Tools�µĵ�һ���˵����Ӧ����Ϣ��Ӧ�������Ѳ�ɫͼ��
  תΪ�Ҷ�ͼ��
------------------------------------------------------------------------------------------------------------*/
void CChildView::OnToolsMakeBW(void)
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		//��ÿ�����شӲ�ɫתΪ�Ҷ�
		COLORREF pixel;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
		byte r,g,b,avg;//r,g,bֵ��ƽ���Ҷ�ֵ����
		for (int x=0; x<maxX; x++) //��ѭ��
		{
			for (int y=0; y<maxY; y++) //��ѭ��
			{
				pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
				r = GetRValue(pixel);//ȡ��pixel��rֵ
				g = GetGValue(pixel);//ȡ��pixel��gֵ
				b = GetBValue(pixel);//ȡ��pixel��bֵ
				avg = ((r + g + b)/3);//�ô�ʽ��rgb��ɫֵתΪ�Ҷ�ֵ
				imgOriginal.SetPixelRGB(x,y,avg,avg,avg);//��λ��(x,y)λ���ϵ����ص�rgbɫ��ͨ����Ϊ(avg,avg,avg)����ת��Ϊ�Ҷ�
			}
		}
	} 
	//�°벿�ֵĴ����Ǵ���ͼ���ÿ������С��8λ�ģ�������ϸע��
	else
	{
		// the image uses an indexed palette, so we will just change the palette table entries to their B&W equivalents 
		int MaxColors = imgOriginal.GetMaxColorTableEntries();
		RGBQUAD* ColorTable;
		ColorTable = new RGBQUAD[MaxColors];

		imgOriginal.GetColorTable(0,MaxColors,ColorTable);
		for (int i=0; i<MaxColors; i++)
		{
			int avg = (ColorTable[i].rgbBlue + ColorTable[i].rgbGreen + ColorTable[i].rgbRed)/3;
			ColorTable[i].rgbBlue = (BYTE)avg;
			ColorTable[i].rgbGreen = (BYTE)avg;
			ColorTable[i].rgbRed = (BYTE)avg;
		}
		imgOriginal.SetColorTable(0,MaxColors,ColorTable);
	
		delete[] ColorTable;
	}

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}



/*-----------------------code for lab2----------------------------------
tasks:
1.Image binarization��
2.Binary image erosion
3.Binary image delation
4.Binary image opening
5.Binary image closing
6.Hit-or-miss transform (optional)
----------------------------------------------------------------------*/
void CChildView::OnHomeworkiiBinarization()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArray = new int[maxX*maxY];

	//Find the maximal and minimal pixel value in the image
	byte MAXavg = 0, MINavg = 255, temp;
	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			temp = GetRValue(pixel);//ȡ��pixel��rֵ
			MAXavg = MAXavg >= temp ? MAXavg:temp;
			MINavg = MINavg <= temp ? MINavg:temp;
			pixelArray[x*maxY+y] = temp;
		}
	}

	//Thresholdingthe image by ��minimal value +1�� to obtain the binary image;
	int threshold = 0;
	int allPix = maxX * maxY;
	double betweenVariance = 0;
	for(int i = MINavg+1; i < MAXavg; ++i)
	{
		int forePix = 0, backPix = 0;
		int foreSum = 0, backSum = 0;
		//Determine the foreground and background in terms of the binary image;
		for(int j = 0; j < allPix; ++j)
		{
			if(pixelArray[j] >= i)
			{
				++backPix;
				backSum += pixelArray[j];
			}
			else
			{
				++forePix;
				foreSum += pixelArray[j];
			}
		}
		//Compute the the between-variance;
		double uf = (double)foreSum/forePix;
		double ub = (double)backSum/backPix;
		double tempVariance = (double)backPix/allPix*forePix/allPix*(uf - ub)*(uf - ub);
		if(betweenVariance < tempVariance)
		{
			betweenVariance = tempVariance;
			threshold = i;
		}
	}

	//Obtain the threshold and do Image binarization
	OnToolsMakeBW();
	byte avg, r;
	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			r = GetRValue(pixel);//ȡ��pixel��rֵ
			avg = (r >= threshold)? 255:0;
			imgOriginal.SetPixelRGB(x,y,avg,avg,avg);//��λ��(x,y)λ���ϵ����ص�rgbɫ��ͨ����Ϊ(avg,avg,avg)����ת��Ϊ�Ҷ�
		}
	}
	delete[] pixelArray;
	Invalidate();
	UpdateWindow();
}

void CChildView::erosion()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArray = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			pixelArray[x*maxY+y] = GetRValue(pixel);
		}
	}

	//ʹ��3*3�ĽṹԪ�ظ�ʴ

	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			int nErosion = 1;
			for(int i=-1; i<2; ++i)
			{
				for(int j=-1; j<2; ++j)
				{
					if(pixelArray[(x+i)*maxY+(y+j)] != 255)
					{
						nErosion = 0;
						break;
					}
				}
			}
			if(!nErosion)
			{
				imgOriginal.SetPixelRGB(x,y,0,0,0);
			}
		}
	}
	Invalidate();
	UpdateWindow();	
}
void CChildView::OnHomeworkiiErosion()
{
	OnHomeworkiiBinarization();
	erosion();
}

void CChildView::delation()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArray = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			pixelArray[x*maxY+y] = GetRValue(pixel);
		}
	}

	//ʹ��3*3�ĽṹԪ������
	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			int nDeflation = 1;
			for(int i=-1; i<2; ++i)
			{
				for(int j=-1; j<2; ++j)
				{
					if(pixelArray[(x+i)*maxY+(y+j)] == 255)
					{
						nDeflation = 0;
						break;
					}
				}
			}
			if(!nDeflation)
			{
				imgOriginal.SetPixelRGB(x,y,255,255,255);
			}
		}
	}	
	Invalidate();
	UpdateWindow();
}
void CChildView::OnHomeworkiiDelation()
{
	OnHomeworkiiBinarization();
	delation();
}


void CChildView::OnHomeworkiiOpening()
{
	OnHomeworkiiBinarization();
	erosion();
	delation();	
}

void CChildView::OnHomeworkiiClosing()
{
	OnHomeworkiiBinarization();
	delation();
	erosion();	
}
void CChildView::OnToolsHomeworkII()
{
}

/*-----------------------code for lab3----------------------------------
tasks:
1. Image logarithmic operation for visibility enhancement
2. Histogram equalization
----------------------------------------------------------------------*/

//Ϊ�˵��������Ϣ��Ӧ���������ʵ�ִ��루ϸ�ڲμ�ppt��
void CChildView::OnToolsHomeworkIII()
{
	// TODO: Add your command handler code here
	//::AfxMessageBox("Hello World!");
}

void CChildView::OnHomeworkiiiImagelogarithmic()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int r,g,b;
	double *yy1 = new double[maxY*maxX];
	double *uu1 = new double[maxY*maxX];
	double *vv1 = new double[maxY*maxX];;
	double y_max = 0;//,y_new;
	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			r = GetRValue(pixel);//ȡ��pixel��rֵ
			g = GetGValue(pixel);//ȡ��pixel��gֵ
			b = GetBValue(pixel);//ȡ��pixel��bֵ
			yy1[x*maxY+y] = 0.299*r+0.587*g+0.114*b;
			uu1[x*maxY+y] = -0.147*r-0.289*g+0.435*b;
			vv1[x*maxY+y] = 0.615*r-0.515*g-0.100*b;
			if(yy1[x*maxY+y]>y_max)
			{
				y_max = yy1[x*maxY+y];
			}
		}
	}
	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			yy1[x*maxY+y] = log((yy1[x*maxY+y]/255 + 1))/log((y_max/255+1))*255;

			r = (int)(yy1[x*maxY+y] + 1.140*vv1[x*maxY+y]); if(r > 255) r=255;
			g = (int)(yy1[x*maxY+y] - 0.394*uu1[x*maxY+y] - 0.581*vv1[x*maxY+y]);if(g > 255) g=255;
			b = (int)(yy1[x*maxY+y] + 2.032*uu1[x*maxY+y]);if(b > 255) b=255;
			imgOriginal.SetPixelRGB(x,y,(BYTE)r,(BYTE)g,(BYTE)b);

		}
	}
	delete [] yy1,uu1,vv1;

	Invalidate();
	UpdateWindow();
}

void CChildView::OnHomeworkiiiHistogramequalization()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int allPix = maxX * maxY;
	int r,g,b;
	int n_r[256], n_g[256], n_b[256];
	int f_r[256], f_g[256], f_b[256];
	double p_r[256], p_g[256], p_b[256];

	for(int i=0; i<256; ++i) //��ʼ��Ϊ0
	{
		n_r[i] = 0; n_g[i] = 0; n_b[i] = 0;
		p_r[i] = 0; p_g[i] = 0; p_b[i] = 0;		
	}
	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			r = GetRValue(pixel);//ȡ��pixel��rֵ
			g = GetGValue(pixel);//ȡ��pixel��gֵ
			b = GetBValue(pixel);//ȡ��pixel��bֵ
			++n_r[r];
			++n_g[g];
			++n_b[b];
		}
	}
	for(int i=0; i<256; ++i)
	{
		p_r[i] = (double)n_r[i]/(double)allPix;
		p_g[i] = (double)n_g[i]/(double)allPix;
		p_b[i] = (double)n_b[i]/(double)allPix;
	}
	for(int i=0; i<256; ++i)
	{
		if(i != 0)
		{
			p_r[i] += p_r[i-1];
			p_g[i] += p_g[i-1];
			p_b[i] += p_b[i-1];
		}
		f_r[i] = (int)((p_r[i]*255)+0.5); 
		f_g[i] = (int)((p_g[i]*255)+0.5);
		f_b[i] = (int)((p_b[i]*255)+0.5);

		if(f_r[i] > 255) f_r[i] = 255;
		if(f_r[i] < 0) f_r[i] = 0;

		if(f_g[i] > 255) f_g[i] = 255;
		if(f_g[i] < 0) f_g[i] = 0;

		if(f_b[i] > 255) f_b[i] = 255;
		if(f_b[i] < 0) f_b[i] = 0;
	}
	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			r = GetRValue(pixel);//ȡ��pixel��rֵ
			g = GetGValue(pixel);//ȡ��pixel��gֵ
			b = GetBValue(pixel);//ȡ��pixel��bֵ
			r = f_r[r];
			g = f_g[g];
			b = f_b[b];
			imgOriginal.SetPixelRGB(x,y,(BYTE)r,(BYTE)g,(BYTE)b);
		}
	}	
	Invalidate();
	UpdateWindow();
}

/*-----------------------code for lab4----------------------------------
tasks:
Simple geometric transformation
	1. Translation
	2. Rotation
	3. Scale
	4. Shear
	5. Mirror
----------------------------------------------------------------------*/
bool ImageCopy(const CImage &srcImage, CImage &destImage)
{
	int i;//ѭ������
	if(srcImage.IsNull())
		return FALSE;
	//Դͼ�����
	BYTE* srcPtr=(BYTE*)srcImage.GetBits();
	int srcBitsCount=srcImage.GetBPP();
	int srcWidth=srcImage.GetWidth();
	int srcHeight=srcImage.GetHeight();
	int srcPitch=srcImage.GetPitch(); 
	//����ԭ��ͼ��
	if( !destImage.IsNull())
	{
		destImage.Destroy();
	}
	//������ͼ��
	if(srcBitsCount==32)   //֧��alphaͨ��
	{
		destImage.Create(srcWidth,srcHeight,srcBitsCount,1);
	}
	else
	{
		destImage.Create(srcWidth,srcHeight,srcBitsCount,0);
	}
	//���ص�ɫ��
	if(srcBitsCount<=8&&srcImage.IsIndexed())//��Ҫ��ɫ��
	{
		RGBQUAD pal[256];
		int nColors=srcImage.GetMaxColorTableEntries();
		if(nColors>0)
		{     
			srcImage.GetColorTable(0,nColors,pal);
			destImage.SetColorTable(0,nColors,pal);//���Ƶ�ɫ�����
		}   
	} 
	//Ŀ��ͼ�����
	BYTE *destPtr=(BYTE*)destImage.GetBits();
	int destPitch=destImage.GetPitch();
	//����ͼ������
	for(i=0 ; i<srcHeight;i++)
	{
		memcpy( destPtr+i*destPitch, srcPtr+i*srcPitch, abs(srcPitch) );
	} 

	return TRUE;
} 

void CChildView::OnHomeworkivTranslation()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	int moveX, moveY;

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		COLORREF pixel;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
		moveX = maxX/6;
		moveY = maxY/6;

		byte r,g,b;//r,g,bֵ
		CImage imgTranslation;
		imgTranslation.Create(maxX+moveX,maxY+moveY,imgOriginal.GetBPP(),0UL);
		for (int x=0; x<imgTranslation.GetWidth(); x++) //��ѭ��
		{
			for (int y=0; y<imgTranslation.GetHeight(); y++) //��ѭ��
			{
				imgTranslation.SetPixelRGB(x,y,255,255,255);
			}
		}
		for (int x=0; x<maxX; x++) //��ѭ��
		{
			for (int y=0; y<maxY; y++) //��ѭ��
			{
				pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
				r = GetRValue(pixel);//ȡ��pixel��rֵ
				g = GetGValue(pixel);//ȡ��pixel��gֵ
				b = GetBValue(pixel);//ȡ��pixel��bֵ
				imgTranslation.SetPixelRGB(x+moveX,y+moveY,r,g,b);
			}
		}
		ImageCopy(imgTranslation,imgOriginal);
		
	} 

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}

#define PI 3.1415
void CChildView::OnHomeworkivRotation()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	double rotateTheta = PI/4;  //45'
	double oriTheta;
	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		//COLORREF pixel;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
		int centerX = maxX/2; //��������
		int centerY = maxY/2;
		double diagonalLen = sqrt((double)(centerX*centerX+centerY*centerX)); //�Խ���
		oriTheta = atan((double)centerY / (double)centerX);

		int X1 = (int)(fabs(diagonalLen*cos(rotateTheta+oriTheta))+0.5);
		int Y1 = (int)(fabs(diagonalLen*sin(rotateTheta+oriTheta))+0.5);
		int X2 = (int)(fabs(diagonalLen*cos(rotateTheta-oriTheta))+0.5);
		int Y2 = (int)(fabs(diagonalLen*sin(rotateTheta-oriTheta))+0.5);		

		int newmaxX = X2>X1? 2*X2:2*X1;
		int newmaxY = Y2>Y1? 2*Y2:2*Y1;

		double sinTheta = sin(rotateTheta);
		double cosTheta = cos(rotateTheta);

		//byte r,g,b;//r,g,bֵ
		CImage imgRotation;
		imgRotation.Create(newmaxX,newmaxY,imgOriginal.GetBPP(),0UL);

		for (int x=0; x<imgRotation.GetWidth(); x++) //��ѭ��
		{
			for (int y=0; y<imgRotation.GetHeight(); y++) //��ѭ��
			{
				imgRotation.SetPixelRGB(x,y,255,255,255);
			}
		}
		//˫���Բ�ֵ
		for (int x=0; x<newmaxX; x++) //��ѭ��
		{
			for (int y=0; y<newmaxY; y++) //��ѭ��
			{
				double yorigin = (double)(y-(double)newmaxY/2)*cosTheta - (double)(x-(double)newmaxX/2)*sinTheta + centerX;
				double xorigin = (double)(y-(double)newmaxY/2)*sinTheta + (double)(x-(double)newmaxX/2)*cosTheta + centerY;
				COLORREF pixel0, pixel1, pixel2, pixel3;//����һ�����ص�ɫ����Ϣ
				byte r,g,b;//r,g,bֵ
				byte r0,g0,b0;//r,g,bֵ
				byte r1,g1,b1;//r,g,bֵ
				byte r2,g2,b2;//r,g,bֵ
				byte r3,g3,b3;//r,g,bֵ

				if(xorigin >= maxX -1 || xorigin < 0 || yorigin >= maxY -1 || yorigin < 0)
				{
					continue;
				}
	
				if(xorigin != maxX-1 && yorigin != maxY-1)
				{
					int i = (int)xorigin;
					int j = (int)yorigin;
					double u = xorigin - i;
					double v = yorigin - j;

					pixel0 = imgOriginal.GetPixel(i,j);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r0 = GetRValue(pixel0);//ȡ��pixel��rֵ
					g0 = GetGValue(pixel0);//ȡ��pixel��gֵ
					b0 = GetBValue(pixel0);//ȡ��pixel��bֵ
					pixel1 = imgOriginal.GetPixel(i+1,j);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r1 = GetRValue(pixel1);//ȡ��pixel��rֵ
					g1 = GetGValue(pixel1);//ȡ��pixel��gֵ
					b1 = GetBValue(pixel1);//ȡ��pixel��bֵ				
					pixel2 = imgOriginal.GetPixel(i,j+1);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r2 = GetRValue(pixel2);//ȡ��pixel��rֵ
					g2 = GetGValue(pixel2);//ȡ��pixel��gֵ
					b2 = GetBValue(pixel2);//ȡ��pixel��bֵ
					pixel3 = imgOriginal.GetPixel(i+1,j+1);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r3 = GetRValue(pixel3);//ȡ��pixel��rֵ
					g3 = GetGValue(pixel3);//ȡ��pixel��gֵ
					b3 = GetBValue(pixel3);//ȡ��pixel��bֵ
	
					r = (byte)((int)((1-u)*(1-v)*r0 + (1-u)*v*r1 + u*(1-v)*r2 + u*v*r3));
					g = (byte)((int)((1-u)*(1-v)*g0 + (1-u)*v*g1 + u*(1-v)*g2 + u*v*g3));
					b = (byte)((int)((1-u)*(1-v)*b0 + (1-u)*v*b1 + u*(1-v)*b2 + u*v*b3));
					imgRotation.SetPixelRGB(x,y,r,g,b);
				}
			}
		}
		ImageCopy(imgRotation,imgOriginal);
		
	} 

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}

//scale 5 functions
void CChildView::OnHomeworkivScale()
{
	// TODO: �ڴ���������������
}

void CChildView::OnScaleSameratio()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	int moveX, moveY;

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		COLORREF pixel;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
		moveX = maxX/2;//set ratio = 1/2
		moveY = maxY/2;

		byte r,g,b;//r,g,bֵ
		CImage imgScale;
		imgScale.Create(moveX,moveY,imgOriginal.GetBPP(),0UL);
		for (int x=0; x<maxX; x++) //��ѭ��
		{
			for (int y=0; y<maxY; y++) //��ѭ��
			{
				pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
				r = GetRValue(pixel);//ȡ��pixel��rֵ
				g = GetGValue(pixel);//ȡ��pixel��gֵ
				b = GetBValue(pixel);//ȡ��pixel��bֵ
				imgScale.SetPixelRGB(x/2,y/2,r,g,b);
			}
		}
		ImageCopy(imgScale,imgOriginal);
		
	} 

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}

void CChildView::OnScaleDifferentratio()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	int moveX, moveY;

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		COLORREF pixel;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
		moveX = maxX/2;//set ratio = 1/2
		moveY = maxY*2/3;

		byte r,g,b;//r,g,bֵ
		CImage imgScale;
		imgScale.Create(moveX,moveY,imgOriginal.GetBPP(),0UL);
		for (int x=0; x<maxX; x++) //��ѭ��
		{
			for (int y=0; y<maxY; y++) //��ѭ��
			{
				pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
				r = GetRValue(pixel);//ȡ��pixel��rֵ
				g = GetGValue(pixel);//ȡ��pixel��gֵ
				b = GetBValue(pixel);//ȡ��pixel��bֵ
				imgScale.SetPixelRGB(x/2,y*2/3,r,g,b);
			}
		}
		ImageCopy(imgScale,imgOriginal);
		
	} 

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}

void CChildView::OnScaleSameratiolarger()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	int moveX, moveY;

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		COLORREF pixel0, pixel1, pixel2, pixel3;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
		moveX = (int)((maxX*1.2));//set ratio = 1.2
		moveY = (int)((maxY*1.2));

		byte r,g,b;//r,g,bֵ
		byte r0,g0,b0;//r,g,bֵ
		byte r1,g1,b1;//r,g,bֵ
		byte r2,g2,b2;//r,g,bֵ
		byte r3,g3,b3;//r,g,bֵ
		CImage imgScale;
		imgScale.Create(moveX,moveY,imgOriginal.GetBPP(),0UL);
		for (int x=0; x<moveX; x++) //��ѭ��
		{
			for (int y=0; y<moveY; y++) //��ѭ��
			{
				double xorigin = x/1.2;
				double yorigin = y/1.2;
				//(i+u,j+v) 
				xorigin = (xorigin >= maxX-1)? maxX-1: (xorigin<0?0:xorigin);
				yorigin = (yorigin >= maxY-1)? maxY-1: (yorigin<0?0:yorigin);
				if(xorigin == maxX-1 || yorigin == maxY-1)
				{
					pixel0 = imgOriginal.GetPixel((int)xorigin,(int)yorigin);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r = GetRValue(pixel0);//ȡ��pixel��rֵ
					g = GetGValue(pixel0);//ȡ��pixel��gֵ
					b = GetBValue(pixel0);//ȡ��pixel��bֵ
				}
				else
				{
					int i = (int)xorigin;
					int j = (int)yorigin;
					double u = xorigin - i;
					double v = yorigin - j;
					//i >= maxX-1? maxX-2: (i<0?0:i);
					//j >= maxY-1? maxY-2: (j<0?0:j);
					pixel0 = imgOriginal.GetPixel(i,j);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r0 = GetRValue(pixel0);//ȡ��pixel��rֵ
					g0 = GetGValue(pixel0);//ȡ��pixel��gֵ
					b0 = GetBValue(pixel0);//ȡ��pixel��bֵ
					pixel1 = imgOriginal.GetPixel(i+1,j);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r1 = GetRValue(pixel1);//ȡ��pixel��rֵ
					g1 = GetGValue(pixel1);//ȡ��pixel��gֵ
					b1 = GetBValue(pixel1);//ȡ��pixel��bֵ				
					pixel2 = imgOriginal.GetPixel(i,j+1);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r2 = GetRValue(pixel2);//ȡ��pixel��rֵ
					g2 = GetGValue(pixel2);//ȡ��pixel��gֵ
					b2 = GetBValue(pixel2);//ȡ��pixel��bֵ
					pixel3 = imgOriginal.GetPixel(i+1,j+1);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r3 = GetRValue(pixel3);//ȡ��pixel��rֵ
					g3 = GetGValue(pixel3);//ȡ��pixel��gֵ
					b3 = GetBValue(pixel3);//ȡ��pixel��bֵ
	
					r = (byte)((int)((1-u)*(1-v)*r0 + (1-u)*v*r1 + u*(1-v)*r2 + u*v*r3));
					g = (byte)((int)((1-u)*(1-v)*g0 + (1-u)*v*g1 + u*(1-v)*g2 + u*v*g3));
					b = (byte)((int)((1-u)*(1-v)*b0 + (1-u)*v*b1 + u*(1-v)*b2 + u*v*b3));
				}

				imgScale.SetPixelRGB(x,y,r,g,b);
			}
		}
		ImageCopy(imgScale,imgOriginal);
		
	} 

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}
void CChildView::OnScaleDifferentratiolarger()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	int moveX, moveY;

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		COLORREF pixel0, pixel1, pixel2, pixel3;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
		moveX = (int)((maxX*1.2)); //set x ratio = 1.2
		moveY = (int)((maxY*1.4));//set y ratio = 1.4

		byte r,g,b;//r,g,bֵ
		byte r0,g0,b0;//r,g,bֵ
		byte r1,g1,b1;//r,g,bֵ
		byte r2,g2,b2;//r,g,bֵ
		byte r3,g3,b3;//r,g,bֵ
		CImage imgScale;
		imgScale.Create(moveX,moveY,imgOriginal.GetBPP(),0UL);
		for (int x=0; x<moveX; x++) //��ѭ��
		{
			for (int y=0; y<moveY; y++) //��ѭ��
			{
				double xorigin = x/1.2;
				double yorigin = y/1.4;
				//(i+u,j+v) 
				xorigin = (xorigin >= maxX-1)? maxX-1: (xorigin<0?0:xorigin);
				yorigin = (yorigin >= maxY-1)? maxY-1: (yorigin<0?0:yorigin);
				if(xorigin == maxX-1 || yorigin == maxY-1)
				{
					pixel0 = imgOriginal.GetPixel((int)xorigin,(int)yorigin);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r = GetRValue(pixel0);//ȡ��pixel��rֵ
					g = GetGValue(pixel0);//ȡ��pixel��gֵ
					b = GetBValue(pixel0);//ȡ��pixel��bֵ
				}
				else    //˫���Բ�ֵ
				{
					int i = (int)xorigin;
					int j = (int)yorigin;
					double u = xorigin - i;
					double v = yorigin - j;
					//i >= maxX-1? maxX-2: (i<0?0:i);
					//j >= maxY-1? maxY-2: (j<0?0:j);
					pixel0 = imgOriginal.GetPixel(i,j);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r0 = GetRValue(pixel0);//ȡ��pixel��rֵ
					g0 = GetGValue(pixel0);//ȡ��pixel��gֵ
					b0 = GetBValue(pixel0);//ȡ��pixel��bֵ
					pixel1 = imgOriginal.GetPixel(i+1,j);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r1 = GetRValue(pixel1);//ȡ��pixel��rֵ
					g1 = GetGValue(pixel1);//ȡ��pixel��gֵ
					b1 = GetBValue(pixel1);//ȡ��pixel��bֵ				
					pixel2 = imgOriginal.GetPixel(i,j+1);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r2 = GetRValue(pixel2);//ȡ��pixel��rֵ
					g2 = GetGValue(pixel2);//ȡ��pixel��gֵ
					b2 = GetBValue(pixel2);//ȡ��pixel��bֵ
					pixel3 = imgOriginal.GetPixel(i+1,j+1);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
					r3 = GetRValue(pixel3);//ȡ��pixel��rֵ
					g3 = GetGValue(pixel3);//ȡ��pixel��gֵ
					b3 = GetBValue(pixel3);//ȡ��pixel��bֵ
	
					r = (byte)((int)((1-u)*(1-v)*r0 + (1-u)*v*r1 + u*(1-v)*r2 + u*v*r3));
					g = (byte)((int)((1-u)*(1-v)*g0 + (1-u)*v*g1 + u*(1-v)*g2 + u*v*g3));
					b = (byte)((int)((1-u)*(1-v)*b0 + (1-u)*v*b1 + u*(1-v)*b2 + u*v*b3));
				}

				imgScale.SetPixelRGB(x,y,r,g,b);
			}
		}
		ImageCopy(imgScale,imgOriginal);
		
	} 

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}

//shear 3 functions
void CChildView::OnHomeworkivShear()
{
	// TODO: �ڴ���������������
}
void CChildView::OnShearOnxaxis()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	int moveX;

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		COLORREF pixel;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY

		moveX = maxY/6;

		byte r,g,b;//r,g,bֵ
		CImage imgShear;
		imgShear.Create(maxX+moveX,maxY,imgOriginal.GetBPP(),0UL);
		for (int x=0; x<imgShear.GetWidth(); x++) //��ѭ��
		{
			for (int y=0; y<imgShear.GetHeight(); y++) //��ѭ��
			{
				imgShear.SetPixelRGB(x,y,255,255,255);
			}
		}
		for (int x=0; x<maxX; x++) //��ѭ��
		{
			for (int y=0; y<maxY; y++) //��ѭ��
			{
				pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
				r = GetRValue(pixel);//ȡ��pixel��rֵ
				g = GetGValue(pixel);//ȡ��pixel��gֵ
				b = GetBValue(pixel);//ȡ��pixel��bֵ
				imgShear.SetPixelRGB(x+y/6,y,r,g,b);
			}
		}
		ImageCopy(imgShear,imgOriginal);
		
	} 

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}
void CChildView::OnShearOnyaxis()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	int moveY;

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		COLORREF pixel;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY

		moveY = maxX/6;

		byte r,g,b;//r,g,bֵ
		CImage imgShear;
		imgShear.Create(maxX,maxY+moveY,imgOriginal.GetBPP(),0UL);
		for (int x=0; x<imgShear.GetWidth(); x++) //��ѭ��
		{
			for (int y=0; y<imgShear.GetHeight(); y++) //��ѭ��
			{
				imgShear.SetPixelRGB(x,y,255,255,255);
			}
		}
		for (int x=0; x<maxX; x++) //��ѭ��
		{
			for (int y=0; y<maxY; y++) //��ѭ��
			{
				pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
				r = GetRValue(pixel);//ȡ��pixel��rֵ
				g = GetGValue(pixel);//ȡ��pixel��gֵ
				b = GetBValue(pixel);//ȡ��pixel��bֵ
				imgShear.SetPixelRGB(x,y+x/6,r,g,b);
			}
		}
		ImageCopy(imgShear,imgOriginal);
		
	} 

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}

void CChildView::OnHomeworkivMirror()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	if (!imgOriginal.IsIndexed()) //�����ǰ�򿪵�ͼ���ÿ�����ش���8λ����û��ʹ��ɫ���������󲿷�ͼ���������֣�
	{
		COLORREF pixel;//����һ�����ص�ɫ����Ϣ
		int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY


		byte r,g,b;//r,g,bֵ
		CImage imgMirror;
		imgMirror.Create(maxX,maxY,imgOriginal.GetBPP(),0UL);
		for (int x=0; x<maxX; x++) //��ѭ��
		{
			for (int y=0; y<maxY; y++) //��ѭ��
			{
				pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
				r = GetRValue(pixel);//ȡ��pixel��rֵ
				g = GetGValue(pixel);//ȡ��pixel��gֵ
				b = GetBValue(pixel);//ȡ��pixel��bֵ
				imgMirror.SetPixelRGB(maxX-x-1,y,r,g,b);
			}
		}
		ImageCopy(imgMirror,imgOriginal);
		
	} 

	//ˢ����ʾͼ��
	Invalidate();
	UpdateWindow();
}

/*-----------------------code for lab5----------------------------------
tasks:
	1. Image mean filtering
	2. Laplacianimage enhancement
----------------------------------------------------------------------*/

void CChildView::OnHomeworkvMeanfiltering()
{
	// TODO: �ڴ���������������
}

//use the following mask
//[1,1,1;1,1,1;1,1,1]
void CChildView::OnMeanfilteringSimplemean()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArrayR = new int[maxX*maxY];
	int *pixelArrayG = new int[maxX*maxY];
	int *pixelArrayB = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			pixelArrayR[x*maxY+y] = GetRValue(pixel);
			pixelArrayG[x*maxY+y] = GetGValue(pixel);
			pixelArrayB[x*maxY+y] = GetBValue(pixel);
		}
	}

	//ʹ��3*3�Ĺ�������

	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;
			for(int i=-1; i<2; ++i)
			{
				for(int j=-1; j<2; ++j)
				{
					sumR += pixelArrayR[(x+i)*maxY+(y+j)];
					sumG += pixelArrayG[(x+i)*maxY+(y+j)];
					sumB += pixelArrayB[(x+i)*maxY+(y+j)];					
				}
			}
			imgOriginal.SetPixelRGB(x,y,(byte)(sumR/9),(byte)(sumG/9),(byte)(sumB/9)); //divide before transfer to byte
		}
	}
	Invalidate();
	UpdateWindow();	
}

//use the following mask
//[1,2,1;2,4,2;1,2,1]
void CChildView::OnMeanfilteringWeightedmean()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArrayR = new int[maxX*maxY];
	int *pixelArrayG = new int[maxX*maxY];
	int *pixelArrayB = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			pixelArrayR[x*maxY+y] = GetRValue(pixel);
			pixelArrayG[x*maxY+y] = GetGValue(pixel);
			pixelArrayB[x*maxY+y] = GetBValue(pixel);
		}
	}

	//ʹ��3*3�Ĺ�������

	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;

			sumR += pixelArrayR[(x-1)*maxY+(y-1)] + 2*pixelArrayR[(x)*maxY+(y-1)] + pixelArrayR[(x+1)*maxY+(y-1)] \
				    + 2*pixelArrayR[(x-1)*maxY+(y)] + 4*pixelArrayR[(x)*maxY+(y)] + 2*pixelArrayR[(x+1)*maxY+(y)] \
				    + pixelArrayR[(x-1)*maxY+(y+1)] + 2*pixelArrayR[(x)*maxY+(y+1)] + pixelArrayR[(x+1)*maxY+(y+1)];
			sumG += pixelArrayG[(x-1)*maxY+(y-1)] + 2*pixelArrayG[(x)*maxY+(y-1)] + pixelArrayG[(x+1)*maxY+(y-1)] \
				    + 2*pixelArrayG[(x-1)*maxY+(y)] + 4*pixelArrayG[(x)*maxY+(y)] + 2*pixelArrayG[(x+1)*maxY+(y)] \
				    + pixelArrayG[(x-1)*maxY+(y+1)] + 2*pixelArrayG[(x)*maxY+(y+1)] + pixelArrayG[(x+1)*maxY+(y+1)];
			sumB += pixelArrayB[(x-1)*maxY+(y-1)] + 2*pixelArrayB[(x)*maxY+(y-1)] + pixelArrayB[(x+1)*maxY+(y-1)] \
				    + 2*pixelArrayB[(x-1)*maxY+(y)] + 4*pixelArrayB[(x)*maxY+(y)] + 2*pixelArrayB[(x+1)*maxY+(y)] \
				    + pixelArrayB[(x-1)*maxY+(y+1)] + 2*pixelArrayB[(x)*maxY+(y+1)] + pixelArrayB[(x+1)*maxY+(y+1)];			

			imgOriginal.SetPixelRGB(x,y,(byte)(sumR/16),(byte)(sumG/16),(byte)(sumB/16));  //divide before transfer to byte
		}
	}
	Invalidate();
	UpdateWindow();	
}

void CChildView::OnHomeworkvLaplacian()
{
	// TODO: �ڴ���������������
}

//use the following mask
//[1,1,1;1,-8,1;1,1,1]
void CChildView::OnLaplacianenhancementLaplacianresult()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArrayR = new int[maxX*maxY];
	int *pixelArrayG = new int[maxX*maxY];
	int *pixelArrayB = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			pixelArrayR[x*maxY+y] = GetRValue(pixel);
			pixelArrayG[x*maxY+y] = GetGValue(pixel);
			pixelArrayB[x*maxY+y] = GetBValue(pixel);
		}
	}

	//ʹ��3*3�Ĺ�������

	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;

			sumR += (-1)*pixelArrayR[(x-1)*maxY+(y-1)] + (-1)*pixelArrayR[(x)*maxY+(y-1)] + (-1)*pixelArrayR[(x+1)*maxY+(y-1)] \
								+ (-1)*pixelArrayR[(x-1)*maxY+(y)] + (8)*pixelArrayR[(x)*maxY+(y)] + (-1)*pixelArrayR[(x+1)*maxY+(y)] \
								+ (-1)*pixelArrayR[(x-1)*maxY+(y+1)] + (-1)*pixelArrayR[(x)*maxY+(y+1)] + (-1)*pixelArrayR[(x+1)*maxY+(y+1)];
			sumG += (-1)*pixelArrayG[(x-1)*maxY+(y-1)] + (-1)*pixelArrayG[(x)*maxY+(y-1)] + (-1)*pixelArrayG[(x+1)*maxY+(y-1)] \
								+ (-1)*pixelArrayG[(x-1)*maxY+(y)] + (8)*pixelArrayG[(x)*maxY+(y)] + (-1)*pixelArrayG[(x+1)*maxY+(y)] \
								+ (-1)*pixelArrayG[(x-1)*maxY+(y+1)] + (-1)*pixelArrayG[(x)*maxY+(y+1)] + (-1)*pixelArrayG[(x+1)*maxY+(y+1)];
			sumB += (-1)*pixelArrayB[(x-1)*maxY+(y-1)] + (-1)*pixelArrayB[(x)*maxY+(y-1)] + (-1)*pixelArrayB[(x+1)*maxY+(y-1)] \
								+ (-1)*pixelArrayB[(x-1)*maxY+(y)] + (8)*pixelArrayB[(x)*maxY+(y)] + (-1)*pixelArrayB[(x+1)*maxY+(y)] \
								+ (-1)*pixelArrayB[(x-1)*maxY+(y+1)] + (-1)*pixelArrayB[(x)*maxY+(y+1)] + (-1)*pixelArrayB[(x+1)*maxY+(y+1)];			
		
			sumR = sumR>255?255: sumR<0?0:sumR;
			sumG = sumG>255?255: sumG<0?0:sumG;
			sumB = sumB>255?255: sumB<0?0:sumB;

			imgOriginal.SetPixelRGB(x,y,(byte)sumR,(byte)sumG,(byte)sumB);  //divide before transfer to byte
		}
	}
	delete[] pixelArrayR;
	delete[] pixelArrayG;
	delete[] pixelArrayB;
	Invalidate();
	UpdateWindow();	
}


void CChildView::OnLaplacianenhancementRearrangedlaplacianresult()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArrayR = new int[maxX*maxY];
	int *pixelArrayG = new int[maxX*maxY];
	int *pixelArrayB = new int[maxX*maxY];

	int *sumR = new int[maxX*maxY];
	int *sumG = new int[maxX*maxY];
	int *sumB = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			pixelArrayR[x*maxY+y] = GetRValue(pixel);
			pixelArrayG[x*maxY+y] = GetGValue(pixel);
			pixelArrayB[x*maxY+y] = GetBValue(pixel);
		}
	}

	//ʹ��3*3�Ĺ�������
	int minR = 10000;
	int maxR = -10000;
	int minG = 10000;
	int maxG = -10000;
	int minB = 10000;
	int maxB = -10000;

	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			sumR[(x)*maxY+(y)] = sumG[(x)*maxY+(y)] = sumB[(x)*maxY+(y)] = 0;

			sumR[(x)*maxY+(y)] += (-1)*pixelArrayR[(x-1)*maxY+(y-1)] + (-1)*pixelArrayR[(x)*maxY+(y-1)] + (-1)*pixelArrayR[(x+1)*maxY+(y-1)] \
								+ (-1)*pixelArrayR[(x-1)*maxY+(y)] + (8)*pixelArrayR[(x)*maxY+(y)] + (-1)*pixelArrayR[(x+1)*maxY+(y)] \
								+ (-1)*pixelArrayR[(x-1)*maxY+(y+1)] + (-1)*pixelArrayR[(x)*maxY+(y+1)] + (-1)*pixelArrayR[(x+1)*maxY+(y+1)];
			sumG[(x)*maxY+(y)] += (-1)*pixelArrayG[(x-1)*maxY+(y-1)] + (-1)*pixelArrayG[(x)*maxY+(y-1)] + (-1)*pixelArrayG[(x+1)*maxY+(y-1)] \
								+ (-1)*pixelArrayG[(x-1)*maxY+(y)] + (8)*pixelArrayG[(x)*maxY+(y)] + (-1)*pixelArrayG[(x+1)*maxY+(y)] \
								+ (-1)*pixelArrayG[(x-1)*maxY+(y+1)] + (-1)*pixelArrayG[(x)*maxY+(y+1)] + (-1)*pixelArrayG[(x+1)*maxY+(y+1)];
			sumB[(x)*maxY+(y)] += (-1)*pixelArrayB[(x-1)*maxY+(y-1)] + (-1)*pixelArrayB[(x)*maxY+(y-1)] + (-1)*pixelArrayB[(x+1)*maxY+(y-1)] \
								+ (-1)*pixelArrayB[(x-1)*maxY+(y)] + (8)*pixelArrayB[(x)*maxY+(y)] + (-1)*pixelArrayB[(x+1)*maxY+(y)] \
								+ (-1)*pixelArrayB[(x-1)*maxY+(y+1)] + (-1)*pixelArrayB[(x)*maxY+(y+1)] + (-1)*pixelArrayB[(x+1)*maxY+(y+1)];			
		
			maxR = maxR<sumR[(x)*maxY+(y)] ? sumR[(x)*maxY+(y)] :maxR;
			minR = minR>sumR[(x)*maxY+(y)] ? sumR[(x)*maxY+(y)] :minR;
			maxG = maxG<sumG[(x)*maxY+(y)] ? sumG[(x)*maxY+(y)] :maxG;
			minG = minG>sumG[(x)*maxY+(y)] ? sumG[(x)*maxY+(y)] :minG;
			maxB = maxB<sumB[(x)*maxY+(y)] ? sumB[(x)*maxY+(y)] :maxB;
			minB = minB>sumB[(x)*maxY+(y)] ? sumB[(x)*maxY+(y)] :minB;

		}
	}

	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			sumR[(x)*maxY+(y)] = (sumR[(x)*maxY+(y)] - minR)*80/(maxR-minR);
			sumG[(x)*maxY+(y)] = (sumG[(x)*maxY+(y)] - minG)*80/(maxG-minG);
			sumB[(x)*maxY+(y)] = (sumB[(x)*maxY+(y)] - minB)*80/(maxB-minB);
			imgOriginal.SetPixelRGB(x,y,(byte)sumR[(x)*maxY+(y)],(byte)sumG[(x)*maxY+(y)],(byte)sumB[(x)*maxY+(y)]);  //divide before transfer to byte
		}
	}
	delete[] pixelArrayR;
	delete[] pixelArrayG;
	delete[] pixelArrayB;
	delete[] sumR;
	delete[] sumG;
	delete[] sumB;
	Invalidate();
	UpdateWindow();	
}


void CChildView::OnLaplacianenhancementAfterfusion()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��
	COLORREF pixel;//����һ�����ص�ɫ����Ϣ
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	int *pixelArrayR = new int[maxX*maxY];
	int *pixelArrayG = new int[maxX*maxY];
	int *pixelArrayB = new int[maxX*maxY];

	int *sumR = new int[maxX*maxY];
	int *sumG = new int[maxX*maxY];
	int *sumB = new int[maxX*maxY];

	for (int x=0; x<maxX; x++) //��ѭ��
	{
		for (int y=0; y<maxY; y++) //��ѭ��
		{
			pixel = imgOriginal.GetPixel(x,y);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			pixelArrayR[x*maxY+y] = GetRValue(pixel);
			pixelArrayG[x*maxY+y] = GetGValue(pixel);
			pixelArrayB[x*maxY+y] = GetBValue(pixel);
		}
	}

	//ʹ��3*3�Ĺ�������
	int minR = 10000;
	int maxR = -10000;
	int minG = 10000;
	int maxG = -10000;
	int minB = 10000;
	int maxB = -10000;

	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			sumR[(x)*maxY+(y)] = sumG[(x)*maxY+(y)] = sumB[(x)*maxY+(y)] = 0;

			sumR[(x)*maxY+(y)] += (-1)*pixelArrayR[(x-1)*maxY+(y-1)] + (-1)*pixelArrayR[(x)*maxY+(y-1)] + (-1)*pixelArrayR[(x+1)*maxY+(y-1)] \
								+ (-1)*pixelArrayR[(x-1)*maxY+(y)] + (8)*pixelArrayR[(x)*maxY+(y)] + (-1)*pixelArrayR[(x+1)*maxY+(y)] \
								+ (-1)*pixelArrayR[(x-1)*maxY+(y+1)] + (-1)*pixelArrayR[(x)*maxY+(y+1)] + (-1)*pixelArrayR[(x+1)*maxY+(y+1)];
			sumG[(x)*maxY+(y)] += (-1)*pixelArrayG[(x-1)*maxY+(y-1)] + (-1)*pixelArrayG[(x)*maxY+(y-1)] + (-1)*pixelArrayG[(x+1)*maxY+(y-1)] \
								+ (-1)*pixelArrayG[(x-1)*maxY+(y)] + (8)*pixelArrayG[(x)*maxY+(y)] + (-1)*pixelArrayG[(x+1)*maxY+(y)] \
								+ (-1)*pixelArrayG[(x-1)*maxY+(y+1)] + (-1)*pixelArrayG[(x)*maxY+(y+1)] + (-1)*pixelArrayG[(x+1)*maxY+(y+1)];
			sumB[(x)*maxY+(y)] += (-1)*pixelArrayB[(x-1)*maxY+(y-1)] + (-1)*pixelArrayB[(x)*maxY+(y-1)] + (-1)*pixelArrayB[(x+1)*maxY+(y-1)] \
								+ (-1)*pixelArrayB[(x-1)*maxY+(y)] + (8)*pixelArrayB[(x)*maxY+(y)] + (-1)*pixelArrayB[(x+1)*maxY+(y)] \
								+ (-1)*pixelArrayB[(x-1)*maxY+(y+1)] + (-1)*pixelArrayB[(x)*maxY+(y+1)] + (-1)*pixelArrayB[(x+1)*maxY+(y+1)];	

			//sumR[(x)*maxY+(y)] = sumR[(x)*maxY+(y)]>0? sumR[(x)*maxY+(y)]+pixelArrayR[(x)*maxY+(y)] : (-1)*sumR[(x)*maxY+(y)]+pixelArrayR[(x)*maxY+(y)];
			//sumG[(x)*maxY+(y)] = sumG[(x)*maxY+(y)]>0? sumG[(x)*maxY+(y)]+pixelArrayG[(x)*maxY+(y)] : (-1)*sumG[(x)*maxY+(y)]+pixelArrayG[(x)*maxY+(y)];
			//sumB[(x)*maxY+(y)] = sumB[(x)*maxY+(y)]>0? sumB[(x)*maxY+(y)]+pixelArrayB[(x)*maxY+(y)] : (-1)*sumB[(x)*maxY+(y)]+pixelArrayB[(x)*maxY+(y)];

			maxR = maxR<sumR[(x)*maxY+(y)] ? sumR[(x)*maxY+(y)] :maxR;
			minR = minR>sumR[(x)*maxY+(y)] ? sumR[(x)*maxY+(y)] :minR;
			maxG = maxG<sumG[(x)*maxY+(y)] ? sumG[(x)*maxY+(y)] :maxG;
			minG = minG>sumG[(x)*maxY+(y)] ? sumG[(x)*maxY+(y)] :minG;
			maxB = maxB<sumB[(x)*maxY+(y)] ? sumB[(x)*maxY+(y)] :maxB;
			minB = minB>sumB[(x)*maxY+(y)] ? sumB[(x)*maxY+(y)] :minB;
			sumR[(x)*maxY+(y)] = sumR[(x)*maxY+(y)]>255?255: sumR[(x)*maxY+(y)]<0?0:sumR[(x)*maxY+(y)];
			sumG[(x)*maxY+(y)] = sumG[(x)*maxY+(y)]>255?255: sumG[(x)*maxY+(y)]<0?0:sumG[(x)*maxY+(y)];
			sumB[(x)*maxY+(y)]= sumB[(x)*maxY+(y)]>255?255: sumB[(x)*maxY+(y)]<0?0:sumB[(x)*maxY+(y)];
			sumR[(x)*maxY+(y)] = sumR[(x)*maxY+(y)]*100/255;
			sumG[(x)*maxY+(y)] = sumG[(x)*maxY+(y)]*100/255;
			sumB[(x)*maxY+(y)] = sumB[(x)*maxY+(y)]*100/255;
		}
	}
	int minR2 = 10000;
	int maxR2 = -10000;
	int minG2 = 10000;
	int maxG2 = -10000;
	int minB2 = 10000;
	int maxB2 = -10000;
	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			//sumR[(x)*maxY+(y)] = (sumR[(x)*maxY+(y)] - minR)*80/(maxR-minR);
			//sumG[(x)*maxY+(y)] = (sumG[(x)*maxY+(y)] - minG)*80/(maxG-minG);
			//sumB[(x)*maxY+(y)] = (sumB[(x)*maxY+(y)] - minB)*80/(maxB-minB);

			sumR[(x)*maxY+(y)] = sumR[(x)*maxY+(y)]>0? sumR[(x)*maxY+(y)]+pixelArrayR[(x)*maxY+(y)] : (-1)*sumR[(x)*maxY+(y)]+pixelArrayR[(x)*maxY+(y)];
			sumG[(x)*maxY+(y)] = sumG[(x)*maxY+(y)]>0? sumG[(x)*maxY+(y)]+pixelArrayG[(x)*maxY+(y)] : (-1)*sumG[(x)*maxY+(y)]+pixelArrayG[(x)*maxY+(y)];
			sumB[(x)*maxY+(y)] = sumB[(x)*maxY+(y)]>0? sumB[(x)*maxY+(y)]+pixelArrayB[(x)*maxY+(y)] : (-1)*sumB[(x)*maxY+(y)]+pixelArrayB[(x)*maxY+(y)];
			maxR2 = maxR2<sumR[(x)*maxY+(y)] ? sumR[(x)*maxY+(y)] :maxR2;
			minR2 = minR2>sumR[(x)*maxY+(y)] ? sumR[(x)*maxY+(y)] :minR2;
			maxG2 = maxG2<sumG[(x)*maxY+(y)] ? sumG[(x)*maxY+(y)] :maxG2;
			minG2 = minG2>sumG[(x)*maxY+(y)] ? sumG[(x)*maxY+(y)] :minG2;
			maxB2 = maxB2<sumB[(x)*maxY+(y)] ? sumB[(x)*maxY+(y)] :maxB2;
			minB2 = minB2>sumB[(x)*maxY+(y)] ? sumB[(x)*maxY+(y)] :minB2;

		}
	}

	for (int x=1; x<maxX-1; x++) //��ѭ��
	{
		for (int y=1; y<maxY-1; y++) //��ѭ��
		{
			sumR[(x)*maxY+(y)] = (sumR[(x)*maxY+(y)] - minR2)*255/(maxR2-minR2);
			sumG[(x)*maxY+(y)] = (sumG[(x)*maxY+(y)] - minG2)*255/(maxG2-minG2);
			sumB[(x)*maxY+(y)] = (sumB[(x)*maxY+(y)] - minB2)*255/(maxB2-minB2);
			sumR[(x)*maxY+(y)] = sumR[(x)*maxY+(y)]>255?255: sumR[(x)*maxY+(y)]<0?0:sumR[(x)*maxY+(y)];
			sumG[(x)*maxY+(y)] = sumG[(x)*maxY+(y)]>255?255: sumG[(x)*maxY+(y)]<0?0:sumG[(x)*maxY+(y)];
			sumB[(x)*maxY+(y)]= sumB[(x)*maxY+(y)]>255?255: sumB[(x)*maxY+(y)]<0?0:sumB[(x)*maxY+(y)];			
			imgOriginal.SetPixelRGB(x,y,(byte)sumR[(x)*maxY+(y)],(byte)sumG[(x)*maxY+(y)],(byte)sumB[(x)*maxY+(y)]);  //divide before transfer to byte
		}
	}
	delete[] pixelArrayR;
	delete[] pixelArrayG;
	delete[] pixelArrayB;
	delete[] sumR;
	delete[] sumG;
	delete[] sumB;
	Invalidate();
	UpdateWindow();	
}

/*-----------------------code for lab6----------------------------------
tasks:
	1. Bilateral filtering
----------------------------------------------------------------------*/
void CChildView::OnHomeworkviBilateralfiltering()
{
	CWaitCursor wait;//�ں���ִ�й�����ʹ���ͼ���ɵȴ�ͼ��

	COLORREF pixel,pixel_temp;//����һ�����ص�ɫ����Ϣ
	CImage imgBF;
	int maxY = imgOriginal.GetHeight(), maxX = imgOriginal.GetWidth();//��imgOriginalȡ��ͼ��ĳ�maxX�͸�maxY
	imgBF.Create(maxX,maxY,imgOriginal.GetBPP(),0UL);

	int r = 5;
	int filterWindow = 2 * r + 1;
	double sigma_s = 100, sigma_r = 20;
	double gauss_color_coeff = -0.5/(sigma_r*sigma_r);
	double gauss_space_coeff = -0.5/(sigma_s*sigma_s);

	double r_matrix[256];  // similarity weight
	// compute similarity weight
	for (int i = 0; i < 256; i++)
	{
		r_matrix[i] = exp(i * i * gauss_color_coeff);
	}

	// spatial weight
	double** d_matrix = (double**)malloc(sizeof(double)*filterWindow);
	for(int i=0;i<filterWindow;++i)
	{
		d_matrix[i] = (double*)malloc(sizeof(double)*filterWindow);
	}
	// compute spatial weight
	for (int i = -r; i <= r; i++)
	{
		for (int j = -r; j <= r; j++)
		{
			int x = i + r;
			int y = j + r;

			d_matrix[x][y] = exp((i * i + j * j) * gauss_space_coeff);
		}
	}



	byte rrr,ggg,bbb;//r,g,bֵ
	byte r_t,g_t,b_t;

	// bilateral filter
	for (int i = 0; i < maxX; i++)
	{
		for (int j = 0; j < maxY; j++)
		{
			double weight_sum_r, weight_sum_g, weight_sum_b;
			double pixcel_sum_r, pixcel_sum_g, pixcel_sum_b;
			weight_sum_r = weight_sum_g = weight_sum_b = 0.0; 
			pixcel_sum_r = pixcel_sum_g = pixcel_sum_b = 0.0;

			pixel = imgOriginal.GetPixel(i,j);//ȡ��λ��(x,y)λ���ϵ����ص�ɫ����Ϣ
			rrr = GetRValue(pixel);//ȡ��pixel��rֵ
			ggg = GetGValue(pixel);//ȡ��pixel��gֵ
			bbb = GetBValue(pixel);//ȡ��pixel��bֵ

			int pixcel_dif_r, pixcel_dif_g, pixcel_dif_b;
			double weight_tmp_r, weight_tmp_g, weight_tmp_b;
			for (int m = -r; m <= r; m++)
			{
				for (int n = -r; n <= r; n++)
				{
					if (m*m + n*n > r*r) continue;

					int y_tmp = j + n;
					int x_tmp = i + m;
					if(x_tmp<0 || x_tmp>maxX-1 ||y_tmp<0 || y_tmp>maxY-1)
						continue;
					/*
					x_tmp = x_tmp < 0 ? 0 : x_tmp;
					x_tmp = x_tmp > maxX - 1 ? maxX - 1 : x_tmp;   // �߽紦��
					y_tmp = y_tmp < 0 ? 0 : y_tmp;
					y_tmp = y_tmp > maxY - 1 ? maxY - 1 : y_tmp;*/

					pixel_temp = imgOriginal.GetPixel(x_tmp,y_tmp);//ȡ�����ص�ɫ����Ϣ
					r_t = GetRValue(pixel_temp);//ȡ��pixel��rֵ
					g_t = GetGValue(pixel_temp);//ȡ��pixel��gֵ
					b_t = GetBValue(pixel_temp);//ȡ��pixel��bֵ

					pixcel_dif_r = (int)abs(rrr - r_t);
					pixcel_dif_g = (int)abs(ggg - g_t);
					pixcel_dif_b = (int)abs(bbb - b_t);
					weight_tmp_r = d_matrix[m + r][n + r] * r_matrix[pixcel_dif_r];  // ����Ȩ��
					weight_tmp_g = d_matrix[m + r][n + r] * r_matrix[pixcel_dif_g];  // ����Ȩ��
					weight_tmp_b = d_matrix[m + r][n + r] * r_matrix[pixcel_dif_b];  // ����Ȩ��

					pixcel_sum_r += r_t * weight_tmp_r;
					pixcel_sum_g += g_t * weight_tmp_g;
					pixcel_sum_b += b_t * weight_tmp_b;
					weight_sum_r += weight_tmp_r;
					weight_sum_g += weight_tmp_g;
					weight_sum_b += weight_tmp_b;
				}
			}
			pixcel_sum_r = pixcel_sum_r / weight_sum_r;
			pixcel_sum_g = pixcel_sum_g / weight_sum_g;
			pixcel_sum_b = pixcel_sum_b / weight_sum_b;
			imgBF.SetPixelRGB(i,j,(byte)pixcel_sum_r,(byte)pixcel_sum_g,(byte)pixcel_sum_b);  //divide before transfer to byte

		}
	}
	for(int i=0;i<filterWindow;++i)
	{
		free(d_matrix[i]);
	}
	free(d_matrix);
	ImageCopy(imgBF,imgOriginal);
	Invalidate();
	UpdateWindow();	
}


/*-----------------------code for lab6--------------------------------*/



/*-------------------------------------------------------------------------------------------------
���´����Ǳ�������ܰ������ֳɵĺ�������򿪡���ʾ���洢ͼ���ļ��Ĵ��룬ͬѧ�ǿ��Բ�ȥ�����о�
--------------------------------------------------------------------------------------------------*/

// CChildView�Ĺ��캯�������������Լ���Ϣӳ���(����ϸ��������ο�C++��MFC�ľ����鼮)

CChildView::CChildView()
{
	m_nImageSize = SIZE_NONE;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_OPENIMAGE, OnFileOpenimage)
	ON_COMMAND(ID_FILE_SAVEIMAGE, OnFileSaveImage)
	ON_COMMAND(ID_TOOLS_MAKEBW, OnToolsMakeBW)
	ON_COMMAND(ID_FILE_IMAGEINFO, OnImageInfo)
	ON_UPDATE_COMMAND_UI(ID_SIZE_HALF,OnUpdateSizeHalf)
	ON_UPDATE_COMMAND_UI(ID_SIZE_ORIGINAL,OnUpdateSizeOriginal)
	ON_UPDATE_COMMAND_UI(ID_SIZE_DOUBLE,OnUpdateSizeDouble)
	ON_UPDATE_COMMAND_UI(ID_SIZE_FILL,OnUpdateSizeFill)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEIMAGE,OnUpdateImage)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMAGEINFO,OnUpdateImage)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MAKEBW,OnUpdateImage)
	ON_COMMAND_RANGE(ID_SIZE_HALF,ID_SIZE_FILL,OnChangeSize)
	ON_COMMAND(ID_TOOLS_HOMEWORKIII, &CChildView::OnToolsHomeworkIII)
	ON_COMMAND(ID_TOOLS_HOMEWORKII, &CChildView::OnToolsHomeworkII)
	ON_COMMAND(ID_HOMEWORKII_EROSION, &CChildView::OnHomeworkiiErosion)
	ON_COMMAND(ID_HOMEWORKII_DELATION, &CChildView::OnHomeworkiiDelation)
	ON_COMMAND(ID_HOMEWORKII_OPENING, &CChildView::OnHomeworkiiOpening)
	ON_COMMAND(ID_HOMEWORKII_CLOSING, &CChildView::OnHomeworkiiClosing)
	ON_COMMAND(ID_HOMEWORKII_BINARIZATION, &CChildView::OnHomeworkiiBinarization)
	ON_COMMAND(ID_HOMEWORKIII_IMAGELOGARITHMIC, &CChildView::OnHomeworkiiiImagelogarithmic)
	ON_COMMAND(ID_HOMEWORKIII_HISTOGRAMEQUALIZATION, &CChildView::OnHomeworkiiiHistogramequalization)
	ON_COMMAND(ID_HOMEWORKIV_TRANSLATION, &CChildView::OnHomeworkivTranslation)
	ON_COMMAND(ID_HOMEWORKIV_ROTATION, &CChildView::OnHomeworkivRotation)
	ON_COMMAND(ID_HOMEWORKIV_SCALE, &CChildView::OnHomeworkivScale)
	ON_COMMAND(ID_SCALE_SAMERATIO, &CChildView::OnScaleSameratio)
	ON_COMMAND(ID_SCALE_DIFFERENTRATIO, &CChildView::OnScaleDifferentratio)
	ON_COMMAND(ID_HOMEWORKIV_SHEAR, &CChildView::OnHomeworkivShear)
	ON_COMMAND(ID_SHEAR_ONX, &CChildView::OnShearOnxaxis)
	ON_COMMAND(ID_SHEAR_ONYAXIS, &CChildView::OnShearOnyaxis)
	ON_COMMAND(ID_HOMEWORKIV_MIRROR, &CChildView::OnHomeworkivMirror)
	ON_COMMAND(ID_SCALE_SAMERATIOLARGER, &CChildView::OnScaleSameratiolarger)
	ON_COMMAND(ID_SCALE_DIFFERENTRATIOLARGER, &CChildView::OnScaleDifferentratiolarger)
	ON_COMMAND(ID_HOMEWORKV_MEANFILTERING, &CChildView::OnHomeworkvMeanfiltering)
	ON_COMMAND(ID_HOMEWORKV_LAPLACIAN, &CChildView::OnHomeworkvLaplacian)
	ON_COMMAND(ID_MEANFILTERING_SIMPLEMEAN, &CChildView::OnMeanfilteringSimplemean)
	ON_COMMAND(ID_MEANFILTERING_WEIGHTEDMEAN, &CChildView::OnMeanfilteringWeightedmean)


	ON_COMMAND(ID_LAPLACIANENHANCEMENT_LAPLACIANRESULT, &CChildView::OnLaplacianenhancementLaplacianresult)
	ON_COMMAND(ID_LAPLACIANENHANCEMENT_REARRANGEDLAPLACIANRESULT, &CChildView::OnLaplacianenhancementRearrangedlaplacianresult)
	ON_COMMAND(ID_LAPLACIANENHANCEMENT_AFTERFUSION, &CChildView::OnLaplacianenhancementAfterfusion)
	ON_COMMAND(ID_HOMEWORKVI_BILATERALFILTERING, &CChildView::OnHomeworkviBilateralfiltering)
END_MESSAGE_MAP()



// CChildView message handlers
// �����еĴ󲿷ְ�ť��Ӧ���ֳɵĺ���
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

//���δ�����ÿһ����Ϣѭ���б���������һ����С������ʾ��ǰͼƬ
void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (!imgOriginal.IsNull()) 
	{

		switch (m_nImageSize)
		{
			case SIZE_HALF:
				imgOriginal.StretchBlt(dc,0,0,imgOriginal.GetWidth()/2,imgOriginal.GetHeight()/2,SRCCOPY);
				break;
			case SIZE_ORIGINAL:
				imgOriginal.StretchBlt(dc,0,0,imgOriginal.GetWidth(),imgOriginal.GetHeight(),SRCCOPY);
				break;
			case SIZE_DOUBLE:
				imgOriginal.StretchBlt(dc,0,0,imgOriginal.GetWidth()*2,imgOriginal.GetHeight()*2,SRCCOPY);
				break;
			case SIZE_FILL:
				CRect rctWindowSize;
				GetClientRect(rctWindowSize);
				imgOriginal.StretchBlt(dc,0,0,rctWindowSize.Width(),rctWindowSize.Height(),SRCCOPY);
		};
	}

}

//���δ���ʵ�ִ��ļ��Ի���ѡ��һ��ͼƬ�ļ�����ʾ
void CChildView::OnFileOpenimage(void)
{
	// TODO: Add your command handler code here
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;

	hResult = imgOriginal.GetExporterFilterString(strFilter,aguidFileTypes);
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format("GetExporterFilter failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return;
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	dlg.m_ofn.nFilterIndex = m_nFilterLoad;
	hResult = (int)dlg.DoModal();
	if(FAILED(hResult)) {
		return;
	}

	m_nFilterLoad = dlg.m_ofn.nFilterIndex;
	imgOriginal.Destroy();
	hResult = imgOriginal.Load(dlg.GetPathName());
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format("Load image failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return;
	}

	m_nImageSize = SIZE_ORIGINAL;
	Invalidate();
	UpdateWindow();

}
//���δ���ʵ�ֽ���ǰ��ͼƬ�洢������
void CChildView::OnFileSaveImage(void) 
{
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;


	strFilter = "Bitmap image|*.bmp|JPEG image|*.jpg|GIF image|*.gif|PNG image|*.png||";

	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	dlg.m_ofn.nFilterIndex = m_nFilterLoad;
	hResult = (int)dlg.DoModal();
	if (FAILED(hResult)) {
		return;
	}

    // Add the appropriate extension if the user didn't type one

	CString strFileName;
	CString strExtension;

	strFileName = dlg.m_ofn.lpstrFile;


	// add the file extension if the user didn't supply one
	if (dlg.m_ofn.nFileExtension == 0) 
	{
		switch (dlg.m_ofn.nFilterIndex)
		{
		case 1:
			strExtension = "bmp";
			break;
		case 2:
			strExtension = "jpg";
			break;
		case 3:
			strExtension = "gif";
			break;
		case 4:
			strExtension = "png";
			break;
		default:
			break;
		}

		strFileName = strFileName + '.' + strExtension;

	}

	// the extension on the file name will determine the file type that is saved
	hResult = imgOriginal.Save(strFileName);
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format("Save image failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return;
	}

}



//���δ����ѯͼ�������Ϣ
void CChildView::OnImageInfo()
{
	CInfoDlg infoDlg(imgOriginal);
	infoDlg.DoModal();
}

//���δ���ı�ͼ����ʾ��С
void CChildView::OnChangeSize(UINT nID)
{
	m_nImageSize = (SizesEnum)(nID - ID_SIZE_BASE);
	Invalidate();
	UpdateWindow();
}

//�����Ķδ����Բ�ͬ�Ĵ�С��ʾͼƬ
void CChildView::OnUpdateSizeHalf(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_HALF));
}

void CChildView::OnUpdateSizeOriginal(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_ORIGINAL));
}

void CChildView::OnUpdateSizeDouble(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_DOUBLE));
}

void CChildView::OnUpdateSizeFill(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
	pCmdUI->SetCheck((UINT)(m_nImageSize == SIZE_FILL));
}

void CChildView::OnUpdateImage(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!imgOriginal.IsNull());
}



















