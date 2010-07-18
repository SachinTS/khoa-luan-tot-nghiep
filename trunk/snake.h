#pragma once

#include "vector"
#include "cv.h"
#include "stdafx.h"

using namespace std;

class Snake
{
public:
	CvPoint center;
	float step;
	vector<CvPoint> templ_points;
	vector<CvPoint> ctrl_points;
	CvMat *ctrl_mat;
	int N, M;
	static CvMat *Global_B;
	static CvMat *Global_B_Step;
	float epsilon;

	float B1(float u);
	float B2(float u);
	float B3(float u);
	float B4(float u);
	float B11(float u);		// dao ham bac 1 cua B1
	float B21(float u);
	float B31(float u);
	float B41(float u);
	CvMat *BuildingGlobalMatrix();				// dung de tinh diem
	CvMat *BuildingGlobalMatrixWithStep();		// dung de ve
	CvMat *CurveCalculate();


public:
	Snake(FILE *data);
	Snake(FILE *data, int m);
	Snake(vector<CvPoint> templ_points);
	Snake(vector<CvPoint> ctrl_points, bool pad);
	CvMat *DrawCurve(IplImage *img);
	CvMat *DrawCurve(IplImage *image, CvPoint location);
	double Likelihood(IplImage *edge, int l, float delta, const CvPoint& location);
	double Likelihood(IplImage *edge, int l, float delta, const CvPoint& location, const CvRect& bounding_rect);
	~Snake(void);
};