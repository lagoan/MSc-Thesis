// DlgOptionMesher.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgOptionMesher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionMesher dialog


CDlgOptionMesher::CDlgOptionMesher(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionMesher::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionMesher)
	m_bDrawExtrSrfcPrtl = FALSE;
	m_bDrawIntrSrfcPrtl = FALSE;
	m_bDrawIntrSrfcTri = FALSE;
	m_bDrawPrtl = FALSE;
	m_bDrawTet = FALSE;
	m_bDrawIsomorphicLine = FALSE;
	m_bDebugMesher = FALSE;
	m_bDrawIntrSrfcNormal = FALSE;
	m_bDrawMesherGrid = FALSE;
	m_bDrawExtrSrfcTri = FALSE;
	m_bDrawSrfcTet = FALSE;
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;

	InitializeMesherOptions();
}


void CDlgOptionMesher::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionMesher)
	DDX_Check(pDX, IDC_CHK_DRAW_EXTR_SRFC_PRTL, m_bDrawExtrSrfcPrtl);
	DDX_Check(pDX, IDC_CHK_DRAW_INTR_SRFC_PRTL, m_bDrawIntrSrfcPrtl);
	DDX_Check(pDX, IDC_CHK_DRAW_INTR_SRFC_TRI, m_bDrawIntrSrfcTri);
	DDX_Check(pDX, IDC_CHK_DRAW_PRTL, m_bDrawPrtl);
	DDX_Check(pDX, IDC_CHK_DRAW_TET, m_bDrawTet);
	DDX_Check(pDX, IDC_CHK_DRAW_ISOMORPHIC_LINE, m_bDrawIsomorphicLine);
	DDX_Check(pDX, IDC_CHK_DEBUG_MESHER, m_bDebugMesher);
	DDX_Check(pDX, IDC_CHK_DRAW_NORMAL, m_bDrawIntrSrfcNormal);
	DDX_Check(pDX, IDC_CHK_DRAW_MESHER_GRID, m_bDrawMesherGrid);
	DDX_Check(pDX, IDC_CHK_DRAW_EXTR_SRFC_TRI, m_bDrawExtrSrfcTri);
	DDX_Check(pDX, IDC_CHK_DRAW_SRFC_TET, m_bDrawSrfcTet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionMesher, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionMesher)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgOptionMesher::InitializeMesherOptions()
{
	m_bDebugMesher			= CSimuManager::DEBUG_MESHER;
	m_bDrawMesherGrid		= CSimuManager::DRAW_MESHER_GRID;
	m_bDrawPrtl				= CSimuManager::DRAW_MESHER_PRTL;
	m_bDrawTet				= CSimuManager::DRAW_MESHER_TETRA;
	m_bDrawSrfcTet			= CSimuManager::DRAW_MESHER_SRFC_TETRA;
	m_bDrawIntrSrfcTri		= CSimuManager::DRAW_INTR_SRFC_TRIANGLES;
	m_bDrawIntrSrfcPrtl		= CSimuManager::DRAW_INTR_SRFC_PARTICLES;
	m_bDrawExtrSrfcPrtl		= CSimuManager::DRAW_EXTR_SRFC_PARTICLES;
	m_bDrawExtrSrfcTri		= CSimuManager::DRAW_EXTR_SRFC_TRIANGLES;
	m_bDrawIntrSrfcNormal	= CSimuManager::DRAW_INTR_SRFC_NORMALS;
	m_bDrawIsomorphicLine	= CSimuManager::DRAW_ISOMORPHIC_LINES;
}

void CDlgOptionMesher::UpdateMesherOptions()
{
	CSimuManager::DEBUG_MESHER				= m_bDebugMesher;
	CSimuManager::DRAW_MESHER_GRID			= m_bDrawMesherGrid;
	CSimuManager::DRAW_MESHER_PRTL			= m_bDrawPrtl;
	CSimuManager::DRAW_MESHER_TETRA			= m_bDrawTet;
	CSimuManager::DRAW_MESHER_SRFC_TETRA	= m_bDrawSrfcTet;
	CSimuManager::DRAW_INTR_SRFC_TRIANGLES	= m_bDrawIntrSrfcTri;
	CSimuManager::DRAW_INTR_SRFC_PARTICLES	= m_bDrawIntrSrfcPrtl;
	CSimuManager::DRAW_EXTR_SRFC_PARTICLES	= m_bDrawExtrSrfcPrtl;
	CSimuManager::DRAW_EXTR_SRFC_TRIANGLES	= m_bDrawExtrSrfcTri;
	CSimuManager::DRAW_INTR_SRFC_NORMALS	= m_bDrawIntrSrfcNormal;
	CSimuManager::DRAW_ISOMORPHIC_LINES		= m_bDrawIsomorphicLine;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionMesher message handlers

void CDlgOptionMesher::OnCancel() 
{
	// TODO: Add extra cleanup here

	UpdateData(false);
	
	CDialog::OnCancel();
}

void CDlgOptionMesher::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();
	
	CDialog::OnOK();
}

void CDlgOptionMesher::OnApply() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	UpdateMesherOptions();

	m_pSimuManager->RefreshView();
}
