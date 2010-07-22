#pragma once
#include "snakewindow.h"
#include "cv.h"
//#include "snake.h"
#include "tinyxml.h"
#include "tinystr.h"

class SnakeDetector
{
public:
	SnakeWindow *bigw;
	SnakeWindow *mediumw;
	SnakeWindow *smallw;
	float threshold;
	int l;					// length, number of vector_space
	float delta;

public:
	SnakeDetector(void);
	SnakeDetector(const char *config);
	Snake *GetSnake(IplImage *gray, int distance, CvPoint location, CvRect rect);
	~SnakeDetector(void);
};
