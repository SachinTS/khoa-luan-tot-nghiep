#include "StdAfx.h"
#include "snakewindow.h"

SnakeWindow::SnakeWindow(const char *sData) {
	FILE *data = fopen(sData, "r");

	int n, m;
	fscanf(data, "%d %d\n", &n, &m);

	this->n = n;
	space = new VectorSpace[n];
	for (int i = 0; i < n; i++) {
		Snake *s = new Snake(data, m);
		space[i].origin_snake = s;
		space[i].GenerateSnake();
		fscanf(data, "\n");
	}
	this->l = 5;
	this->threshold = 0.15f;
	this->delta = 4;
	fclose(data);
}

SnakeWindow::SnakeWindow(const char *sData, float threshold, float delta, int l) {
	FILE *data = fopen(sData, "r");

	int n, m;
	fscanf(data, "%d %d\n", &n, &m);

	this->n = n;
	space = new VectorSpace[n];
	for (int i = 0; i < n; i++) {
		Snake *s = new Snake(data, m);
		space[i].origin_snake = s;
		space[i].GenerateSnake();
		fscanf(data, "\n");
	}
	this->l = l;
	this->threshold = threshold;
	this->delta = delta;
	fclose(data);
}


SnakeWindow::SnakeWindow(FILE *data) {
	int n, m;
	fscanf(data, "%d %d\n", &n, &m);

	this->n = n;
	space = new VectorSpace[n];
	for (int i = 0; i < n; i++) {
		Snake *s = new Snake(data, m);
		space[i].origin_snake = s;
		space[i].GenerateSnake();
		fscanf(data, "\n");
	}
	this->l = 5;
	this->threshold = 0.15f;
	this->delta = 4;
}

SnakeWindow::~SnakeWindow(void)
{
	if (space != NULL) {
		delete[] space;
	}
}

Snake *SnakeWindow::GetSnake(IplImage *image, IplImage *edge, const CvPoint& location) {
	vector<Snake*>::iterator it;
	double p, max = -1;
	int t=-1;
	Snake *fit_snake = NULL;
	for (int i = 0; i < this->n; i++) {
		for (it = space[i].snakes.begin();
			it != space[i].snakes.end();
			it++) {
			p = (*it)->Likelihood(edge, l, delta, location);
			if (p > max && p > threshold) {
				t = i;
				max = p;
				fit_snake = (*it);
			}
		}
	}

	return fit_snake;
}

Snake *SnakeWindow::GetSnake(IplImage *image, IplImage *edge, const CvPoint& location, const CvRect& bounding_rect) {
	vector<Snake*>::iterator it;
	double p, max = -1;
	Snake *fit_snake = NULL;


	for (int i = 0; i < this->n; i++) {
		for (it = space[i].snakes.begin();
				it != space[i].snakes.end();
				it++) {
			p = (*it)->Likelihood(edge, l, delta, location, bounding_rect);
			if (p > max && p > threshold) {
				max = p;
				fit_snake = (*it);
			}
		}
	}


	return fit_snake;
}

Snake *SnakeWindow::GetSnake(IplImage *edge, const CvPoint& location, const CvRect& bounding_rect) {
	vector<Snake*>::iterator it;
	double p, max = -1;
	Snake *fit_snake = NULL;


	for (int i = 0; i < this->n; i++) {
		for (it = space[i].snakes.begin();
				it != space[i].snakes.end();
				it++) {
			p = (*it)->Likelihood(edge, l, delta, location, bounding_rect);
			if (p > max && p > threshold) {
				max = p;
				fit_snake = (*it);
			}
		}
	}


	return fit_snake;
}