// DlgOptionViewing.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgOptionViewing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionViewing dialog


CDlgOptionViewing::CDlgOptionViewing(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionViewing::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionViewing)
	m_fBdryBottom = 0.0f;
	m_fBdryFar = 0.0f;
	m_fBdryLeft = 0.0f;
	m_fBdryNear = 0.0f;
	m_fBdryRight = 0.0f;
	m_fBdryTop = 0.0f;
	m_bDrawLight0 = FALSE;
	m_bDrawLight1 = FALSE;
	m_bDrawXZCoordinates = FALSE;
	m_bEnableLight0 = FALSE;
	m_bEnableLight1 = FALSE;
	m_iDrawPrtlAs = -1;
	m_fSphereRadius = 0.0f;
	m_iSphereSlices = 0;
	m_iSphereStacks = 0;
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;

	InitializeViewingOptions();
}


void CDlgOptionViewing::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionViewing)
	DDX_Text(pDX, IDC_BOUNDARY_BOTTOM, m_fBdryBottom);
	DDX_Text(pDX, IDC_BOUNDARY_FAR, m_fBdryFar);
	DDX_Text(pDX, IDC_BOUNDARY_LEFT, m_fBdryLeft);
	DDX_Text(pDX, IDC_BOUNDARY_NEAR, m_fBdryNear);
	DDX_Text(pDX, IDC_BOUNDARY_RIGHT, m_fBdryRight);
	DDX_Text(pDX, IDC_BOUNDARY_TOP, m_fBdryTop);
	DDX_Check(pDX, IDC_CHK_DRAW_LIGHT0, m_bDrawLight0);
	DDX_Check(pDX, IDC_CHK_DRAW_LIGHT1, m_bDrawLight1);
	DDX_Check(pDX, IDC_CHK_DRAW_XZ_COORDINATES, m_bDrawXZCoordinates);
	DDX_Check(pDX, IDC_CHK_ENABLE_LIGHT0, m_bEnableLight0);
	DDX_Check(pDX, IDC_CHK_ENABLE_LIGHT1, m_bEnableLight1);
	DDX_Radio(pDX, IDC_RADIO_SPHERE, m_iDrawPrtlAs);
	DDX_Text(pDX, IDC_SPHERE_RADIUS, m_fSphereRadius);
	DDX_Text(pDX, IDC_SPHERE_SLICES, m_iSphereSlices);
	DDX_Text(pDX, IDC_SPHERE_STACKS, m_iSphereStacks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionViewing, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionViewing)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgOptionViewing::InitializeViewingOptions()
{
	if (CSimuManager::DRAW_PARTICLES_BY_SPHERE)
		m_iDrawPrtlAs = 0;
	else
		m_iDrawPrtlAs = 1;

	m_fSphereRadius = CSimuManager::m_sphereRadius;
	m_iSphereSlices = CSimuManager::m_sphereSlices;
	m_iSphereStacks = CSimuManager::m_sphereStacks;

	m_bDrawXZCoordinates = CSimuManager::DRAW_X_Z_COORDINATES;
	m_bEnableLight0 = CSimuManager::ENABLE_LIGHT0;
	m_bEnableLight1 = CSimuManager::ENABLE_LIGHT1;
	m_bDrawLight0 = CSimuManager::DRAW_LIGHT0;
	m_bDrawLight1 = CSimuManager::DRAW_LIGHT1;

	m_fBdryLeft		= CSimuManager::m_viewBoundary->v[BOUNDARY_LEFT];
	m_fBdryRight	= CSimuManager::m_viewBoundary->v[BOUNDARY_RIGHT];
	m_fBdryBottom	= CSimuManager::m_viewBoundary->v[BOUNDARY_BOTTOM];
	m_fBdryTop		= CSimuManager::m_viewBoundary->v[BOUNDARY_TOP];
	m_fBdryNear		= CSimuManager::m_viewBoundary->v[BOUNDARY_NEAR];
	m_fBdryFar		= CSimuManager::m_viewBoundary->v[BOUNDARY_FAR];
}

void CDlgOptionViewing::UpdateViewingOptions()
{
	switch (m_iDrawPrtlAs)
	{
		case 0:
			CSimuManager::DRAW_PARTICLES_BY_SPHERE = true;
			break;
		case 1:
			CSimuManager::DRAW_PARTICLES_BY_SPHERE = false;
			break;
	};
	CSimuManager::m_sphereRadius = m_fSphereRadius;
	CSimuManager::m_sphereSlices = m_iSphereSlices;
	CSimuManager::m_sphereStacks = m_iSphereStacks;

	CSimuManager::DRAW_X_Z_COORDINATES = m_bDrawXZCoordinates;
	CSimuManager::ENABLE_LIGHT0 = m_bEnableLight0;
	CSimuManager::ENABLE_LIGHT1 = m_bEnableLight1;
	CSimuManager::DRAW_LIGHT0 = m_bDrawLight0;
	CSimuManager::DRAW_LIGHT1 = m_bDrawLight1;

	CSimuManager::m_viewBoundary->v[BOUNDARY_LEFT]		= m_fBdryLeft;
	CSimuManager::m_viewBoundary->v[BOUNDARY_RIGHT]		= m_fBdryRight;
	CSimuManager::m_viewBoundary->v[BOUNDARY_BOTTOM]	= m_fBdryBottom;
	CSimuManager::m_viewBoundary->v[BOUNDARY_TOP]		= m_fBdryTop;
	CSimuManager::m_viewBoundary->v[BOUNDARY_NEAR]		= m_fBdryNear;
	CSimuManager::m_viewBoundary->v[BOUNDARY_FAR]		= m_fBdryFar;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionViewing message handlers

void CDlgOptionViewing::OnCancel() 
{
	// TODO: Add extra cleanup here

	UpdateData(false);
	
	CDialog::OnCancel();
}

void CDlgOptionViewing::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();
	
	CDialog::OnOK();
}

void CDlgOptionViewing::OnApply() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	UpdateViewingOptions();

	m_pSimuManager->RefreshView();
}
