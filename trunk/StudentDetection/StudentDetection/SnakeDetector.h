#pragma once
#include "snakewindow.h"
#include "tinyxml.h"
#include "tinystr.h"

class SnakeDetector
{
public:
	SnakeWindow *big, *medium, *small;
	float threshold;
	int l;					// length, number of vector_space
	float delta;

public:
	SnakeDetector(void);
	SnakeDetector(const char *config);
	~SnakeDetector(void);
};
