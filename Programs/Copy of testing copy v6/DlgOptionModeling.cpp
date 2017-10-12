// DlgOptionModeling.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgOptionModeling.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionModeling dialog


CDlgOptionModeling::CDlgOptionModeling(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionModeling::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionModeling)
	m_iFluidType = -1;
	m_fCourantFactor = 0.0f;
	m_fPrtlPosJitter = 0.0f;
	m_fSrfcPrtlDenThreshold = 0.0f;
	m_fShearModulus = 0.0f;
	m_iTensorIntegration = -1;
	m_bApplyGravity = FALSE;
	m_fPrtlDistance = 0.0f;
	m_fSmoothLenRatio = 0.0f;
	m_fSoundSpeed = 0.0f;
	m_fVelocityTuner = 0.0f;
	m_iSpecifyMass = -1;
	m_fPrtlDensity = 0.0f;
	m_fPrtlMass = 0.0f;
	m_nPrtlFluidModel = -1;
	m_fCGTolerance = 0.0f;
	m_iFluidCompressibility = -1;
	m_bApplySrfcTension = FALSE;
	m_fSurfaceTension = 0.0f;
	m_fDistDumpRate = 0.0f;
	m_fEnergyDumpRate = 0.0f;
	m_fFrictionRate = 0.0f;
	m_fRelaxationTime = 0.0f;
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;

	InitializeModelingOptions();
}


void CDlgOptionModeling::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionModeling)
	DDX_Radio(pDX, IDC_RDO_NON_NEWTONIAN, m_iFluidType);
	DDX_Text(pDX, IDC_COURANT_FACTOR, m_fCourantFactor);
	DDX_Text(pDX, IDC_PRTL_POS_JITTER, m_fPrtlPosJitter);
	DDX_Text(pDX, IDC_SRFC_PRTL_DEN_THRESHOLD, m_fSrfcPrtlDenThreshold);
	DDX_Text(pDX, IDC_SHEAR_MODULUS, m_fShearModulus);
	DDX_Radio(pDX, IDC_RDO_IMPLICIT, m_iTensorIntegration);
	DDX_Check(pDX, IDC_CHK_GRAVITY, m_bApplyGravity);
	DDX_Text(pDX, IDC_PRTL_DISTANCE, m_fPrtlDistance);
	DDX_Text(pDX, IDC_SMOOTH_LEN_RATIO, m_fSmoothLenRatio);
	DDX_Text(pDX, IDC_SOUND_SPEED, m_fSoundSpeed);
	DDX_Text(pDX, IDC_VELOCITY_TUNER, m_fVelocityTuner);
	DDX_Radio(pDX, IDC_RDO_SPECIFY_MASS, m_iSpecifyMass);
	DDX_Text(pDX, IDC_PRTL_DENSITY, m_fPrtlDensity);
	DDX_Text(pDX, IDC_PRTL_MASS, m_fPrtlMass);
	DDX_Radio(pDX, IDC_RDO_PRTL_FLUID_MODEL, m_nPrtlFluidModel);
	DDX_Text(pDX, IDC_CG_TOLERANCE, m_fCGTolerance);
	DDX_Radio(pDX, IDC_RDO_FLUID_COMPRESSIBILITY, m_iFluidCompressibility);
	DDX_Check(pDX, IDC_CHK_APPLY_SRFC_TENSION, m_bApplySrfcTension);
	DDX_Text(pDX, IDC_SURFACE_TENSION, m_fSurfaceTension);
	DDX_Text(pDX, IDC_DIST_DUMP_RATE, m_fDistDumpRate);
	DDX_Text(pDX, IDC_ENERGY_DUMP_RATE, m_fEnergyDumpRate);
	DDX_Text(pDX, IDC_FRICTION_RATE, m_fFrictionRate);
	DDX_Text(pDX, IDC_RELAXATION_TIME, m_fRelaxationTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionModeling, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionModeling)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgOptionModeling::InitializeModelingOptions()
{
	m_bApplyGravity = CSimuManager::APPLY_GRAVITY;
	m_bApplySrfcTension = CSimuManager::APPLY_SURFACE_TENSION;
	m_fSurfaceTension = CSimuManager::m_surfaceTension;

	switch (CSimuManager::m_eFluidModel)
	{
		case CSimuManager::SPH:
			m_nPrtlFluidModel = 0;
			break;
		case CSimuManager::REMESH:
			m_nPrtlFluidModel = 1;
			break;
		case CSimuManager::RESAMPLE:
			m_nPrtlFluidModel = 2;
			break;
		case CSimuManager::REDISTRIBUTE:
			m_nPrtlFluidModel = 3;
			break;
		case CSimuManager::UPSAMPLE:
			m_nPrtlFluidModel = 4;
			break;
	}
	switch (CSimuManager::m_eCompressibility)
	{
		case CSimuManager::NO_PRESSURE:
			m_iFluidCompressibility = 0;
			break;
		case CSimuManager::POISSON:
			m_iFluidCompressibility = 1;
			break;
		case CSimuManager::GAS_STATE:
			m_iFluidCompressibility = 2;
			break;
	}
	m_iFluidType = CSimuManager::NON_NEWTONIAN_FLUID ? 0 : 1;
	m_iTensorIntegration = CSimuManager::IMPLICIT_TENSOR_INTEGRATION ? 0 : 1;

	m_iSpecifyMass = CSimuManager::UNIFORM_FIXED_PARTICLE_MASS ? 0 : 1;
	m_fPrtlMass = CSimuManager::m_prtlMass;
	m_fPrtlDensity = CSimuManager::m_prtlDensity;
	m_fPrtlDistance = CSimuManager::m_prtlDistance;
	m_fPrtlPosJitter = CSimuManager::m_prtlPosJitter;
	m_fFrictionRate = CSimuManager::m_frictionRate;
	m_fDistDumpRate = CSimuManager::m_distDumpingRate;
	m_fEnergyDumpRate = CSimuManager::m_energyDumpingRate;

	m_fSoundSpeed = CSimuManager::m_soundSpeed;
	m_fVelocityTuner = CSimuManager::m_velocityTuner;
	m_fCourantFactor = CSimuManager::m_courantFactor;
	m_fSmoothLenRatio = CSimuManager::m_smoothLengthRatio;
	m_fSrfcPrtlDenThreshold = CSimuManager::m_srfcDenThreshold;
	m_fCGTolerance = CSimuManager::m_CGSolverTolerance;

	m_fShearModulus = CSimuManager::m_shearModulus;
	m_fRelaxationTime = CSimuManager::m_relaxationTime;
}

void CDlgOptionModeling::UpdateModelingOptions()
{
	CSimuManager::APPLY_GRAVITY = m_bApplyGravity;
	CSimuManager::APPLY_SURFACE_TENSION = m_bApplySrfcTension;
	CSimuManager::m_surfaceTension = m_fSurfaceTension;

	switch (m_nPrtlFluidModel)
	{
		case 0:
			CSimuManager::m_eFluidModel = CSimuManager::SPH;
			break;
		case 1:
			CSimuManager::m_eFluidModel = CSimuManager::REMESH;
			break;
		case 2:
			CSimuManager::m_eFluidModel = CSimuManager::RESAMPLE;
			break;
		case 3:
			CSimuManager::m_eFluidModel = CSimuManager::REDISTRIBUTE;
			break;
		case 4:
			CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE;
			break;
	}
	switch (m_iFluidCompressibility)
	{
		case 0:
			CSimuManager::m_eCompressibility = CSimuManager::NO_PRESSURE;
			break;
		case 1:
			CSimuManager::m_eCompressibility = CSimuManager::POISSON;
			break;
		case 2:
			CSimuManager::m_eCompressibility = CSimuManager::GAS_STATE;
			break;
	}
	CSimuManager::NON_NEWTONIAN_FLUID = m_iFluidType == 0? true : false;
	CSimuManager::IMPLICIT_TENSOR_INTEGRATION = m_iTensorIntegration == 0? true : false;

	CSimuManager::UNIFORM_FIXED_PARTICLE_MASS = m_iSpecifyMass == 0? true : false;
	CSimuManager::m_prtlMass = m_fPrtlMass;
	CSimuManager::m_prtlDensity = m_fPrtlDensity;
	CSimuManager::m_prtlDistance = m_fPrtlDistance;
	CSimuManager::m_prtlPosJitter = m_fPrtlPosJitter;
	CSimuManager::m_frictionRate = m_fFrictionRate;
	CSimuManager::m_distDumpingRate = m_fDistDumpRate;
	CSimuManager::m_energyDumpingRate = m_fEnergyDumpRate;

	CSimuManager::m_soundSpeed = m_fSoundSpeed;
	CSimuManager::m_velocityTuner = m_fVelocityTuner;
	CSimuManager::m_courantFactor = m_fCourantFactor;
	CSimuManager::m_smoothLengthRatio = m_fSmoothLenRatio;
	CSimuManager::m_srfcDenThreshold = m_fSrfcPrtlDenThreshold;
	CSimuManager::m_CGSolverTolerance = m_fCGTolerance;

	CSimuManager::m_shearModulus = m_fShearModulus;
	CSimuManager::m_relaxationTime = m_fRelaxationTime;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionModeling message handlers

void CDlgOptionModeling::OnCancel() 
{
	// TODO: Add extra cleanup here

	UpdateData(false);
	
	CDialog::OnCancel();
}

void CDlgOptionModeling::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();
	
	CDialog::OnOK();
}

void CDlgOptionModeling::OnApply() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	UpdateModelingOptions();

	m_pSimuManager->RefreshView();
}
