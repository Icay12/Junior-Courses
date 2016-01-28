#include <stdio.h>
#include <string>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <math.h>
#include <iostream>

#define PI 3.1415

#define BLOCK_SIZE 5

#define MAX_CORNERS 100000

using namespace std;
using namespace cv;

char *wdSource = "Source";			// name of the app window
char *wdResult = "Result";		    // name of the window that is used for debuging
char *wdMin = "Min";
char *wdMax = "Max";
char *wdR = "R";

//char *tbThreshName = "Threshold";	// name of the tab slider
//char *tbDistanceName = "Distance";	// name of the distance tab slider

//int count = 0;			/* number of corners found */
int Q_size = 5;			/* the size of N that is used in the neighborhood Q */
float K = 0.04;
int APETURE_SIZE = 3;
int EIGENVALUE_THRESHOLD = 200000;
int max_thresh = 320000;
// Load the source image. HighGUI use.
IplImage *image = 0, *harris_responce = 0, *gray=0;
IplImage *min_image=0, *max_image=0, *R_image=0;

// An array of points representing the found corners
CvPoint3D32f *corners;


//----------------------- Compute functions -----------------------//

void computeDerivatives(CvMat* dx1, CvMat* dy1)
{
	CvSize src_size = cvGetSize (gray);

	// create temporary images to store the two image derivatives
	CvMat *dx = cvCreateMat (src_size.height, src_size.width, CV_16SC1);
    CvMat *dy = cvCreateMat (src_size.height, src_size.width, CV_16SC1);

    cvSobel (gray, dx, 1, 0, APETURE_SIZE);
    cvSobel (gray, dy, 0, 1, APETURE_SIZE);

	cvConvertScale( dx, dx1 );
	cvConvertScale( dy, dy1 );	
	cvReleaseMat( &dx );
    cvReleaseMat( &dy );
}
int computeLambdaAndR(CvMat* dx1, CvMat* dy1, CvMat* LMin, CvMat* LMax, CvMat* R)
{
	//Go through all pixels
	int count = 0;
	int localSize = 3;
	CvMat *M = cvCreateMat(2, 2, CV_64FC1);
	for (int x = 0; x < image->height; ++x)
	{
		for (int y = 0; y < image->width; ++y)
		{
			int Ix2 = 0;
			int Iy2 = 0;
			int Ixy = 0;
			for (int i = x - floor((double)BLOCK_SIZE/2); i <= x + floor((double)BLOCK_SIZE/2); ++i)
			{
				for (int j = y - floor((double)BLOCK_SIZE/2); j <= y + floor((double)BLOCK_SIZE/2); ++j)
				{
					if (i < 0 || j < 0 || i >= image->height || j >= image->width)
						continue;
					//float weight = 1/sqrt(2*PI)*exp(-0.5*((x-i)*(x-i)+(y-j)*(y-j)));
					Ix2 += pow(cvmGet(dx1, i, j), 2);//*weight;
					Iy2 += pow(cvmGet(dy1, i, j), 2);//*weight;
					Ixy += cvmGet(dx1, i, j) * cvmGet(dy1, i, j);//*weight;
				}
			}
			
			
			cvmSet(M, 0, 0, Ix2);
			cvmSet(M, 0, 1, Ixy);
			cvmSet(M, 1, 0, Ixy);
			cvmSet(M, 1, 1, Iy2);
			CvMat *vecs = cvCreateMat(2, 2, CV_64FC1);
			CvMat *vals = cvCreateMat(1, 2, CV_64FC1);
			cvEigenVV(M, vecs, vals, -1 , -1);

			double det = (cvmGet(vals, 0, 0) * cvmGet(vals, 0, 1));
			double trace = (cvmGet(vals, 0, 0) + cvmGet(vals, 0, 1));
			double Ri = (det - K * pow(trace, 2));

			//set matrix LMin, LMax, and R
			cvmSet(R,x,y,Ri);
			if(cvmGet(vals, 0, 0) > cvmGet(vals, 0, 1))
			{
				cvmSet(LMin,x,y,cvmGet(vals, 0, 1));
				cvmSet(LMax,x,y,cvmGet(vals, 0, 0));
			}
			else
			{
				cvmSet(LMin,x,y,cvmGet(vals, 0, 0));
				cvmSet(LMax,x,y,cvmGet(vals, 0, 1));
			}

			if (cvmGet(vals, 0, 0) > EIGENVALUE_THRESHOLD && cvmGet(vals, 0, 1) > EIGENVALUE_THRESHOLD && Ri >= EIGENVALUE_THRESHOLD)
			{
				CvPoint3D32f point;
				point.x = x;
				point.y = y;
				point.z = 0;
				corners[count++] = point; 
			}

			cvReleaseMat( &vecs );
			cvReleaseMat( &vals );
		}
	}
	// for(int x=localSize/2;x < R->height; x+= localSize/2)
	// {
	// 	for(int y=localSize/2;y < R->width; y+= localSize/2)
	// 	{
	// 		int localMax = cvmGet(R,x,y);
	// 		int localX = x;
	// 		int localY = y;
	// 		for(int i=-localSize/2;i<=localSize/2;++i)
	// 		{
	// 			if((x+i)>=R->height)
	// 				break;
	// 			for(int j=-localSize/2;j<=localSize/2;++j)
	// 			{
	// 				if((y+j)>=R->width)
	// 					break;
	// 				int tempR = cvmGet(R,x+i,y+j);
	// 				if(localMax<tempR)
	// 				{
	// 					localMax = tempR;
	// 					localX = x+i;
	// 					localY = y+j;
	// 				}
	// 			}
	// 		}
	// 		if(localMax > EIGENVALUE_THRESHOLD)
	// 		{
	// 			CvPoint3D32f point;
	// 			point.x = localX;
	// 			point.y = localY;
	// 			point.z = 0;
	// 			corners[count++] = point; 
	// 		}

	// 	}
	// }
	cvReleaseMat( &M );

	//printf("Corner count: %d", count);
	return count;
}

//----------------------- Image painting functions -----------------------//


/* Displays the found corners as points */
void drawCorners (IplImage *img, CvPoint3D32f *corners, int count) {
	/* Displays the found corners as points */
    int i;

    CvScalar color = cvScalar(200,0,0,0);

    for(i = 0; i < count; i++){
		CvPoint pt = cvPoint(corners[i].y, corners[i].x);
		cvCircle(img, pt, 1, color, 1, 8, 0);
	}

	return;
}

void drawMin(CvMat* LMin)
{
	cvNormalize(LMin,LMin,255.0,0.0);
	cvShowImage (wdMin, LMin);
}
void drawMax(CvMat* LMax)
{
	cvNormalize(LMax,LMax,255.0,0.0);
	cvShowImage (wdMax, LMax);
}
void drawR(CvMat* R)
{
	cvNormalize(R,R,255.0,0.0);
	cvShowImage (wdR, R);
}
//----------------------- Harris function -----------------------//
void Harris( )
{
	int count;
	CvSize src_size = cvGetSize (gray);
	CvMat *dx1 = cvCreateMat(src_size.height, src_size.width, CV_64FC1);
	CvMat *dy1 = cvCreateMat(src_size.height, src_size.width, CV_64FC1);
	CvMat *LMin = cvCreateMat(src_size.height, src_size.width, CV_64FC1);
	CvMat *LMax = cvCreateMat(src_size.height, src_size.width, CV_64FC1);
	CvMat *R = cvCreateMat(src_size.height, src_size.width, CV_64FC1);

    computeDerivatives(dx1,dy1);
	// Setup the buffers
    harris_responce = cvCloneImage (image);
	// This array will store all corners found in the image
	corners = (CvPoint3D32f*)cvAlloc (MAX_CORNERS * sizeof (corners));
    // Draw all corners
	count = computeLambdaAndR(dx1,dy1,LMin,LMax,R);

	// Display the images
	//createTrackbar( "Threshold: ", wdSource, &EIGENVALUE_THRESHOLD, max_thresh, computeLambdaAndR );
	drawCorners (harris_responce, corners, count);
    drawMin(LMin);
    drawMax(LMax);
    drawR(R);
	cvShowImage (wdSource, image);
	cvShowImage (wdResult, harris_responce);

	//cvSaveImage("Corner-out.jpg", harris_responce);

	// Release the buffers

	cvReleaseMat( &dx1 );
	cvReleaseMat( &dy1 );
	cvReleaseImage (&harris_responce);
}

//----------------------- Application main function -----------------------//

int main( int argc, char** argv )
{

    //initialize parameters
    if(argc < 2 || argc > 4)
    {
        cout << "Wrong Form!" << endl;
    }
    char* filename = argv[1];
    if(argc > 2)
    {
        K = atof(argv[2]);
    }
    if(argc > 3)
    {
        APETURE_SIZE = atoi(argv[3]);
    }

	// Load the image from the file
	if ((image = cvLoadImage (filename,1)) == 0)
	{
		cout << "can't load image" <<endl;
		return -1;
	}

    // Convert to grayscale
    gray= cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
    cvCvtColor(image, gray, CV_BGR2GRAY);

    // Create windows.
    cvNamedWindow(wdSource, 1);
    cvNamedWindow(wdResult, 1);
	cvNamedWindow(wdMin, 1);
	cvNamedWindow(wdMax, 1);
	cvNamedWindow(wdR, 1);

	// Get all corners
    Harris();


    // Wait for a key stroke
    cvWaitKey(0);
    cvReleaseImage(&image);
    cvReleaseImage(&harris_responce);
 
    cvDestroyWindow(wdSource);
    cvDestroyWindow(wdResult);
    cvDestroyWindow(wdMin);
    cvDestroyWindow(wdMax);
    cvDestroyWindow(wdR);
    return 0;
}