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
	m_editThreshold.SetWindowTextW(utils.ConvertToCString(CStudentDetectionDlg::detector->threshold));
	m_editLength.SetWindowTextW(utils.ConvertToCString(CStudentDetectionDlg::detector->l));
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
	DDX_Control(pDX, IDC_EDIT_THRESHOLD, m_editThreshold);
	DDX_Control(pDX, IDC_EDIT_LENGTH, m_editLength);
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
		m_editLength.SetWindowTextW(_T(""));
		m_editLength.SetWindowTextW(utils.ConvertToCString((int)nPos));
		CStudentDetectionDlg::detector->l = nPos;
	}
	else if (nPos >= 11 && nPos <= 99) {
		m_sttThreshold.SetWindowTextW(_T(""));
		m_sttThreshold.SetWindowTextW(utils.ConvertToCString((float)(nPos*1.0/100)));
		m_editThreshold.SetWindowTextW(_T(""));
		m_editThreshold.SetWindowTextW(utils.ConvertToCString((float)(nPos*1.0/100)));
		CStudentDetectionDlg::detector->threshold = (float)nPos*1.0/100;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// HeadParamDlg message handlers
HBRUSH CShapeParamsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	/*
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
	*/
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

    DWORD dwStyle = pWnd->GetStyle();
    if ( dwStyle & (BS_AUTORADIOBUTTON | BS_RADIOBUTTON | BS_AUTOCHECKBOX |BS_CHECKBOX) )
	{
        hbr = (HBRUSH) GetStockObject (WHITE_BRUSH);
    }
	else
	{
		switch(nCtlColor)
		{
		case CTLCOLOR_STATIC:
		case CTLCOLOR_EDIT:
			pDC->SetBkMode(TRANSPARENT);			
			hbr = (HBRUSH)GetStockObject( NULL_BRUSH );			
			break;	
		default:
			hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
			break;
		}
	}

	int id = pWnd->GetDlgCtrlID();
	if(id == IDC_EDIT_LENGTH || id == IDC_EDIT_THRESHOLD)
	{		
		pDC->SetBkMode(TRANSPARENT);		  
		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);	
	}

    return hbr;
}

BOOL CShapeParamsDlg::OnEraseBkgnd(CDC* pDC)
{
	/*
	SBitdraw(pDC,IDB_BMP_BACKGROUND1);
	CDialog::OnEraseBkgnd(pDC);
	return TRUE;
	*/
	CBitmap m_bitmap;
	BOOL rVal = FALSE;

	if( m_bitmap.LoadBitmap( IDB_BMP_BACKGROUND1 ) )
	{
		CRect rect;
		GetClientRect( &rect );

		CDC dc;
		dc.CreateCompatibleDC( pDC );
		CBitmap* pOldBitmap = dc.SelectObject( &m_bitmap );

		pDC->BitBlt( 0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);

		dc.SelectObject(pOldBitmap);
		rVal = TRUE;
	}

	return rVal;
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
