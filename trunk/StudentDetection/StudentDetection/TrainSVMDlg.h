#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// TrainSVMDlg dialog

class TrainSVMDlg : public CDialog
{
	DECLARE_DYNAMIC(TrainSVMDlg)

public:
	TrainSVMDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~TrainSVMDlg();

// Dialog Data
	enum { IDD = IDD_DLG_TRAIN_SVM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:		
	CEdit m_editStartIndex;
	CEdit m_editEndIndex;
	CEdit m_editOutPath;
	CButton m_btnBrowseOutPath;
	CEdit m_editWidthCell;
	CEdit m_editHeightCell;
	CEdit m_editStepOverLap;
	CEdit m_editWidthBlock;
	CEdit m_editHeightBlock;
	CButton m_btnTrainSVM;
	afx_msg void OnBnClickedBtnTrainSvm();	
	afx_msg void OnBnClickedBtnBwsOutSvm();		
	CEdit m_editStartIdxNeg;
	CEdit m_editEndIndxNeg;	
	CEdit m_editPrefixPos;
	CEdit m_editSubfixPos;
	CEdit m_editPrefixNeg;
	CEdit m_editSubfixNeg;
	CEdit m_editProgress;	
};
