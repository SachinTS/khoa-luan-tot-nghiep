// HeadParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "HeadParamDlg.h"
#include "Utils.h"

// HeadParamDlg dialog

IMPLEMENT_DYNAMIC(HeadParamDlg, CDialog)

HeadParamDlg::HeadParamDlg(CWnd* pParent )
	: CDialog(HeadParamDlg::IDD, pParent)
{
	
}

HeadParamDlg::~HeadParamDlg()
{
}

BOOL HeadParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_params.LoadParamsFromXML("config.xml");
	Utils utils;
	m_editMaxHeadArea.SetWindowTextW(utils.ConvertToCString(m_params.m_DetectionParams.m_Head_Params.m_iMaxHeadArea));
	m_editMaxHeadAreaAtTop.SetWindowTextW(utils.ConvertToCString(m_params.m_DetectionParams.m_Head_Params.m_iMaxHeadAreaTop));
	m_editMaxWidthHead.SetWindowTextW(utils.ConvertToCString(m_params.m_DetectionParams.m_Head_Params.m_iMaxWidth));
	m_editMinAreaAtBottom.SetWindowTextW(utils.ConvertToCString(m_params.m_DetectionParams.m_Head_Params.m_iMinHeadAreaBottom));
	m_editMinHeadArea.SetWindowTextW(utils.ConvertToCString(m_params.m_DetectionParams.m_Head_Params.m_iMinHeadArea));
	m_editMinWidthHead.SetWindowTextW(utils.ConvertToCString(m_params.m_DetectionParams.m_Head_Params.m_iMinWidth));
	m_editRelativeHeightWidth.SetWindowTextW(utils.ConvertToCString(m_params.m_DetectionParams.m_Head_Params.m_iRelative_Height_Width));
	m_editRelativeWidthHeight.SetWindowTextW(utils.ConvertToCString(m_params.m_DetectionParams.m_Head_Params.m_iRelative_Width_Height));

	return TRUE;
}


void HeadParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_EDIT_MIN_WIDTH_HEAD, m_editMinWidthHead);
	DDX_Control(pDX, IDC_EDIT_MAX_WIDTH_HEAD, m_editMaxWidthHead);
	DDX_Control(pDX, IDC_EDIT_MIN_HEAD_AREA, m_editMinHeadArea);
	DDX_Control(pDX, IDC_EDIT_MAX_HEAD_AREA, m_editMaxHeadArea);
	DDX_Control(pDX, IDC_EDIT_MAX_AREA_AT_TOP, m_editMaxHeadAreaAtTop);
	DDX_Control(pDX, IDC_EDIT_MIN_AREA_AT_BOTTOM, m_editMinAreaAtBottom);
	DDX_Control(pDX, IDC_EDIT_RELATIVE_WIDTH_HEIGHT, m_editRelativeWidthHeight);
	DDX_Control(pDX, IDC_EDIT_RELATIVE_HEIGHT_WIDTH, m_editRelativeHeightWidth);
}


BEGIN_MESSAGE_MAP(HeadParamDlg, CDialog)	
END_MESSAGE_MAP()


// HeadParamDlg message handlers

