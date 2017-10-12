// SimuFlexApp.cpp : Defines the class behaviors for the application.
//

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuManager.h"
#include "ConvexHull.h"
#include "DelaunayTetrahedron.h"

#include <QTime>
#include <math.h>


/////////////////////////////////////////////////////////////////////////////
// CSimuFlexApp

//BEGIN_MESSAGE_MAP(CSimuFlexApp, CWinApp)
	//{{AFX_MSG_MAP(CSimuFlexApp)
//	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuFlexApp construction

CSimuFlexApp::CSimuFlexApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSimuFlexApp object

CSimuFlexApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSimuFlexApp initialization

bool CSimuFlexApp::InitInstance()
{
	/*
	// Find correct data directory name
	CFileStatus tmpStatus;
	if (CFile::GetStatus(".", tmpStatus))
	{
		CSimuManager::m_strAppDataDir = tmpStatus.m_szFullName;
		CSimuManager::m_strAppDataDir += "\\";
		CSimuManager::m_strAppDataDir += APP_RELATIVE_DATA_DIR;
		CSimuManager::m_strImportDir = CSimuManager::m_strAppDataDir;
	}
	else
	{
		//TRACE("********** Current application directory not found. **********\n");
		exit(1);
	}

	*/

	//CTime curTime	= CTime::GetCurrentTime();
	QTime curTime	= QTime::currentTime();
	QString timeStr	= curTime.toString();
	CSimuManager::m_strDataDebugFile = "dataDebugFile_";
	CSimuManager::m_strDataDebugFile += timeStr;
	CSimuManager::m_strDataDebugFile += ".txt";

	CreateTemporaryStaticObjects();

	CSimuManager::GRAVITY->ParseStringForValue(GRAVITY_ACCELERATION);
	CSimuManager::m_hilightColor->ParseStringForValue(COLOR_ORANGE_STR);
	CSimuManager::COLOR_WHITE->ParseStringForValue(COLOR_WHITE_STR);
	CSimuManager::COLOR_RED->ParseStringForValue(COLOR_RED_STR);
	CSimuManager::COLOR_GREEN->ParseStringForValue(COLOR_GREEN_STR);
	CSimuManager::COLOR_BLUE->ParseStringForValue(COLOR_BLUE_STR);
	CSimuManager::COLOR_YELLOW->ParseStringForValue(COLOR_YELLOW_STR);
	CSimuManager::COLOR_MAGENTA->ParseStringForValue(COLOR_MAGENTA_STR);
	CSimuManager::COLOR_CYAN->ParseStringForValue(COLOR_CYAN_STR);
	CSimuManager::COLOR_ORANGE->ParseStringForValue(COLOR_ORANGE_STR);

	if (CSimuManager::DIFFER_SRFC_COLOR)
		CSimuManager::COLOR_SRFC->ParseStringForValue(COLOR_BLUE_STR);
	else
		CSimuManager::COLOR_SRFC->ParseStringForValue(COLOR_WHITE_STR);
	CSimuManager::COLOR_INSIDE->ParseStringForValue(COLOR_WHITE_STR);
	CSimuManager::COLOR_FIXED->ParseStringForValue(COLOR_GREEN_STR);

	CConvexHull::m_chCtrColor->SetValue(CSimuManager::COLOR_YELLOW);
	CConvexHull::m_chCurPColor->SetValue(CSimuManager::COLOR_RED);
	CConvexHull::m_chTriPColor->SetValue(CSimuManager::COLOR_MAGENTA);
	CConvexHull::m_chCoveredPColor->SetValue(CSimuManager::COLOR_GREEN);
	CConvexHull::m_chNonCoveredPColor->SetValue(CSimuManager::COLOR_WHITE);

	CDelaunayTetrahedron::m_dtrCtrColor->SetValue(CSimuManager::COLOR_RED);
	CDelaunayTetrahedron::m_dtrTriPColor->SetValue(CSimuManager::COLOR_YELLOW);
	CDelaunayTetrahedron::m_dtrTetraPColor->SetValue(CSimuManager::COLOR_GREEN);
	CDelaunayTetrahedron::m_dtrNgbrPColor->SetValue(CSimuManager::COLOR_WHITE);
	CDelaunayTetrahedron::m_dtrTestPColor->SetValue(CSimuManager::COLOR_BLUE);
	CDelaunayTetrahedron::m_dtrCircumCtrColor->SetValue(CSimuManager::COLOR_MAGENTA);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
*/
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CSimuManager* pFrame = new CSimuManager;
	//m_pMainWnd = pFrame;

	// create and load the frame with its resources
	//pFrame->LoadFrame(IDR_MAINFRAME,
	//	WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
	//	NULL);

	// The one and only window has been initialized, so show and update it.
	//pFrame->ShowWindow(SW_SHOW);
	//pFrame->UpdateWindow();

	//CSimuMsg::InitializeDialogs(pFrame);

	return TRUE;
}

int CSimuFlexApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	//CSimuMsg::DeleteDialogs();

	DeleteTemporaryStaticObjects();

	//return CWinApp::ExitInstance();
	return 0;
}

void CSimuFlexApp::CreateTemporaryStaticObjects()
{
	int rowId;
	//CSimuManager::m_enclosingBall	= new CSimuEnclosingBall();
	CSimuManager::m_hilightColor= new CVector3D();
	CSimuManager::GRAVITY		= new CVector3D();
	CSimuManager::COLOR_WHITE	= new CVector3D();
	CSimuManager::COLOR_RED		= new CVector3D();
	CSimuManager::COLOR_GREEN	= new CVector3D();
	CSimuManager::COLOR_BLUE	= new CVector3D();
	CSimuManager::COLOR_YELLOW	= new CVector3D();
	CSimuManager::COLOR_MAGENTA	= new CVector3D();
	CSimuManager::COLOR_CYAN	= new CVector3D();
	CSimuManager::COLOR_ORANGE	= new CVector3D();
	CSimuManager::COLOR_SRFC	= new CVector3D();
	CSimuManager::COLOR_INSIDE	= new CVector3D();
	CSimuManager::COLOR_FIXED	= new CVector3D();
	CSimuManager::m_viewBoundary= new CVector(BOUNDARY_DIM, "view boundary");

	CConvexHull::m_chCtrColor = new CVector3D();
	CConvexHull::m_chCurPColor = new CVector3D();
	CConvexHull::m_chTriPColor = new CVector3D();
	CConvexHull::m_chCoveredPColor = new CVector3D();
	CConvexHull::m_chNonCoveredPColor = new CVector3D();

	CDelaunayTetrahedron::m_dtrCtrColor = new CVector3D();
	CDelaunayTetrahedron::m_dtrTriPColor = new CVector3D();
	CDelaunayTetrahedron::m_dtrTetraPColor = new CVector3D();
	CDelaunayTetrahedron::m_dtrNgbrPColor = new CVector3D();
	CDelaunayTetrahedron::m_dtrTestPColor = new CVector3D();
	CDelaunayTetrahedron::m_dtrCircumCtrColor = new CVector3D();

	CVector3D::m_staticV3D1 = new CVector3D();
	CVector3D::m_staticV3D2 = new CVector3D();
	CVector3D::m_staticV3D3 = new CVector3D();
	CVector3D::m_staticV3D4 = new CVector3D();

	int matrixRow = CSimuUtility::m_suEdgeMatrixRow;
	int matrxiCol = CSimuUtility::m_suEdgeMatrixCol;
	CSimuUtility::m_suEdgeMatrix = new SimuValue*[matrixRow];
	for (rowId=0; rowId < matrixRow; rowId++)
	{
		CSimuUtility::m_suEdgeMatrix[rowId] = new SimuValue[matrxiCol];
		memset(CSimuUtility::m_suEdgeMatrix[rowId], 0, matrxiCol*sizeof(SimuValue));
	}
	CSimuUtility::m_suCtrPos = new SimuValue[matrixRow];
	memset(CSimuUtility::m_suCtrPos, 0, matrixRow*sizeof(SimuValue));

	CSimuUtility::m_suEdge0 = new CVector3D();
	CSimuUtility::m_suEdge1 = new CVector3D();
	CSimuUtility::m_suEdge2 = new CVector3D();
	CSimuUtility::m_suMidP0 = new CVector3D();
	CSimuUtility::m_suMidP1 = new CVector3D();
	CSimuUtility::m_suMidP2 = new CVector3D();
	CSimuUtility::m_suTriNormal = new CVector3D();
	CSimuUtility::m_suIntersectLineVector0 = new CVector3D();
	CSimuUtility::m_suCircumcircleCenter = new CVector3D();

	matrixRow = CFluidPrtlNonNew::m_fpnnMRow;
	matrxiCol = CFluidPrtlNonNew::m_fpnnMCol;
	CFluidPrtlNonNew::m_fpnnM = new SimuValue*[matrixRow];
	
	for (rowId=0; rowId < matrixRow; rowId++)
	{
		CFluidPrtlNonNew::m_fpnnM[rowId] = new SimuValue[matrxiCol];
		memset(CFluidPrtlNonNew::m_fpnnM[rowId], 0, matrxiCol*sizeof(SimuValue));
	}
	CFluidPrtlNonNew::m_fpnnX = new SimuValue[matrixRow];
	memset(CFluidPrtlNonNew::m_fpnnX, 0, matrixRow*sizeof(SimuValue));

	CBoundary::m_bdryBalls = new TClassArray<CBoundary>(true, SIMU_PARA_BOUNDARY_NUM_ALLOC_SIZE);
	CBoundary::m_bdryNotBalls = new TClassArray<CBoundary>(true, SIMU_PARA_BOUNDARY_NUM_ALLOC_SIZE);
}

void CSimuFlexApp::DeleteTemporaryStaticObjects()
{
	//delete CSimuManager::m_enclosingBall;
	delete CSimuManager::m_hilightColor;
	delete CSimuManager::GRAVITY;
	delete CSimuManager::COLOR_WHITE;
	delete CSimuManager::COLOR_RED;
	delete CSimuManager::COLOR_GREEN;
	delete CSimuManager::COLOR_BLUE;
	delete CSimuManager::COLOR_YELLOW;
	delete CSimuManager::COLOR_MAGENTA;
	delete CSimuManager::COLOR_CYAN;
	delete CSimuManager::COLOR_ORANGE;
	delete CSimuManager::COLOR_SRFC;
	delete CSimuManager::COLOR_INSIDE;
	delete CSimuManager::COLOR_FIXED;
	delete CSimuManager::m_viewBoundary;

	delete CConvexHull::m_chCtrColor;
	delete CConvexHull::m_chCurPColor;
	delete CConvexHull::m_chTriPColor;
	delete CConvexHull::m_chCoveredPColor;
	delete CConvexHull::m_chNonCoveredPColor;

	delete CDelaunayTetrahedron::m_dtrCtrColor;
	delete CDelaunayTetrahedron::m_dtrTriPColor;
	delete CDelaunayTetrahedron::m_dtrTetraPColor;
	delete CDelaunayTetrahedron::m_dtrNgbrPColor;
	delete CDelaunayTetrahedron::m_dtrTestPColor;
	delete CDelaunayTetrahedron::m_dtrCircumCtrColor;

	delete CVector3D::m_staticV3D1;
	delete CVector3D::m_staticV3D2;
	delete CVector3D::m_staticV3D3;
	delete CVector3D::m_staticV3D4;

	int matrixRow = CSimuUtility::m_suEdgeMatrixRow;
	int matrxiCol = CSimuUtility::m_suEdgeMatrixCol;
	delete CSimuUtility::m_suCtrPos;
	for (int rowId=0; rowId < matrixRow; rowId++) delete CSimuUtility::m_suEdgeMatrix[rowId];
	delete CSimuUtility::m_suEdgeMatrix;

	delete CSimuUtility::m_suEdge0;
	delete CSimuUtility::m_suEdge1;
	delete CSimuUtility::m_suEdge2;
	delete CSimuUtility::m_suMidP0;
	delete CSimuUtility::m_suMidP1;
	delete CSimuUtility::m_suMidP2;
	delete CSimuUtility::m_suTriNormal;
	delete CSimuUtility::m_suIntersectLineVector0;
	delete CSimuUtility::m_suCircumcircleCenter;

	matrixRow = CFluidPrtlNonNew::m_fpnnMRow;
	matrxiCol = CFluidPrtlNonNew::m_fpnnMCol;
	delete CFluidPrtlNonNew::m_fpnnX;
	for (int rowId=0; rowId < matrixRow; rowId++) delete CFluidPrtlNonNew::m_fpnnM[rowId];
	delete CFluidPrtlNonNew::m_fpnnM;

	delete CBoundary::m_bdryBalls;
	delete CBoundary::m_bdryNotBalls;
}

/////////////////////////////////////////////////////////////////////////////
// CSimuFlexApp message handlers




/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#if 0
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSimuFlexApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
#endif