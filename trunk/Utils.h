#pragma once
#include "cv.h"
#include "highgui.h"
#include "Params.h"
#include "vector"

#include "HoGProcessor.h"

class Utils
{
public:
	bool CheckRectHead(CvRect rectHead, int iMaxHeight, Head_Params headParams);
	vector<CvRect> ConnectOverlapRects(vector<CvRect> inVector);
	void OutputResult(IplImage* img, vector<CvRect> vRect, CvScalar color);

	Utils(void);
	~Utils(void);
};
