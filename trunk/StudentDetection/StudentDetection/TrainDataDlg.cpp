// TrainDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "TrainDataDlg.h"


// TrainDataDlg dialog

IMPLEMENT_DYNAMIC(TrainDataDlg, CDialog)

TrainDataDlg::TrainDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TrainDataDlg::IDD, pParent)
{

}

TrainDataDlg::~TrainDataDlg()
{
}

BOOL TrainDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//create tab
	TC_ITEM tci;
	tci.mask = TCIF_TEXT;
	tci.iImage = -1;
	tci.pszText = _T("Train SVM");
	m_tabTrainData.InsertItem(0, &tci);

	tci.pszText = _T("Train Gaussian");
	m_tabTrainData.InsertItem(1, &tci);
	
	// MG: Create the page dialogs, and set the initial Dialog
	RECT Rect;
	m_tabTrainData.GetItemRect( 0, &Rect );
	m_tabTrainSVM.Create( IDD_DLG_TRAIN_SVM, &m_tabTrainData );
	m_tabTrainSVM.SetWindowPos( 0, Rect.left + 2, Rect.bottom + 2, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
	
	m_tabTrainGauss.Create( IDD_DLG_TRAIN_GAUSS, &m_tabTrainData );
	m_tabTrainGauss.SetWindowPos( 0, Rect.left + 2, Rect.bottom + 2, 0, 0, SWP_NOSIZE|SWP_NOZORDER );

	m_tabTrainSVM.ShowWindow( SW_SHOWNA ); // MG: Sets the initial dialog

	return true;
}

void TrainDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_TRAIN_DATA, m_tabTrainData);
}


BEGIN_MESSAGE_MAP(TrainDataDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TRAIN_DATA, &TrainDataDlg::OnTcnSelchangeTabTrainData)
END_MESSAGE_MAP()


// TrainDataDlg message handlers

void TrainDataDlg::OnTcnSelchangeTabTrainData(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if( m_tabTrainData.GetCurSel()==0)
	{
		m_tabTrainGauss.ShowWindow( SW_HIDE );
		m_tabTrainSVM.ShowWindow( SW_SHOWNA );	
	}
	else if( m_tabTrainData.GetCurSel()==1)
	{
		m_tabTrainSVM.ShowWindow( SW_HIDE );
		m_tabTrainGauss.ShowWindow( SW_SHOWNA );				
	}
	*pResult = 0;
}
