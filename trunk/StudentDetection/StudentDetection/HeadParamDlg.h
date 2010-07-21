#pragma once
#include "afxwin.h"
#include "WindowParams.h"

// HeadParamDlg dialog

class HeadParamDlg : public CDialog
{
	DECLARE_DYNAMIC(HeadParamDlg)

public:
	HeadParamDlg(CWnd* pParent = NULL);   // standard constructor
	HeadParamDlg(WindowParams params, CWnd* pParent = NULL);   // standard constructor
	virtual ~HeadParamDlg();
	void SetParam(WindowParams param);
	void Init();
	BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DLG_HEAD_PARAMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnApply;
	afx_msg void OnBnClickedBtnApply();
	WindowParams m_params;
	CEdit m_editMinWidthHead;
	CEdit m_editMaxWidthHead;
	CEdit m_editMinHeadArea;
	CEdit m_editMaxHeadArea;
	CEdit m_editMaxHeadAreaAtTop;
	CEdit m_editMinAreaAtBottom;
	CEdit m_editRelativeWidthHeight;
	CEdit m_editRelativeHeightWidth;
};
