#include "myPCA.h"

myPCA::myPCA()
{}

myPCA::myPCA(const Mat& sampleMatrix, const Mat& sampleMatrixOri, vector<int>&trainImageID, double a)
{
	this->sampleMatrix = sampleMatrix;
	this->sampleMatrixOri = sampleMatrixOri;
	Mat ID(1, trainImageID.size(), CV_16SC1);
	vector<int>::iterator it = trainImageID.begin();
	int i = 0;
	for(; it != trainImageID.end(); ++it)
	{
		ID.at<short>(0,i++) = (int)*it;
	}
	this->trainImageID = ID;
	this->a = a < 1 ? a : 0.99;
}

Mat myPCA::getMean()
{
	Mat meanFace(sampleMatrix.rows, 1, CV_32FC1);
	for(int i = 0; i < sampleMatrix.cols; ++i)
	{
		Mat tmp = sampleMatrix.col(i);
		meanFace = meanFace + tmp;
	}
	meanFace /= sampleMatrix.cols;
	this->meanFace = meanFace;
	return meanFace;
}

Mat myPCA::getEigenVectors()
{
	//get difference
	Mat diffFace(sampleMatrix.size(), CV_32FC1);
	for(int i = 0; i < sampleMatrix.cols; ++i)
	{
		Mat temp1 = diffFace.col(i);
		Mat temp2 = sampleMatrix.col(i) - meanFace;
		temp2.copyTo(temp1);
	}
	//get covariance matrix
	//AA'is too large, comput A'A instead
	Mat dt = diffFace.t();
	Mat covMatrix = dt * diffFace;
	
	SVD isvd(diffFace,SVD::FULL_UV);
	Mat U = isvd.u;
	Mat W = isvd.w;
	
	/*
	Mat W;
	Mat U;
	eigen(covMatrix,W,U);*/

	double sum = 0;
	int i;
	for(i = 1; i < W.rows; ++i)
	{
		float t = W.at<float>(i,0)* W.at<float>(i,0);
		//cout << t << endl;
		sum += t;
	}
	sum *= this->a;

	i = 1;
	double x = 0;
	while(x < sum)
	{
		float t = W.at<float>(i,0)* W.at<float>(i,0);
		x += t;
		++i;
	}

	//number of eigen faces
	int p = i - 1;
	
	if(p > 80)
		p = 80;
	Mat eigenVectors(sampleMatrix.rows, p, CV_32FC1);
	for(int i = 0; i < p; ++i)
	{
		Mat temp1 = eigenVectors.col(i);
		Mat temp2 = U.col(i);//diffFace * U.col(i) / sqrt(W.at<float>(i,0));
		temp2.copyTo(temp1);
	}

	this->eigenVectors = eigenVectors;
	return eigenVectors;
}

Mat myPCA::projection()
{
	Mat diffFace(sampleMatrix.size(), CV_32FC1);
	for(int i = 0; i < sampleMatrix.cols; ++i)
	{
		Mat temp1 = diffFace.col(i);
		Mat temp2 = sampleMatrix.col(i) - meanFace;
		temp2.copyTo(temp1);
	}

	Mat projFaces = eigenVectors.t() * diffFace;
	this->projFaces = projFaces;
	return projFaces;
}

// Scale every value in a matrix so that it is in the 0-255 range.
Mat myPCA::toGrayscale(const Mat& mat)
{
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

void myPCA::show10EigenFaces()
{
	Mat all;
	
	for (int i =0; i < min(10, eigenVectors.cols); i++)
	{
		Mat imgFace = eigenVectors.col(i).clone();
		if(i == 0)
			all = imgFace.clone();
		else
			all  = all + imgFace;
	}
	all.reshape(1, ROW).copyTo(all);
	all = toGrayscale(all);
	resize(all,all,Size(COL*4,ROW*4),0,0,1);
	imshow("Top 10",all);
	waitKey(0);
}

//Because of Unknow Error, so I have to use 6 XML files to save those matrixs
void myPCA::saveModel()
{
	FileStorage fs1("mean.xml", FileStorage::WRITE);
	FileStorage fs2("sample.xml", FileStorage::WRITE);
	FileStorage fs3("eigen.xml", FileStorage::WRITE);
	FileStorage fs4("proj.xml", FileStorage::WRITE);
	FileStorage fs5("ID.xml", FileStorage::WRITE);
	FileStorage fs6("sampleOri.xml", FileStorage::WRITE);

	fs1 << "MeanFace" << meanFace;
	fs2 << "SampleMatrix" << sampleMatrix ;
	fs3 << "EigenVectors" << eigenVectors;
	fs4 << "ProjFaces" << projFaces;
	fs5 << "TrainImageID" << trainImageID;
	fs6 << "SampleMatrixOri" << sampleMatrixOri ;
}

void myPCA::loadModel()
{
	FileStorage fs1("mean.xml", FileStorage::READ);
	FileStorage fs2("sample.xml", FileStorage::READ);
	FileStorage fs3("eigen.xml", FileStorage::READ);
	FileStorage fs4("proj.xml", FileStorage::READ);
	FileStorage fs5("ID.xml", FileStorage::READ);
	FileStorage fs6("sampleOri.xml", FileStorage::READ);

	fs1["MeanFace"] >> meanFace;
	fs2["SampleMatrix"] >> sampleMatrix ;
	fs3["EigenVectors"] >> eigenVectors;
	fs4["ProjFaces"] >> projFaces;
	fs5["TrainImageID"]  >> trainImageID;
	fs6["SampleMatrixOri"] >> sampleMatrixOri ;
		
}

Mat myPCA::recogProject(Mat& testFace)
{
	Mat tmp = testFace - meanFace;
	Mat result = eigenVectors.t() * tmp;
	return result;
}

double myPCA::recogComputeThreshold()
{
	double max = 0;
	for(int i = 0; i < projFaces.cols; ++i)
	{
		for(int j = i+1; j < projFaces.cols; ++j)
		{
			double temp = norm(projFaces.col(i),projFaces.col(j),NORM_L2);
			if(max < temp)
			{
				max = temp;
			}
		}
	}
	return max;
	
}

Mat myPCA::recogComputeEachFaceDis(Mat& testFaceProj)
{
	Mat dis(1,projFaces.cols,CV_32FC1);
	for(int i = 0; i < projFaces.cols; ++i)
	{
		double x = norm(testFaceProj, projFaces.col(i), NORM_L2);
		dis.at<float>(0,i) = (float)x;
	}
	return dis;
}

double myPCA::recogComputeReconsDis(Mat& testFace, Mat& testFaceProj)
{
	Mat recons = eigenVectors * testFaceProj + meanFace;
	double dis = norm(recons, testFace,NORM_L2);
	return dis;
}

void myPCA::recoginize(double dis, double threshold, Mat& disMatrix)
{
	float min = disMatrix.at<float>(0,0);
	int minCol = 0;
	int flagKnow = 1;
	int flagUnknow = 1;
	int i;
	for(i = 0; i < disMatrix.cols; ++i)
	{
		float x = disMatrix.at<float>(0,i);
		if(x < threshold)
		{
			flagUnknow = 0;
			if(min > x)
			{
				min = x;
				minCol = i;
			}
		}
		else
		{
			flagKnow = 0;
		}
		if(flagKnow + flagUnknow == 0)
		{
			cout << "No I don't know this guy!" << endl;
			break;
		}
	}
	if(flagKnow)
	{
		cout << "Yes I know this guy!" <<endl;
		cout << "It's the "<<trainImageID.at<short>(0,minCol) << "th person in the database.";
		Mat imgFace = sampleMatrixOri.col(minCol).clone();
		Mat toShow;
		imgFace.reshape(1,ROWDB).copyTo(toShow);
		toShow = toGrayscale(toShow);

		imshow("recognize",toShow);
		waitKey(0);
	}
	if(flagUnknow)
	{
		cout << "Sorry I think it's not a human face" <<endl;
	}
}

