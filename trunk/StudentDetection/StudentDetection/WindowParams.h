#pragma once
#include "Params.h"
class WindowParams
{
public:
	Params m_DetectionParams;
	int m_isViewHairDetection;
	int m_isViewSVMDetection;
	int m_isViewShapeDetection;
	int m_isStopVideo;

	char* m_videoPath;
	char* m_maskPath;
	char* m_modelSVMPath;
	char* m_modelGaussPath;

	HWND m_hWnd;

	WindowParams(void);
	~WindowParams(void);
	int LoadParamsFromXML(char fileName[]);
};
