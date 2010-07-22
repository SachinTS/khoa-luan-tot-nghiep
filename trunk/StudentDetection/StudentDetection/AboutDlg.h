#pragma once


// AboutDlg dialog

class AboutDlg : public CDialog
{
	DECLARE_DYNAMIC(AboutDlg)

public:
	AboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~AboutDlg();
	BOOL OnEraseBkgnd(CDC* pDC);
	bool SBitdraw(CDC *pDC, UINT nIDResource);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
