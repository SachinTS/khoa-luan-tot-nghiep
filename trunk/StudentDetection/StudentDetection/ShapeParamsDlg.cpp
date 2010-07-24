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
	m_editThreshold.SetWindowTextW(utils.ConvertToCString(CStudentDetectionDlg::detector->threshold));
	m_editLength.SetWindowTextW(utils.ConvertToCString(CStudentDetectionDlg::detector->l));
	m_editGaussianThreshold.SetWindowTextW(utils.ConvertToCString(CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Gaussian_Params.m_fThreshold));
	m_editConfidenceScore.SetWindowTextW(utils.ConvertToCString(CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_SVM_Params.m_fConfidenceScore));
	
	m_sliderThreshold.SetRange(11, 99);
	m_sliderLength.SetRange(2, 10);
	m_sliderGaussianThreshold.SetRange(500, 800);
	m_sliderConfidenceScore.SetRange(200, 600);
	
	m_sliderThreshold.SetPos(CStudentDetectionDlg::detector->threshold*100);
	m_sliderLength.SetPos(CStudentDetectionDlg::detector->l);
	m_sliderGaussianThreshold.SetPos(CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Gaussian_Params.m_fThreshold*-100);		
	m_sliderConfidenceScore.SetPos((CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_SVM_Params.m_fConfidenceScore + 4)*100);
	return TRUE;
}

void CShapeParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, m_sliderThreshold);
	DDX_Control(pDX, IDC_SLIDER_LENGTH, m_sliderLength);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD, m_editThreshold);
	DDX_Control(pDX, IDC_EDIT_LENGTH, m_editLength);
	DDX_Control(pDX, IDC_EDIT_GAUSSIAN_THRESHOLD, m_editGaussianThreshold);
	DDX_Control(pDX, IDC_SLIDER_GAUSSIAN_THRESHOLD, m_sliderGaussianThreshold);
	DDX_Control(pDX, IDC_SLIDER_SVM_CONFIDENCESCORE, m_sliderConfidenceScore);
	DDX_Control(pDX, IDC_EDIT_SVM_CONFIDENCESCORE, m_editConfidenceScore);
}



BEGIN_MESSAGE_MAP(CShapeParamsDlg, CDialog)
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void CShapeParamsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Utils utils;
	int id = pScrollBar->GetDlgCtrlID();
	switch(id)
	{
	case IDC_SLIDER_SVM_CONFIDENCESCORE:
		m_editConfidenceScore.SetWindowTextW(utils.ConvertToCString((float)(nPos*-1.0/100)));
		CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_SVM_Params.m_fConfidenceScore = (float)nPos*1.0/100 - 4;
		break;
	case IDC_SLIDER_GAUSSIAN_THRESHOLD:
		m_editGaussianThreshold.SetWindowTextW(utils.ConvertToCString((float)(nPos*-1.0/100)));
		CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Gaussian_Params.m_fThreshold = (float)nPos*-1.0/100;
		break;
	case IDC_SLIDER_LENGTH:
		m_editLength.SetWindowTextW(utils.ConvertToCString((int)nPos));
		CStudentDetectionDlg::detector->l = nPos;
		break;
	case IDC_SLIDER_THRESHOLD:
		m_editThreshold.SetWindowTextW(utils.ConvertToCString((float)(nPos*1.0/100)));
		CStudentDetectionDlg::detector->threshold = (float)nPos*1.0/100;
		break;
	default:
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
		break;
	}
}

// HeadParamDlg message handlers
HBRUSH CShapeParamsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
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
	if(id == IDC_EDIT_LENGTH || id == IDC_EDIT_THRESHOLD || id == IDC_EDIT_GAUSSIAN_THRESHOLD || id == IDC_EDIT_SVM_CONFIDENCESCORE)
	{		
		pDC->SetBkMode(TRANSPARENT);		  
		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);	
	}

    return hbr;
}

BOOL CShapeParamsDlg::OnEraseBkgnd(CDC* pDC)
{	
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
