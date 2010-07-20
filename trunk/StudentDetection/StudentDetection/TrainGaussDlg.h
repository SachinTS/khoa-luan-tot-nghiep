#pragma once


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
};
