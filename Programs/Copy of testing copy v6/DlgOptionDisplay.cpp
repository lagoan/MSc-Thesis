// DlgOptionDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgOptionDisplay.h"
#include "SimuManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionDisplay dialog


CDlgOptionDisplay::CDlgOptionDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionDisplay)
	m_bDrawParticles = FALSE;
	m_bDrawPrtlId = FALSE;
	m_bPrtlDensColor = FALSE;
	m_fDisplayTuner = 0.0f;
	m_bColorSrfcPrtls = FALSE;
	m_bRenderGridPrtls = FALSE;
	m_bPrtlSpeedColor = FALSE;
	m_bPrtlMassColor = FALSE;
	m_bDrawHighlitOnly = FALSE;
	m_bDrawBoundary = FALSE;
	m_fParticleIdSize = 0.0f;
	m_bDrawCurvNorm = FALSE;
	m_bDrawSrfcMesh = FALSE;
	m_bDrawSrfcPrtlNorm = FALSE;
	m_bDrawTetrahedra = FALSE;
	m_bDrawTriNorm = FALSE;
	m_bDrawSrfcPrtl = FALSE;
	m_bDrawConvexHull = FALSE;
	m_bDrawDelaunay = FALSE;
	m_bUpdateEntireDisplay = FALSE;
	m_bDrawEdgeOnly = FALSE;
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;

	InitializeDisplayOptions();
}


void CDlgOptionDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionDisplay)
	DDX_Check(pDX, IDC_CHK_DRAW_PARTICLES, m_bDrawParticles);
	DDX_Check(pDX, IDC_CHK_DRAW_PRTL_ID, m_bDrawPrtlId);
	DDX_Check(pDX, IDC_CHK_PRTL_DENS_COLOR, m_bPrtlDensColor);
	DDX_Text(pDX, IDC_DISPLAY_TUNER, m_fDisplayTuner);
	DDX_Check(pDX, IDC_CHK_COLOR_SRFC_PRTLS, m_bColorSrfcPrtls);
	DDX_Check(pDX, IDC_CHK_RENDER_GRID_PRTLS, m_bRenderGridPrtls);
	DDX_Check(pDX, IDC_CHK_PRTL_SPEED_COLOR, m_bPrtlSpeedColor);
	DDX_Check(pDX, IDC_CHK_PRTL_MASS_COLOR, m_bPrtlMassColor);
	DDX_Check(pDX, IDC_CHK_DRAW_HIGHLIT_ONLY, m_bDrawHighlitOnly);
	DDX_Check(pDX, IDC_CHK_DRAW_BOUNDARY, m_bDrawBoundary);
	DDX_Text(pDX, IDC_PARTICLE_ID_SIZE, m_fParticleIdSize);
	DDX_Check(pDX, IDC_CHK_DRAW_CURV_NORM, m_bDrawCurvNorm);
	DDX_Check(pDX, IDC_CHK_DRAW_SRFC_MESH, m_bDrawSrfcMesh);
	DDX_Check(pDX, IDC_CHK_DRAW_SRFC_PRTL_NORM, m_bDrawSrfcPrtlNorm);
	DDX_Check(pDX, IDC_CHK_DRAW_TETRAHEDRA, m_bDrawTetrahedra);
	DDX_Check(pDX, IDC_CHK_DRAW_TRI_NORM, m_bDrawTriNorm);
	DDX_Check(pDX, IDC_CHK_DRAW_SRFC_PRTL, m_bDrawSrfcPrtl);
	DDX_Check(pDX, IDC_CHK_DRAW_CONVEX_HULL, m_bDrawConvexHull);
	DDX_Check(pDX, IDC_CHK_DRAW_DELAUNAY, m_bDrawDelaunay);
	DDX_Check(pDX, IDC_CHK_UPDATE_DISPLAY, m_bUpdateEntireDisplay);
	DDX_Check(pDX, IDC_CHK_DRAW_EDGE_ONLY, m_bDrawEdgeOnly);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionDisplay, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionDisplay)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgOptionDisplay::InitializeDisplayOptions()
{
	m_bUpdateEntireDisplay = CSimuManager::UPDATE_ENTIRE_DISPLAY;
	m_bDrawParticles = CSimuManager::DRAW_PARTICLES;
	m_bDrawPrtlId = CSimuManager::DRAW_PARTICLE_ID;
	m_bPrtlSpeedColor = CSimuManager::DRAW_PARTICLE_SPEED_BY_COLOR;
	m_bPrtlMassColor = CSimuManager::DRAW_PARTICLE_MASS_BY_COLOR;
	m_bPrtlDensColor = CSimuManager::DRAW_PARTICLE_DENSITIES_BY_COLOR;
	m_bColorSrfcPrtls = CSimuManager::COLOR_SURFACE_PARTICLES;
	m_bRenderGridPrtls = CSimuManager::DRAW_GRID_PARTICLES;

	m_bDrawSrfcMesh = CSimuManager::DRAW_SURFACE_MESH;
	m_bDrawSrfcPrtl = CSimuManager::DRAW_SRFC_PARTICLES;
	m_bDrawSrfcPrtlNorm = CSimuManager::DRAW_SRFC_PRTL_NORMALS;
	m_bDrawCurvNorm = CSimuManager::DRAW_CURVATURE_NORMALS;
	m_bDrawTriNorm = CSimuManager::DRAW_TRIANGLE_NORMALS;
	m_bDrawTetrahedra = CSimuManager::DRAW_TETRAHEDRA;
	m_bDrawEdgeOnly = CSimuManager::DRAW_EDGE_ONLY;
	m_bDrawHighlitOnly = CSimuManager::DRAW_HIGHLIT_ONLY;
	m_bDrawBoundary = CSimuManager::DRAW_BOUNDARY;
	m_bDrawDelaunay = CSimuManager::DRAW_DELAUNAY;
	m_bDrawConvexHull = CSimuManager::DRAW_CONVEX_HULL;

	m_fDisplayTuner = CSimuManager::m_displayTuner;
	m_fParticleIdSize = CSimuManager::m_particleIdSize;
}

void CDlgOptionDisplay::UpdateDisplayOptions()
{
	CSimuManager::UPDATE_ENTIRE_DISPLAY = m_bUpdateEntireDisplay;
	CSimuManager::DRAW_PARTICLES = m_bDrawParticles;
	CSimuManager::DRAW_PARTICLE_ID = m_bDrawPrtlId;
	CSimuManager::DRAW_PARTICLE_SPEED_BY_COLOR = m_bPrtlSpeedColor;
	CSimuManager::DRAW_PARTICLE_MASS_BY_COLOR = m_bPrtlMassColor;
	CSimuManager::DRAW_PARTICLE_DENSITIES_BY_COLOR = m_bPrtlDensColor;
	CSimuManager::COLOR_SURFACE_PARTICLES = m_bColorSrfcPrtls;
	CSimuManager::DRAW_GRID_PARTICLES = m_bRenderGridPrtls;

	CSimuManager::DRAW_SURFACE_MESH = m_bDrawSrfcMesh;
	CSimuManager::DRAW_SRFC_PARTICLES = m_bDrawSrfcPrtl;
	CSimuManager::DRAW_SRFC_PRTL_NORMALS = m_bDrawSrfcPrtlNorm;
	CSimuManager::DRAW_CURVATURE_NORMALS = m_bDrawCurvNorm;
	CSimuManager::DRAW_TRIANGLE_NORMALS = m_bDrawTriNorm;
	CSimuManager::DRAW_TETRAHEDRA = m_bDrawTetrahedra;
	CSimuManager::DRAW_EDGE_ONLY = m_bDrawEdgeOnly;
	CSimuManager::DRAW_HIGHLIT_ONLY = m_bDrawHighlitOnly;
	CSimuManager::DRAW_BOUNDARY = m_bDrawBoundary;
	CSimuManager::DRAW_DELAUNAY = m_bDrawDelaunay;
	CSimuManager::DRAW_CONVEX_HULL = m_bDrawConvexHull;

	CSimuManager::m_displayTuner = m_fDisplayTuner;
	CSimuManager::m_particleIdSize = m_fParticleIdSize;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionDisplay message handlers

void CDlgOptionDisplay::OnApply() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	UpdateDisplayOptions();

	m_pSimuManager->RefreshView();
}

void CDlgOptionDisplay::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();

	CDialog::OnOK();
}

void CDlgOptionDisplay::OnCancel() 
{
	// TODO: Add extra cleanup here

	InitializeDisplayOptions();
	UpdateData(false);

	CDialog::OnCancel();
}
