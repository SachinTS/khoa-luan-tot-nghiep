#pragma once
#include "afxcmn.h"
#include "TrainGaussDlg.h"
#include "TrainSVMDlg.h"

// TrainDataDlg dialog

class TrainDataDlg : public CDialog
{
	DECLARE_DYNAMIC(TrainDataDlg)

public:
	TrainDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~TrainDataDlg();

// Dialog Data
	enum { IDD = IDD_DLG_TRAIN_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabTrainData;
	TrainGaussDlg m_tabTrainGauss;
	TrainSVMDlg m_tabTrainSVM;
	afx_msg void OnTcnSelchangeTabTrainData(NMHDR *pNMHDR, LRESULT *pResult);
};
