#include "highgui.h"
#include <opencv2/opencv.hpp>
#include <io.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace cv;

vector<string> picName;
vector<string> videoName;

// <io.h> 中的读取文件函数
bool get_filelist_from_dir(string path, vector<string>& files)
{
    long   hFile   =   0;
    struct _finddata_t fileinfo;
    files.clear();
    if((hFile = _findfirst(path.c_str(), &fileinfo)) !=  -1)
    {
        do
        {
            if(!(fileinfo.attrib &  _A_SUBDIR))
                files.push_back(fileinfo.name);
        }while(_findnext(hFile, &fileinfo)  == 0);
        _findclose(hFile);
        return true;
    }
    else
        return false;
}

//从给定路径中读取avi视频和所有的jpg文件（假设有一个avi和多个jpg）
void getFileNames(string path)
{
    string searchJPG = path + "\\*." + "jpg";
	cvLoadImage(searchJPG.c_str(),1);
    if(!get_filelist_from_dir(searchJPG,picName))
    {
        cout << "open JPG file error!" <<endl;
        return;
    }
    string searchAVI = path + "\\*." + "avi";
    if(!get_filelist_from_dir(searchAVI,videoName))
    {
        cout << "open AVI file error!" <<endl;
        return;
    }
}

//加字幕函数
void makeSubtitle(IplImage* img)
{
    int i_height = img->height;
    int i_width = img->width;
    char i_text[50] = "Xie Ke: 3130102281";
    int textSize = sizeof(i_text);

    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX, 0.5f,0.5f, 0.5, 0);
	CvPoint point = cvPoint( i_width/3 , i_height*27/28);
    cvPutText(img, i_text, point, &font,CV_RGB(87, 92, 159) );  
}


int main( int argc, char** argv ) { 

	//string path = "e:\\files";   //used for debug
    string path = argv[1];
	getFileNames(path);
	string ivideo =path + "\\"+ videoName[0];
    //open video
	CvCapture* capture = cvCaptureFromAVI( ivideo.c_str() );

    //get info of the video
    int framecount = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);
    int frameH = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
    int frameW = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
    //由于我下载的文件fps过小，不受支持，故暂定fps为25
    //int framefps = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FPS); 
	int framefps = 25;

    int isColor = 1;
    CvVideoWriter *writer = 0;
    writer=cvCreateVideoWriter("newvideo.avi",CV_FOURCC('P','I','M','1'), framefps,cvSize(frameW,frameH),isColor);
	int piccount = picName.size();
    IplImage* frame;


    IplImage* img = 0;
	
    //写入片头图片
	for(int i=0;i<piccount; i++)
	{
		img = cvLoadImage( (path+"\\"+picName[i]).c_str(),isColor);
        //设置图片大小
		IplImage *imageresize = 0;
		imageresize = cvCreateImage(cvSize(frameW,frameH),img->depth,img->nChannels);
		cvResize(img,imageresize,CV_INTER_LINEAR);
        //插入字幕
		makeSubtitle(imageresize);
        //每张图片show time设置为1s，即插入fps帧图片
		for(int j=0; j<framefps; j++)
		{
			cvWriteFrame(writer,imageresize);
		}
		cvReleaseImage(&imageresize);
	}
    //写入原视频
    for(int i=0;i<framecount-7;i++) 
    {
        img=cvQueryFrame(capture); // retrieve the captured frame 
		makeSubtitle(img);
        cvWriteFrame(writer,img); // add the frame to the file 
    }
    cvReleaseCapture( &capture );
	cvReleaseVideoWriter(&writer);

}

