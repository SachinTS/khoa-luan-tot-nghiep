#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CShapeParamsDlg dialog

class CShapeParamsDlg : public CDialog
{
	DECLARE_DYNAMIC(CShapeParamsDlg)

public:
	CShapeParamsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShapeParamsDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SHAPE_PARAMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnApply;
	CSliderCtrl m_sliderThreshold;
	CSliderCtrl m_sliderLength;
	int m_iThreshold;
	int m_iLength;
	CStatic m_sttThreshold;
	CStatic m_sttLength;
};
