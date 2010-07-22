#pragma once

#include "cv.h"
#include "vector_space.h"
#include "vector"

using namespace std;

class SnakeWindow
{
public:
	VectorSpace *space;
	float threshold;
	int l, n;					// length, number of vector_space
	float delta;

public:
	SnakeWindow(const char *sData);
	SnakeWindow(const char *sData, float threshold, float delta, int l);
	SnakeWindow(FILE *data);
	Snake *GetSnake(IplImage *image, IplImage *edge, const CvPoint& location);
	Snake *GetSnake(IplImage *image, IplImage *edge, const CvPoint& location, const CvRect& bounding_rect);
	Snake *GetSnake(IplImage *edge, const CvPoint& location, const CvRect& bounding_rect);
	~SnakeWindow(void);
};
