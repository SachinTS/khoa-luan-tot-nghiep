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

HeadParamDlg::HeadParamDlg(Head_Params params, CWnd *pParent)
{
	m_params = params;
}

BOOL HeadParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	Utils utils;
	m_editMaxHeadArea.SetWindowTextW(utils.ConvertToCString(m_params.m_iMaxHeadArea));
	m_editMaxHeadAreaAtTop.SetWindowTextW(utils.ConvertToCString(m_params.m_iMaxHeadAreaTop));
	m_editMaxWidthHead.SetWindowTextW(utils.ConvertToCString(m_params.m_iMaxWidth));
	m_editMinAreaAtBottom.SetWindowTextW(utils.ConvertToCString(m_params.m_iMinHeadAreaBottom));
	m_editMinHeadArea.SetWindowTextW(utils.ConvertToCString(m_params.m_iMinHeadArea));
	m_editMinWidthHead.SetWindowTextW(utils.ConvertToCString(m_params.m_iMinWidth));
	m_editRelativeHeightWidth.SetWindowTextW(utils.ConvertToCString(m_params.m_iRelative_Height_Width));
	m_editRelativeWidthHeight.SetWindowTextW(utils.ConvertToCString(m_params.m_iRelative_Width_Height));

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
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// HeadParamDlg message handlers
HBRUSH HeadParamDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	HBRUSH hbr;

	if( nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(255, 0, 0));
		hbr = (HBRUSH)GetStockObject( NULL_BRUSH );
	}

	else
	{
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	
	return hbr;
	
}
BOOL HeadParamDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnEraseBkgnd(pDC);
	SBitdraw(pDC,IDB_BMP_BACKGROUND1);
	return true;
	
}
bool HeadParamDlg::SBitdraw(CDC *pDC, UINT nIDResource)
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
