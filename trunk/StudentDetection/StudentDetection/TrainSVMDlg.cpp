// TrainSVMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "TrainSVMDlg.h"


// TrainSVMDlg dialog

IMPLEMENT_DYNAMIC(TrainSVMDlg, CDialog)

TrainSVMDlg::TrainSVMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TrainSVMDlg::IDD, pParent)
{

}

TrainSVMDlg::~TrainSVMDlg()
{
}

void TrainSVMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TrainSVMDlg, CDialog)
END_MESSAGE_MAP()


// TrainSVMDlg message handlers
