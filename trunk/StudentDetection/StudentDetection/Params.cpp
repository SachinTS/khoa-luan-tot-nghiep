#include "stdafx.h"
#include "Params.h"

Params::Params(void)
{
	
}

Params::~Params(void)
{
}

int Params::LoadParamsFromXML(char fileName[])
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
		pElem->QueryIntAttribute("cell_width", &m_HOG_Params.m_cell.width);
		pElem->QueryIntAttribute("cell_height", &m_HOG_Params.m_cell.height);
		pElem->QueryIntAttribute("block_width", &m_HOG_Params.m_block.width);
		pElem->QueryIntAttribute("block_height", &m_HOG_Params.m_block.height);
		pElem->QueryIntAttribute("total_bin", &m_HOG_Params.m_inBin);
		pElem->QueryFloatAttribute("overlap", &m_HOG_Params.m_fStepOverlap);
	}
	
	pElem = hRoot.FirstChild("SVMParams").Element();
	if (pElem)
	{
		//read params
	}
	
	pElem = hRoot.FirstChild("GaussianParams").Element();
	if (pElem)
	{		
		pElem->QueryFloatAttribute("threshold", &m_Gaussian_Params.m_fThreshold);
	}

	pElem = hRoot.FirstChild("HeadParams").Element();
	if (pElem)
	{		
		pElem->QueryIntAttribute("min_area", &m_Head_Params.m_iMinHeadArea);
		pElem->QueryIntAttribute("max_area", &m_Head_Params.m_iMaxHeadArea);
		pElem->QueryIntAttribute("relative_width_height", &m_Head_Params.m_iRelative_Width_Height);
		pElem->QueryIntAttribute("relative_height_width", &m_Head_Params.m_iRelative_Height_Width);
		pElem->QueryIntAttribute("max_width", &m_Head_Params.m_iMaxWidth);
		pElem->QueryIntAttribute("min_width", &m_Head_Params.m_iMinWidth);
		pElem->QueryIntAttribute("max_head_area_top", &m_Head_Params.m_iMaxHeadAreaTop);
		pElem->QueryIntAttribute("min_head_area_bottom", &m_Head_Params.m_iMinHeadAreaBottom);
	}
	
	return 0;
}