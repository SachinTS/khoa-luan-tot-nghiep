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
END_MESSAGE_MAP()


// AboutDlg message handlers
