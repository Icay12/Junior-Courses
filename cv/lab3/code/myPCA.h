#ifndef _myPCA_H_
#define _myPCA_H_

#include <iostream>
#include <vector>
#include <float.h>
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include "opencv/highgui.h"

#define ROW 28
#define COL 23
#define ROWDB 112

using namespace std;
using namespace cv;

class myPCA {

public:
	myPCA();
	myPCA(const Mat& sampleMatrix, const Mat& sampleMatrixOri, vector<int>&trainImageID,double a);
	~myPCA(){}
	//train
	Mat getMean();
	Mat getEigenVectors();
	Mat projection();
	Mat toGrayscale(const Mat& mat);
	void show10EigenFaces();
	void saveModel();

	//test
	void loadModel();
	Mat recogProject(Mat& testFace);
	double recogComputeThreshold();
	Mat recogComputeEachFaceDis(Mat& testFaceProj);
	double recogComputeReconsDis(Mat& testFace, Mat& testFaceProj);
	void recoginize(double dis, double threshold, Mat& disMatrix);


private:
	double a;
	double threshold;
	Mat meanFace;
	Mat sampleMatrix;
	Mat sampleMatrixOri;
	Mat eigenVectors;
	//Mat eigenValues;
	Mat projFaces;
	Mat trainImageID;	
};

#endif