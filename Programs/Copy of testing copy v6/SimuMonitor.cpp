// SimuMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuMsg.h"
#include "SimuMonitor.h"

#include "SimuManager.h"
#include "SimuDriver.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuMonitor dialog


CSimuMonitor::CSimuMonitor(CWnd* pParent)
	: CDialog(CSimuMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimuMonitor)
	m_uPrevFwdIntvls = 0;
	m_strSimuPropInfo = _T("");
	m_dbFrameIntvl = 0.0;
	m_bEnableMessage = FALSE;
	m_dbAnimationClock = 0.0;
	m_fFluidDensity = 0.0f;
	m_fPrtlMass = 0.0f;
	m_strAverageTime = _T("");
	m_fFluidSmoothLen = 0.0f;
	m_fPrtlDist = 0.0f;
	m_dbTimeStep = 0.0;
	//}}AFX_DATA_INIT

	m_bGo				= false;
	// all time step are in second.
	m_dbFrameIntvl		= 1.0/CSimuManager::m_numFramesPerSecond;
	m_dbTimeStep		= CSimuManager::ComputeTimeStep();
	m_uPrevFwdIntvls	= 1;
	m_uFwdIntvls		= 1;
	m_bEnableMessage	= CSimuManager::m_bShowMsg;

	if (pParent == NULL)
	{
		CSimuMsg::ExitWithMessage("CSimuMonitor::CSimuMonitor", "pParent == NULL");
	}
	m_pSimuManager	= (CSimuManager*)pParent;
	m_pNewFont		= NULL;
}

CSimuMonitor::~CSimuMonitor()
{
	if (m_pNewFont != NULL)
		delete m_pNewFont;
}

/////////////////////////////////////////////////////////////////////////////
// CSimuMonitor public methods
void CSimuMonitor::ShowSimulationInformation()
{
	char	tmpBuf[LEN_MESSAGE];

	m_strSimuPropInfo = m_pSimuManager->m_pSimuView->GetFocusPositionValueString();

	if (CSimuManager::EXPORT_FLUID_DATA)
	{
		if (CSimuManager::m_nCurFrame != 0)
		{
			sprintf(tmpBuf, "%f s/frame",
					CSimuManager::m_dbTotalRunningTime/CSimuManager::m_nCurFrame);
			m_strAverageTime = tmpBuf;
		}
	}
	else if (fabs(m_dbAnimationClock) > SIMU_VALUE_EPSILON)
	{
		sprintf(tmpBuf, "%f s/step", CSimuManager::m_dbTotalRunningTime/CSimuManager::m_nCurStep);
		m_strAverageTime = tmpBuf;
	}


	m_editSimuProgInfo.SetWindowText(CSimuMsg::GetSimuInfo());
	m_editSimuProgInfo.LineScroll(0);
	UpdateData(false);
}

/////////////////////////////////////////////////////////////////////////////
// CSimuMonitor private methods


void CSimuMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuMonitor)
	DDX_Control(pDX, IDC_EDIT_SIMU_PROG_INFO, m_editSimuProgInfo);
	DDX_Text(pDX, IDC_EDIT_FWD_INTVLS, m_uPrevFwdIntvls);
	DDX_Text(pDX, IDC_EDIT_SIMU_PROP_INFO, m_strSimuPropInfo);
	DDX_Text(pDX, IDC_EDIT_FRAME_INTVL, m_dbFrameIntvl);
	DDX_Check(pDX, IDC_CHK_ENABLE_MESSAGE, m_bEnableMessage);
	DDX_Text(pDX, IDC_EDIT_ANIMATION_CLOCK, m_dbAnimationClock);
	DDX_Text(pDX, IDC_EDIT_FLUID_DENSITY, m_fFluidDensity);
	DDX_Text(pDX, IDC_EDIT_PRTL_MASS, m_fPrtlMass);
	DDX_Text(pDX, IDC_EDIT_AVERAGE_TIME, m_strAverageTime);
	DDX_Text(pDX, IDC_EDIT_FLUID_SMOOTH_LEN, m_fFluidSmoothLen);
	DDX_Text(pDX, IDC_EDIT_PRTL_DIST, m_fPrtlDist);
	DDX_Text(pDX, IDC_EDIT_TIME_STEP, m_dbTimeStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimuMonitor, CDialog)
	//{{AFX_MSG_MAP(CSimuMonitor)
	ON_BN_CLICKED(IDC_BTN_GO, OnBtnGo)
	ON_BN_CLICKED(IDC_BTN_FORWARD, OnBtnForward)
	ON_BN_CLICKED(IDC_BTN_PAUSE, OnBtnPause)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_CHK_ENABLE_MESSAGE, OnChkEnableMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuMonitor message handlers

BOOL CSimuMonitor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	LOGFONT logFont;
	CFont* pOldFont = m_editSimuProgInfo.GetFont();
	if (pOldFont->GetLogFont(&logFont) > 0)
	{
//		logFont.lfWeight = 400;
//		logFont.lfHeight = 20;
//		logFont.lfWidth = 0;
//		strcpy(logFont.lfFaceName, "Courier New");
		m_pNewFont = new CFont();
		m_pNewFont->CreateFontIndirect(&logFont);
		m_editSimuProgInfo.SetFont(m_pNewFont);
	}

	((CWnd*)GetDlgItem(IDC_EDIT_FRAME_INTVL))->EnableWindow(false);
	
	m_bEnableMessage		= CSimuManager::m_bShowMsg;
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSimuMonitor::OnBtnGo() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	m_bGo = true;
	m_uFwdIntvls = 0;
	m_pSimuManager->SetTimer(SIMU_TIMER_ID, 0, NULL);
}

void CSimuMonitor::OnBtnForward() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	m_bGo = false;
	m_uFwdIntvls = m_uPrevFwdIntvls;
	m_pSimuManager->SetTimer(SIMU_TIMER_ID, 0, NULL);
}

void CSimuMonitor::OnBtnPause() 
{
	// TODO: Add your control notification handler code here
	
	m_pSimuManager->KillTimer(SIMU_TIMER_ID);
}

void CSimuMonitor::OnBtnReset() 
{
	// TODO: Add your control notification handler code here
	
	m_pSimuManager->ResetAnimation();
}

void CSimuMonitor::OnChkEnableMessage() 
{
	// TODO: Add your control notification handler code here
	
	if (CSimuManager::m_bShowMsg)
	{
		CSimuManager::m_bShowMsg = false;
	}
	else
	{
		CSimuManager::m_bShowMsg = true;
	}
	m_bEnableMessage = CSimuManager::m_bShowMsg;
	UpdateData(false);
}
