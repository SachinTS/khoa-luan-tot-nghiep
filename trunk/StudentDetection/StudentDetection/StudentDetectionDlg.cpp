// StudentDetectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StudentDetection.h"
#include "StudentDetectionDlg.h"
#include "TrainDataDlg.h"
#include "AboutDlg.h"
#include "InputDlg.h"
#include "ImageProcessor.h"
#include "afxwin.h"
#include "BtnST.h"
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
public:
	CButtonST m_btnOK;
	afx_msg void OnBnClickedOk();
	BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnOK.SetBitmaps(IDB_BMP_OK, RGB(255,0,0));
	m_btnOK.SetFlat();
	return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CStudentDetectionDlg dialog

SnakeDetector * CStudentDetectionDlg::detector = NULL;
WindowParams * CStudentDetectionDlg::m_windowParam = NULL;

CStudentDetectionDlg::CStudentDetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStudentDetectionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	video_thread = NULL;

	CStudentDetectionDlg::m_windowParam = new WindowParams();
	CStudentDetectionDlg::m_windowParam->LoadParamsFromXML("config.xml");

	CStudentDetectionDlg::detector = new SnakeDetector( "shape.xml" );
}

CStudentDetectionDlg::~CStudentDetectionDlg() {
	if (CStudentDetectionDlg::detector != NULL)
		delete CStudentDetectionDlg::detector;
	if (CStudentDetectionDlg::m_windowParam != NULL)
		delete CStudentDetectionDlg::m_windowParam;
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
	DDX_Control(pDX, IDC_BTN_APPLY_PARAMS, m_btnApplyParams);	
	DDX_Control(pDX, IDC_PLAY_VIDEO, m_videoPlayer);
	DDX_Control(pDX, IDC_EDIT_STUDENT_COUNT, m_editStudentCount);
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
	ON_MESSAGE(WM_USER_THREAD_UPDATE_INFO, OnThreadUpdateInfo)	
	ON_BN_CLICKED(IDC_BTN_APPLY_PARAMS, &CStudentDetectionDlg::OnBnClickedBtnApplyParams)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_PLAY_VIDEO, &CStudentDetectionDlg::OnStnClickedPlayVideo)
END_MESSAGE_MAP()


// CStudentDetectionDlg message handlers


BOOL CStudentDetectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CStudentDetectionDlg::m_windowParam->m_hWnd = this->m_hWnd;

	//create tab
	TC_ITEM tci;
	tci.mask = TCIF_TEXT;
	tci.iImage = -1;
	tci.pszText = _T("Head Params");
	m_tabParams.InsertItem(0, &tci);

	tci.pszText = _T("Shape Params");
	m_tabParams.InsertItem(1, &tci);
	
	// MG: Create the page dialogs, and set the initial Dialog
	RECT Rect;
	m_tabParams.GetItemRect( 0, &Rect );

	m_tabHeadParams = new HeadParamDlg(CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Head_Params);

	m_tabHeadParams->Create( IDD_DLG_HEAD_PARAMS, &m_tabParams );
	m_tabHeadParams->SetWindowPos( 0, Rect.left + 2, Rect.bottom + 2, 0, 0, SWP_NOSIZE|SWP_NOZORDER );	
	m_tabHeadParams->ShowWindow( SW_SHOWNA ); // MG: Sets the initial dialog

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
	//init checkboxes
	m_checkViewHair.SetCheck(1);
	CStudentDetectionDlg::m_windowParam->m_isViewHairDetection = 1;
	m_checkViewShape.SetCheck(1);
	CStudentDetectionDlg::m_windowParam->m_isViewShapeDetection = 1;
	m_checkViewSVM.SetCheck(1);
	CStudentDetectionDlg::m_windowParam->m_isViewSVMDetection = 1;

	//init image for buttons
	m_btnPlay.SetBitmaps(IDB_BMP_PLAY, RGB(255, 255, 255));
	m_btnPlay.SetFlat();
	m_btnStop.SetBitmaps(IDB_BMP_STOP, RGB(255, 255, 255));
	m_btnStop.SetFlat();

	m_btnApplyParams.SetBitmaps(IDB_BMP_OK, RGB(255, 0, 0));
	m_btnApplyParams.SetFlat();

	m_bIsPlayVideo = false;

	m_editStudentCount.SetWindowTextW(_T("0"));
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
		m_tabHeadParams->ShowWindow( SW_SHOWNA );
		m_tabShapeParams.ShowWindow( SW_HIDE );
	}
	else if( m_tabParams.GetCurSel()==1)
	{
		m_tabHeadParams->ShowWindow( SW_HIDE );
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
		CStudentDetectionDlg::m_windowParam->m_videoPath = utils.ConvertToChar(dlg.m_videoPath);		
		CStudentDetectionDlg::m_windowParam->m_maskPath = utils.ConvertToChar(dlg.m_maskPath);
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
	IplImage *hair_canny = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);


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

		cvSmooth(subtract, hair_canny, CV_MEDIAN);
		cvCanny(hair_canny, hair_canny, 10, 100);

		
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

		for (unsigned int i = 0; i < vectorRect.size(); i++) {
			// kiem tra rect co shape
			CvRect rect = vectorRect.at(i);
			CvPoint location = cvPoint(rect.x+rect.width*1.0f/3, rect.y+rect.height*1.0f/3);

			Snake *fit_snake;
			int current_y = rect.y+rect.height*1.0f/3;
			int frame_height_step = frame->height*1.0/3;
			int dis = 1;
			if (current_y >= 0 && current_y < frame_height_step) {
				// far -> small shape
				dis = 3;
			}
			else if (current_y >= frame_height_step && current_y < frame_height_step*2) {
				// medium shape
				dis = 2;
			}
			else if (current_y >= frame_height_step*2 && current_y <frame->height) {
				// near -> big shape
				dis = 1;
			}
			fit_snake = CStudentDetectionDlg::detector->GetSnake(hair_canny, dis, location, rect);

			// rect co shape dau nguoi trong if
			if (fit_snake != NULL && param->m_isViewShapeDetection) {
				// ve shape
				fit_snake->DrawCurve(result, location);
				// ***********************************
				// tang bien diem so nguoi
				// ***********************************
			}
		}
		
		if(param->m_isViewSVMDetection)
			utils.OutputResult(result, vectorRect, CV_RGB(255,0,0));
					
		count++;

		PostMessage(param->m_hWnd,WM_USER_THREAD_UPDATE_PROGRESS,(WPARAM)result,0);
		// chinh lai cho nay, doi lai bien dem count (thoa ca 2 SVM + shape)
		PostMessage(param->m_hWnd,WM_USER_THREAD_UPDATE_INFO,(WPARAM)vectorRect.size(),0);
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
	if(m_btnPlay.IsWindowEnabled())
	{
		if(CStudentDetectionDlg::m_windowParam->m_videoPath == NULL)
		{
			MessageBox(_T("Please select video"), _T("Information"));
			return;
		}

		m_btnPlay.EnableWindow(0);
		video_thread = AfxBeginThread(playVideoThread, CStudentDetectionDlg::m_windowParam, THREAD_PRIORITY_NORMAL, 0, 0);
		m_bIsPlayVideo = true;
	}
}

void CStudentDetectionDlg::OnBnClickedBtnStop()
{
	if(video_thread != NULL)
	{
		video_thread->SuspendThread();		
		m_btnPlay.EnableWindow();
		m_bIsPlayVideo = false;
	}
}

void CStudentDetectionDlg::OnBnClickedCheckViewHair()
{
	CStudentDetectionDlg::m_windowParam->m_isViewHairDetection = m_checkViewHair.GetCheck();
}

void CStudentDetectionDlg::OnBnClickedCheckViewSvm()
{
	CStudentDetectionDlg::m_windowParam->m_isViewSVMDetection = m_checkViewSVM.GetCheck();
}

void CStudentDetectionDlg::OnBnClickedCheckViewShape()
{
	CStudentDetectionDlg::m_windowParam->m_isViewShapeDetection = m_checkViewShape.GetCheck();
}

LRESULT CStudentDetectionDlg::OnThreadFinished(WPARAM wParam,LPARAM lParam)
{
	m_btnPlay.EnableWindow();
	m_bIsPlayVideo = false;
	return 0;
}

LRESULT CStudentDetectionDlg::OnThreadUpdateProgress(WPARAM wParam,LPARAM lParam)
{
	IplImage* frame = (IplImage*)wParam;
	ImageProcessor imgProcess;
	CRect clientRect;
	//m_video.GetClientRect(clientRect);
	m_videoPlayer.GetClientRect(clientRect);

	CvvImage cvv;
	cvv.CopyOf(imgProcess.getSubImageAndResize(frame, cvRect(0,0,frame->width, frame->height), clientRect.Width(), clientRect.Height()));
	//cvv.Show(m_video.GetDC()->m_hDC, 0, 0, clientRect.Width(), clientRect.Height());
	cvv.Show(m_videoPlayer.GetDC()->m_hDC, 0, 0, clientRect.Width(), clientRect.Height());
	cvv.Destroy();	

	return 0;
}

LRESULT CStudentDetectionDlg::OnThreadUpdateInfo(WPARAM wParam,LPARAM lParam)
{
	Utils utils;
	int count = (int)wParam;
	//m_static_student_count.SetWindowTextW(utils.ConvertToCString(count));
	m_editStudentCount.SetWindowTextW(utils.ConvertToCString(count));
	//m_static_student_count.Invalidate();
	return 0;
}

void CStudentDetectionDlg::OnBnClickedBtnApplyParams()
{
	// TODO: Add your control notification handler code here
	//apply head params
	CString tmp;
	Utils utils;
	
	m_tabHeadParams->m_editMaxHeadArea.GetWindowTextW(tmp);
	CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Head_Params.m_iMaxHeadArea = utils.ConvertToInt(tmp);

	m_tabHeadParams->m_editMaxHeadAreaAtTop.GetWindowTextW(tmp);
	CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Head_Params.m_iMaxHeadAreaTop = utils.ConvertToInt(tmp);
		
	m_tabHeadParams->m_editMaxWidthHead.GetWindowTextW(tmp);
	CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Head_Params.m_iMaxWidth  = utils.ConvertToInt(tmp);

	m_tabHeadParams->m_editMinAreaAtBottom.GetWindowTextW(tmp);
	CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Head_Params.m_iMinHeadAreaBottom = utils.ConvertToInt(tmp);

	m_tabHeadParams->m_editMinHeadArea.GetWindowTextW(tmp);
	CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Head_Params.m_iMinHeadArea = utils.ConvertToInt(tmp);

	m_tabHeadParams->m_editMinWidthHead.GetWindowTextW(tmp);
	CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Head_Params.m_iMinWidth  = utils.ConvertToInt(tmp);

	m_tabHeadParams->m_editRelativeHeightWidth.GetWindowTextW(tmp);
	CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Head_Params.m_iRelative_Height_Width  = utils.ConvertToInt(tmp);

	m_tabHeadParams->m_editRelativeWidthHeight.GetWindowTextW(tmp);
	CStudentDetectionDlg::m_windowParam->m_DetectionParams.m_Head_Params.m_iRelative_Width_Height  = utils.ConvertToInt(tmp);
}
/*
BOOL CStudentDetectionDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnEraseBkgnd(pDC);
	SBitdraw(pDC,IDB_BMP_BACKGROUND);
	return true;	
}
*/
BOOL CStudentDetectionDlg::OnEraseBkgnd(CDC* pDC) 
{
	CBitmap m_bitmap;
	BOOL rVal = FALSE;

	if( m_bitmap.LoadBitmap( IDB_BMP_BACKGROUND ) )
	{
		CRect rect;
		GetClientRect( &rect );

		CDC dc;
		dc.CreateCompatibleDC( pDC );
		CBitmap* pOldBitmap = dc.SelectObject( &m_bitmap );

		pDC->BitBlt( 0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);

		dc.SelectObject(pOldBitmap);
		rVal = TRUE;
	}

	return rVal;
}

bool CStudentDetectionDlg::SBitdraw(CDC *pDC, UINT nIDResource)
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
HBRUSH CStudentDetectionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	/*
	HBRUSH hbr;
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);		
		hbr = (HBRUSH)GetStockObject( NULL_BRUSH );
	}
	else
	{
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);		
	}	
	return hbr;	
	*/

	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

    DWORD dwStyle = pWnd->GetStyle();
    if ( dwStyle & (BS_AUTORADIOBUTTON | BS_RADIOBUTTON | BS_AUTOCHECKBOX |BS_CHECKBOX) )
	{
        hbr = (HBRUSH) GetStockObject (WHITE_BRUSH);
    }
	else
	{
		switch(nCtlColor)
		{
		case CTLCOLOR_STATIC:
		case CTLCOLOR_EDIT:
			pDC->SetBkMode(TRANSPARENT);			
			hbr = (HBRUSH)GetStockObject( NULL_BRUSH );			
			break;	
		default:
			hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
			break;
		}
	}

	if(pWnd->GetDlgCtrlID() == IDC_EDIT_STUDENT_COUNT)
	{		
		pDC->SetBkMode(TRANSPARENT);		  
		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);	
	}
    return hbr;
}
void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CStudentDetectionDlg::OnStnClickedPlayVideo()
{
	// TODO: Add your control notification handler code here
	if(!m_bIsPlayVideo)
	{
		InputDlg dlg;
		if(dlg.DoModal() == IDOK)
		{
			Utils utils;			
			CStudentDetectionDlg::m_windowParam->m_videoPath = utils.ConvertToChar(dlg.m_videoPath);		
			CStudentDetectionDlg::m_windowParam->m_maskPath = utils.ConvertToChar(dlg.m_maskPath);
		}
	}
}
