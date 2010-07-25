#include "StdAfx.h"
#include "vector_space.h"

VectorSpace::VectorSpace(void) {
}

VectorSpace::VectorSpace(Snake *init)
{
	this->origin_snake = init;
}

VectorSpace::~VectorSpace(void)
{
	if (this->snakes.size()) {
		for (vector<Snake*>::iterator it = this->snakes.begin();
			it != this->snakes.end();
			it++) {
			delete (*it);
		}
	}
	if (this->origin_snake != NULL) {
		delete this->origin_snake;
	}
}

void VectorSpace::GenerateSnake() {
	// check if already generated
	if (snakes.size() > 0) {
		return;
	}

	vector<CvPoint> ctrl_points = this->origin_snake->ctrl_points;
	int size = ctrl_points.size();
	// (Qx Qy)^T
	CvMat *Q0_mat = cvCreateMat(size*2, 1, CV_32FC1);

	vector<CvPoint>::iterator ctrl_it = ctrl_points.begin();
	for (int i = 0; i < size; i++) {
		CV_MAT_ELEM(*Q0_mat, float, i, 0) = ctrl_it->x;
		CV_MAT_ELEM(*Q0_mat, float, i+size, 0) = ctrl_it->y;
		ctrl_it++;
	}


	CvMat *W_mat = cvCreateMat(size*2, 6, CV_32FC1);
	

	CvMat *result_mat = cvCreateMat(size*2, 1, CV_32FC1);


	CvMat *X_mat = cvCreateMat(6, 1, CV_32FC1);

	CV_MAT_ELEM(*X_mat, float, 0, 0) = 0;
	CV_MAT_ELEM(*X_mat, float, 1, 0) = 0;
	CV_MAT_ELEM(*X_mat, float, 2, 0) = 0;
	CV_MAT_ELEM(*X_mat, float, 3, 0) = 0;
	CV_MAT_ELEM(*X_mat, float, 4, 0) = 0;
	CV_MAT_ELEM(*X_mat, float, 5, 0) = 0;

	//////////////////////////////////////////////////////////
	// result
	// translate Ox
	for (int i = -12; i <= 18; i+=3) {
		// translate Oy
		for (int j = -21; j <= 12; j+=3) {
			// scale x
			for (float k = 0.9; k <= 1.1; k += 0.1) {
				// rotate
				for (float r = -5; r <= 5; r += 5) {

					ctrl_it = ctrl_points.begin();
					for (int ii = 0; ii < size; ii++) {
						CV_MAT_ELEM(*W_mat, float, ii, 0) = 1;
						CV_MAT_ELEM(*W_mat, float, ii, 1) = 0;
						CV_MAT_ELEM(*W_mat, float, ii, 2) = ctrl_it->x;
						CV_MAT_ELEM(*W_mat, float, ii, 3) = 0;
						CV_MAT_ELEM(*W_mat, float, ii, 4) = 0;
						CV_MAT_ELEM(*W_mat, float, ii, 5) = ctrl_it->y;

						CV_MAT_ELEM(*W_mat, float, ii+size, 0) = 0;
						CV_MAT_ELEM(*W_mat, float, ii+size, 1) = 1;
						CV_MAT_ELEM(*W_mat, float, ii+size, 2) = 0;
						CV_MAT_ELEM(*W_mat, float, ii+size, 3) = ctrl_it->y;
						CV_MAT_ELEM(*W_mat, float, ii+size, 4) = ctrl_it->x;
						CV_MAT_ELEM(*W_mat, float, ii+size, 5) = 0;
						ctrl_it++;
					}

					// tinh tien ve tam
					CV_MAT_ELEM(*X_mat, float, 0, 0) = -this->origin_snake->center.x;
					CV_MAT_ELEM(*X_mat, float, 1, 0) = -this->origin_snake->center.y;
					CV_MAT_ELEM(*X_mat, float, 2, 0) = 0;
					CV_MAT_ELEM(*X_mat, float, 3, 0) = 0;
					CV_MAT_ELEM(*X_mat, float, 4, 0) = 0;
					CV_MAT_ELEM(*X_mat, float, 5, 0) = 0;


					// (2Mx6 * 6x1 = 2Mx1) + 2Mx1 = 2Mx1
					cvMatMulAdd(W_mat, X_mat, Q0_mat, result_mat);


					CV_MAT_ELEM(*X_mat, float, 0, 0) = i;
					CV_MAT_ELEM(*X_mat, float, 1, 0) = j;
					CV_MAT_ELEM(*X_mat, float, 2, 0) = k*cos(r*PI/180)-1;
					CV_MAT_ELEM(*X_mat, float, 3, 0) = k*cos(r*PI/180)-1;
					CV_MAT_ELEM(*X_mat, float, 4, 0) = k*-sin(r*PI/180);
					CV_MAT_ELEM(*X_mat, float, 5, 0) = k*sin(r*PI/180);

					// change W mat
					for (int q = 0; q < size; q++) {
						CV_MAT_ELEM(*W_mat, float, q, 2) = CV_MAT_ELEM(*result_mat, float, q, 0);
						CV_MAT_ELEM(*W_mat, float, q, 5) = CV_MAT_ELEM(*result_mat, float, q+size, 0);
						CV_MAT_ELEM(*W_mat, float, q+size, 3) = CV_MAT_ELEM(*result_mat, float, q+size, 0);
						CV_MAT_ELEM(*W_mat, float, q+size, 4) = CV_MAT_ELEM(*result_mat, float, q, 0);

					}
					cvMatMulAdd(W_mat, X_mat, result_mat, result_mat);


					// tinh tien tro lai vi tri cu
					CV_MAT_ELEM(*X_mat, float, 0, 0) = this->origin_snake->center.x;
					CV_MAT_ELEM(*X_mat, float, 1, 0) = this->origin_snake->center.y;
					CV_MAT_ELEM(*X_mat, float, 2, 0) = 0;
					CV_MAT_ELEM(*X_mat, float, 3, 0) = 0;
					CV_MAT_ELEM(*X_mat, float, 4, 0) = 0;
					CV_MAT_ELEM(*X_mat, float, 5, 0) = 0;

					// change W mat
					for (int q = 0; q < size; q++) {
						CV_MAT_ELEM(*W_mat, float, q, 2) = CV_MAT_ELEM(*result_mat, float, q, 0);
						CV_MAT_ELEM(*W_mat, float, q, 5) = CV_MAT_ELEM(*result_mat, float, q+size, 0);
						CV_MAT_ELEM(*W_mat, float, q+size, 3) = CV_MAT_ELEM(*result_mat, float, q+size, 0);
						CV_MAT_ELEM(*W_mat, float, q+size, 4) = CV_MAT_ELEM(*result_mat, float, q, 0);
					}
					cvMatMulAdd(W_mat, X_mat, result_mat, result_mat);

					vector<CvPoint> vect;
					
					vect.clear();
					for (int ii = 0; ii < size; ii++) {
						CvPoint pt;
						pt.x = CV_MAT_ELEM(*result_mat, float, ii, 0);
						pt.y = CV_MAT_ELEM(*result_mat, float, ii+size, 0);
						vect.push_back(pt);
					}
					Snake *s = new Snake(vect, false);
					snakes.push_back(s);
				}
			}
		}
	}

	cvReleaseMat(&result_mat);
	cvReleaseMat(&Q0_mat);
	cvReleaseMat(&X_mat);
	cvReleaseMat(&W_mat);
}

void VectorSpace::DrawSnakes(IplImage *img) {
	for (vector<Snake*>::iterator it = this->snakes.begin();
		it != this->snakes.end();
		it++) {
		(*it)->DrawCurve(img);
	}
}