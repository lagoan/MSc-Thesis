// DlgOptionSurface.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgOptionSurface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionSurface dialog


CDlgOptionSurface::CDlgOptionSurface(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionSurface::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionSurface)
	m_bCreateSrfcMesh = FALSE;
	m_fIsoDenRadiusRatio = 0.0f;
	m_fMeshSizeOverPrtlDist = 0.0f;
	m_fIsoDensityRatio = 0.0f;
	m_rdoIsoDensityChoice = -1;
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;

	InitializeSurfaceOptions();
}


void CDlgOptionSurface::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionSurface)
	DDX_Check(pDX, IDC_CHK_CREATE_SRFC_MESH, m_bCreateSrfcMesh);
	DDX_Text(pDX, IDC_ISO_DENSITY_RADIUS_RATIO, m_fIsoDenRadiusRatio);
	DDX_Text(pDX, IDC_MESH_SIZE_OVER_PRTL_DIST, m_fMeshSizeOverPrtlDist);
	DDX_Text(pDX, IDC_ISO_DENSITY_RATIO, m_fIsoDensityRatio);
	DDX_Radio(pDX, IDC_RDO_PRTL_NUM_DENSITY, m_rdoIsoDensityChoice);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionSurface, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionSurface)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgOptionSurface::InitializeSurfaceOptions()
{
	m_bCreateSrfcMesh = CSimuManager::CREATE_SURFACE_MESH;
	m_rdoIsoDensityChoice = CSimuManager::USE_PRTL_NUM_DENSITY ? 0 : 1;
	m_fIsoDensityRatio = CSimuManager::m_isoDensityRatio;
	m_fIsoDenRadiusRatio = CSimuManager::m_isoDenRadiusRatio;
	m_fMeshSizeOverPrtlDist = CSimuManager::m_meshSizeOverPrtlDist;
}

void CDlgOptionSurface::UpdateSurfaceOptions()
{
	CSimuManager::m_isoDensityRatio = m_fIsoDensityRatio;
	CSimuManager::m_isoDenRadiusRatio = m_fIsoDenRadiusRatio;
	CSimuManager::m_meshSizeOverPrtlDist = m_fMeshSizeOverPrtlDist;
	if (m_rdoIsoDensityChoice == 0)
		CSimuManager::USE_PRTL_NUM_DENSITY = true;
	else
		CSimuManager::USE_PRTL_NUM_DENSITY = false;
	CSimuManager::CREATE_SURFACE_MESH = m_bCreateSrfcMesh;
	CSimuManager::DRAW_SURFACE_MESH = CSimuManager::CREATE_SURFACE_MESH;
	m_pSimuManager->InitializeOptions();
}

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionSurface message handlers

void CDlgOptionSurface::OnCancel() 
{
	// TODO: Add extra cleanup here

	UpdateData(false);
	
	CDialog::OnCancel();
}

void CDlgOptionSurface::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();
	
	CDialog::OnOK();
}

void CDlgOptionSurface::OnApply() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	UpdateSurfaceOptions();

	m_pSimuManager->RefreshView();
}
