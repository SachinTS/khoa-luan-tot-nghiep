// ShapeParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "ShapeParamsDlg.h"
#include "Utils.h"
#include "StudentDetectionDlg.h"


// CShapeParamsDlg dialog

IMPLEMENT_DYNAMIC(CShapeParamsDlg, CDialog)

CShapeParamsDlg::CShapeParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShapeParamsDlg::IDD, pParent)
{
}

CShapeParamsDlg::~CShapeParamsDlg()
{
}

BOOL CShapeParamsDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	Utils utils;
	m_sttThreshold.SetWindowTextW(utils.ConvertToCString(CStudentDetectionDlg::detector->threshold));
	m_sttLength.SetWindowTextW(utils.ConvertToCString(CStudentDetectionDlg::detector->l));
	m_sliderThreshold.SetRange(100, 200);
	m_sliderLength.SetRange(2, 10);
	m_sliderThreshold.SetPos(CStudentDetectionDlg::detector->threshold*1000);
	m_sliderLength.SetPos(CStudentDetectionDlg::detector->l);
	return TRUE;
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
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CShapeParamsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Utils utils;
	
	if (nPos >= 2 && nPos <= 10) {
		m_sttLength.SetWindowTextW(_T(""));
		m_sttLength.SetWindowTextW(utils.ConvertToCString((int)nPos));
		CStudentDetectionDlg::detector->l = nPos;
	}
	else if (nPos >= 100 && nPos <= 200) {
		m_sttThreshold.SetWindowTextW(_T(""));
		m_sttThreshold.SetWindowTextW(utils.ConvertToCString((float)(nPos*1.0/1000)));
		CStudentDetectionDlg::detector->threshold = (float)nPos*1.0/1000;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
