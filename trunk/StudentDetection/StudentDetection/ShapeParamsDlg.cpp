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
	m_sliderThreshold.SetRange(11, 99);
	m_sliderLength.SetRange(2, 10);
	m_sliderThreshold.SetPos(CStudentDetectionDlg::detector->threshold*100);
	m_sliderLength.SetPos(CStudentDetectionDlg::detector->l);
	return TRUE;
}

void CShapeParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, m_sliderThreshold);
	DDX_Control(pDX, IDC_SLIDER_LENGTH, m_sliderLength);
	DDX_Slider(pDX, IDC_SLIDER_THRESHOLD, m_iThreshold);
	DDX_Slider(pDX, IDC_SLIDER_LENGTH, m_iLength);
	DDX_Control(pDX, IDC_STATIC_THRESHOLD, m_sttThreshold);
	DDX_Control(pDX, IDC_STATIC_LENGTH, m_sttLength);
}



BEGIN_MESSAGE_MAP(CShapeParamsDlg, CDialog)
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void CShapeParamsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Utils utils;
	
	if (nPos >= 2 && nPos <= 10) {
		m_sttLength.SetWindowTextW(_T(""));
		m_sttLength.SetWindowTextW(utils.ConvertToCString((int)nPos));
		CStudentDetectionDlg::detector->l = nPos;
	}
	else if (nPos >= 11 && nPos <= 99) {
		m_sttThreshold.SetWindowTextW(_T(""));
		m_sttThreshold.SetWindowTextW(utils.ConvertToCString((float)(nPos*1.0/100)));
		CStudentDetectionDlg::detector->threshold = (float)nPos*1.0/100;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// HeadParamDlg message handlers
HBRUSH CShapeParamsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr;

	if( nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject( NULL_BRUSH );
	}

	else
	{
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	
	return hbr;
}

BOOL CShapeParamsDlg::OnEraseBkgnd(CDC* pDC)
{
	CDialog::OnEraseBkgnd(pDC);
	SBitdraw(pDC,IDB_BMP_BACKGROUND1);
	return true;
	
}

bool CShapeParamsDlg::SBitdraw(CDC *pDC, UINT nIDResource)
{
	CBitmap* m_bitmap;
	m_bitmap=new CBitmap();
	m_bitmap->LoadBitmap(nIDResource);
	if(!m_bitmap->m_hObject)
		return true;
	CRect rect;
	GetClientRect(&rect);
	CDC dc;
	dc.CreateCompatibleDC(pDC);	
	dc.SelectObject(m_bitmap);

	int xo=0, yo=0;
	pDC->BitBlt(xo, yo, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY);
	return true;
}
