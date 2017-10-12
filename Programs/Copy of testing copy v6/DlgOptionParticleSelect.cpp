// DlgOptionParticleSelect.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgOptionParticleSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionParticleSelect dialog


CDlgOptionParticleSelect::CDlgOptionParticleSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionParticleSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionParticleSelect)
	m_nPrtlId = 0;
	m_nPrtlNgbrs = 0;
	m_strPrtlPos = _T("");
	m_fEnclosingBallRadius = 0.0f;
	m_bShowSelectedPrtls = FALSE;
	m_bShowEnclosingBall = FALSE;
	m_nSelectedNum = 0;
	m_fBallCtrX = 0.0f;
	m_fBallCtrY = 0.0f;
	m_fBallCtrZ = 0.0f;
	m_strPrtlVel = _T("");
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;
	m_curSelPrtl = NULL;

	InitializeParticleSelectOptions();
}


void CDlgOptionParticleSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionParticleSelect)
	DDX_Control(pDX, IDC_LIST_SELECTED_PRTLS, m_listSelectedPrtls);
	DDX_Text(pDX, IDC_PRTL_ID, m_nPrtlId);
	DDX_Text(pDX, IDC_PRTL_NGBRS, m_nPrtlNgbrs);
	DDX_Text(pDX, IDC_PRTL_POS, m_strPrtlPos);
	DDX_Text(pDX, IDC_ENCLOSING_BALL_RADIUS, m_fEnclosingBallRadius);
	DDX_Check(pDX, IDC_CHK_SHOW_SELECTED_PRTLS, m_bShowSelectedPrtls);
	DDX_Check(pDX, IDC_CHK_SHOW_ENCLOSING_BALL, m_bShowEnclosingBall);
	DDX_Text(pDX, IDC_SELECTED_NUM, m_nSelectedNum);
	DDX_Text(pDX, IDC_BALL_CTR_X, m_fBallCtrX);
	DDX_Text(pDX, IDC_BALL_CTR_Y, m_fBallCtrY);
	DDX_Text(pDX, IDC_BALL_CTR_Z, m_fBallCtrZ);
	DDX_Text(pDX, IDC_PRTL_VEL, m_strPrtlVel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionParticleSelect, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionParticleSelect)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(ID_BTN_SELECT_PRTLS, OnBtnSelectPrtls)
	ON_BN_CLICKED(ID_BTN_DESELECT_PRTLS, OnBtnDeselectPrtls)
	ON_BN_CLICKED(ID_BTN_DESELECT_ALL, OnBtnDeselectAll)
	ON_LBN_SELCHANGE(IDC_LIST_SELECTED_PRTLS, OnSelchangeListSelectedPrtls)
	ON_BN_CLICKED(ID_BTN_CTR_FROM_PRTL, OnBtnCtrFromPrtl)
	ON_BN_CLICKED(ID_BTN_HIGHLIGHT_CUR_PRTL, OnBtnHighlightCurPrtl)
	ON_BN_CLICKED(ID_BTN_HIGHLIGHT_NGBRS, OnBtnHighlightNgbrs)
	ON_BN_CLICKED(ID_BTN_DEHIGHLIGHT_ALL, OnBtnDehighlightAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgOptionParticleSelect::RefreshData()
{
	OnSelchangeListSelectedPrtls();
}

void CDlgOptionParticleSelect::InitializeParticleSelectOptions()
{
	m_bShowEnclosingBall = CSimuManager::m_enclosingBall->m_ebShow;
	m_bShowSelectedPrtls = CSimuManager::SHOW_SELECTED_PRTL_ONLY;
	m_fBallCtrX = CSimuManager::m_enclosingBall->m_ebPos[X];
	m_fBallCtrY = CSimuManager::m_enclosingBall->m_ebPos[Y];
	m_fBallCtrZ = CSimuManager::m_enclosingBall->m_ebPos[Z];
	m_fEnclosingBallRadius = CSimuManager::m_enclosingBall->m_ebRadius;
}

void CDlgOptionParticleSelect::UpdateParticleSelectOptions()
{
	CSimuManager::m_enclosingBall->m_ebShow = m_bShowEnclosingBall;
	CSimuManager::SHOW_SELECTED_PRTL_ONLY = m_bShowSelectedPrtls;
	CSimuManager::m_enclosingBall->m_ebPos[X] = m_fBallCtrX;
	CSimuManager::m_enclosingBall->m_ebPos[Y] = m_fBallCtrY;
	CSimuManager::m_enclosingBall->m_ebPos[Z] = m_fBallCtrZ;
	CSimuManager::m_enclosingBall->m_ebRadius = m_fEnclosingBallRadius;
}

void CDlgOptionParticleSelect::FillInSelectedParticleList()
{
	char strPrtlId[10];
	m_nSelectedNum = 0;
	m_listSelectedPrtls.ResetContent();
	if (m_pSimuManager->m_curPrtlFluid != NULL)
	{
		for (int i=0; i < m_pSimuManager->m_curPrtlFluid->m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = m_pSimuManager->m_curPrtlFluid->m_pfPrtls[i];
			if (!pPrtl->m_geSelected) continue;
			sprintf(strPrtlId, "%d", pPrtl->GetGeometryElementId());
			m_listSelectedPrtls.InsertString(m_nSelectedNum, strPrtlId);
			m_listSelectedPrtls.SetItemDataPtr(m_nSelectedNum++, pPrtl);
		}
		UpdateData(false);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionParticleSelect message handlers

void CDlgOptionParticleSelect::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	InitializeParticleSelectOptions();
	UpdateData(false);

	CDialog::OnCancel();
}

void CDlgOptionParticleSelect::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();
	
	CDialog::OnOK();
}

void CDlgOptionParticleSelect::OnApply() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	UpdateParticleSelectOptions();

	m_pSimuManager->RefreshView();
}

void CDlgOptionParticleSelect::OnBtnSelectPrtls() 
{
	// TODO: Add your control notification handler code here
	
	OnApply();
	m_pSimuManager->SelectEnclosedParticles();
	FillInSelectedParticleList();
	m_pSimuManager->RefreshView();
}

void CDlgOptionParticleSelect::OnBtnDeselectPrtls() 
{
	// TODO: Add your control notification handler code here
	
	OnApply();
	m_pSimuManager->DeSelectEnclosedParticles();
	FillInSelectedParticleList();
	m_pSimuManager->RefreshView();
}

void CDlgOptionParticleSelect::OnBtnDeselectAll() 
{
	// TODO: Add your control notification handler code here
	
	OnApply();
	m_pSimuManager->DeSelectAllParticles();
	FillInSelectedParticleList();
	m_pSimuManager->RefreshView();
}

void CDlgOptionParticleSelect::OnSelchangeListSelectedPrtls() 
{
	// TODO: Add your control notification handler code here
	
	int curId = m_listSelectedPrtls.GetCurSel();
	if (curId == -1)
		return;
	if (m_pSimuManager->m_curPrtlFluid != NULL)
	{
		CFluidPrtl* pPrtl = (CFluidPrtl*)m_listSelectedPrtls.GetItemDataPtr(curId);
		m_nPrtlId = pPrtl->GetGeometryElementId();
		m_nPrtlNgbrs = 0;
		pPrtl->m_vpPos->MakeStringFromValue(m_strPrtlPos);
		pPrtl->m_vpVel->MakeStringFromValue(m_strPrtlVel);
		m_curSelPrtl = pPrtl;
	}
	UpdateData(false);
}

void CDlgOptionParticleSelect::OnBtnCtrFromPrtl() 
{
	// TODO: Add your control notification handler code here
	
	if (m_curSelPrtl == NULL) return;

	m_fBallCtrX = m_curSelPrtl->m_vpPos->v[X];
	m_fBallCtrY = m_curSelPrtl->m_vpPos->v[Y];
	m_fBallCtrZ = m_curSelPrtl->m_vpPos->v[Z];

	UpdateData(false);
	OnApply();
}

void CDlgOptionParticleSelect::OnBtnHighlightCurPrtl() 
{
	// TODO: Add your control notification handler code here
	
	if (m_curSelPrtl == NULL) return;
	m_curSelPrtl->m_geHighlit = true;
	m_pSimuManager->RefreshView();
}

void CDlgOptionParticleSelect::OnBtnHighlightNgbrs() 
{
	// TODO: Add your control notification handler code here
	
	if (m_curSelPrtl == NULL) return;
	if (m_pSimuManager->m_curPrtlFluid != NULL)
	{
		for (int i=0; i < m_curSelPrtl->m_fpNgbrs.m_paNum; i++)
		{
			m_curSelPrtl->m_fpNgbrs[i]->m_geHighlit = true;
		}
		m_pSimuManager->RefreshView();
	}
}

void CDlgOptionParticleSelect::OnBtnDehighlightAll() 
{
	// TODO: Add your control notification handler code here
	
	if (m_pSimuManager->m_curPrtlFluid != NULL)
	{
		for (int i=0; i < m_pSimuManager->m_curPrtlFluid->m_pfPrtls.m_paNum; i++)
		{
			m_pSimuManager->m_curPrtlFluid->m_pfPrtls[i]->m_geHighlit = false;
		}
		m_pSimuManager->RefreshView();
	}
}
