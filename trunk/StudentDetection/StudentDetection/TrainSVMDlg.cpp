// TrainSVMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "TrainSVMDlg.h"
#include "HoGProcessor.h"
#include "Utils.h"

// TrainSVMDlg dialog

IMPLEMENT_DYNAMIC(TrainSVMDlg, CDialog)

TrainSVMDlg::TrainSVMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TrainSVMDlg::IDD, pParent)
{

}

TrainSVMDlg::~TrainSVMDlg()
{
}

void TrainSVMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_EDIT_START_INDEX_SVM, m_editStartIndex);
	DDX_Control(pDX, IDC_EDIT_END_INDEX_SVM, m_editEndIndex);
	DDX_Control(pDX, IDC_EDIT_OUT_SVM, m_editOutPath);
	DDX_Control(pDX, IDC_BTN_BWS_OUT_SVM, m_btnBrowseOutPath);
	DDX_Control(pDX, IDC_EDIT_WIDTH_CELL, m_editWidthCell);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_CELL, m_editHeightCell);
	DDX_Control(pDX, IDC_EDIT_STEPOVERLAP, m_editStepOverLap);
	DDX_Control(pDX, IDC_EDIT_WIDTH_BLOCK, m_editWidthBlock);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_BLOCK, m_editHeightBlock);
	DDX_Control(pDX, IDC_BTN_TRAIN_SVM, m_btnTrainSVM);	
	DDX_Control(pDX, IDC_EDIT_START_INDEX_SVM_NEG, m_editStartIdxNeg);
	DDX_Control(pDX, IDC_EDIT_END_INDEX_SVM_NEG, m_editEndIndxNeg);
	DDX_Control(pDX, IDC_EDIT_PREFIX_POS, m_editPrefixPos);
	DDX_Control(pDX, IDC_EDIT_SUBFIX_POS, m_editSubfixPos);
	DDX_Control(pDX, IDC_EDIT_PREFIX_NEG, m_editPrefixNeg);
	DDX_Control(pDX, IDC_EDIT_SUBFIX_NEG, m_editSubfixNeg);
	DDX_Control(pDX, IDC_EDIT_PROGRESS, m_editProgress);
}


BEGIN_MESSAGE_MAP(TrainSVMDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_TRAIN_SVM, &TrainSVMDlg::OnBnClickedBtnTrainSvm)
	ON_BN_CLICKED(IDC_BTN_BWS_OUT_SVM, &TrainSVMDlg::OnBnClickedBtnBwsOutSvm)	
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// TrainSVMDlg message handlers

void TrainSVMDlg::OnBnClickedBtnTrainSvm()
{
	// TODO: Add your control notification handler code here		
	HoGProcessor hog;
	Utils utils;
	CString tmp;
	CString prefix, posfix;

	m_editStartIndex.GetWindowTextW(tmp);
	int startIndexPos = utils.ConvertToInt(tmp);
	
	m_editEndIndex.GetWindowTextW(tmp);
	int endIndexPos = utils.ConvertToInt(tmp);
	
	m_editStartIdxNeg.GetWindowTextW(tmp);
	int startIndexNeg = utils.ConvertToInt(tmp);
	
	m_editEndIndxNeg.GetWindowTextW(tmp);
	int endIndexNeg = utils.ConvertToInt(tmp);	
	
	m_editWidthCell.GetWindowTextW(tmp);
	int cellWidht = utils.ConvertToInt(tmp);
	
	m_editHeightCell.GetWindowTextW(tmp);
	int cellHeight = utils.ConvertToInt(tmp);
	
	m_editWidthBlock.GetWindowTextW(tmp);
	int blockWidth = utils.ConvertToInt(tmp);
	
	m_editHeightBlock.GetWindowTextW(tmp);
	int blockHeight = utils.ConvertToInt(tmp);
	
	m_editStepOverLap.GetWindowTextW(tmp);
	float stepOverlap = utils.ConvertToInt(tmp);	
	
	m_editPrefixPos.GetWindowTextW(prefix);
	m_editSubfixPos.GetWindowTextW(posfix);

	m_editProgress.SetWindowTextW(_T("Begin training positive images... \n"));
	CvMat* pos_mat = hog.train_64x128(utils.ConvertToChar(prefix),utils.ConvertToChar(posfix), cvSize(48,48), endIndexPos - startIndexPos + 1, startIndexPos , endIndexPos);
	m_editProgress.SetWindowTextW(_T("Train positive images completed... \n"));
	
	m_editPrefixNeg.GetWindowTextW(prefix);
	m_editSubfixNeg.GetWindowTextW(posfix);
	m_editProgress.SetWindowTextW(_T("Begin training negative images... \n"));
	CvMat* neg_mat = hog.train_large(utils.ConvertToChar(prefix),utils.ConvertToChar(posfix), cvSize(48,48), endIndexNeg - startIndexNeg + 1, 3, 3, startIndexNeg, endIndexNeg);
	m_editProgress.SetWindowTextW(_T("Training negative images completed... \n"));

	m_editProgress.SetWindowTextW(_T("Begin training SVM... \n"));	
	m_editOutPath.GetWindowTextW(tmp);
	hog.trainSVM(pos_mat, neg_mat, utils.ConvertToChar(tmp) );
	m_editProgress.SetWindowTextW(_T("Train SVM completed... \n"));
}

void TrainSVMDlg::OnBnClickedBtnBwsOutSvm()
{
	// TODO: Add your control notification handler code here
	CFileDialog m_fdlg(false,NULL,NULL,OFN_HIDEREADONLY,_T("XML Files (*.xml)|*.xml||"));
	m_fdlg.m_ofn.lpstrInitialDir = _T("C:\\");
	if(m_fdlg.DoModal()==IDOK)
	{
		m_editOutPath.SetWindowText(m_fdlg.GetPathName());		
		UpdateData(false);
	}
}

BOOL TrainSVMDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnEraseBkgnd(pDC);
	SBitdraw(pDC,IDB_BMP_BACKGROUND1);
	return true;
	
}
bool TrainSVMDlg::SBitdraw(CDC *pDC, UINT nIDResource)
{
	CBitmap* m_bitmap;
	m_bitmap=new CBitmap();
	m_bitmap->LoadBitmap(nIDResource);
	if(!m_bitmap->m_hObject)
		return true;
	CRect rect;
	GetClientRect(&rect);
	CDC dc;
	dc.CreateCompatibleDC(pDC);	
	dc.SelectObject(m_bitmap);

	int xo=0, yo=0;
	pDC->BitBlt(xo, yo, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY);
	return true;
}