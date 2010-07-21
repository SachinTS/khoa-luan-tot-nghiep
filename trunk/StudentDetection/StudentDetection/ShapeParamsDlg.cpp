// ShapeParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "ShapeParamsDlg.h"


// CShapeParamsDlg dialog

IMPLEMENT_DYNAMIC(CShapeParamsDlg, CDialog)

CShapeParamsDlg::CShapeParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShapeParamsDlg::IDD, pParent)
	, m_sThreshold(_T(""))
	, m_sLength(_T(""))
	, m_iThreshold(0)
	, m_iLength(0)
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
	DDX_Control(pDX, IDC_EDIT_THRESHOLD, m_editThreshold);
	DDX_Control(pDX, IDC_EDIT_LENGTH, m_editLength);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_sThreshold);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_sLength);
	DDX_Slider(pDX, IDC_SLIDER_THRESHOLD, m_iThreshold);
	DDX_Slider(pDX, IDC_SLIDER_LENGTH, m_iLength);
}


BEGIN_MESSAGE_MAP(CShapeParamsDlg, CDialog)
END_MESSAGE_MAP()


// CShapeParamsDlg message handlers
