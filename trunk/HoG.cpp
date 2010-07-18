// HoG.cpp : Defines the entry point for the console application.
//


#include "cv.h"
#include "cvaux.h"
#include "highgui.h"
#include "HoGProcessor.h"
#include "stdio.h"
#include "conio.h"
#include "fstream"
#include "string"
#include "iostream"
#include "sstream"
#include "time.h"
using namespace std;

#define TRAIN_DATA 1
#define VIDEO_PATH "E://Data//3_13.45.00.avi"
#define IMG_MASK "E://Data//mask.PNG"

//string convertToChar(double source);
int main1(int argc, char* argv[])
{
	argc = 12;
	argv = new char*[argc];

	if(TRAIN_DATA)
	{
		argv[0] = "HoG";
		argv[1] = "train_model";
		argv[2] = "E:\\Data\\Head\\pos\\pic";
		argv[3] = ".png";
		argv[4] = "1";
		argv[5] = "89";
		argv[6] = "E:\\Data\\Head\\neg\\pic";
		argv[7] = ".png";
		argv[8] = "1";
		argv[9] = "125";
		argv[10] = "E:\\Data\\model.xml";
		argv[11] = "false";//Test positive

		 if(argc == 12){
			HoGProcessor hog;

			int pos_startindex = atoi(argv[4]);
			int pos_endindex   = atoi(argv[5]);
			int pos_numberofsample = pos_endindex - pos_startindex+1;

			CvMat* pos_mat = hog.train_64x128(argv[2], argv[3], cvSize(48,48), pos_numberofsample, pos_startindex, pos_endindex);

			int neg_startindex = atoi(argv[8]);
			int neg_endindex = atoi(argv[9]);
			int neg_numberofsample = neg_endindex - neg_startindex + 1;

			//CvMat* neg_mat = hog.train_large(argv[6], argv[7], cvSize(8,8), cvSize(64,128), neg_numberofsample, 2, 2, neg_startindex, neg_endindex);
			CvMat* neg_mat = hog.train_64x128(argv[6], argv[7], cvSize(48,48), neg_numberofsample, neg_startindex, neg_endindex);
			hog.trainSVM(pos_mat, neg_mat, argv[10]);

			CvMat* feature;

			bool isTestPositive = (strcmp(argv[11],"true") == 0) ? true : false;

			if(isTestPositive)
			{
				feature =  hog.train_64x128(argv[2], argv[3], cvSize(48,48),pos_numberofsample,pos_startindex,pos_endindex);	
			}else{
				feature =  hog.train_64x128(argv[6],argv[7], cvSize(48,48),neg_numberofsample,neg_startindex,neg_endindex);
			}		

			//Test Positive Value
			argv[6] = argv[10];
			CvSVM svm;
			svm.load(argv[6]);
			CvMat* output = hog.svm_predict(&svm,feature);
			
			float positive = 0;
			uchar testClass;
			if(isTestPositive){
				testClass = 1;
			}else{
				testClass = 2;
			}

			for(int i = 0 ; i < output->rows; i++){
				uchar classname  = CV_MAT_ELEM(*output, uchar, i, 0);
				if(classname == testClass)
				{
					positive = positive + 1;
				}
			}

			printf("\nDo chinh xac = %f",positive/output->rows);

			//cvReleaseMat(&pos_mat);
			//cvReleaseMat(&neg_mat);
			//cvReleaseMat(&feature);
			cvReleaseMat(&output);
		}

		delete[] argv;
		printf("\n\n");

		printf("Syntax :12 parameter :\n");
		printf("HoG.exe train_model <positive_folder_prefix> <positive_subfix> <positive_start_index> <positive_end_index> <negative_folder_prefix>  <negative_sufix> <negative_startindex> <negative_endindex> <outputfile> <isTestPositive>\n" );
		printf("\n\n");

		int a;
		cin>>a;
	}
	else
	{		
		CvCapture *capture = cvCreateFileCapture(VIDEO_PATH);
		double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		
		IplImage *frame;
		IplImage *mask = cvLoadImage(IMG_MASK);
		
		IplImage *result = cvCloneImage(mask);

		cvNamedWindow("remove");

		HoGProcessor hog;
		CvSVM svm;
		svm.load("E://Data//model.xml");

		int frame_count = 0;
		clock_t start, end;

		start = clock();
		while(1)
		{
			frame = cvQueryFrame(capture);
			if(frame == 0)
				break;
			if(cvWaitKey(fps)==27)
				break;

			//cvSub(frame, mask, result);

			cvCopyImage(frame, result);

			hog.detectObject(&svm, frame, result, cvSize(8, 8), cvSize(48, 48));

			cvShowImage("remove", result);

			frame_count++;
		}
		end = clock();

		fps = frame_count*1.0/(end - start);

		cout<<"Frame per second: "<<fps<<endl;
		cin>>fps;

		cvReleaseImage(&mask);
		cvReleaseImage(&result);

		cvDestroyAllWindows();
}

	return 0;
}

/*
string convertToChar(double source){
   char *buffer;
   int decimal,sign;
   buffer = _fcvt( source, 7, &decimal, &sign ); // C4996
   int length  = strlen(buffer);
   string str;
   

   if(decimal <= 0){
	   str.append("0");
	   str.append(".");
	   for(int i = 0 ; i > decimal; i--)
	   {
		   str.append("0");
	   }
	   str.append(buffer);
   }else{
	   for(int i = 0; i < strlen(buffer); i++)
	   {

		   if(i == decimal)
		   {
			   str.append(".");
		   }
		   str.append(1,buffer[i]);
		   

	   }
   }  
   
   return str;
}
*/