// StudentDetectionDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "HeadParamDlg.h"
#include "afxcmn.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include "time.h"
#include "HoGProcessor.h"
#include "SnakeDetector.h"
#include "GaussFilterColor.h"
#include "Utils.h"
#include "WindowParams.h"
#include "ShapeParamsDlg.h"
#include "BtnST.h"

#define WM_USER_THREAD_FINISHED (WM_USER+0x101)
#define WM_USER_THREAD_UPDATE_PROGRESS (WM_USER+0x102)
#define WM_USER_THREAD_UPDATE_INFO (WM_USER+0x103)

// CStudentDetectionDlg dialog
class CStudentDetectionDlg : public CDialog
{
// Construction
public:
	CStudentDetectionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_STUDENTDETECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL OnEraseBkgnd(CDC* pDC);
	bool SBitdraw(CDC *pDC, UINT nIDResource);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	~CStudentDetectionDlg(void);

	CTabCtrl m_tabParams;
	HeadParamDlg *m_tabHeadParams;
	afx_msg void OnTcnSelchangeTabParam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMainVideo();
	afx_msg void OnMainExit();
	afx_msg void OnMainTraindata();
	afx_msg void OnMainAbout();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnStop();
	CButtonST m_btnPlay;
	CButtonST m_btnStop;
	CButton m_checkViewHair;
	CButton m_checkViewSVM;
	CButton m_checkViewShape;
	afx_msg void OnBnClickedCheckViewHair();
	afx_msg void OnBnClickedCheckViewSvm();
	afx_msg void OnBnClickedCheckViewShape();
	
	afx_msg LRESULT OnThreadFinished(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnThreadUpdateProgress(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnThreadUpdateInfo(WPARAM wParam,LPARAM lParam);
	
	static WindowParams *m_windowParam;
	CWinThread *video_thread;
		
	CShapeParamsDlg m_tabShapeParams;
	CButtonST m_btnApplyParams;
	afx_msg void OnBnClickedBtnApplyParams();	
	CStatic m_videoPlayer;
	afx_msg void OnStnClickedPlayVideo();
	bool m_bIsPlayVideo;
	static SnakeDetector *detector;
	CEdit m_editStudentCount;
};