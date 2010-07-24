#pragma once
#include "cv.h"
#include "tinyxml.h"
#include "tinystr.h"

struct HOG_Params{
	CvSize m_cell;
	CvSize m_block;
	float m_fStepOverlap;
	int m_inBin;
};

struct SVM_Params{
	float m_fConfidenceScore;
};

struct Gaussian_Params{
	float m_fThreshold;
};

struct Head_Params{
	int m_iMinHeadArea;
	int m_iMaxHeadArea;
	int m_iMaxHeadAreaTop;
	int m_iMinHeadAreaBottom;
	int m_iRelative_Width_Height;
	int m_iRelative_Height_Width;
	int m_iMaxWidth;
	int m_iMinWidth;
};
class Params
{
public:
	HOG_Params m_HOG_Params;
	Gaussian_Params m_Gaussian_Params;
	SVM_Params m_SVM_Params;
	Head_Params m_Head_Params;		

	int LoadParamsFromXML(char fileName[]);
	Params(void);
	~Params(void);
};
