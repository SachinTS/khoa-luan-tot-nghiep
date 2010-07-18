#include "StdAfx.h"
#include "snake.h"

CvMat* Snake::Global_B = NULL;
CvMat* Snake::Global_B_Step = NULL;

Snake::Snake(FILE *data) {
	// reading size of snake window
	int w = 1, h = 1;
	fseek(data, 0, 0);
	fscanf(data, "%d %d", &w, &h);

	// reading points
	int x, y;
	while (!feof(data)) {
		fscanf(data, "%d %d\n", &x, &y);
		this->templ_points.push_back(cvPoint(x, y));			// adding point to template point
	}

	this->step = 0.1f;
	CvPoint first_pt = templ_points.at(0);
	CvPoint last_pt = templ_points.at(templ_points.size()-1);
	this->templ_points.insert(this->templ_points.begin(), first_pt);
	this->templ_points.push_back(last_pt);
	this->M = this->templ_points.size();
	this->N = this->M - 3;



	// building matrix
	Snake::Global_B = BuildingGlobalMatrix();
	Snake::Global_B_Step = BuildingGlobalMatrixWithStep();

	// tinh ra control points
	CurveCalculate();


	int sum_x = 0, sum_y = 0;
	for (vector<CvPoint>::iterator it = this->ctrl_points.begin()+1;
		it != this->ctrl_points.end()-1;
		it++) {
		sum_x += it->x;
		sum_y += it->y;
	}
	this->center.x = sum_x/((int)this->ctrl_points.size()-2);
	this->center.y = sum_y/((int)this->ctrl_points.size()-2);

	this->epsilon = 70.0f;
}

Snake::Snake(FILE *data, int m) {
	// reading points
	int x, y;
	for (int i = 0; i < m; i++) {
		fscanf(data, "%d %d\n", &x, &y);
		this->templ_points.push_back(cvPoint(x, y));			// adding point to template point
	}

	this->step = 0.1f;
	CvPoint first_pt = templ_points.at(0);
	CvPoint last_pt = templ_points.at(templ_points.size()-1);
	this->templ_points.insert(this->templ_points.begin(), first_pt);
	this->templ_points.push_back(last_pt);
	this->M = this->templ_points.size();
	this->N = this->M - 3;


	// building matrix
	Snake::Global_B = BuildingGlobalMatrix();
	Snake::Global_B_Step = BuildingGlobalMatrixWithStep();

	// tinh ra control points
	CurveCalculate();

	int sum_x = 0, sum_y = 0;
	for (vector<CvPoint>::iterator it = this->ctrl_points.begin()+1;
		it != this->ctrl_points.end()-1;
		it++) {
		sum_x += it->x;
		sum_y += it->y;
	}
	this->center.x = sum_x/((int)this->ctrl_points.size()-2);
	this->center.y = sum_y/((int)this->ctrl_points.size()-2);

	this->epsilon = 70.0f;
}

Snake::Snake(vector<CvPoint> templ_points)
{
	this->step = 0.1f;
	this->templ_points = templ_points;
	CvPoint first_pt = templ_points.at(0);
	CvPoint last_pt = templ_points.at(templ_points.size()-1);
	this->templ_points.insert(this->templ_points.begin(), first_pt);
	this->templ_points.push_back(last_pt);
	this->M = this->templ_points.size();
	this->N = this->M - 3;


	// building matrix
	Snake::Global_B = BuildingGlobalMatrix();
	Snake::Global_B_Step = BuildingGlobalMatrixWithStep();

	// tinh ra control points
	CurveCalculate();


	int sum_x = 0, sum_y = 0;
	for (vector<CvPoint>::iterator it = this->ctrl_points.begin()+1;
		it != this->ctrl_points.end()-1;
		it++) {
		sum_x += it->x;
		sum_y += it->y;
	}
	this->center.x = sum_x/((int)this->ctrl_points.size()-2);
	this->center.y = sum_y/((int)this->ctrl_points.size()-2);

	this->epsilon = 70.0f;
}

Snake::Snake(std::vector<CvPoint> ctrl_points, bool pad) {
	this->step = 0.1f;
	this->ctrl_points = ctrl_points;
	if (pad) {
		CvPoint first_pt = ctrl_points.at(0);
		CvPoint last_pt = ctrl_points.at(ctrl_points.size()-1);
		this->ctrl_points.insert(this->ctrl_points.begin(), first_pt);
		this->ctrl_points.push_back(last_pt);
	}
	this->M = this->ctrl_points.size();
	this->N = this->M - 3;

	
	// building matrix
	Snake::Global_B = BuildingGlobalMatrix();
	Snake::Global_B_Step = BuildingGlobalMatrixWithStep();

	// init control point matrix
	ctrl_mat = cvCreateMat(M, 2, CV_32FC1);
	int sum_x = 0, sum_y = 0, i = 1;

	// row 0
	CV_MAT_ELEM(*ctrl_mat, float, 0, 0) = this->ctrl_points.begin()->x;
	CV_MAT_ELEM(*ctrl_mat, float, 0, 1) = this->ctrl_points.begin()->y;

	// row 1->M-2
	for (vector<CvPoint>::iterator it = this->ctrl_points.begin()+1;
		it != this->ctrl_points.end()-1;
		it++) {
		sum_x += it->x;
		sum_y += it->y;
		CV_MAT_ELEM(*ctrl_mat, float, i, 0) = it->x;
		CV_MAT_ELEM(*ctrl_mat, float, i, 1) = it->y;
		i++;
	}
	
	// row M-1
	CV_MAT_ELEM(*ctrl_mat, float, i, 0) = (this->ctrl_points.end()-1)->x;
	CV_MAT_ELEM(*ctrl_mat, float, i, 1) = (this->ctrl_points.end()-1)->y;

	this->center.x = sum_x/((int)this->ctrl_points.size()-2);
	this->center.y = sum_y/((int)this->ctrl_points.size()-2);

	this->epsilon = 70.0f;
}


Snake::~Snake(void)
{
	this->templ_points.clear();
	this->ctrl_points.clear();
	cvReleaseMat(&Snake::Global_B);
	cvReleaseMat(&Snake::Global_B_Step);
}

float Snake::B1(float u) {
	float result = 0;
	result = (1.0f/6)*(1-u)*(1-u)*(1-u);
	return result;
}

float Snake::B11(float u) {
	float result = 0;
	result = (1.0f/2)*(-u*u+2u-1);
	return result;
}

float Snake::B2(float u) {
	float result = 0;
	result = (1.0f/6)*(3*u*u*u - 6*u*u + 4);
	return result;
}

float Snake::B21(float u) {
	float result = (3.0f/2)*u*u-2*u;
	return result;
}

float Snake::B3(float u) {
	float result = 0;
	result = (1.0f/6)*(-3*u*u*u + 3*u*u + 3*u + 1);
	return result;
}

float Snake::B31(float u) {
	float result = (1.0f/6)*(-9*u*u+6*u+3);
	return result;
}

float Snake::B4(float u) {
	float result = 0;
	result = (1.0f/6)*(u*u*u);
	return result;
}

float Snake::B41(float u) {
	float result = (1.0f/2)*u*u;
	return result;
}

CvMat *Snake::BuildingGlobalMatrix() {
	int row = N+3, col = M;
	CvMat *mat = cvCreateMat(row, col, CV_32FC1);

	// assign first and last row;
	for (int i = 0; i < col-1; i++) {
		CV_MAT_ELEM(*mat, float, 0, i+1) = 0;
		CV_MAT_ELEM(*mat, float, row-1, i) = 0;
	}
	CV_MAT_ELEM(*mat, float, 0, 0) = 1;
	CV_MAT_ELEM(*mat, float, row-1, col-1) = 1;

	for (int i = 1; i < row-1; i++) {
		for (int j = 0; j < col; j++) {
			float g = 0;
			if (j-3<=i-1 && i-1<j-2) {
				g = B4(i-j-2+4);
			}
			else if (j-2<=i-1 && i-1<j-1) {
				g = B3(i-j-2+3);
			}
			else if (j-1<=i-1 && i-1<j) {
				g = B2(i-j-2+2);
			}
			else if (j<=i-1 && i-1<j+1) {
				g = B1(i-j-2+1);
			}
			CV_MAT_ELEM(*mat, float, i, j) = g;
		}
	}
	//N+3 x M matrix
	return mat;
}

CvMat *Snake::CurveCalculate() {
	// template matrix
	CvMat *points_mat = cvCreateMat(M, 2, CV_32FC1);
	for (int i = 0; i < M; i++) {
		CV_MAT_ELEM(*points_mat, float, i, 0) = templ_points.at(i).x;
		CV_MAT_ELEM(*points_mat, float, i, 1) = templ_points.at(i).y;
	}
	// control point matrix
	ctrl_mat = cvCreateMat(M, 2, CV_32FC1);
	CvMat *global_mat = Snake::Global_B;
	CvMat *global_mat_inv = cvCreateMat(global_mat->rows, global_mat->cols, CV_32FC1);
	cvInvert(global_mat, global_mat_inv);

	cvMatMul(global_mat_inv, points_mat, ctrl_mat);
	for (int i = 0; i < ctrl_mat->rows; i++) {
		CvPoint pt;
		pt.x = CV_MAT_ELEM(*ctrl_mat, float, i, 0);
		pt.y = CV_MAT_ELEM(*ctrl_mat, float, i, 1);
		// xac dinh cac diem control points
		this->ctrl_points.push_back(pt);
	}

	cvReleaseMat(&points_mat);
	cvReleaseMat(&global_mat_inv);
	//N+3 x 2 matrix
	return ctrl_mat;
}

CvMat *Snake::DrawCurve(IplImage *image) {

    CvMat *curve_mat = cvCreateMat(round(N*1.0/this->step+3), 2, CV_32FC1);
	CvMat *global_mat = Snake::Global_B_Step;
    cvMatMul(global_mat, ctrl_mat, curve_mat);
	int h = curve_mat->height;
	for (int i = 0; i < h; i++) {
		float x = CV_MAT_ELEM(*curve_mat, float, i, 0);
		float y = CV_MAT_ELEM(*curve_mat, float, i, 1);

		cvCircle(image, cvPoint(x, y), 0, CV_RGB(255, 0, 0), 1);
	}
    //N+3 x 2 matrix
    return curve_mat;
}

CvMat *Snake::DrawCurve(IplImage *image, CvPoint location) {

	CvMat *curve_mat = cvCreateMat(round(N*1.0/this->step+3), 2, CV_32FC1);
	CvMat *global_mat = Snake::Global_B_Step;
    cvMatMul(global_mat, ctrl_mat, curve_mat);
	int h = curve_mat->height;
	for (int i = 0; i < h; i++) {
		int x = CV_MAT_ELEM(*curve_mat, float, i, 0);
		int y = CV_MAT_ELEM(*curve_mat, float, i, 1);

		cvCircle(image, cvPoint(x+location.x, y+location.y), 0, CV_RGB(0, 255, 255), 2);
	}
    //N+3 x 2 matrix
    return curve_mat;
}

CvMat *Snake::BuildingGlobalMatrixWithStep() {
    int row = round(N*1.0/this->step+3), col = M;
    CvMat *mat = cvCreateMat(row, col, CV_32FC1);

    // assign first and last row;
    for (int i = 0; i < col-1; i++) {
        CV_MAT_ELEM(*mat, float, 0, i+1) = 0;
        CV_MAT_ELEM(*mat, float, row-1, i) = 0;
    }
    CV_MAT_ELEM(*mat, float, 0, 0) = 1;
    CV_MAT_ELEM(*mat, float, row-1, col-1) = 1;

    float s = 0;
    for (int i = 1; i < row-1; i++) {
        for (int j = 0; j < col; j++) {
            float g = 0;
            if (j-3<=s && s<j-2) {
                g = B4(s-j+3);
            }
            else if (j-2<=s && s<j-1) {
                g = B3(s-j+2);
            }
            else if (j-1<=s && s<j) {
                g = B2(s-j+1);
            }
            else if (j<=s && s<j+1) {
                g = B1(s-j);
            }
            CV_MAT_ELEM(*mat, float, i, j) = g;
        }
        s += this->step;
    }

    //N+3 x M matrix
    return mat;
}


double Snake::Likelihood(IplImage *edge, int l, float delta, const CvPoint& location, const CvRect& bounding_rect) {
	// su dung voi canny edge
	double p = 0;

    CvMat *curve_mat = cvCreateMat(round(N*1.0/this->step+3), 2, CV_32FC1);
	CvMat *global_mat = Snake::Global_B_Step;
    cvMatMul(global_mat, ctrl_mat, curve_mat);

	int height = curve_mat->height;
	int size = round(1.0f/this->step);

	
	int left = bounding_rect.x;
	int top = bounding_rect.y;
	int bottom = top + bounding_rect.height;
	int right = left + bounding_rect.width;


	int sx = location.x, sy = location.y;
	float sum = 0;
	for (int i = 1; i < height-1; i+=size) {
		int x  = CV_MAT_ELEM(*curve_mat, float, i  , 0);
		int y  = CV_MAT_ELEM(*curve_mat, float, i  , 1);
		int x1 = CV_MAT_ELEM(*curve_mat, float, i-1, 0);
		int y1 = CV_MAT_ELEM(*curve_mat, float, i-1, 1);
		int x2 = CV_MAT_ELEM(*curve_mat, float, i+1, 0);
		int y2 = CV_MAT_ELEM(*curve_mat, float, i+1, 1);

		uchar *data = &CV_IMAGE_ELEM(edge, uchar, x, y);

		int dx = x2-x1, dy = y2-y1;
		float a, b;		// he so goc phap tuyen
		float d = this->epsilon;

		int h = edge->height, w = edge->width;
		uchar value1 = 0;
		uchar value2 = 0;
		if (dy == 0) {
			for (int k = 0; k <= l; k++) {

				if (y-k+sy < top || y-k+sy >= bottom) {
					value1 = 0;
				}
				else if (y+k+sy < top || y+k+sy >= bottom) {
					value2 = 0;
				}
				else if (x+sx < left || x+sx >= right) {
					value1 = 0;
					value2 = 0;
				}
				else {
					value1 = CV_IMAGE_ELEM(edge, uchar, y-k+sy, x+sx);
					value2 = CV_IMAGE_ELEM(edge, uchar, y+k+sy, x+sx);
				}

				if (value2) {
					if (d > dist(x, y, x, y+k) || d == this->epsilon) {
						d = dist(x, y, x, y+k);
					}
				}
			}
		}
		else {
			a = -dx*1.0f/dy;
			b = y - a*x;
			if (a > 1 || a < -1) {
				for (int k = 0; k <= l; k++) {
					CvPoint pt1 = cvPoint(((y-k)-b)*1.0f/a, y-k);
					CvPoint pt2 = cvPoint(((y+k)-b)*1.0f/a, y+k);

					if (pt1.x+sx < left || pt1.x+sx >= right ||
						pt1.y+sy < top || pt1.y+sy >= bottom) {
						value1 = 0;
					}
					else if (pt2.x+sx < left || pt2.x+sx >= right ||
						pt2.y+sy < top || pt2.y+sy >= bottom) {
						value2 = 0;
					}
					else {
						value1 = CV_IMAGE_ELEM(edge, uchar, pt1.y+sy, pt1.x+sx);
						value2 = CV_IMAGE_ELEM(edge, uchar, pt2.y+sy, pt2.x+sx);
					}

					float distance1 = dist(x, y, pt1.x, pt1.y);
					float distance2 = dist(x, y, pt2.x, pt2.y);

					if (value1) {
						if (d > distance1 || d == this->epsilon) {
							d = distance1;
						}
					}
					if (value2) {
						if (d > distance2 || d == this->epsilon) {
							d = distance2;
						}
					}
				}
			}
			else {
				for (int k = 0; k <= l; k++) {
					CvPoint pt1 = cvPoint(x-k, a*(x-k)+b);
					CvPoint pt2 = cvPoint(x+k, a*(x+k)+b);

					if (pt1.x+sx < left || pt1.x+sx >= right ||
						pt1.y+sy < top || pt1.y+sy >= bottom) {
						value1 = 0;
					}
					else if (pt2.x+sx < left || pt2.x+sx >= right ||
						pt2.y+sy < top || pt2.y+sy >= bottom) {
						value2 = 0;
					}
					else {
						value1 = CV_IMAGE_ELEM(edge, uchar, pt1.y+sy, pt1.x+sx);
						value2 = CV_IMAGE_ELEM(edge, uchar, pt2.y+sy, pt2.x+sx);
					}

					float distance1 = dist(x, y, pt1.x, pt1.y);
					float distance2 = dist(x, y, pt2.x, pt2.y);

					if (value1) {
						if (d > distance1 || d == this->epsilon) {
							d = distance1;
						}
					}
					if (value2) {
						if (d > distance2 || d == this->epsilon) {
							d = distance2;
						}
					}
				}
			}
		}

		sum += d*d;
	}
	p = -sum/(2*delta*delta);
	p = exp(p);

	return p;
}

double Snake::Likelihood(IplImage *edge, int l, float delta, const CvPoint& location) {
	// su dung voi canny edge
	double p = 0;

    CvMat *curve_mat = cvCreateMat(round(N*1.0/this->step+3), 2, CV_32FC1);
	CvMat *global_mat = Snake::Global_B_Step;
    cvMatMul(global_mat, ctrl_mat, curve_mat);

	int height = curve_mat->height;
	int size = round(1.0f/this->step);

	int sx = location.x, sy = location.y;
	float sum = 0;
	for (int i = 1; i < height-1; i+=size) {
		int x  = CV_MAT_ELEM(*curve_mat, float, i  , 0);
		int y  = CV_MAT_ELEM(*curve_mat, float, i  , 1);
		int x1 = CV_MAT_ELEM(*curve_mat, float, i-1, 0);
		int y1 = CV_MAT_ELEM(*curve_mat, float, i-1, 1);
		int x2 = CV_MAT_ELEM(*curve_mat, float, i+1, 0);
		int y2 = CV_MAT_ELEM(*curve_mat, float, i+1, 1);

		uchar *data = &CV_IMAGE_ELEM(edge, uchar, x, y);

		int dx = x2-x1, dy = y2-y1;
		float a, b;		// he so goc phap tuyen
		float d = this->epsilon;

		int h = edge->height, w = edge->width;
		uchar value1 = 0;
		uchar value2 = 0;
		if (dy == 0) {
			for (int k = 0; k <= l; k++) {

				if (y-k+sy < 0 || y-k+sy >= h) {
					value1 = 0;
				}
				else if (y+k+sy < 0 || y+k+sy >= h) {
					value2 = 0;
				}
				else if (x+sx < 0 || x+sx >= w) {
					value1 = 0;
					value2 = 0;
				}
				else {
					value1 = CV_IMAGE_ELEM(edge, uchar, y-k+sy, x+sx);
					value2 = CV_IMAGE_ELEM(edge, uchar, y+k+sy, x+sx);
				}

				if (value2) {
					if (d > dist(x, y, x, y+k) || d == this->epsilon) {
						d = dist(x, y, x, y+k);
					}
				}
			}
		}
		else {
			a = -dx*1.0f/dy;
			b = y - a*x;
			if (a > 1 || a < -1) {
				for (int k = 0; k <= l; k++) {
					CvPoint pt1 = cvPoint(((y-k)-b)*1.0f/a, y-k);
					CvPoint pt2 = cvPoint(((y+k)-b)*1.0f/a, y+k);

					if (pt1.x+sx < 0 || pt1.x+sx >= w ||
						pt1.y+sy < 0 || pt1.y+sy >= h) {
						value1 = 0;
					}
					else if (pt2.x+sx < 0 || pt2.x+sx >= w ||
						pt2.y+sy < 0 || pt2.y+sy >= h) {
						value2 = 0;
					}
					else {
						value1 = CV_IMAGE_ELEM(edge, uchar, pt1.y+sy, pt1.x+sx);
						value2 = CV_IMAGE_ELEM(edge, uchar, pt2.y+sy, pt2.x+sx);
					}

					float distance1 = dist(x, y, pt1.x, pt1.y);
					float distance2 = dist(x, y, pt2.x, pt2.y);

					if (value1) {
						if (d > distance1 || d == this->epsilon) {
							d = distance1;
						}
					}
					if (value2) {
						if (d > distance2 || d == this->epsilon) {
							d = distance2;
						}
					}
				}
			}
			else {
				for (int k = 0; k <= l; k++) {
					CvPoint pt1 = cvPoint(x-k, a*(x-k)+b);
					CvPoint pt2 = cvPoint(x+k, a*(x+k)+b);

					if (pt1.x+sx < 0 || pt1.x+sx >= w ||
						pt1.y+sy < 0 || pt1.y+sy >= h) {
						value1 = 0;
					}
					else if (pt2.x+sx < 0 || pt2.x+sx >= w ||
						pt2.y+sy < 0 || pt2.y+sy >= h) {
						value2 = 0;
					}
					else {
						value1 = CV_IMAGE_ELEM(edge, uchar, pt1.y+sy, pt1.x+sx);
						value2 = CV_IMAGE_ELEM(edge, uchar, pt2.y+sy, pt2.x+sx);
					}

					float distance1 = dist(x, y, pt1.x, pt1.y);
					float distance2 = dist(x, y, pt2.x, pt2.y);

					if (value1) {
						if (d > distance1 || d == this->epsilon) {
							d = distance1;
						}
					}
					if (value2) {
						if (d > distance2 || d == this->epsilon) {
							d = distance2;
						}
					}
				}
			}
		}

		sum += d*d;
	}
	p = -sum/(2*delta*delta);
	p = exp(p);

	return p;
}