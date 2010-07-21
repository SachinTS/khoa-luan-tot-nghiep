// StudentDetectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "StudentDetectionDlg.h"
#include "TrainDataDlg.h"
#include "AboutDlg.h"
#include "InputDlg.h"
#include "ImageProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CStudentDetectionDlg dialog




CStudentDetectionDlg::CStudentDetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStudentDetectionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	video_thread = NULL;
}

void CStudentDetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_TAB_PARAM, m_tabParams);
	DDX_Control(pDX, IDC_BTN_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_CHECK_VIEW_HAIR, m_checkViewHair);
	DDX_Control(pDX, IDC_CHECK_VIEW_SVM, m_checkViewSVM);
	DDX_Control(pDX, IDC_CHECK_VIEW_SHAPE, m_checkViewShape);
	DDX_Control(pDX, IDC_SHOW_VIDEO, m_video);
}

BEGIN_MESSAGE_MAP(CStudentDetectionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PARAM, &CStudentDetectionDlg::OnTcnSelchangeTabParam)
	ON_COMMAND(ID_MAIN_VIDEO, &CStudentDetectionDlg::OnMainVideo)
	ON_COMMAND(ID_MAIN_EXIT, &CStudentDetectionDlg::OnMainExit)
	ON_COMMAND(ID_MAIN_TRAINDATA, &CStudentDetectionDlg::OnMainTraindata)
	ON_COMMAND(ID_MAIN_ABOUT, &CStudentDetectionDlg::OnMainAbout)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CStudentDetectionDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &CStudentDetectionDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_CHECK_VIEW_HAIR, &CStudentDetectionDlg::OnBnClickedCheckViewHair)
	ON_BN_CLICKED(IDC_CHECK_VIEW_SVM, &CStudentDetectionDlg::OnBnClickedCheckViewSvm)
	ON_BN_CLICKED(IDC_CHECK_VIEW_SHAPE, &CStudentDetectionDlg::OnBnClickedCheckViewShape)
	ON_MESSAGE(WM_USER_THREAD_FINISHED, OnThreadFinished)
	ON_MESSAGE(WM_USER_THREAD_UPDATE_PROGRESS, OnThreadUpdateProgress)
	ON_BN_CLICKED(IDC_BUTTON1, &CStudentDetectionDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CStudentDetectionDlg message handlers

BOOL CStudentDetectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_windowParam.LoadParamsFromXML("config.xml");
	m_windowParam.m_hWnd = this->m_hWnd;

	//create tab
	TC_ITEM tci;
	tci.mask = TCIF_TEXT;
	tci.iImage = -1;
	tci.pszText = _T("Head Params");
	m_tabParams.InsertItem(0, &tci);

	tci.pszText = _T("Omega Params");
	m_tabParams.InsertItem(1, &tci);
	
	// MG: Create the page dialogs, and set the initial Dialog
	RECT Rect;
	m_tabParams.GetItemRect( 0, &Rect );
	m_tabHeadParams.Create( IDD_DLG_HEAD_PARAMS, &m_tabParams );
	m_tabHeadParams.SetWindowPos( 0, Rect.left + 2, Rect.bottom + 2, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
		
	m_tabHeadParams.ShowWindow( SW_SHOWNA ); // MG: Sets the initial dialog


	m_tabParams.GetItemRect( 0, &Rect );
	m_tabShapeParams.Create( IDD_DLG_SHAPE_PARAMS, &m_tabParams );
	m_tabShapeParams.SetWindowPos( 0, Rect.left + 2, Rect.bottom + 2, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
		
	m_tabShapeParams.ShowWindow( SW_HIDE ); // MG: Sets the initial dialog
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_checkViewHair.SetCheck(1);
	m_windowParam.m_isViewHairDetection = 1;

	m_checkViewShape.SetCheck(1);
	m_windowParam.m_isViewShapeDetection = 1;

	m_checkViewSVM.SetCheck(1);
	m_windowParam.m_isViewSVMDetection = 1;

	//m_tabHeadParams.Init();
	//m_tabHeadParams.SetParam(m_windowParam);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStudentDetectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStudentDetectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStudentDetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStudentDetectionDlg::OnTcnSelchangeTabParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	
	if( m_tabParams.GetCurSel()==0)
	{
		m_tabHeadParams.ShowWindow( SW_SHOWNA );
		m_tabShapeParams.ShowWindow( SW_HIDE );
	}
	else if( m_tabParams.GetCurSel()==1)
	{
		m_tabHeadParams.ShowWindow( SW_HIDE );
		m_tabShapeParams.ShowWindow( SW_SHOWNA );
	}
	
	*pResult = 0;
}

void CStudentDetectionDlg::OnMainVideo()
{
	// TODO: Add your command handler code here
	InputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		Utils utils;			
		m_windowParam.m_videoPath = utils.ConvertToChar(dlg.m_videoPath);		
		m_windowParam.m_maskPath = utils.ConvertToChar(dlg.m_maskPath);
	}
}

void CStudentDetectionDlg::OnMainExit()
{
	// TODO: Add your command handler code here
	OnOK();
}

void CStudentDetectionDlg::OnMainTraindata()
{
	// TODO: Add your command handler code here
	TrainDataDlg dlg;
	dlg.DoModal();
}

void CStudentDetectionDlg::OnMainAbout()
{
	// TODO: Add your command handler code here
	AboutDlg dlg;
	dlg.DoModal();
}

UINT playVideoThread(LPVOID lParam)
{	
	WindowParams* param = (WindowParams*)lParam;
	Utils utils;
	
	CvCapture *capture = cvCaptureFromFile(param->m_videoPath);	
	if (capture == NULL) {
		printf("Cannot open video.\n");
		return EXIT_FAILURE;
	}

	int count = 0;
	float speed;
	time_t start = time(NULL);

	IplImage *frame = cvQueryFrame(capture);
	IplImage *subtract;
	IplImage *mask = cvLoadImage(param->m_maskPath, CV_LOAD_IMAGE_GRAYSCALE);	
	IplImage *result = cvCloneImage(frame);

	int fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);

	CvMemStorage *storage = cvCreateMemStorage();		
	CvSeq *contours = 0;

	GaussFilterColor m_gauss;
	m_gauss.LoadData(param->m_modelGaussPath);
	m_gauss.SetThreshold(param->m_DetectionParams.m_Gaussian_Params.m_fThreshold);

	HoGProcessor hog;
	hog.SetParams(param->m_DetectionParams.m_HOG_Params.m_cell, param->m_DetectionParams.m_HOG_Params.m_block, param->m_DetectionParams.m_HOG_Params.m_fStepOverlap);

	CvSVM svm;
	
	CvRect window = cvRect(0,0,48,48);
	
	svm.load(param->m_modelSVMPath);
			
	vector<CvRect> vectorRect;
	while (1) {
		frame = cvQueryFrame(capture);
		
		if (frame == NULL) {
			printf("End video.\n");
			break;
		}				
		
		m_gauss.SetThreshold(param->m_DetectionParams.m_Gaussian_Params.m_fThreshold);
		subtract = m_gauss.Classify(frame, mask);										
		
		cvFindContours(subtract, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL);

		cvCopyImage(frame, result);

		if (contours) {				
			vectorRect.clear();						
			
			while (contours != NULL) {				
				CvRect rectHead = cvBoundingRect(contours);					
				if(!utils.CheckRectHead(rectHead, frame->height, param->m_DetectionParams.m_Head_Params))
				{										
					if(param->m_isViewHairDetection)
						cvRectangle(result, cvPoint(rectHead.x, rectHead.y), cvPoint(rectHead.x + rectHead.width, rectHead.y + rectHead.height), CV_RGB(255,255,0));
					contours = contours->h_next;
					continue;
				}					
					
				if(param->m_isViewHairDetection)
					cvRectangle(result, cvPoint(rectHead.x, rectHead.y), cvPoint(rectHead.x + rectHead.width, rectHead.y + rectHead.height), CV_RGB(255,255,255));
					
				CvRect detectedRect = hog.detectObject(&svm, frame, result, rectHead);
				if(detectedRect.width > 0){
					vectorRect.push_back(detectedRect);
				}
		
				contours = contours->h_next;					
			}
		}
		
		vectorRect = utils.ConnectOverlapRects(vectorRect);
		
		if(param->m_isViewSVMDetection)
			utils.OutputResult(result, vectorRect, CV_RGB(255,0,0));
					
		count++;

		PostMessage(param->m_hWnd,WM_USER_THREAD_UPDATE_PROGRESS,(WPARAM)result,0);
	
		cvReleaseImage(&subtract);
					
		if(cvWaitKey(fps) == 27)
			break;
	}

	PostMessage(param->m_hWnd,WM_USER_THREAD_FINISHED,(WPARAM)result,0);

	time_t end = time(NULL);
	speed = count*1.0/(end-start);
	printf("fps: %2.3f.\n", speed);

	cvDestroyAllWindows();

	cvReleaseCapture(&capture);
	cvReleaseImage(&mask);
	cvReleaseImage(&result);		
	cvReleaseImage(&subtract);		
	cvReleaseMemStorage(&storage);	
}
void CStudentDetectionDlg::OnBnClickedBtnPlay()
{
	// TODO: Add your control notification handler code here
	if(m_btnPlay.IsWindowEnabled())
	{
		if(m_windowParam.m_videoPath == NULL)
		{
			MessageBox(_T("Please select video"), _T("Information"));
			return;
		}

		m_btnPlay.EnableWindow(0);
		video_thread = AfxBeginThread(playVideoThread, &m_windowParam, THREAD_PRIORITY_NORMAL, 0, 0);
	}
}

void CStudentDetectionDlg::OnBnClickedBtnStop()
{
	// TODO: Add your control notification handler code here
	if(video_thread != NULL)
	{
		video_thread->SuspendThread();		
		m_btnPlay.EnableWindow();
	}
}

void CStudentDetectionDlg::OnBnClickedCheckViewHair()
{
	// TODO: Add your control notification handler code here
	m_windowParam.m_isViewHairDetection = m_checkViewHair.GetCheck();
}

void CStudentDetectionDlg::OnBnClickedCheckViewSvm()
{
	// TODO: Add your control notification handler code here
	m_windowParam.m_isViewSVMDetection = m_checkViewSVM.GetCheck();
}

void CStudentDetectionDlg::OnBnClickedCheckViewShape()
{
	// TODO: Add your control notification handler code here
	m_windowParam.m_isViewShapeDetection = m_checkViewShape.GetCheck();
}

LRESULT CStudentDetectionDlg::OnThreadFinished(WPARAM wParam,LPARAM lParam)
{
	m_btnPlay.EnableWindow();
	return 0;
}

LRESULT CStudentDetectionDlg::OnThreadUpdateProgress(WPARAM wParam,LPARAM lParam)
{
	IplImage* frame = (IplImage*)wParam;
	ImageProcessor imgProcess;
	CRect clientRect;
	m_video.GetClientRect(clientRect);

	CvvImage cvv;
	cvv.CopyOf(imgProcess.getSubImageAndResize(frame, cvRect(0,0,frame->width, frame->height), clientRect.Width(), clientRect.Height()));
	cvv.Show(m_video.GetDC()->m_hDC, 0, 0, clientRect.Width(), clientRect.Height());
	cvv.Destroy();	

	return 0;
}

void CStudentDetectionDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_tabHeadParams.Init();
}
