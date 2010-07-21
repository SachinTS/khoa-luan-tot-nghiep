#pragma once

#include "vector"
#include "snake.h"
#include "stdafx.h"

using namespace std;

class VectorSpace
{
public:
	Snake *origin_snake;
	vector<Snake*> snakes;
public:
	VectorSpace(void);
	VectorSpace(Snake *init);
	void GenerateSnake();
	void DrawSnakes(IplImage *img);
	~VectorSpace(void);
};
