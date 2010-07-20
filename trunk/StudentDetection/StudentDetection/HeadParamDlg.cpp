// HeadParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "HeadParamDlg.h"


// HeadParamDlg dialog

IMPLEMENT_DYNAMIC(HeadParamDlg, CDialog)

HeadParamDlg::HeadParamDlg(WindowParams params, CWnd* pParent /*=NULL*/)
	: CDialog(HeadParamDlg::IDD, pParent)
{
	
}

HeadParamDlg::HeadParamDlg(CWnd* pParent )
	: CDialog(HeadParamDlg::IDD, pParent)
{
	
}

HeadParamDlg::~HeadParamDlg()
{
}

void HeadParamDlg::SetParam(WindowParams param)
{
	m_params = param;
}

void HeadParamDlg::Init()
{	

	CString t;
	t.Format(_T("%d"), m_params.m_DetectionParams.m_Head_Params.m_iMinWidth);
	m_editMinHeadWidth.SetWindowText(t);

//	m_editMinHeadWidth.SetDlgItemInt(IDC_EDIT_MIN_HEAD_WIDTH, 10);
	
	
}

BOOL HeadParamDlg::OnInitDialog()
{
	//OnInitDialog();



	return TRUE;
}


void HeadParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_EDIT_MIN_HEAD_WIDTH, m_editMinHeadWidth);
	DDX_Control(pDX, IDC_EDIT_MAX_HEAD_WIDTH, m_editMaxHeadWidth);
	DDX_Control(pDX, IDC_EDIT_MIN_HEAD_AREA, m_editMinHeadArea);
	DDX_Control(pDX, IDC_MAX_HEAD_AREA, m_editMaxHeadArea);
	DDX_Control(pDX, IDC_EDIT_MAX_HEAD_AREA_AT_TOP, m_editMaxHeadAreaAtTop);
	DDX_Control(pDX, IDC_EDIT_MIN_HEAD_AREA_BOTTOM, m_editMinHeadAreaAtBottom);
	DDX_Control(pDX, IDC_EDIT_RELATIVE_WIDTH_HEIGHT, m_editRelativeWidthHeight);
	DDX_Control(pDX, IDC_EDIT_RELATIVE_HEIGHT_WIDTH, m_editRelativeHeightWidth);
}


BEGIN_MESSAGE_MAP(HeadParamDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_APPLY, &HeadParamDlg::OnBnClickedBtnApply)
END_MESSAGE_MAP()


// HeadParamDlg message handlers

void HeadParamDlg::OnBnClickedBtnApply()
{
	// TODO: Add your control notification handler code here
	
}
