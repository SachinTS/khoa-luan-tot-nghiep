#pragma once
#include "afxwin.h"
#include "BtnST.h"

// InputDlg dialog

class InputDlg : public CDialog
{
	DECLARE_DYNAMIC(InputDlg)

public:
	InputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~InputDlg();

// Dialog Data
	enum { IDD = IDD_DLG_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnEraseBkgnd(CDC* pDC);
	bool SBitdraw(CDC *pDC, UINT nIDResource);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CButton m_btnBrowseVideo;
	CButton m_btnBrowseMask;
	
	CEdit m_editVideoPath;
	CEdit m_editMaskPath;
	
	afx_msg void OnBnClickedBtnBrowseVideo();
	afx_msg void OnBnClickedBtnBrowseMask();	
	afx_msg void OnBnClickedOk();

	BOOL OnInitDialog();

	CString m_maskPath;
	CString m_videoPath;
	
	CButtonST m_btnOK;
	CButtonST m_btnCancel;
};
