#include "cv.h"
#include "highgui.h"
#include <fstream>
using namespace std;

#define PI 3.14

#pragma once

class GaussFilterColor
{
public:	
	double phi;
	double logOfSqrt2PiVariance;
	double threshold;

	CvMat* mean;
	CvMat* corrvariant;

	void GaussFilterColor::TrainData(char *prefix, char *suffix, int number_images, int start_index, int end_index, char* fileOutput);
	void LoadData(char* fileOutput);
	IplImage* Classify(IplImage *imgColor, IplImage *mask);
	IplImage* Classify(IplImage *imgColor);
	void SetThreshold(double threshold);

	GaussFilterColor(void);
	~GaussFilterColor(void);
};
