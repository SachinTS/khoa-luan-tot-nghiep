#include "Utils.h"

Utils::Utils(void)
{
}

Utils::~Utils(void)
{
}

void Utils::OutputResult(IplImage* img, vector<CvRect> vRect, CvScalar color){	
	vector<CvRect>::iterator iter;
	for(iter = vRect.begin(); iter != vRect.end(); iter++){
		cvRectangle(img, cvPoint(iter->x, iter->y), cvPoint(iter->x + iter->width,iter->y + iter->height), color, 2);
	}
}

vector<CvRect> Utils::ConnectOverlapRects(vector<CvRect> inVector){
	if(inVector.size()>0){
		vector<CvRect> result;
		vector<CvRect>::iterator iter1, iter2;
		
		for(iter1 = inVector.begin(); iter1 != inVector.end()-1; )
		{			
			for(iter2 = iter1 + 1; iter2 != inVector.end(); )
			{
				CvRect tmp = cvRectIntersection(*iter1, *iter2);
				if(tmp.width > 0 && tmp.height > 0)
				{
					int areaOverlap = tmp.width * tmp.height;
					int area1 = iter1->width * iter1->height;
					int area2 = iter2->width * iter2->height;
					if(areaOverlap*1.0/area1 >= 0.7 || areaOverlap*1.0/area2 >= 0.7)
					{
						//merge rect
						iter1->x = (iter1->x < iter2->x)?iter1->x:iter2->x;
						iter1->y = (iter1->y < iter2->y)?iter1->y:iter2->y;
						iter1->width = iter1->width + iter2->width - tmp.width;
						iter1->height = iter2->height + iter1->height - tmp.height;

						iter2 = inVector.erase(iter2);
					}
					else
					{
						iter2++;
					}
				}
				else
					iter2++;
			}
			result.push_back(*iter1);
			iter1++;
			if(iter1 == iter2)
				break;
		}
		
		if(iter1 != inVector.end()){
			result.push_back(*iter1);
		}

		return result;
	}
	else
		return inVector;
}

bool Utils::CheckRectHead(CvRect rectHead, int iMaxHeight, Head_Params headParams){

	if(rectHead.width > headParams.m_iMaxWidth || rectHead.width < headParams.m_iMinWidth)
		return false;

	if((rectHead.width / rectHead.height >= headParams.m_iRelative_Width_Height) 
		|| (rectHead.height / rectHead.width >= headParams.m_iRelative_Height_Width) 
		|| ((rectHead.width * rectHead.height) < headParams.m_iMinAreaHead)
		|| (rectHead.width * rectHead.height > headParams.m_iMaxAreaHead)){		
		return false;
	}

	if(rectHead.y < iMaxHeight/3)
	{		
		if(rectHead.width * rectHead.height >= 1800){
			return false;
		}
	}
	else
	{
	/*
		if(rectHead.y < (1.0*2*iMaxHeight)/3)
		{						
			if(rectHead.width * rectHead.height > 2500 
				|| rectHead.width * rectHead.height < 500)
			{
				return false;
			}
		}
		else
		{			
			if(rectHead.width * rectHead.height <= 1200){
				return false;
			}													
		}		
	*/	
	}
	
	return true;
}