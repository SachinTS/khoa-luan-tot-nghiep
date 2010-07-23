// ShapeParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "ShapeParamsDlg.h"


// CShapeParamsDlg dialog

IMPLEMENT_DYNAMIC(CShapeParamsDlg, CDialog)

CShapeParamsDlg::CShapeParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShapeParamsDlg::IDD, pParent)
{
}

CShapeParamsDlg::~CShapeParamsDlg()
{
}

void CShapeParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, m_sliderThreshold);
	DDX_Control(pDX, IDC_SLIDER_LENGTH, m_sliderLength);
	DDX_Slider(pDX, IDC_SLIDER_THRESHOLD, m_iThreshold);
	DDX_Slider(pDX, IDC_SLIDER_LENGTH, m_iLength);
	DDX_Control(pDX, IDC_STATIC_THRESHOLD, m_sttThreshold);
	DDX_Control(pDX, IDC_STATIC_LENGTH, m_sttLength);
}


BEGIN_MESSAGE_MAP(CShapeParamsDlg, CDialog)
END_MESSAGE_MAP()


// CShapeParamsDlg message handlers
