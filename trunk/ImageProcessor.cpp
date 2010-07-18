
#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(void)
{
}

ImageProcessor::~ImageProcessor(void)
{
}


IplImage* ImageProcessor::getSubImageAndResize(IplImage* img,CvRect rect,int new_width,int new_height){

	if((rect.x >= 0) && (rect.y >= 0) && (rect.x + rect.width <= img->width) && (rect.y + rect.height <= img->height))
	{
		IplImage* subimg = cvCreateImage(cvSize(rect.width,rect.height),img->depth,img->nChannels);

		int nchannels = img->nChannels;
		for(int i = 0; i < rect.height; i++)
		{
			uchar* ptr1Tmp = (uchar*)img->imageData + (i+rect.y)*img->widthStep;
			uchar* ptr2Tmp = (uchar*)subimg->imageData + i*subimg->widthStep;

			for(int j = 0; j < rect.width; j++){
				uchar* ptr1 = ptr1Tmp + (j + rect.x)*nchannels;
				uchar* ptr2 = ptr2Tmp + j*nchannels;

				for(int t = 0; t < nchannels; t++){
					ptr2[t] = ptr1[t];
				}
			}		
		}

		IplImage* imageScale = cvCreateImage(cvSize(new_width,new_height),img->depth, nchannels);
		cvResize(subimg,imageScale);

		cvReleaseImage(&subimg);
		return imageScale;
	}else
		return NULL;
}

