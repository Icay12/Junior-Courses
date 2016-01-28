#include <stdio.h>
#include <string>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include "myPCA.h"

using namespace std;
using namespace cv;

// Scale every value in a matrix so that it is in the 0-255 range.
Mat toGrayscale(const Mat& mat) {
	Mat gMat(mat.rows, mat.cols, CV_8UC1);

	double min, max;
	minMaxLoc(mat, &min, &max);

	for(int row = 0; row < mat.rows; row++) {
		for(int col = 0; col < mat.cols; col++) {
			gMat.at<uchar>(row, col) = 255 * ((mat.at<float>(row, col) - min) / (max - min));
		}
	}

	return gMat;
}

// Test lists consist of the subject ID of the person of whom a face image is and the
// path to that image.
void readFileName(const string& fileName, vector<string>& files, vector<int>& indexNum)
{
	std::ifstream file(fileName.c_str(), ifstream::in);

	if(!file) {
		cerr << "Unable to open file: " << fileName << endl;
		exit(0);
	}

	string line, path, trueSubjectID;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, trueSubjectID, ';');
		getline(liness, path);

		path.erase(remove(path.begin(), path.end(), '\r'), path.end());
		path.erase(remove(path.begin(), path.end(), '\n'), path.end());
		path.erase(remove(path.begin(), path.end(), ' '), path.end());

		files.push_back(path);
		indexNum.push_back(atoi(trueSubjectID.c_str()));
	}
}

void loadFile(vector<string>& files, Mat& sampleMatrix, int imgSize)
{
	for(int i = 0; i < files.size(); ++i)
	{
		Mat tmpImg;
		Mat tmp = sampleMatrix.col(i);
		Mat readin = imread(files[i], 0);
		resize(readin, readin, Size(COL,ROW),0,0,1);
		readin.convertTo(tmpImg, CV_32FC1);
		tmpImg.reshape(1, imgSize).copyTo(tmp);
	}
}


int main( int argc, char** argv )
{
	Mat testFace;

	Mat tmpImg;
	imread(argv[1], 0).convertTo(tmpImg, CV_32FC1);
	myPCA ipca;
	ipca.loadModel();
	resize(tmpImg, tmpImg, Size(COL,ROW),0,0,1);

	tmpImg.reshape(1, COL*ROW).copyTo(testFace);

	Mat testFaceProj;
	testFaceProj = ipca.recogProject(testFace);
	double threshold = ipca.recogComputeThreshold();

	Mat disMatrix;
	disMatrix = ipca.recogComputeEachFaceDis(testFaceProj);

	double dis;
	dis = ipca.recogComputeReconsDis(testFace, testFaceProj);

	ipca.recoginize(dis, threshold, disMatrix);
	//testFace.push_back
	
}