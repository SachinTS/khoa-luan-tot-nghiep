// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "AboutDlg.h"


// AboutDlg dialog

IMPLEMENT_DYNAMIC(AboutDlg, CDialog)

AboutDlg::AboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AboutDlg::IDD, pParent)
{

}

AboutDlg::~AboutDlg()
{
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// AboutDlg message handlers
BOOL AboutDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnEraseBkgnd(pDC);
	SBitdraw(pDC,IDB_BMP_BACKGROUND);
	return true;
	
}
bool AboutDlg::SBitdraw(CDC *pDC, UINT nIDResource)
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
