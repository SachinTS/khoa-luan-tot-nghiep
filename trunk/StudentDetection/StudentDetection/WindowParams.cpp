#include "StdAfx.h"
#include "WindowParams.h"

WindowParams::WindowParams(void)
{
	m_modelGaussPath = NULL;
	m_modelSVMPath = NULL;
	m_maskPath = NULL;
	m_videoPath = NULL;

	m_isStopVideo = false;
}

WindowParams::~WindowParams(void)
{
	if(m_modelGaussPath != NULL)
		delete[] m_modelGaussPath;
	if(m_modelSVMPath != NULL)
		delete[] m_modelSVMPath;
	if(m_maskPath != NULL)
		delete[] m_maskPath;
	if(m_videoPath != NULL)
		delete[] m_videoPath;
}

int WindowParams::LoadParamsFromXML(char fileName[])
{		
	TiXmlDocument doc(fileName);
	if (!doc.LoadFile()) return -1;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem) return -1;
	hRoot = TiXmlHandle(pElem);	
	
	pElem = hRoot.FirstChild("HOGParams").Element();
	if (pElem)
	{
		pElem->QueryIntAttribute("cell_width", &m_DetectionParams.m_HOG_Params.m_cell.width);
		pElem->QueryIntAttribute("cell_height", &m_DetectionParams.m_HOG_Params.m_cell.height);
		pElem->QueryIntAttribute("block_width", &m_DetectionParams.m_HOG_Params.m_block.width);
		pElem->QueryIntAttribute("block_height", &m_DetectionParams.m_HOG_Params.m_block.height);
		pElem->QueryIntAttribute("total_bin", &m_DetectionParams.m_HOG_Params.m_inBin);
		pElem->QueryFloatAttribute("overlap", &m_DetectionParams.m_HOG_Params.m_fStepOverlap);
	}
		
	pElem = hRoot.FirstChild("SVMParams").Element();
	if (pElem)
	{
		pElem->QueryFloatAttribute("confidence_score", &m_DetectionParams.m_SVM_Params.m_fConfidenceScore);
	}
	
	pElem = hRoot.FirstChild("GaussianParams").Element();
	if (pElem)
	{		
		pElem->QueryFloatAttribute("threshold", &m_DetectionParams.m_Gaussian_Params.m_fThreshold);
	}

	pElem = hRoot.FirstChild("HeadParams").Element();
	if (pElem)
	{		
		pElem->QueryIntAttribute("min_area", &m_DetectionParams.m_Head_Params.m_iMinHeadArea);
		pElem->QueryIntAttribute("max_area", &m_DetectionParams.m_Head_Params.m_iMaxHeadArea);
		pElem->QueryIntAttribute("relative_width_height", &m_DetectionParams.m_Head_Params.m_iRelative_Width_Height);
		pElem->QueryIntAttribute("relative_height_width", &m_DetectionParams.m_Head_Params.m_iRelative_Height_Width);
		pElem->QueryIntAttribute("max_width", &m_DetectionParams.m_Head_Params.m_iMaxWidth);
		pElem->QueryIntAttribute("min_width", &m_DetectionParams.m_Head_Params.m_iMinWidth);
		pElem->QueryIntAttribute("max_head_area_top", &m_DetectionParams.m_Head_Params.m_iMaxHeadAreaTop);
		pElem->QueryIntAttribute("min_head_area_bottom", &m_DetectionParams.m_Head_Params.m_iMinHeadAreaBottom);
	}

	const char* tmp;
	pElem = hRoot.FirstChild("ModelSVM").Element();
	if (pElem)
	{		
		tmp = pElem->GetText();
		m_modelSVMPath = new char[strlen(tmp)];
		strcpy(m_modelSVMPath,pElem->GetText());
	}
	
	pElem = hRoot.FirstChild("ModelGauss").Element();
	if (pElem)
	{	
		tmp = pElem->GetText();
		m_modelGaussPath = new char[strlen(tmp)];
		strcpy( m_modelGaussPath, pElem->GetText());
	}	
	return 0;
}
