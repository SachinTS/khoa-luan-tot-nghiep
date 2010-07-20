#pragma once
#include "cv.h"
#include "highgui.h"

class ImageProcessor
{
public:
	ImageProcessor(void);
	IplImage* getSubImageAndResize(IplImage* img,CvRect rect,int new_width,int new_height);
public:
	~ImageProcessor(void);
};
