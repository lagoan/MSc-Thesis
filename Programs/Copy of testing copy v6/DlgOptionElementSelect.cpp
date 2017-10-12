// DlgOptionElementSelect.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgOptionElementSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionElementSelect dialog


CDlgOptionElementSelect::CDlgOptionElementSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionElementSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionElementSelect)
	m_fBallCtrX = 0.0f;
	m_fBallCtrY = 0.0f;
	m_fBallCtrZ = 0.0f;
	m_bShowEnclosingBall = FALSE;
	m_fEnclosingBallRadius = 0.0f;
	m_nElmntId = 0;
	m_strElmntPos = _T("");
	m_nSelectedNum = 0;
	m_bShowSelectedElmnt = FALSE;
	m_nElmntType = -1;
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;
	m_curSelElmnt = NULL;

	InitializeElementSelectOptions();
}


void CDlgOptionElementSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionElementSelect)
	DDX_Control(pDX, IDC_LIST_SELECTED_ELMNT, m_listSelectedElmnt);
	DDX_Text(pDX, IDC_BALL_CTR_X, m_fBallCtrX);
	DDX_Text(pDX, IDC_BALL_CTR_Y, m_fBallCtrY);
	DDX_Text(pDX, IDC_BALL_CTR_Z, m_fBallCtrZ);
	DDX_Check(pDX, IDC_CHK_SHOW_ENCLOSING_BALL, m_bShowEnclosingBall);
	DDX_Text(pDX, IDC_ENCLOSING_BALL_RADIUS, m_fEnclosingBallRadius);
	DDX_Text(pDX, IDC_ELMNT_ID, m_nElmntId);
	DDX_Text(pDX, IDC_ELMNT_POS, m_strElmntPos);
	DDX_Text(pDX, IDC_SELECTED_NUM, m_nSelectedNum);
	DDX_Check(pDX, IDC_CHK_SHOW_SELECTED_ELMNT, m_bShowSelectedElmnt);
	DDX_Radio(pDX, IDC_RDO_ELMNT_TYPE, m_nElmntType);
	//}}AFX_DATA_MAP
}

void CDlgOptionElementSelect::RefreshData()
{
}

void CDlgOptionElementSelect::InitializeElementSelectOptions()
{
	m_bShowEnclosingBall = CSimuManager::m_enclosingBall->m_ebShow;
	m_fBallCtrX = CSimuManager::m_enclosingBall->m_ebPos[X];
	m_fBallCtrY = CSimuManager::m_enclosingBall->m_ebPos[Y];
	m_fBallCtrZ = CSimuManager::m_enclosingBall->m_ebPos[Z];
	m_fEnclosingBallRadius = CSimuManager::m_enclosingBall->m_ebRadius;
	m_bShowSelectedElmnt = CSimuManager::SHOW_SELECTED_ELMNT_ONLY;
	switch (CSimuManager::m_eSelectedElmntType)
	{
		case CSimuManager::SELECT_TETRAHEDRON:
			m_nElmntType = 0;
			break;
		case CSimuManager::SELECT_TRIANGLE:
			m_nElmntType = 1;
			break;
		case CSimuManager::SELECT_PARTICLE:
			m_nElmntType = 2;
			break;
	}
}

void CDlgOptionElementSelect::UpdateElementSelectOptions()
{
	CSimuManager::m_enclosingBall->m_ebShow = m_bShowEnclosingBall;
	CSimuManager::m_enclosingBall->m_ebPos[X] = m_fBallCtrX;
	CSimuManager::m_enclosingBall->m_ebPos[Y] = m_fBallCtrY;
	CSimuManager::m_enclosingBall->m_ebPos[Z] = m_fBallCtrZ;
	CSimuManager::m_enclosingBall->m_ebRadius = m_fEnclosingBallRadius;
	CSimuManager::SHOW_SELECTED_ELMNT_ONLY = m_bShowSelectedElmnt;
	switch (m_nElmntType)
	{
		case 0:
			CSimuManager::m_eSelectedElmntType = CSimuManager::SELECT_TETRAHEDRON;
			break;
		case 1:
			CSimuManager::m_eSelectedElmntType = CSimuManager::SELECT_TRIANGLE;
			break;
		case 2:
			CSimuManager::m_eSelectedElmntType = CSimuManager::SELECT_PARTICLE;
			break;
	}
}

void CDlgOptionElementSelect::FillInSelectedElementList()
{
	char strElmntId[10];
	m_nSelectedNum = 0;
	m_listSelectedElmnt.ResetContent();
	if (m_pSimuManager->m_curPrtlFluid != NULL)
	{
		if (CSimuManager::m_eSelectedElmntType == CSimuManager::SELECT_TRIANGLE)
		{
		}
	}
	UpdateData(false);
}

BEGIN_MESSAGE_MAP(CDlgOptionElementSelect, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionElementSelect)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(ID_BTN_SELECT_ELMNT, OnBtnSelectElmnt)
	ON_BN_CLICKED(ID_BTN_DESELECT_ELMNT, OnBtnDeselectElmnt)
	ON_BN_CLICKED(ID_BTN_DESELECT_ALL, OnBtnDeselectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionElementSelect message handlers

void CDlgOptionElementSelect::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	InitializeElementSelectOptions();
	UpdateData(false);

	CDialog::OnCancel();
}

void CDlgOptionElementSelect::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();
	
	CDialog::OnOK();
}

void CDlgOptionElementSelect::OnApply() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	UpdateElementSelectOptions();

	m_pSimuManager->RefreshView();
}

void CDlgOptionElementSelect::OnBtnSelectElmnt() 
{
	// TODO: Add your control notification handler code here
	
	OnApply();
	if (m_pSimuManager->m_curPrtlFluid != NULL
	 && CSimuManager::m_eSelectedElmntType == CSimuManager::SELECT_TRIANGLE)
	{
	}
	FillInSelectedElementList();
	m_pSimuManager->RefreshView();
}

void CDlgOptionElementSelect::OnBtnDeselectElmnt() 
{
	// TODO: Add your control notification handler code here
	
	OnApply();
	if (m_pSimuManager->m_curPrtlFluid != NULL
	 && CSimuManager::m_eSelectedElmntType == CSimuManager::SELECT_TRIANGLE)
	{
	}
	FillInSelectedElementList();
	m_pSimuManager->RefreshView();
}

void CDlgOptionElementSelect::OnBtnDeselectAll() 
{
	// TODO: Add your control notification handler code here
	
	OnApply();
	if (m_pSimuManager->m_curPrtlFluid != NULL
	 && CSimuManager::m_eSelectedElmntType == CSimuManager::SELECT_TRIANGLE)
	{
	}
	FillInSelectedElementList();
	m_pSimuManager->RefreshView();
}
