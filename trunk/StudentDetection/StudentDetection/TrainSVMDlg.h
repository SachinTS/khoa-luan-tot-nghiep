#pragma once


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
};
