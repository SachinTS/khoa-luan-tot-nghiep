#pragma once
#include "afxwin.h"


// TrainGaussDlg dialog

class TrainGaussDlg : public CDialog
{
	DECLARE_DYNAMIC(TrainGaussDlg)

public:
	TrainGaussDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~TrainGaussDlg();

// Dialog Data
	enum { IDD = IDD_DLG_TRAIN_GAUSS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool SBitdraw(CDC *pDC, UINT nIDResource);
	BOOL OnEraseBkgnd(CDC* pDC);

	CEdit m_editPrefixGauss;
	CEdit m_editStartIndxGauss;
	CEdit m_editPosfixGauss;
	CEdit m_editEndIndxGauss;
	CEdit m_editOutPath;
	CButton m_btnBrowseOutPath;
	CButton m_btnTrainGauss;
	afx_msg void OnBnClickedBtnBwsOutGauss();
	afx_msg void OnBnClickedBtnTrainGauss();
	CEdit m_editProgress;
};
