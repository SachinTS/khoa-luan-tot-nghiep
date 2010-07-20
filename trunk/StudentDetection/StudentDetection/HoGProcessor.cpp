#include "stdafx.h"
#include "HoGProcessor.h"
#include "ImageProcessor.h"
#include "Vector"

HoGProcessor::HoGProcessor(void)
{		
	m_cell = cvSize(4, 4);
	m_block = cvSize(2, 2);
	m_fStepOverlap = 0.5;
}

HoGProcessor::~HoGProcessor(void)
{	
}

void HoGProcessor::SetParams(CvSize cell, CvSize block, float stepOverLap)
{
	m_cell = cell;
	m_block = block;
	m_fStepOverlap = stepOverLap;
}

CvMat* HoGProcessor::train_large(char *prefix, char *suffix,
       CvSize window, int number_images,
       int horizontal_scans, int vertical_scans,
       int start_index, int end_index,
       char *savexml, int normalization)
{
 char filename[200] = "\0", number[8];
 int prefix_length;
 prefix_length = strlen(prefix);
 int bins = 9;

 /* A default block size of 2x2 cells is considered */

 int block_width = m_block.width , block_height = m_block.height;
 
 /* Calculation of the length of a feature vector for
 an image (64x128 pixels)*/

 int feature_vector_length;
 feature_vector_length = (((window.width -
	 m_cell.width * block_width) / (m_cell.width * m_fStepOverlap)) + 1) *
   (((window.height - m_cell.height * block_height)
   / (m_cell.height * m_fStepOverlap)) + 1) * (9 * m_block.height * m_block.width);
    
 /* Matrix to store the feature vectors for
 all(number_samples) the training samples */

 CvMat* training = cvCreateMat(number_images
      * horizontal_scans * vertical_scans,
      feature_vector_length, CV_32FC1);

 CvMat row;
 CvMat* img_feature_vector;
 IplImage** integrals;
 int i = 0, j = 0;
 strcat(filename, prefix);

 printf("Beginning to extract HoG features from negative images\n");
 
 /* Loop to calculate hog features for each
 image one by one */

 for (i = start_index; i <= end_index; i++) 
 {
	 
  itoa(i,number,10);
  strcat(filename, number);
  strcat(filename, suffix);
  IplImage* im = cvLoadImage(filename);


  IplImage* img = im;
  //Chi xet nhung anh nao co kich thuoc lon hon hoac bang window
  if(im->width < window.width || im->height < window.height)
  {
	  img = cvCreateImage(window,im->depth, im->nChannels);
	  cvResize(im,img);
  }

	  integrals = calculateIntegralHOG(img);	  

	  for (int l = 0; l < vertical_scans - 1; l++)
	  {
		for (int k = 0; k < horizontal_scans - 1; k++)
		{
		  cvGetRow(training, &row, j);
		  img_feature_vector = calculateHOG_window(
			 integrals, cvRect(window.width * k,
			 window.height * l, window.width,
			 window.height), normalization);
	 
		  cvCopy(img_feature_vector, &row);
		  j++;
		}

		cvGetRow(training, &row, j);

		img_feature_vector = calculateHOG_window(
		 integrals, cvRect(img->width - window.width,
			window.height * l, window.width,
			window.height), normalization);

		cvCopy(img_feature_vector, &row);
		j++;
	  }

	  for (int k = 0; k < horizontal_scans - 1; k++)
	  {
		cvGetRow(training, &row, j);

		img_feature_vector = calculateHOG_window(
			 integrals, cvRect(window.width * k,
			 img->height - window.height, window.width,
			 window.height), normalization);

		cvCopy(img_feature_vector, &row);
		j++;
	   }

	   cvGetRow(training, &row, j);

	   img_feature_vector = calculateHOG_window(integrals,
			cvRect(img->width - window.width, img->height -
			window.height, window.width, window.height),
			normalization);

	   cvCopy(img_feature_vector, &row);
	   j++;

	   printf("%s\n", filename);
	   filename[prefix_length] = '\0';
	   for (int k = 0; k < 9; k++)
	   {
		 cvReleaseImage(&integrals[k]);
	   }
  

   cvReleaseImage(&img);

 }
 
 printf("%d negative samples created \n",
        training->rows);

 if (savexml != NULL)
 {
   cvSave(savexml, training);
   printf("Negative samples saved as %s\n",
          savexml);
 }

 return training;

}

CvSVM* HoGProcessor::trainSVM(CvMat* pos_mat, CvMat* neg_mat, char *savexml, char *pos_file, char *neg_file) 
{    
	/* Read the feature vectors for positive samples */
	if (pos_file != NULL) 
	{
		printf("positive loading...\n");
		pos_mat = (CvMat*) cvLoad(pos_file);
		printf("positive loaded\n");
	}

	/* Read the feature vectors for negative samples */
	if (neg_file != NULL)
	{
		neg_mat = (CvMat*) cvLoad(neg_file);
		printf("negative loaded\n");
	}

	int n_positive, n_negative;
	n_positive = pos_mat->rows;
	n_negative = neg_mat->rows;
	int feature_vector_length = pos_mat->cols;
	int total_samples;
	
	total_samples = n_positive + n_negative;

	CvMat* trainData = cvCreateMat(total_samples, feature_vector_length, CV_32FC1);

	CvMat* trainClasses = cvCreateMat(total_samples, 1, CV_32FC1 );

	CvMat trainData1, trainData2, trainClasses1, trainClasses2;

	printf("Number of positive Samples : %d\n",
	pos_mat->rows);

	/*Copy the positive feature vectors to training
	data*/
	cvGetRows(trainData, &trainData1, 0, n_positive);
	cvCopy(pos_mat, &trainData1);
	cvReleaseMat(&pos_mat);

	/*Copy the negative feature vectors to training
	data*/
	cvGetRows(trainData, &trainData2, n_positive,total_samples);

	cvCopy(neg_mat, &trainData2);
	cvReleaseMat(&neg_mat);

	printf("Number of negative Samples : %d\n",	trainData2.rows);

	/*Form the training classes for positive and
	negative samples. Positive samples belong to class
	1 and negative samples belong to class 2 */
	cvGetRows(trainClasses, &trainClasses1, 0, n_positive);
	cvSet(&trainClasses1, cvScalar(1));

	cvGetRows(trainClasses, &trainClasses2, n_positive,	total_samples);
	cvSet(&trainClasses2, cvScalar(2));

	/* Train a linear support vector machine to learn from
	the training data. The parameters may played and
	experimented with to see their effects*/
	
	/*
	CvMat* class_weight = cvCreateMat(1, 1, CV_32FC1);
	(*(float*)CV_MAT_ELEM_PTR(*class_weight, 0, 0)) = 0;
	//(*(float*)CV_MAT_ELEM_PTR(*class_weight, 0, 1)) = 10;
	//(*(float*)CV_MAT_ELEM_PTR(*class_weight, 1, 0)) = 100;
	//(*(float*)CV_MAT_ELEM_PTR(*class_weight, 1, 1)) = 0;
*/

	CvSVM* svm = new CvSVM(trainData, trainClasses, 0, 0,
	CvSVMParams(CvSVM::C_SVC, CvSVM::LINEAR, 0, 0, 0, 2,
	0, 0, 0, cvTermCriteria(CV_TERMCRIT_EPS,0, 0.01)));	

	printf("SVM Training Complete!!\n");

	/*Save the learnt model*/
	if (savexml != NULL) 
	{
		svm->save(savexml);
	}

	cvReleaseMat(&trainClasses);
	cvReleaseMat(&trainData);

	return svm;
}

void HoGProcessor::writeFeatureVector(CvMat* mat,char* className,ofstream &fout)
{
	CvSize posSize = cvGetSize(mat);
	int numberOfSamples = posSize.height;
	int featureVectorLength = posSize.width;

	char buffer[200];
	char buffer2[200];
	

	for(int  i = 0; i < numberOfSamples; i++){
		fout<<className;
		for(int j = 0; j < featureVectorLength; j++)
		{			
			fout<<" ";
			float element = CV_MAT_ELEM( *mat, float, i, j);
			sprintf(buffer,"%d",j);
			fout<<buffer;
			fout<<":";

			sprintf(buffer2,"%f",element);
			fout<<buffer2;
		}

		fout<<"\n";
	}
}

CvMat* HoGProcessor::train_64x128(char *prefix, char *suffix, 
	CvSize window, int number_samples, int start_index,
       int end_index, char *savexml, int canny,
       int block, int normalization){

	 char filename[1000] = "\0", number[8];
	 int prefix_length;
	 prefix_length = strlen(prefix);
	 int bins = 9;

	 /* A default block size of 2x2 cells is considered */

	 int block_width = m_block.width , block_height = m_block.height;
	 
	 /* Calculation of the length of a feature vector for
	 an image (64x128 pixels)*/

	 int feature_vector_length;
	 feature_vector_length = (((window.width -  m_cell.width * block_width)/ (m_cell.width * m_fStepOverlap)) + 1) *
	   (((window.height - m_cell.height * block_height)
	   / (m_cell.height * m_fStepOverlap)) + 1) * (9 * m_block.height * m_block.width);
	 
	 /* Matrix to store the feature vectors for
	 all(number_samples) the training samples */

	 CvMat* training = cvCreateMat(number_samples,
		 feature_vector_length, CV_32FC1);

	 CvMat row;
	 CvMat* img_feature_vector;
	 IplImage** integrals;
	 int i = 0, j = 0;
	 
	 printf("Beginning to extract HoG features\n");
	 
	 strcat(filename, prefix);
	 
	 /* Loop to calculate hog features for each
	 image one by one */

	 for (i = start_index; i <= end_index; i++) 
	 {
	  itoa(i,number,10);	 
	  strcat(filename, number);
	  strcat(filename, suffix);
	  IplImage* im = cvLoadImage(filename);
	  
	  //Convert Image To 64x128
	  IplImage* dst = cvCreateImage(window,im->depth,im->nChannels);
	  cvResize(im,dst);
	  
	  /* Calculation of the integral histogram for
	  fast calculation of hog features*/
	  integrals = calculateIntegralHOG(dst);

	  cvGetRow(training, &row, j);

	  img_feature_vector
		= calculateHOG_window(integrals, cvRect(0, 0,
		  window.width, window.height), normalization);

	  cvCopy(img_feature_vector, &row);

	  j++;
	  printf("%s\n", filename);
	  filename[prefix_length] = '\0';
	  for (int k = 0; k < 9; k++) 
	  {
		cvReleaseImage(&integrals[k]);
	  }
	 }
	 if (savexml != NULL) 
	 {
		 cvSave(savexml, training);
	 }

	 return training;
}

/*
	This function takes in a window(64x128 pixels,
	but can be easily modified for other window sizes)
	and calculated the hog features for the window. It
	can be used to calculate the feature vector for a 
	64x128 pixel image as well. This window/image is the 
	training/detection window which is used for training 
	or on which the final detection is done. The hog features
	are computed by dividing the window into overlapping blocks,
	calculating the hog vectors for each block using calculateHOG_block(...)
	and concatenating the so obtained vectors to obtain the
	hog feature vector for the window
*/
CvMat* HoGProcessor::calculateHOG_window(IplImage **integrals, CvRect window, int normalization){
	/*
	 A cell size of 8x8 pixels is considered and each block
	 is divided into 2x2 such cells(i.e. the block is 16x16 pixels).
	 So a 64x128 pixels window would be divided into 7x15 overlapping blocks
	*/
	int block_start_x, block_start_y, cell_width = m_cell.width;
	int cell_height = m_cell.height;
	int block_width = m_block.width, block_height = m_block.height;

	/* The length of the feature vector for a cell is 9
	(since no. of bins is 9), for block it would be 
	9*(no. of cells in the block) = 9*4 = 36. And the lenght
	of the feature vector for a window would be 36*(no. of 
	blocks in the window)
	*/
	int lenghth_feature_of_block = 9 * m_block.height * m_block.width;
	int num_overlap_block_of_width = ((window.width - cell_width * block_width) / (cell_width * m_fStepOverlap)) + 1;
	int num_overlap_block_of_height = ((window.height - cell_height * block_height)/ (cell_height * m_fStepOverlap)) + 1;
	int total_block_of_window = num_overlap_block_of_width * num_overlap_block_of_height ;

	CvMat* window_feature_vector = cvCreateMat(1
		, total_block_of_window	* lenghth_feature_of_block
		, CV_32FC1);
	
	CvMat vector_block;
	int startcol = 0;

	for(block_start_y = window.y; 
		block_start_y <= window.y + window.height - cell_height * block_height; 
		block_start_y += cell_height * m_fStepOverlap)
	{//overllap

			for(block_start_x = window.x; 
				block_start_x <= window.x + window.width - cell_width * block_width;
				block_start_x += cell_width * m_fStepOverlap)
			{//overllap

			 cvGetCols(window_feature_vector, &vector_block, startcol, startcol+36);

			calculateHOG_block(cvRect(block_start_x,
				  block_start_y, cell_width * block_width, cell_height
				  * block_height), &vector_block, integrals, cvSize(
				  cell_width, cell_height), normalization);

			startcol+= lenghth_feature_of_block;
			}
	}

	return window_feature_vector;
}

/* This function takes in a block as a rectangle and
calculates the hog features for the block by dividing
it into cells of size cell( the supplied parameter),
calculating the hog features for each cell using the
function calulateHOG_rect(...), concatenating the so
obtained vectors for each cell and then normalizing over
the concatenated vector to obtain the hog features for a 
block */
void HoGProcessor::calculateHOG_block(CvRect block, CvMat *hog_block, IplImage **integrals, CvSize cell, int normalization){
	int cell_start_x, cell_start_y;
	CvMat vector_cell;
	int startcol = 0;
	for(cell_start_y = block.y; cell_start_y <= block.y + block.height - cell.height; cell_start_y += cell.height){
		for(cell_start_x = block.x; cell_start_x <=	block.x + block.width - cell.width;	cell_start_x += cell.width){
			cvGetCols(hog_block, &vector_cell, startcol, startcol + 9);

			calulateHOG_rect(cvRect(cell_start_x, cell_start_y,	cell.width, cell.height), &vector_cell, integrals, -1);

			startcol += 9;
		}
	}

	if(normalization != -1){
		cvNormalize(hog_block, hog_block, 1, 0, normalization);
	}
}

CvRect HoGProcessor::MergeRect(vector<CvRect> lstRect)
{
	int x = 0; 
	int y = 0;
	int width = 0;
	int height = 0;

	int total = 0;

	CvPoint leftPoint = cvPoint(999999,999999);
	CvPoint rightPoint = cvPoint(-1,-1);

	vector<CvRect>::iterator it;
	for(it = lstRect.begin(); it!=lstRect.end();it++)
	{	
		if(it->x < leftPoint.x)
			leftPoint.x = it->x;
		if(it->y < leftPoint.y)
			leftPoint.y = it->y;
		
		if((it->x + it->width) > rightPoint.x)
			rightPoint.x = (it->x + it->width);
		if((it->y + it->height) > rightPoint.y)
			rightPoint.y = it->y + it->height;
	}
	
	CvRect r = cvRect(leftPoint.x, leftPoint.y, rightPoint.x - leftPoint.x, rightPoint.y - leftPoint.y);

	return r;	
}

CvRect HoGProcessor::detectObject(CvSVM *svmModel, IplImage *input, IplImage *result, CvRect rectHead, int normalization){
	int StepWidth = 10;
	int StepHeight = 10;
	
	int nWindow = 3;
	
	int scaleWidth = 2;
	int scaleHeight = 2;

	ImageProcessor ip;

	//loai bo truong hop toc dai
	if(rectHead.height > rectHead.width)
		rectHead.height = rectHead.width;
	
	vector<CvRect> lstRect;

	CvRect rectHuman = cvRect(rectHead.x + rectHead.width/2 - rectHead.width*scaleWidth/2, 
		rectHead.y - 6, 
		rectHead.width*scaleWidth, 
		rectHead.height*scaleHeight);

	for(int i = 0; i < nWindow; i++)
	{
		for(int j = 0; j < nWindow; j++)
		{
			CvRect rect;
			rect.width = rectHuman.width + StepWidth*i;
			rect.height = rectHuman.height + StepHeight*j;
			rect.x = rectHuman.x - StepWidth*i/2;
			rect.y = rectHuman.y - StepHeight*j/2;
						
			if(rect.x < 0) rect.x = 0;
			if(rect.y < 0) rect.y = 0;
			if(rect.x + rect.width > input->width) rect.width = input->width - rect.x;
			if(rect.y + rect.height > input->height) rect.height = input->height - rect.y;

			CvMat* img_feature_vector;
			
			IplImage* candidate_img = ip.getSubImageAndResize(input, rect, 48, 48);
			if(candidate_img)
			{
				IplImage **newIntegrals = calculateIntegralHOG(candidate_img);

				img_feature_vector = calculateHOG_window(newIntegrals,cvRect(0,0,48,48),4);
				
				for (int k = 0; k < 9; k++)
				{
					cvReleaseImage(&newIntegrals[k]);				
				}
				cvReleaseImage(newIntegrals);
				cvReleaseImage(&candidate_img);

				double predict_rs = svmModel->predict(img_feature_vector, true);
				//double predict_rs = svmModel->predict(img_feature_vector);
				
				//if(predict_rs == 1)
				if(predict_rs >= -0.2)
				{
					lstRect.push_back(rect);		
					//cvRectangle(result, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y+rect.height), CV_RGB(0,255,0));
				}	
				else
					//cvRectangle(result, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y+rect.height), CV_RGB(0,0,255));
								
				cvReleaseMat(&img_feature_vector);	
			}				
		}
	}	

	if(lstRect.size() > 0)
	{		
		return MergeRect(lstRect);				
	}	
	return cvRect(0,0,-1,-1);	
}

/*
 The following function takes as input the rectangular cell
 for which the histogram of oriented gradients has to be calculated, 
 a matrix hog_cell of dimensions 1x9 to store the bin values for
 the histogram, the integral histogram, and the normalization scheme
 to be used. No normalization is done if nomalization = -1
*/
void HoGProcessor::calulateHOG_rect(CvRect cell, CvMat* hog_cell, IplImage** integrals,int normalization){
	/*Calculate the bin values for each of the bin of the histogram one by one*/
	char buffer[50];
	float* temp;
	for(int i = 0; i < 9; i++){
		float a = ((double*)(integrals[i]->imageData + (cell.y)*(integrals[i]->widthStep)))[cell.x];		
		//float a = CV_IMAGE_ELEM(integrals[i],double,cell.y,cell.x);
			
		float b = ((double*)(integrals[i]->imageData + (cell.y + cell.height)*(integrals[i]->widthStep)))[cell.x + cell.width];
		//float b = CV_IMAGE_ELEM(integrals[i],double,cell.y + cell.height,cell.x+cell.width);
	
		float c = ((double*)(integrals[i]->imageData + (cell.y)*(integrals[i]->widthStep)))[cell.x + cell.width];
		//float c= CV_IMAGE_ELEM(integrals[i], double, cell.y, cell.x+cell.width);
		
		float d = ((double*)(integrals[i]->imageData + (cell.y + cell.height)*(integrals[i]->widthStep)))[cell.x];
		//float d = CV_IMAGE_ELEM(integrals[i],double,cell.y+cell.height,cell.x);
		
		((float*)hog_cell->data.fl)[i] = (a+b)-(c+d);
	}

	/* Nomalize the matrix */
	if(normalization != -1){
		cvNormalize(hog_cell,hog_cell,1,0,normalization);
	}
}

IplImage* HoGProcessor::doSobel(IplImage *in, int xorder, int yorder, int aperture_size){
	IplImage* derivative = cvCreateImage(cvGetSize(in), IPL_DEPTH_32F, 1);
	cvSobel(in, derivative, xorder, yorder, aperture_size);
	return derivative;
}

IplImage**  HoGProcessor::calculateIntegralHOG(IplImage *in){
	/* Convert the input image to grayscale*/
	IplImage* img_gray = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U, 1);
	cvCvtColor(in, img_gray, CV_BGR2GRAY);
	cvEqualizeHist(img_gray, img_gray);

	/* Calculate the derivatives of the grayscale image in the x and y
	directions using a sobel operator and obtain 2 gradient images for 
	the x and y directions*/
	IplImage *xsobel, *ysobel;
	xsobel = doSobel(img_gray, 1, 0, 3);
	ysobel = doSobel(img_gray, 0, 1, 3);
	cvReleaseImage(&img_gray);

	/* Create an array of 9 images(9 because I assume bin size 20
	degrees and unsigned gradient (180/2 = 9), one for each bin
	which will have zeroes for all pixels, except for the pixels
	in the original image for which the gradient values correspond
	to the particular bin. These will be referred to as bin images.
	*/
	IplImage** bins = (IplImage**)malloc(9*sizeof(IplImage*));
	for(int i = 0; i < 9; i++){
		bins[i] = cvCreateImage(cvGetSize(in), IPL_DEPTH_32F, 1);
		cvSetZero(bins[i]);
	}

	/* Create an array of 9 images (note the dimensions of the image,
	the cvIntegral() function requires the size to be that), to store
	the integral images calculated from the above bin images. These 9 
	integral images together constitute the integral histogram */
	IplImage** integrals = (IplImage**)malloc(9*sizeof(IplImage*));
	for(int i = 0; i < 9; i++){
		integrals[i] = cvCreateImage(cvSize(in->width+1, in->height +1), IPL_DEPTH_64F,1);
	}

	/*Calculate the bin images. The magnitude and orientation of the gradient
	at each pixel is calculated using the xsobel and ysobel images.{
	Magnitude = sqrt(sq(xsobel) + sq(ysobel)), 
	gradient = itan( ysobel/ xsobel)}. Then according to the orientation
	of the gradient, the value of the corresponding pixel in the 
	corresponding image is set*/
	char buffer[50];
	int x, y;
	float xDevValue, yDevValue;
	float temp_gradient, temp_magnitude;
	float** ptrs = (float**)malloc(9*sizeof(float*));

	for(y = 0; y < in->height; y++){
		/*ptr1 and ptr2 point to beginning of the current row in the xsobel
		and ysobel images respectively. ptrs[i] point to the beginning of the 
		current rows in the bin images*/		
		
		for(int i = 0; i < 9; i++){
			ptrs[i] = (float*)(bins[i]->imageData + y*(bins[i]->widthStep));
		}
		
		/*For every pixel in a row gradient orientation and magnitude are
		calculated and corresponding values set for the bin images.*/
		
		for( x = 0; x < in->width; x++){
			/*
			 if the xsobel derivative is zero for a pixel , a small value is added to it,
			 to avoid division by zero, atan returns values in radians, which on being
			 converted to degrees, correspond to values between -90 and 90 degrees.
			 90 is added to each orientation, to shift the orientation values range from
			 {-90 -90} to {0 -180}. This is just a matter of convention.
			 {-90 -90} values can also be used for the calulation
			*/
			
			//xDevValue = CV_IMAGE_ELEM(xsobel, float, y,  x);
			xDevValue = ((float*)(xsobel->imageData + y*(xsobel->widthStep)))[x];

			//yDevValue = CV_IMAGE_ELEM(ysobel, float, y,  x);
			yDevValue = ((float*)(ysobel->imageData + y*(ysobel->widthStep)))[x];

			if(xDevValue == 0){
				temp_gradient = ((atan(yDevValue/(xDevValue + 0.00001)))*(180/PI)) + 90;
			} else {
				temp_gradient = ((atan(yDevValue/xDevValue))*(180/PI)) + 90;
			}

			temp_magnitude = sqrt((xDevValue*xDevValue)+ (yDevValue*yDevValue));			

			/* The bin image is selected according to the gradient values. The 
			corresponding pixel value is made equal to the gradient magnitude
			at that pixel in the corresponding bin image
			*/

			if(temp_gradient <= 20){
				ptrs[0][x] = temp_magnitude;
			} else if ( temp_gradient <= 40){
				ptrs[1][x] = temp_magnitude;
			} else if ( temp_gradient <= 60){
				ptrs[2][x] = temp_magnitude;
			} else if ( temp_gradient <= 80) {
				ptrs[3][x] = temp_magnitude;
			} else if ( temp_gradient <= 100) {
				ptrs[4][x] = temp_magnitude;
			} else if ( temp_gradient <= 120) {
				ptrs[5][x] = temp_magnitude;
			} else if ( temp_gradient <= 140) {
				ptrs[6][x] = temp_magnitude;
			} else if ( temp_gradient <= 160) {
				ptrs[7][x] = temp_magnitude;
			} else if (temp_gradient <= 180) {
				ptrs[8][x] = temp_magnitude;
			}
		}
	}
	
	cvReleaseImage(&xsobel);
	cvReleaseImage(&ysobel);

	/*Integral images for each of the bin images are calculated*/
	for(int i = 0; i < 9; i++){
		cvIntegral(bins[i],integrals[i]);
	}

	for(int i = 0; i < 9; i++){
		cvReleaseImage(&bins[i]);
	}
	cvReleaseImage(bins);	
	
	free(ptrs);

	/* The function returns an array of 9 images which constitue the
	integral histogram*/
	return (integrals);
}