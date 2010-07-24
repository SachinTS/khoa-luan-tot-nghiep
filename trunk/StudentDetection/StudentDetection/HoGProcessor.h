#pragma once
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
#include "ml.h"
#include "iostream"
#include "fstream"
using namespace std;
#define PI 3.14

class HoGProcessor
{
	CvRect HoGProcessor::MergeRect(vector<CvRect> lstRect);
public:	
	CvSize m_cell;
	CvSize m_block;
	float m_fStepOverlap;
				
	HoGProcessor(void);
	void SetParams(CvSize cell, CvSize block, float stepOverLap);
	void writeFeatureVector(CvMat* mat,char* className,ofstream &fout);
	IplImage** calculateIntegralHOG(IplImage* in);
	void calulateHOG_rect(CvRect cell, CvMat* hog_cell, IplImage** integrals,
		int normalization);
	IplImage* doSobel(IplImage* in,int xorder, int yorder, int aperture_size);

	CvMat* train_64x128(char *prefix, char *suffix, 
       CvSize window, int number_samples, int start_index,
       int end_index, char *savexml = NULL, int canny = 0,
       int block = 1, int normalization = 4);

	CvMat* train_large(char *prefix, char *suffix,
       CvSize window, int number_images,
       int horizontal_scans, int vertical_scans,
       int start_index, int end_index,
       char *savexml = NULL, int normalization = 4);

	CvSVM* trainSVM(CvMat* pos_mat, CvMat* neg_mat, char *savexml,
              char *pos_file = NULL, char *neg_file = NULL);
	
	void calculateHOG_block(CvRect block, CvMat* hog_block, 
	IplImage** integrals,CvSize cell, int normalization);
		
	CvRect detectObject(CvSVM *svmModel, IplImage *input, IplImage* result, CvRect rectHead, int normalization = 4);
	CvRect detectObject(CvSVM *svmModel, IplImage *input, IplImage *result, CvRect rectHead, float confidenceScore, int normalization);
	CvMat* calculateHOG_window(IplImage** integrals, CvRect window, int normalization) ;
public:
	~HoGProcessor(void);
};
