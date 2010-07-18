#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include "time.h"
#include "HoGProcessor.h"
#include "GaussFilterColor.h"
#include "Utils.h"
#include "Params.h"
#include "snake.h"
#include "snakewindow.h"


#define _VIDEO_ "data\\video\\video.avi"
#define _MASK_ "data\\mask.jpg"
#define _MODEL_PATH "data\\model.xml"
#define _COLOR_MODEL "data\\out.txt"
#define TRAIN_DATA 0
#define TRAIN_HAIR 0

int main() {
	Utils utils;

	if(TRAIN_HAIR){
		GaussFilterColor gauss;
		gauss.TrainData("E:\\Data\\Hair\\hair",".png", 20, 1, 20, _COLOR_MODEL);
	}
	if(TRAIN_DATA)
	{		
		char* prefixPos = "E:\\Data\\Head\\pos\\pic";
		char* posfixPos = ".png";
		
		int pos_startindex = 1;
		int pos_endindex   = 352;
		int pos_numberofsample = pos_endindex - pos_startindex+1;

		char* prefixNeg = "E:\\Data\\Head\\neg\\pic";
		char* posfixNeg = ".png";

		int neg_startindex = 1;
		int neg_endindex = 425;
		int neg_numberofsample = neg_endindex - neg_startindex + 1;
							 
		HoGProcessor hog;
		
		CvMat* pos_mat = hog.train_64x128(prefixPos, posfixPos, cvSize(48,48), pos_numberofsample, pos_startindex, pos_endindex);		
		CvMat* neg_mat = hog.train_large(prefixNeg, posfixNeg, cvSize(48,48), neg_numberofsample, 3, 3, neg_startindex, neg_endindex);

		hog.trainSVM(pos_mat, neg_mat, _MODEL_PATH);		

		printf("\n\n");
		printf("Syntax :12 parameter :\n");
		printf("HoG.exe train_model <positive_folder_prefix> <positive_subfix> <positive_start_index> <positive_end_index> <negative_folder_prefix>  <negative_sufix> <negative_startindex> <negative_endindex> <outputfile> <isTestPositive>\n" );
		printf("\n\n");
	}
	else
	{	
		// global variables
		IplImage *hair_grayscale, *hair_canny;
		int frame_count = 0;


		FILE *big_data, *medium_data, *small_data;
		big_data = fopen("data\\shape\\big.txt", "r");
		medium_data = fopen("data\\shape\\medium.txt", "r");
		small_data = fopen("data\\shape\\small.txt", "r");

		SnakeWindow *bigw, *mediumw, *smallw;
		bigw = new SnakeWindow(big_data);
		mediumw = new SnakeWindow(medium_data);
		smallw = new SnakeWindow(small_data);


		// init font
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4);


		Params param;
		param.LoadParamsFromXML("data\\Config.xml");

		CvCapture *capture = cvCaptureFromFile(_VIDEO_);
		if (capture == NULL) {
			printf("Cannot open video.\n");
			return EXIT_FAILURE;
		}

		int count = 0;
		float speed;
		time_t start = time(NULL);

		IplImage *frame = cvQueryFrame(capture);
		IplImage *subtract;
		IplImage *mask = cvLoadImage(_MASK_, CV_LOAD_IMAGE_GRAYSCALE);
		IplImage *result = cvCloneImage(frame);
		hair_grayscale = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		hair_canny = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);


		int fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);

		cvNamedWindow("Video");

		CvMemStorage *storage = cvCreateMemStorage();		
		CvSeq *contours = 0;

		
		HoGProcessor hog;
		hog.SetParams(param.m_HOG_Params.m_cell, param.m_HOG_Params.m_block, param.m_HOG_Params.m_fStepOverlap);

		CvSVM svm;
		
		CvRect window = cvRect(0,0,48,48);
		
		svm.load(_MODEL_PATH);
		
		GaussFilterColor gauss;
		gauss.LoadData(_COLOR_MODEL);
		gauss.SetThreshold(param.m_Gaussian_Params.m_fThreshold);

		vector<CvRect> vectorRect;
		while (1) {
			frame = cvQueryFrame(capture);
			if (frame == NULL) {
				printf("End video.\n");
				break;
			}
			
			subtract = gauss.Classify(frame, mask);	

			cvSmooth(subtract, hair_canny, CV_MEDIAN);
			cvCanny(hair_canny, hair_canny, 10, 100);

			
			cvFindContours(subtract, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL);

			cvCopyImage(frame, result);


			if (contours) {				
				vectorRect.clear();						
				
				while (contours != NULL) {
					
					CvRect rectHead = cvBoundingRect(contours);	
					
					if(!utils.CheckRectHead(rectHead, frame->height, param.m_Head_Params))
					{										
						contours = contours->h_next;
						continue;
					}					
						
						
					CvRect detectedRect = hog.detectObject(&svm, frame, result, rectHead);
					if(detectedRect.width > 0){
						vectorRect.push_back(detectedRect);
					}
			
					contours = contours->h_next;					
				}
			}

			vectorRect = utils.ConnectOverlapRects(vectorRect);
			for (unsigned int i = 0; i < vectorRect.size(); i++) {
				CvRect rect = vectorRect.at(i);
				CvPoint location = cvPoint(rect.x+rect.width*1.0f/3, rect.y+rect.height*1.0f/3);

				Snake *fit_snake;
				SnakeWindow *win;
				int current_y = rect.y+rect.height*1.0f/3;
				int frame_height_step = frame->height*1.0/3;
				if (current_y >= 0 && current_y < frame_height_step) {
					win = smallw;
				}
				else if (current_y >= frame_height_step && current_y < frame_height_step*2) {
					win = mediumw;
				}
				else if (current_y >= frame_height_step*2 && current_y <frame->height) {
					win = bigw;
				}
				fit_snake = win->GetSnake(result, hair_canny, location, rect);

				if (fit_snake != NULL) {
					fit_snake->DrawCurve(result, location);
				}
			}
			utils.OutputResult(result, vectorRect, CV_RGB(255,0,0));
						
				
			count++;
			cvShowImage("Video", result);

			cvReleaseImage(&subtract);
						
			if(cvWaitKey(fps) == 27)
				break;
		}

		time_t end = time(NULL);
		speed = count*1.0/(end-start);
		printf("fps: %2.3f.\n", speed);

		delete bigw;
		delete mediumw;
		delete smallw;

		fclose(big_data);
		fclose(medium_data);
		fclose(small_data);

		cvDestroyAllWindows();

		cvReleaseCapture(&capture);
		cvReleaseImage(&mask);
		cvReleaseImage(&result);		
		cvReleaseImage(&subtract);		
		cvReleaseMemStorage(&storage);
	}
	
	return EXIT_SUCCESS;
}