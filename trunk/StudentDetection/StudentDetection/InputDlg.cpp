// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "InputDlg.h"


// InputDlg dialog

IMPLEMENT_DYNAMIC(InputDlg, CDialog)

InputDlg::InputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(InputDlg::IDD, pParent)
{
	
}


InputDlg::~InputDlg()
{
}

void InputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_BROWSE_VIDEO, m_btnBrowseVideo);
	DDX_Control(pDX, IDC_BTN_BROWSE_MASK, m_btnBrowseMask);
	DDX_Control(pDX, IDC_EDIT_VIDEO_PATH, m_editVideoPath);
	DDX_Control(pDX, IDC_EDIT_MASK_PATH, m_editMaskPath);	
}


BEGIN_MESSAGE_MAP(InputDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSE_VIDEO, &InputDlg::OnBnClickedBtnBrowseVideo)
	ON_BN_CLICKED(IDC_BTN_BROWSE_MASK, &InputDlg::OnBnClickedBtnBrowseMask)
	ON_BN_CLICKED(IDOK, &InputDlg::OnBnClickedOk)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// InputDlg message handlers
BOOL InputDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnEraseBkgnd(pDC);
	SBitdraw(pDC,IDB_BMP_BACKGROUND);
	return true;
	
}
bool InputDlg::SBitdraw(CDC *pDC, UINT nIDResource)
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
void InputDlg::OnBnClickedBtnBrowseVideo()
{
	// TODO: Add your control notification handler code here
	CFileDialog m_fdlg(true,NULL,NULL,OFN_HIDEREADONLY,_T("Avi Files (*.avi)|*.avi||"));
	m_fdlg.m_ofn.lpstrInitialDir = _T("C:\\");
	if(m_fdlg.DoModal()==IDOK)
	{
		m_editVideoPath.SetWindowText(m_fdlg.GetPathName());		
		UpdateData(false);
	}
}

void InputDlg::OnBnClickedBtnBrowseMask()
{
	// TODO: Add your control notification handler code here
	CFileDialog m_fdlg(true,NULL,NULL,OFN_HIDEREADONLY,_T("image Files (*.jpg)|*.jpg||"));
	m_fdlg.m_ofn.lpstrInitialDir = _T("C:\\");
	if(m_fdlg.DoModal()==IDOK)
	{
		m_editMaskPath.SetWindowText(m_fdlg.GetPathName());
		UpdateData(false);
	}
}

void InputDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	//WindowParams m_windowParams
	m_editVideoPath.GetWindowText(m_videoPath);	
	m_editMaskPath.GetWindowText(m_maskPath);
	
	OnOK();
}
