// SimuManager.cpp : implementation of the CSimuManager class
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// current fluid modeling classes
CSimuManager::EFluidClass CSimuManager::m_eFluidClass = PrtlFluid;
bool CSimuManager::m_bSpikyGradient = false;

// modeling options
int CSimuManager::m_numFramesPerSecond		= 32;
int CSimuManager::m_numStepDivPerFrame		= 2;
int CSimuManager::m_maxPoissonPauseSteps	= 1;
int CSimuManager::m_numTensorSubIntegraions = 1;
int CSimuManager::m_maxSrfcDetectPauseSteps	= 1;
int CSimuManager::m_maxDownsamplePauseSteps	= 1;
int CSimuManager::m_maxUpsamplePauseSteps	= 1;
SimuValue CSimuManager::m_framesPerResample	= 1;
SimuValue CSimuManager::m_onlyGravityTime	= 0;
SimuValue CSimuManager::m_onlyBdryMovingTime= 0;
bool CSimuManager::APPLY_GRAVITY			= true;
bool CSimuManager::APPLY_SURFACE_TENSION	= false;
SimuValue CSimuManager::m_surfaceTension	= 100000;
bool CSimuManager::ADAPTIVE_TIME_STEP		= false;
bool CSimuManager::ADJUST_VEL_WITH_NGBRS	= true;
SimuValue CSimuManager::m_velocityTuner		= 0.5;
SimuValue CSimuManager::m_interFluidVelTuner= 0.5;
bool CSimuManager::ADJUST_TENSOR_WITH_NGBRS	= true;
SimuValue CSimuManager::m_stressTensorTuner	= 0.5;
bool CSimuManager::DUMP_STRESS_TENSOR		= true;
bool CSimuManager::IMPLICIT_STRESS_INTEGRATION = false;
bool CSimuManager::IMPLICIT_TENSOR_INTEGRATION = false;
CSimuManager::EFluidModel CSimuManager::m_eFluidModel = UPSAMPLE;
CSimuManager::ECompressibility CSimuManager::m_eCompressibility = POISSON;
SimuValue CSimuManager::m_airPressure		= -100;
bool CSimuManager::m_bConvexHull4SrfcDetect	= false;
bool CSimuManager::m_bLocalFluidOrgDensity	= true;
CSimuManager::ELaplacian CSimuManager::m_eLaplacian	= DENSITY_VARIATION; //VELOCITY_DIVERGENCE; //
bool CSimuManager::NON_NEWTONIAN_FLUID		= true;
CSimuManager::EStressModel CSimuManager::m_eStressModel = VISCOELASTIC; // PLASTIC; // 
ERotation CSimuManager::m_eRotationDerivative = COROTATIONAL; // NO_ROTATION; // 
SimuValue CSimuManager::m_rotationFactor	= 10;
bool CSimuManager::VISCOSITY				= false;
SimuValue CSimuManager::m_viscosity			= 1;
//SimuValue CSimuManager::m_viscosity		= m_shearModulus*m_relaxationTime;

CSimuManager::EInterFluidModel CSimuManager::m_eInterFluidModel = DETECT_BY_CONVEX_HULL;
bool CSimuManager::m_bRepulsiveResponseOnly	= false;
bool CSimuManager::m_bStickyInterFluids		= false;
SimuValue CSimuManager::m_interFluidStickyness = 100;
SimuValue CSimuManager::m_interFluidDamping	= 0; // range 0 to 1, and 0 means complete damping
SimuValue CSimuManager::m_interFluidTension	= 1000;

bool CSimuManager::UNIFORM_FIXED_PARTICLE_MASS = false;
SimuValue CSimuManager::m_prtlMass			= 100;
SimuValue CSimuManager::m_prtlDensity		= 0.1;
SimuValue CSimuManager::m_prtlDistance		= 10;
SimuValue CSimuManager::m_minPrtlDistance	= SIMU_PARA_MIN_PRTL_DISTANCE_RATIO*CSimuManager::m_prtlDistance;
SimuValue CSimuManager::m_prtlPosJitter		= 0.0;	// ratio of original particle distance
SimuValue CSimuManager::m_interFluidDumping	= 0;	// 0 means complete dumping
SimuValue CSimuManager::m_interFluidFriction= 0;	// 0 means full-stop friction
SimuValue CSimuManager::m_distDumpingRate	= 0;	// 0 means complete dumping
SimuValue CSimuManager::m_energyDumpingRate	= 0;	// 0 means complete dumping
SimuValue CSimuManager::m_frictionRate		= 0;	// 0 means full-stop friction
SimuValue CSimuManager::m_stressDumpingRate	= 0;	// 0 means complete dumping
SimuValue CSimuManager::m_ballDumpingRate	= 0.5;	// 0 means complete dumping
SimuValue CSimuManager::m_soundSpeed		= 1000;
SimuValue CSimuManager::m_courantFactor		= 0.1;	// ratio of moving distance over smoothing length
SimuValue CSimuManager::m_smoothLengthRatio	= 2.0;	// ratio of smooth length over particle distance
SimuValue CSimuManager::m_srfcDenThreshold	= 0.9;
SimuValue CSimuManager::m_srfcWSThreshold	= 0.9;
int CSimuManager::m_srfcNgbrNumThreshold	= 5;
int CSimuManager::m_interSrfcNgbrNumThreshold= 10;
int CSimuManager::m_minNumNgbrForUpsample	= 22;
SimuValue CSimuManager::m_springConstant	= pow(10.0f, 6);
SimuValue CSimuManager::m_CGSolverTolerance		= 0.001*m_prtlDistance;
SimuValue CSimuManager::m_lowerPrtlMassLimitRatio = 0.1;
SimuValue CSimuManager::m_upperPrtlMassLimitRatio = 4;
SimuValue CSimuManager::m_downSampleDistRatio	= 0.6;
SimuValue CSimuManager::m_intolerablePrtlDist	= CSimuManager::m_prtlDistance*SIMU_PARA_MIN_PRTL_DISTANCE_RATIO;
SimuValue CSimuManager::m_srfcThicknessRatio	= 0.1;
SimuValue CSimuManager::m_upsampleMinRadiusRatio= 0.95;
SimuValue CSimuManager::m_upsampleMaxRadiusRatio= 1.5;
bool CSimuManager::SAME_DOWN_UP_SAMPLE_RATE	= true;
bool CSimuManager::INTERLEAVE_DOWN_UP_SAMPLE= false;
bool CSimuManager::NOT_MERGE_COLLIDING_PRTLS= true;
bool CSimuManager::CONSERVE_KINETIC_ENERGY	= true;
// non-newtonian fluid options
SimuValue CSimuManager::m_shearModulus		= pow(10.0f, 4);
SimuValue CSimuManager::m_relaxationTime	= pow(10.0f, -1); // = m_viscosity/m_shearModulus
SimuValue CSimuManager::m_plasticYieldValue	= 0;
// thermal model parameters
bool CSimuManager::HEAT_TRANSFER			= false;
bool CSimuManager::NOT_CONSERVE_HEAT		= false;
bool CSimuManager::OVERWRITE_TEXTURE		= false;
int CSimuManager::m_subThermalSteps			= 1;
SimuValue CSimuManager::m_heatConductivity	= 1000.0;
SimuValue CSimuManager::m_minTemperature	= 100.0;
SimuValue CSimuManager::m_maxTemperature	= 500.0;
SimuValue CSimuManager::m_prtlIniTemperature= CSimuManager::m_minTemperature;
SimuValue CSimuManager::m_bdryMaxHeatingDist= CSimuManager::m_prtlDistance;
SimuValue CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;
SimuValue CSimuManager::m_minShearModulus	= pow(10.0f, 3);
SimuValue CSimuManager::m_maxShearModulus	= pow(10.0f, 5);
CSimuManager::ETptrCtrl CSimuManager::m_eETptrCtrl = LINEAR_MELTING_0;
// edge length threshold ratio
SimuValue CSimuManager::m_minEdgeLenRatio	= 0.5;
SimuValue CSimuManager::m_maxEdgeLenRatio	= 2;

// mesher options
CSimuManager::ERemeshing CSimuManager::m_remeshingMode = MARCHING_TETRAHEDRA;
bool CSimuManager::DEBUG_MESHER				= false;
bool CSimuManager::DRAW_MESHER_GRID			= false;
bool CSimuManager::DRAW_MESHER_PRTL			= false;
bool CSimuManager::DRAW_MESHER_TETRA		= false;
bool CSimuManager::DRAW_MESHER_SRFC_TETRA	= false;
bool CSimuManager::DRAW_INTR_SRFC_TRIANGLES	= false;
bool CSimuManager::DRAW_INTR_SRFC_PARTICLES	= false;
bool CSimuManager::DRAW_EXTR_SRFC_PARTICLES	= false;
bool CSimuManager::DRAW_EXTR_SRFC_TRIANGLES	= false;
bool CSimuManager::DRAW_INTR_SRFC_NORMALS	= false;
bool CSimuManager::DRAW_ISOMORPHIC_LINES	= false;

// display options
bool CSimuManager::UPDATE_ENTIRE_DISPLAY	= true;
bool CSimuManager::DRAW_HIGHLIT_ONLY		= false;
bool CSimuManager::DRAW_PARTICLES			= true;
bool CSimuManager::DRAW_PARTICLE_ID			= false;
bool CSimuManager::COLOR_SURFACE_PARTICLES	= false;
bool CSimuManager::DRAW_PARTICLE_DENSITIES_BY_COLOR	= false;
bool CSimuManager::DRAW_PARTICLE_MASS_BY_COLOR = false;
bool CSimuManager::DRAW_PARTICLE_SPEED_BY_COLOR = false;
bool CSimuManager::DRAW_GRID_PARTICLES		= false;
bool CSimuManager::DRAW_SURFACE_MESH		= false;
bool CSimuManager::DRAW_EDGE_ONLY			= false;
bool CSimuManager::DRAW_SRFC_PARTICLES		= false;
bool CSimuManager::DRAW_SRFC_PRTL_NORMALS	= false;
bool CSimuManager::DRAW_CURVATURE_NORMALS	= false;
bool CSimuManager::DRAW_TRIANGLE_NORMALS	= false;
bool CSimuManager::DRAW_TETRAHEDRA			= false;
bool CSimuManager::DRAW_BOUNDARY			= true;
bool CSimuManager::DRAW_DELAUNAY			= false;
bool CSimuManager::DRAW_CONVEX_HULL			= false;
SimuValue CSimuManager::m_displayTuner		= 10;
SimuValue CSimuManager::m_particleIdSize	= m_prtlDistance/2;
// display variables
bool CSimuManager::DIFFER_SRFC_COLOR		= true;
bool CSimuManager::DRAW_MC_BOUNDING_BOX		= false;

// viewing options
bool CSimuManager::DRAW_PARTICLES_BY_SPHERE = true;
bool CSimuManager::DRAW_X_Z_COORDINATES		= true;
bool CSimuManager::ENABLE_LIGHT0			= true;
bool CSimuManager::ENABLE_LIGHT1			= true;
bool CSimuManager::DRAW_LIGHT0				= true;
bool CSimuManager::DRAW_LIGHT1				= true;
SimuValue CSimuManager::m_sphereRadius		= 0.2*CSimuManager::m_prtlDistance;
int CSimuManager::m_sphereSlices			= 5;
int CSimuManager::m_sphereStacks			= 5;
CVector* CSimuManager::m_viewBoundary		= NULL;

// particle selecting options
CSimuEnclosingBall* CSimuManager::m_enclosingBall	= NULL;
bool CSimuManager::SHOW_SELECTED_PRTL_ONLY	= false;
CVector3D* CSimuManager::m_hilightColor	= NULL;
// element selecting options
CSimuManager::EElementType CSimuManager::m_eSelectedElmntType = SELECT_TRIANGLE;
bool CSimuManager::SHOW_SELECTED_ELMNT_ONLY	= false;

// surface options
bool CSimuManager::CREATE_SURFACE_MESH		= false;
bool CSimuManager::USE_PRTL_NUM_DENSITY		= false;
bool CSimuManager::NOT_COUNT_FIXED_PRTLS	= true;
SimuValue CSimuManager::m_isoDensityRatio	= 0.3; // ratio of iso-density over fluid density
SimuValue CSimuManager::m_isoDenRadiusRatio = 5;
SimuValue CSimuManager::m_meshSizeOverPrtlDist	= 0.7;

// mesh texture options
SimuValue CSimuManager::m_ratioColorR		= 0.8;
SimuValue CSimuManager::m_ratioColorG		= -0.8;
SimuValue CSimuManager::m_ratioColorB		= 0;
SimuValue CSimuManager::m_constColorR		= 0.05;
SimuValue CSimuManager::m_constColorG		= 0.85;
SimuValue CSimuManager::m_constColorB		= 0.05;
SimuValue CSimuManager::m_ratioFilter		= 1;
SimuValue CSimuManager::m_constFilter		= 0;

// import/export options
CString	CSimuManager::m_strAppDataDir		= "";
CString CSimuManager::m_strImportDir		= "";
CString CSimuManager::m_strImportParaFile	= "";
UINT CSimuManager::m_nImportFrameInterval	= CSimuManager::m_numFramesPerSecond/32;
bool CSimuManager::IMPORT_FLUID_DATA		= false;
bool CSimuManager::EXPORT_FLUID_DATA		= false;
bool CSimuManager::OVERWRITE_EXISTING_DATA	= false;
bool CSimuManager::IM_EXPORT_FLUID_PARTICLES= true;
bool CSimuManager::IM_EXPORT_FLUID_SURFACE	= false;

// import/export variables
time_t CSimuManager::m_t2005_JUNE_29_5PM	= GetSpecifiedTime(2005, 5, 29, 17);
enum CSimuManager::IMEXPORT_FORMAT CSimuManager::m_enumImExportFormat = AFTER_2005_JUNE_29_5PM;
CString CSimuManager::m_strExportDir		= "";
CString CSimuManager::m_strSimuInfoFile		= "";
CString CSimuManager::m_strSimuEnvPOVFile	= "SimuEnv.pov";
bool CSimuManager::m_bNeedExportDirectory	= true;
UINT CSimuManager::m_nCurFrame				= 1;
SimuValue CSimuManager::m_fPreExportTime	= SIMU_NEGATIVE_NUMBER; // so that the first frame will be exported
CString CSimuManager::m_strDataDebugFile	= "dataDebugFile.txt";
FILE* CSimuManager::m_dataDebugFile			= NULL;
bool CSimuManager::m_bWriteToDataDebugFile	= false;

// animation monitor variables
UINT CSimuManager::m_nCurStep				= 0;
SimuValue CSimuManager::m_dbTotalRunningTime= 0;
SimuValue CSimuManager::m_dbTotalMotionTime	= 0;
SimuValue CSimuManager::m_dbTotalInteractTime = 0;
long CSimuManager::m_dbTotalDownsampleClock	= 0;
long CSimuManager::m_dbTotalUpsampleClock	= 0;

// debug control variable
bool CSimuManager::m_bShowMsg				= false;
bool CSimuManager::m_bSkipWarning			= false;
bool CSimuManager::m_bInvalidValue			= false;
SimuValue CSimuManager::m_maxAngVelTenElmnt	= 0;

// Will be initialized in SimuFlexApp::InitInstance()
CVector3D* CSimuManager::GRAVITY		= NULL;
CVector3D* CSimuManager::COLOR_WHITE	= NULL;
CVector3D* CSimuManager::COLOR_RED		= NULL;
CVector3D* CSimuManager::COLOR_GREEN	= NULL;
CVector3D* CSimuManager::COLOR_BLUE		= NULL;
CVector3D* CSimuManager::COLOR_YELLOW	= NULL;
CVector3D* CSimuManager::COLOR_MAGENTA	= NULL;
CVector3D* CSimuManager::COLOR_CYAN		= NULL;
CVector3D* CSimuManager::COLOR_ORANGE	= NULL;
CVector3D* CSimuManager::COLOR_SRFC		= NULL;
CVector3D* CSimuManager::COLOR_INSIDE	= NULL;
CVector3D* CSimuManager::COLOR_FIXED	= NULL;

/////////////////////////////////////////////////////////////////////////////
// CSimuManager

IMPLEMENT_DYNAMIC(CSimuManager, CFrameWnd)

BEGIN_MESSAGE_MAP(CSimuManager, CFrameWnd)
	//{{AFX_MSG_MAP(CSimuManager)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_OUTPUTBODYDATATO, OnFileOutputbodydatato)
	ON_COMMAND(ID_VIEW_MONITOR, OnViewMonitor)
	ON_COMMAND(ID_OPTIONS_DISPLAY, OnOptionsDisplay)
	ON_COMMAND(ID_OPTIONS_SURFACE, OnOptionsSurface)
	ON_COMMAND(ID_OPTIONS_MODELING, OnOptionsModeling)
	ON_COMMAND(ID_OPTIONS_IMPORTEXPORT, OnOptionsImportexport)
	ON_COMMAND(ID_OPTIONS_PARTICLESELECT, OnOptionsParticleselect)
	ON_COMMAND(ID_OPTIONS_MESHER, OnOptionsMesher)
	ON_COMMAND(ID_OPTIONS_VIEWING, OnOptionsViewing)
	ON_COMMAND(ID_OPTIONS_ELEMENTSELECT, OnOptionsElementselect)
	ON_COMMAND(ID_FILE_BATCHMOVECOPYFILES, OnFileBatchmovecopyfiles)
	ON_COMMAND(ID_OPTIONS_MESHTEXTURE, OnOptionsMeshtexture)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CSimuManager construction/destruction

CSimuManager::CSimuManager()
{
	// TODO: add member initialization code here

	m_bRunningTimerEvent= false;
	m_pSimuView			= new CSimuView(this);
	m_pSimuModeler		= new CSimuModeler(this);
	m_pSimuMonitor		= new CSimuMonitor(this);
	m_pSimuDriver		= new CSimuDriver(this);

	m_dlgOptionDisplay	= new CDlgOptionDisplay(this);
	m_dlgOptionSurface	= new CDlgOptionSurface(this);
	m_dlgOptionModeling	= new CDlgOptionModeling(this);
	m_dlgOptionImExport	= new CDlgOptionImExport(this);
	m_dlgOptionParticleSelect = new CDlgOptionParticleSelect(this);
	m_dlgOptionMeshTexture = new CDlgOptionMeshTexture(this);
	m_dlgOptionMesher	= new CDlgOptionMesher(this);
	m_dlgOptionViewing	= new CDlgOptionViewing(this);
	m_dlgOptionElementSelect = new CDlgOptionElementSelect(this);
	m_dlgFileMoveCopy	= new CDlgFileMoveCopy(this);

	m_pMainWinRect		= new CVector(4, "main window rect");
	m_tNewImExportFormatTime = GetSpecifiedTime(2005, 3, 11, 18); // 6pm Apr. 11 2005

	m_curPrtlFluid		= NULL;

	m_aryPrtlFluid.InitializeClassArray(true, SIMU_PARA_DEFAULT_ARRAY_ALLOC_SIZE);
	m_aryBoundaries.InitializeClassArray(true, SIMU_PARA_DEFAULT_ARRAY_ALLOC_SIZE);
}

CSimuManager::~CSimuManager()
{
	delete m_pSimuView;
	delete m_pSimuModeler;
	delete m_pSimuMonitor;
	delete m_pSimuDriver;

	delete m_dlgOptionDisplay;
	delete m_dlgOptionSurface;
	delete m_dlgOptionModeling;
	delete m_dlgOptionImExport;
	delete m_dlgOptionParticleSelect;
	delete m_dlgOptionMesher;
	delete m_dlgOptionMeshTexture;
	delete m_dlgOptionViewing;
	delete m_dlgOptionElementSelect;
	delete m_dlgFileMoveCopy;

	delete m_pMainWinRect;

	if (m_curPrtlFluid != NULL)
		delete m_curPrtlFluid;
}

/////////////////////////////////////////////////////////////////////////////
// CSimuManager public methods

time_t CSimuManager::GetDirectoryCreationTime(CString dir)
{
	struct _stat buf;
	int result = _stat(dir, &buf);
	if (result != 0)
	{
		CSimuMsg::ExitWithMessage("CSimuManager::GetDirectoryCreationTime(dir)", "Bad directory.");
		return -1;
	}
	return buf.st_ctime;
}

time_t CSimuManager::GetFileModificationTime(FILE* fp)
{
	struct _stat buf;
	int fileHandle = _fileno(fp);
	int result = _fstat(fileHandle, &buf);
	if (result != 0)
	{
		CSimuMsg::ExitWithMessage("CSimuManager::GetFileTime(fp)", "Bad file handle.");
		return -1;
	}
	return buf.st_mtime;
}

// month: 0-11; day: 1-31; hour: 0-23
time_t CSimuManager::GetSpecifiedTime(int year, int month, int day, int hour)
{
	time_t tmpTime;
	time(&tmpTime);
	struct tm* specifiedTime;
	specifiedTime = localtime(&tmpTime);

	specifiedTime->tm_year = year - 1900;
	specifiedTime->tm_mon = month;
	specifiedTime->tm_mday = day;
	specifiedTime->tm_hour = hour;
	return mktime(specifiedTime);
}

bool CSimuManager::FirstTimeIsLaterThanSecondTime(time_t firstTime, time_t secondTime)
{
	if (difftime(firstTime, secondTime) > 0)
		return true;
	return false;
}

void CSimuManager::OpenDataDebugFile()
{
	// open data debug file
	CString dataDebugFile = CSimuManager::m_strAppDataDir;
	dataDebugFile += "\\";
	dataDebugFile += CSimuManager::m_strDataDebugFile;
	if ((CSimuManager::m_dataDebugFile = fopen(dataDebugFile, "a")) == NULL)
	{
		sprintf(CSimuMsg::GetEmptyBuf(), "Can't open file %s to write.", dataDebugFile);
		CSimuMsg::ExitWithMessage("CSimuManager::OpenDataDebugFile()",
									CSimuMsg::GetCheckedBuf());
	}
}

void CSimuManager::CloseDataDebugFile()
{
	fclose(CSimuManager::m_dataDebugFile);
}

void CSimuManager::ExportDebugData(char* location)
{
	CSimuManager::OpenDataDebugFile();
	fprintf(CSimuManager::m_dataDebugFile,
//			"n=%d x=%d vp=%d tt=%d tot=%d t3d=%d p3d=%d e3d=%d at %s\n",
			"n=%d x=%d vp=%d tt=%d tot=%d at %s\n",
			CSimuManager::m_nCurStep, CGeometryElement::m_geMaxId, CVirtualPrtl::m_vpMaxId, 
			CTetrahedron::m_ttrMaxId, CTriangleOnTetra::m_totMaxId,
//			CTriangle3D::m_t3dMaxId, CPoint3D::m_p3dMaxId, CEdge3D::m_e3dMaxId,
			location);
	CSimuManager::CloseDataDebugFile();
}

void CSimuManager::SetTuner(bool bEnabled)
{
	SetTuner(bEnabled, bEnabled);
}

void CSimuManager::SetTuner(bool bEnableVel, bool bEnableStress)
{
	CSimuManager::ADJUST_VEL_WITH_NGBRS	= bEnableVel;
	CSimuManager::ADJUST_TENSOR_WITH_NGBRS	= bEnableStress;
}

void CSimuManager::SetTunerValue(SimuValue ratio) // implicitly both enabled
{
	SetTunerValue(ratio, ratio);
}

void CSimuManager::SetTunerValue(SimuValue ratioVel, SimuValue ratioStress)
{
	SetTuner(true);
	CSimuManager::m_velocityTuner		= ratioVel;
	CSimuManager::m_stressTensorTuner	= ratioStress;
}

void CSimuManager::SetPrtlDistance(SimuValue prtlDistance)
{
	CSimuManager::m_prtlDistance = prtlDistance;
	CSimuManager::m_minPrtlDistance	= SIMU_PARA_MIN_PRTL_DISTANCE_RATIO*CSimuManager::m_prtlDistance;
	CSimuManager::m_intolerablePrtlDist	= CSimuManager::m_prtlDistance*SIMU_PARA_MIN_PRTL_DISTANCE_RATIO;
	CSimuManager::m_sphereRadius		= 0.5*CSimuManager::m_prtlDistance;
}

void CSimuManager::TurnOnMemoryDebugFlags()
{
	// turn on all memory checks even it will slow down the execution
	// Get current flag
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	// Turn on leak-checking bit
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	// Turn on memory check for entire heap at every allocation and deallocation
	// it slows down the execution
	tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	// Turn on debug allocation bit
	tmpFlag |= _CRTDBG_ALLOC_MEM_DF;
	// Set flag to the new value
	_CrtSetDbgFlag( tmpFlag );
}
/*
void CSimuManager::DisplayThisMessage(char* msg, bool bDisabled)
{
	#ifdef	SIMU_PARA_DISABLE_MONITOR_MESSAGE
	if (bDisabled) return;
	#endif

	if (m_pSimuMonitor != NULL)
	{
		CSimuMsg::PushSimuInfo(msg);
		m_pSimuMonitor->ShowSimulationInformation();
	}
}

void CSimuManager::EraseLastMessage(bool bDisabled)
{
	#ifdef	SIMU_PARA_DISABLE_MONITOR_MESSAGE
	if (bDisabled) return;
	#endif

	if (m_pSimuMonitor != NULL)
	{
		CSimuMsg::PopLastSimuInfo();
	}
}

void CSimuManager::CleanOldMessages(int maxMsgLen, int numOfMsgsToKeep)
{
	if (CSimuMsg::strSimuInfo.GetLength() <= maxMsgLen)
		return;
	CString newSimuInfo = "";
	int i;
	for (i=0; i < numOfMsgsToKeep && newSimuInfo.GetLength() <= maxMsgLen; i++)
	{
		newSimuInfo += CSimuMsg::PopLastSimuInfo();
		newSimuInfo += RETURN_STRING;
	}
	CSimuMsg::numSimuInfo += i;
	CSimuMsg::strSimuInfo = newSimuInfo;
}
*/
void CSimuManager::DisplayOperationCountingMessage(const char* elmntName,
													UINT countNum, UINT totalNum,
													UINT period,	// default = 100
													bool bDisabled)	// default = true
{
	#ifdef	SIMU_PARA_DISABLE_MONITOR_MESSAGE
	if (bDisabled) return;
	#endif

	if (countNum % period != 0) return;

	sprintf(CSimuMsg::GetEmptyBuf(), "\t%d of %d %s", countNum, totalNum, elmntName);
	CSimuMsg::DisplayMessage(CSimuMsg::GetCheckedBuf(), bDisabled);
	// only erase from CSimuMsg::strSimuInfo, and the message has been shown in m_pSimuMonitor
	CSimuMsg::EraseLastMessage(bDisabled);
}

void CSimuManager::InitNamedVector(CVector* vector)
{
	CString valueStr;

	if (strcmp(vector->GetName(), SIMU_TYPE_NO_NAME) == 0)
	{
		CSimuMsg::ExitWithMessage("CSimuView::InitNamedVector", "This is a no-name vector.");
	}
	else if (m_cParaList.Lookup(vector->GetName(), valueStr))
	{
		vector->ParseStringForValue(valueStr);
	}
	else
		CSimuMsg::ExitWithMessage("CSimuView::InitNamedVector",
								"No value is found for vector, %s, in %s.",
								vector->name);
}

void CSimuManager::PlaceDialogToTheRight(CDialog* pDialog)
{
	RECT rectMainWnd, rectDlg, rectWorkArea;

	GetWindowRect(&rectMainWnd);
	pDialog->GetWindowRect(&rectDlg);
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWorkArea, 0);

	if (rectMainWnd.right + rectDlg.right - rectDlg.left > rectWorkArea.right)
		// not enough space in horizontal dimension
		pDialog->SetWindowPos(NULL,
									 rectWorkArea.right - rectDlg.right + rectDlg.left,
									 rectMainWnd.top,
									 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	else
		pDialog->SetWindowPos(NULL,
									 rectMainWnd.right,
									 rectMainWnd.top,
									 0, 0, SWP_NOSIZE|SWP_NOZORDER);;
}
void CSimuManager::RefreshParametersOnInterface()
{
	m_pSimuMonitor->UpdateData(false);
}

void CSimuManager::DrawSimulationBodies()
{
//	TRACE("Draw Simulation Bodies\r\n");

	if (m_curPrtlFluid != NULL)
		m_curPrtlFluid->RenderPrtlFluid();

	for (int i=0; i < m_aryPrtlFluid.m_paNum; i++)
	{
		CPrtlFluid* pPrtlFluid = m_aryPrtlFluid[i];
		pPrtlFluid->RenderPrtlFluid();
	}
	int i;
	for (i=0; i < m_aryBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_aryBoundaries[i];
		if (pBdry->m_bdryVisible)
			pBdry->DrawBoundary();
	}

	if (m_enclosingBall->m_ebShow)
		m_enclosingBall->RenderEnclosingBall();

	if (DRAW_CONVEX_HULL)
		CConvexHull::RenderConvexHullElements();

	if (DRAW_DELAUNAY)
		CDelaunayTetrahedron::RenderDelaunayTetrahedralization();
}

void CSimuManager::ResetAnimation()
{
	m_pSimuMonitor->UpdateData();
	m_pSimuMonitor->m_bGo	= false;
	m_pSimuMonitor->m_uFwdIntvls		= 0;
	m_pSimuMonitor->m_dbAnimationClock	= 0;
	m_pSimuMonitor->m_dbTimeStep = ComputeTimeStep();

	CSimuMsg::ResetMessageData();
	CSimuManager::CREATE_SURFACE_MESH	= false;
	CSimuManager::m_bNeedExportDirectory= true;
	CSimuManager::m_nCurStep			= 0;
	CSimuManager::m_fPreExportTime		= SIMU_NEGATIVE_NUMBER;
	CSimuManager::m_dbTotalRunningTime	= 0;
	CSimuManager::m_dbTotalMotionTime	= 0;
	CSimuManager::m_dbTotalInteractTime	= 0;
	CSimuManager::m_dbTotalDownsampleClock = 0;
	CSimuManager::m_dbTotalUpsampleClock = 0;

//	m_pSimuView->ResetFocusPosition();
//	m_pSimuView->Invalidate(false);

	m_pSimuMonitor->ShowSimulationInformation();
	m_pSimuMonitor->UpdateData(false);

	InitializeOptions();
}

void CSimuManager::PrepareImportFluidData()
{
	ImportFluidParameters();
	if (IM_EXPORT_FLUID_SURFACE)
	{
		DRAW_SURFACE_MESH = true;
		DRAW_PARTICLES = false;
		if (EXPORT_FLUID_DATA
		 && (m_eFluidModel == UPSAMPLE || m_eFluidModel == SPH))
		{
			CREATE_SURFACE_MESH = true;
			InitializeOptions();
		}
	}
	else
	{
		DRAW_SURFACE_MESH = false;
		DRAW_PARTICLES = true;
	}
	m_dlgOptionMeshTexture->InitializeMeshTextureOptions();
}

void CSimuManager::SearchNeighboringPrtls(CFluidPrtl* pPrtl, CPrtlFluid* curFluid,
										  TGeomElemArray<CFluidPrtl> &pNgbrs)
{
	for (int i=0; i < m_aryPrtlFluid.m_paNum; i++)
	{
		CPrtlFluid* pFluid = m_aryPrtlFluid[i];
		if (pFluid == curFluid) continue;
		pFluid->m_pfPrtlSearch.GetNgbrPrtls(pPrtl->m_vpPos, curFluid->m_pfSmoothLen, pNgbrs);
	}
}

void CSimuManager::CreateCurrentPrtlFluid()
{
	if (m_curPrtlFluid != NULL)
		delete m_curPrtlFluid;

	m_curPrtlFluid = CreatePrtlFluid();

	ResetAnimation();
}

CPrtlFluid* CSimuManager::CreatePrtlFluid()
{
	char location[] = "CSimuManager::CreatePrtlFluid()";

	m_eFluidClass = CSimuManager::PrtlFluid;
	m_maxSrfcDetectPauseSteps = 1;
	m_maxDownsamplePauseSteps = 1;
	m_maxUpsamplePauseSteps = 1;
	for (int i=0; i < m_numStepDivPerFrame; i++)
	{
		m_maxSrfcDetectPauseSteps *= 2;
		m_maxDownsamplePauseSteps *= 2;
		m_maxUpsamplePauseSteps *= 2;
	}
	m_maxDownsamplePauseSteps = (int)floor(m_framesPerResample*m_maxDownsamplePauseSteps);
	m_maxUpsamplePauseSteps = (int)floor(m_framesPerResample*m_maxUpsamplePauseSteps);

	CPrtlFluid* pPrtlFluid = NULL;

	CSimuManager::DRAW_PARTICLES = true;
	CSimuManager::DRAW_SURFACE_MESH = false;
	if (m_eFluidModel == SPH)
	{
		pPrtlFluid = new CPrtlFluid();
	}
	else if (m_eFluidModel == UPSAMPLE)
	{
		pPrtlFluid = new CPrtlFluidUpsample();
	}
	else if (m_eFluidModel == SPRING)
	{
		pPrtlFluid = new CPrtlFluidSpring();
	}
	else if (m_eFluidModel == REMESH)
	{
		CSimuMsg::ExitWithMessage(location, "REMESH is obsolete.");
	}
	else if (m_eFluidModel == RESAMPLE)
	{
		CSimuMsg::ExitWithMessage(location, "RESAMPLE is obsolete.");
	}
	else if (m_eFluidModel == REDISTRIBUTE)
	{
		CSimuMsg::ExitWithMessage(location, "REDISTRIBUTE is obsolete.");
	}
	else
		CSimuMsg::ExitWithMessage("CSimuManager::CreateCurrentPrtlFluid()",
								"Invalid fluid model mode");

	return pPrtlFluid;
}

CFluidPrtl* CSimuManager::CreateOneFluidPrtl()
{
	CFluidPrtl* newPrtl;

	if (CSimuManager::m_eFluidModel == CSimuManager::SPRING)
	{
		newPrtl = new CFluidMeshPrtl();
	}
	else if (CSimuManager::NON_NEWTONIAN_FLUID)
		newPrtl = new CFluidPrtlNonNew();
	else if (CSimuManager::m_eCompressibility == CSimuManager::POISSON)
		newPrtl = new CFluidPrtlPoisson();
	else
		newPrtl = new CFluidPrtl();

	return newPrtl;
}

bool CSimuManager::IsTimeToExport()
{
	if (m_fPreExportTime < 0
		|| (m_pSimuMonitor->m_dbAnimationClock - m_fPreExportTime)
			>= m_pSimuMonitor->m_dbFrameIntvl
	   )
	   return true;
	return false;
}

void CSimuManager::RefreshView()
{
	m_pSimuView->RefreshView();
}

void CSimuManager::InitializeOptions()
{
	if (m_dlgOptionDisplay->IsWindowVisible())
	{
		m_dlgOptionDisplay->InitializeDisplayOptions();
		m_dlgOptionDisplay->UpdateData(false);
	}
	if (m_dlgOptionSurface->IsWindowVisible())
	{
		m_dlgOptionSurface->InitializeSurfaceOptions();
		m_dlgOptionSurface->UpdateData(false);
	}
	if (m_dlgOptionModeling->IsWindowVisible())
	{
		m_dlgOptionModeling->InitializeModelingOptions();
		m_dlgOptionModeling->UpdateData(false);
	}
	if (m_dlgOptionImExport->IsWindowVisible())
	{
		m_dlgOptionImExport->InitializeImExportOptions();
		m_dlgOptionImExport->UpdateData(false);
	}
	if (m_dlgOptionParticleSelect->IsWindowVisible())
	{
		m_dlgOptionParticleSelect->InitializeParticleSelectOptions();
		m_dlgOptionParticleSelect->UpdateData(false);
	}
	if (m_dlgOptionMesher->IsWindowVisible())
	{
		m_dlgOptionMesher->InitializeMesherOptions();
		m_dlgOptionMesher->UpdateData(false);
	}
	if (m_dlgOptionViewing->IsWindowVisible())
	{
		m_dlgOptionViewing->InitializeViewingOptions();
		m_dlgOptionViewing->UpdateData(false);
	}
	if (m_dlgOptionElementSelect->IsWindowVisible())
	{
		m_dlgOptionElementSelect->InitializeElementSelectOptions();
		m_dlgOptionElementSelect->UpdateData(false);
	}
	if (m_dlgOptionMeshTexture->IsWindowVisible())
	{
		m_dlgOptionMeshTexture->InitializeMeshTextureOptions();
		m_dlgOptionMeshTexture->UpdateData(false);
	}

	if (m_pSimuMonitor->IsWindowVisible())
	{
		if (m_curPrtlFluid != NULL)
		{
			m_pSimuMonitor->m_fFluidDensity = m_curPrtlFluid->m_pfDensity;
			m_pSimuMonitor->m_fPrtlMass = m_curPrtlFluid->m_pfPrtlMass;
			m_pSimuMonitor->m_fFluidSmoothLen = m_curPrtlFluid->m_pfSmoothLen;
			m_pSimuMonitor->m_fPrtlDist = m_curPrtlFluid->m_pfPrtlDist;
		}
		else
		{
			m_pSimuMonitor->m_fFluidDensity = m_prtlDensity;
			m_pSimuMonitor->m_fPrtlMass = m_prtlMass;
			m_pSimuMonitor->m_fFluidSmoothLen = m_prtlDistance*m_smoothLengthRatio;
			m_pSimuMonitor->m_fPrtlDist = m_prtlDistance;
		}
		m_pSimuMonitor->m_dbFrameIntvl = 1.0/m_numFramesPerSecond;
		m_pSimuMonitor->m_dbTimeStep = ComputeTimeStep();
		m_pSimuMonitor->UpdateData(false);
	}
}

SimuValue CSimuManager::ComputeTimeStep()
{
	SimuValue timeStep = 1.0/CSimuManager::m_numFramesPerSecond;
	timeStep  /= pow(2.0f, CSimuManager::m_numStepDivPerFrame);
	return timeStep;
}

void CSimuManager::UpdateTimeStep()
{
	if (m_curPrtlFluid != NULL)
		m_curPrtlFluid->m_pfTimeStep = m_pSimuMonitor->m_dbTimeStep;
}

void CSimuManager::SelectEnclosedParticles()
{
	if (m_curPrtlFluid != NULL)
		m_curPrtlFluid->SelectParticlesEnclosedBySphere(
							CSimuManager::m_enclosingBall->m_ebPos,
							CSimuManager::m_enclosingBall->m_ebRadius);

}

void CSimuManager::DeSelectEnclosedParticles()
{
	if (m_curPrtlFluid != NULL)
		m_curPrtlFluid->DeSelectParticlesEnclosedBySphere(
							CSimuManager::m_enclosingBall->m_ebPos,
							CSimuManager::m_enclosingBall->m_ebRadius);
}

void CSimuManager::DeSelectAllParticles()
{
	if (m_curPrtlFluid != NULL)
		m_curPrtlFluid->DeSelectAllParticles();
}

/////////////////////////////////////////////////////////////////////////////
// CSimuManager private methods

/////////////////////////////////////////////////////////////////////////////
//	It's assumed that string <code>src</code> contains a '='
//	The substring before '=' will be copied into <code>name</code>,
//	and the one after into <code>string</code>.
/////////////////////////////////////////////////////////////////////////////
bool CSimuManager::DelimiteNameValuePair(char* src, CString& name, CString& string)
{
	if (src == NULL || strlen(src) <= 0)
	{
		CSimuMsg::ExitWithMessage("CSimuManager::DelimiteNameValuePair(...)",
									"src is NULL or empty.");
		return false;
	}
	int len = strlen(src);
	int nameLen = 0;
	int i;
	name = "";
	string = "";
	for(i=0; i < len; i++)
	{
		if (src[i] == '=')
			nameLen = i;
		else if (nameLen == 0)
			name += src[i];
		else
			string += src[i];
	}
	if (nameLen >= len-1)
	{
		sprintf(CSimuMsg::GetEmptyBuf(), "No value in src, %s", src);
		CSimuMsg::ExitWithMessage("CSimuManager::DelimiteNameValuePair",
								CSimuMsg::GetCheckedBuf());
		return false;
	}
	return true;
}

void CSimuManager::ReadSimulationParameters()
{
	char location[] = "CSimuManager::ReadSimulationParameters()";

	CString	strFileRef(m_strAppDataDir);
	strFileRef += "\\";
	strFileRef += APP_PARAMETER_FILE_NAME;

	FILE*	fp;
	char buf[LEN_DATA_LINE];

	if ((fp=fopen(strFileRef,"r")) == NULL)
	{
		sprintf(CSimuMsg::GetEmptyBuf(), "Can't open data file %s.", strFileRef);
		CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
	}

	while(!feof(fp))
		// Read the file until the end.
		// Any empty line or the line that starts with '#' will be skipped.
	{
		fgets(buf, sizeof(buf), fp);
		if (ferror(fp))
		{
			fclose(fp);
			sprintf(CSimuMsg::GetEmptyBuf(), "Error in reading data file %s.", strFileRef);
			CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
		}
		if (strlen(buf) > 0) buf[strlen(buf)-1] = '\0';	// trim the last '\n' in line
		if ((strlen(buf) <=0)||(buf[0] == '#')) continue;
		TRACE("%s\n", buf);

		CString name;
		CString valueStr;
		if (DelimiteNameValuePair(buf, name, valueStr))
		{
			name.TrimLeft();	valueStr.TrimLeft();	
			name.TrimRight();	valueStr.TrimRight();
			m_cParaList.SetAt((LPCTSTR)name, (LPCTSTR)valueStr);
		}
		else
		{
			sprintf(CSimuMsg::GetEmptyBuf(),
					"Error in delimite string: %s in file, %s.", buf, strFileRef);
			CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
		}
	}
	fclose(fp);
}

void CSimuManager::InitManagerParameters()
{
	InitNamedVector(m_pMainWinRect);
}

void CSimuManager::MakeExportDirectory()
{
	CTime curTime	= CTime::GetCurrentTime();
	CString timeStr	= curTime.Format("%b_%d_%H_%M_%S");
	m_strExportDir = m_strAppDataDir;
	m_strExportDir += "\\";
	m_strExportDir += timeStr;
	sprintf(CSimuMsg::GetEmptyBuf(), "mkdir \"%s\"", m_strExportDir);
	if (system(CSimuMsg::GetCheckedBuf()) == -1)
		CSimuMsg::ExitWithMessage("CSimuManager::CSimuManager()",
								"Failed command: %s.", CSimuMsg::GetCheckedBuf());

	m_strSimuInfoFile = m_strExportDir;
	m_strSimuInfoFile += "\\";
	m_strSimuInfoFile += APP_OUTPUT_INFO_FILE_NAME;
}

void CSimuManager::ImportFluidParameters()
{
	char location[] = "CSimuManager::ImportFluidParameters()";

	// make m_strSimuInfoFile
	m_strSimuInfoFile = m_strImportDir;
	m_strSimuInfoFile += "\\";
	m_strSimuInfoFile += APP_OUTPUT_INFO_FILE_NAME;

	// make the fluid info file
	CString fileName = m_strImportDir;
	fileName += "\\";
	fileName += m_strImportParaFile;
	// Open the fluid info file
	FILE* fp;
	if ((fp=fopen(fileName, "r")) == NULL)
	{
		sprintf(CSimuMsg::GetEmptyBuf(), "Can't open fluid info file %s to read.", fileName);
		CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
		return;
	}
	sprintf(CSimuMsg::GetEmptyBuf(), "Import fluid info from file: %s.", fileName);
	CSimuMsg::DisplayMessage(CSimuMsg::GetCheckedBuf());

	time_t fileTime = GetFileModificationTime(fp);
	if (fileTime == -1)
	{
		CSimuMsg::ExitWithMessage(location, "Cannot get file time.");
		return;
	}

	if (FirstTimeIsLaterThanSecondTime(fileTime, m_t2005_JUNE_29_5PM))
		m_enumImExportFormat = AFTER_2005_JUNE_29_5PM;
	else if (FirstTimeIsLaterThanSecondTime(fileTime, m_tNewImExportFormatTime))
		m_enumImExportFormat = BEFORE_2005_JUNE_29_5PM;
	else
		m_enumImExportFormat = BEFORE_2005_APR_11_6PM;

	if (m_enumImExportFormat == AFTER_2005_JUNE_29_5PM)
	{
		ImportGlobalParameters(fp);
		// import fluids
		int tmpInt;
		TClassArray<CPrtlFluid> newPrtlFluids;
		fscanf(fp, "fluid number=%d\n", &tmpInt);
		for (int i=0; i < tmpInt; i++)
		{
			CPrtlFluid* pFluid = CreatePrtlFluid();
			newPrtlFluids.AppendOnePointer(pFluid);
			pFluid->m_pfSimuManager = this;
			pFluid->ImportPrtlFluidVariables(fp);
		}
		// load parameters of old fluids into new fluids
		for (int i=0; i < tmpInt; i++)
		{
			CPrtlFluid* newFluid = newPrtlFluids[i];
			for (int j=0; j < m_aryPrtlFluid.m_paNum; j++)
			{
				CPrtlFluid* oldFluid = m_aryPrtlFluid[j];
				if (newFluid->m_deiName == oldFluid->m_deiName)
				{
					newFluid->LoadPrtlFluidVariablesFrom(oldFluid);
					break;
				}
			}
		}
		m_aryPrtlFluid.ResetArraySize(0);
		m_aryPrtlFluid.TakeOverPointersFromArray(newPrtlFluids);
		// import boundaries
		int bdryType;
		m_aryBoundaries.ResetArraySize(0);
		fscanf(fp, "boundary number=%d\n", &tmpInt);
		for (int i=0; i < tmpInt; i++)
		{
			fscanf(fp, "boundary type=%d\n", &bdryType);
			CBoundary* pBdry = NULL;
			switch (bdryType)
			{
				case CBoundary::BOWL:
					pBdry = new CBdryBowl();
					break;
				case CBoundary::BOX:
					pBdry = new CSimuBoundaryBox();
					break;
				case CBoundary::CYLINDER:
					pBdry = new CSimuBoundaryCylinder();
					break;
				case CBoundary::SOLID_TORUS:
					pBdry = new CBdrySolidTorus();
					break;
				case CBoundary::NOZZLE:
					pBdry = new CSimuFluidNozzle();
					break;
				case CBoundary::SQUARE_SLOPE:
					pBdry = new CBdrySquareSlope();
					break;
				case CBoundary::SOLID_ROD:
					pBdry = new CBdrySolidRod();
					break;
				case CBoundary::SOLID_BOX:
					pBdry = new CBdrySolidBox();
					break;
				case CBoundary::TABLE:
					pBdry = new CSimuBoundaryTable();
					break;
				case CBoundary::RIGID_BALL:
					pBdry = new CRigidBall();
					break;
				case CBoundary::HOLLOW_CONE:
					pBdry = new CBdryHollowCone();
					break;
				default:
					CSimuMsg::ExitWithMessage(location, "Unknown boundary type.");
					break;
			}
			if (pBdry != NULL)
			{
				pBdry->ImportBoundaryParameters(fp);
				m_aryBoundaries.AppendOnePointer(pBdry);
			}
		}
		if (m_aryPrtlFluid.m_paNum == 0)
		{
			CreateCurrentPrtlFluid();
			m_curPrtlFluid->m_deiImported = true;
			m_curPrtlFluid->ImportPrtlFluidParameters(fp);
		}
		fclose(fp);
	}
	else if (m_enumImExportFormat == BEFORE_2005_JUNE_29_5PM)
	{
		ImportGlobalParameters(fp);
		switch (CSimuManager::m_eFluidClass)
		{
			case PrtlFluid:
				CreateCurrentPrtlFluid();
				m_curPrtlFluid->m_deiImported = true;
				m_curPrtlFluid->ImportPrtlFluidParameters(fp);
				break;
		};
		fclose(fp);
	}
	else
	{
		fscanf(fp, "%d\n", &CSimuManager::m_eFluidClass);
		switch (CSimuManager::m_eFluidClass)
		{
			case PrtlFluid:
				fscanf(fp, "%d\n", &CSimuManager::m_eFluidModel);
				CreateCurrentPrtlFluid();
				m_curPrtlFluid->m_deiImported = true;
				m_curPrtlFluid->ImportPrtlFluidParameters(fp);
				break;
		};
		float tmpFloat;
		fscanf(fp, "frame interval=%f\n", &tmpFloat); m_pSimuMonitor->m_dbFrameIntvl = tmpFloat;
		fclose(fp);
	}
}

void CSimuManager::ExportFluidParameters()
{
	char location[] = "CSimuManager::ExportFluidParameters()";

	FILE* fp;
	// make the fluid info file
	CString fileName = m_strExportDir;
	fileName += "\\";
	fileName += APP_FLUID_PARA_FILE_NAME;
	// Open the fluid info file
	if ((fp=fopen(fileName, "w")) == NULL)
	{
		sprintf(CSimuMsg::GetEmptyBuf(), "Can't open fluid info file %s to write.", fileName);
		CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
		return;
	}

	time_t curTime;
	time(&curTime);
	m_enumImExportFormat = BEFORE_2005_JUNE_29_5PM;
	if (FirstTimeIsLaterThanSecondTime(curTime, m_t2005_JUNE_29_5PM))
	{
		m_enumImExportFormat = AFTER_2005_JUNE_29_5PM;

		ExportGlobalParameters(fp);
		// export fluids
		fprintf(fp, "fluid number=%d\n", m_aryPrtlFluid.m_paNum);
		for (int i=0; i < m_aryPrtlFluid.m_paNum; i++)
		{
			CPrtlFluid* pFluid = m_aryPrtlFluid[i];
			pFluid->ExportPrtlFluidVariables(fp);
		}
		// export boundaries
		fprintf(fp, "boundary number=%d\n", m_aryBoundaries.m_paNum);
		for (int i=0; i < m_aryBoundaries.m_paNum; i++)
		{
			CBoundary* pBdry = m_aryBoundaries[i];
			fprintf(fp, "boundary type=%d\n", pBdry->m_bdryType);
			pBdry->ExportBoundaryParameters(fp);
		}
		if (m_aryPrtlFluid.m_paNum == 0)
		{
			if (m_curPrtlFluid != NULL)
			{
				sprintf(CSimuMsg::GetEmptyBuf(), "Output fluid(%s) info into file: %s.",
													m_curPrtlFluid->m_deiName, fileName);
				CSimuMsg::DisplayMessage(CSimuMsg::GetCheckedBuf());
				m_curPrtlFluid->ExportPrtlFluidParameters(fp);
			}
		}
		fclose(fp);
	}
	else if (CSimuManager::FirstTimeIsLaterThanSecondTime(curTime, m_tNewImExportFormatTime))
	{
		ExportGlobalParameters(fp);
		switch (CSimuManager::m_eFluidClass)
		{
			case PrtlFluid:
				if (m_curPrtlFluid != NULL)
				{
					sprintf(CSimuMsg::GetEmptyBuf(), "Output fluid(%s) info into file: %s.",
														m_curPrtlFluid->m_deiName, fileName);
					CSimuMsg::DisplayMessage(CSimuMsg::GetCheckedBuf());
					m_curPrtlFluid->ExportPrtlFluidParameters(fp);
				}
				break;
		};
		fclose(fp);
	}
	else
	{
		fprintf(fp, "%d\n", CSimuManager::m_eFluidClass);
		switch (CSimuManager::m_eFluidClass)
		{
			case PrtlFluid:
				if (m_curPrtlFluid != NULL)
				{
					sprintf(CSimuMsg::GetEmptyBuf(), "Output fluid(%s) info into file: %s.",
														m_curPrtlFluid->m_deiName, fileName);
					CSimuMsg::DisplayMessage(CSimuMsg::GetCheckedBuf());
					fprintf(fp, "%d\n", CSimuManager::m_eFluidModel);
					m_curPrtlFluid->ExportPrtlFluidParameters(fp);
				}
				break;
		};
		fprintf(fp, "frame interval=%f\n", m_pSimuMonitor->m_dbFrameIntvl);
		fclose(fp);
	}
}

void CSimuManager::ImportGlobalParameters(FILE* fp)
{
	char location[] = "CSimuManager::ImportGlobalParameters(fp)";

	char buf[LEN_DATA_LINE];
	char tmp[LEN_DATA_LINE];
	float tmpFloat, x, y, z;
	int tmpInt;

	while(!feof(fp)) // Read the file until the end.
	{
		fgets(buf, sizeof(buf), fp);
		if (ferror(fp))
		{
			fclose(fp);
			CSimuMsg::ExitWithMessage(location, "Error in reading global parameters.");
			return;
		}
		// Any empty line or the line that starts with '#' will be skipped.
		if ((strlen(buf) <=0)||(buf[0] == '#')) continue;
		TRACE("%s\n", buf);

		if (strncmp(buf, "m_eFluidClass", strlen("m_eFluidClass")) == 0)
		{
			sscanf(buf, "m_eFluidClass=%d\n", &tmpInt); m_eFluidClass = (EFluidClass)tmpInt;
		}
		else if (strncmp(buf, "m_numFramesPerSecond", strlen("m_numFramesPerSecond")) == 0)
		{
			sscanf(buf, "m_numFramesPerSecond=%d\n", &tmpInt); m_numFramesPerSecond = tmpInt;
		}
		else if (strncmp(buf, "m_numStepDivPerFrame", strlen("m_numStepDivPerFrame")) == 0)
		{
			sscanf(buf, "m_numStepDivPerFrame=%d\n", &tmpInt); m_numStepDivPerFrame = tmpInt;
		}
		else if (strncmp(buf, "m_onlyGravityTime", strlen("m_onlyGravityTime")) == 0)
		{
			sscanf(buf, "m_onlyGravityTime=%f\n", &tmpFloat); m_onlyGravityTime = tmpFloat;
		}
		else if (strncmp(buf, "m_onlyBdryMovingTime", strlen("m_onlyBdryMovingTime")) == 0)
		{
			sscanf(buf, "m_onlyBdryMovingTime=%f\n", &tmpFloat); m_onlyBdryMovingTime = tmpFloat;
		}
		else if (strncmp(buf, "APPLY_GRAVITY", strlen("APPLY_GRAVITY")) == 0)
		{
			sscanf(buf, "APPLY_GRAVITY=%d\n", &tmpInt); APPLY_GRAVITY = tmpInt;
		}
		else if (strncmp(buf, "GRAVITY", strlen("GRAVITY")) == 0)
		{
			sscanf(buf, "GRAVITY=(%f, %f, %f)\n", &x, &y, &z);
			GRAVITY->v[X] = x; GRAVITY->v[Y] = y; GRAVITY->v[Z] = z;
		}
		else if (strncmp(buf, "APPLY_SURFACE_TENSION", strlen("APPLY_SURFACE_TENSION")) == 0)
		{
			sscanf(buf, "APPLY_SURFACE_TENSION=%d\n", &tmpInt); APPLY_SURFACE_TENSION = tmpInt;
		}
		else if (strncmp(buf, "m_surfaceTension", strlen("m_surfaceTension")) == 0)
		{
			sscanf(buf, "m_surfaceTension=%f\n", &tmpFloat);
			m_surfaceTension = tmpFloat;
		}
		else if (strncmp(buf, "ADAPTIVE_TIME_STEP", strlen("ADAPTIVE_TIME_STEP")) == 0)
		{
			sscanf(buf, "ADAPTIVE_TIME_STEP=%d\n", &tmpInt); ADAPTIVE_TIME_STEP = tmpInt;
		}
		else if (strncmp(buf, "ADJUST_VEL_WITH_NGBRS", strlen("ADJUST_VEL_WITH_NGBRS")) == 0)
		{
			sscanf(buf, "ADJUST_VEL_WITH_NGBRS=%d\n", &tmpInt); ADJUST_VEL_WITH_NGBRS = tmpInt;
		}
		else if (strncmp(buf, "ADJUST_TENSOR_WITH_NGBRS", strlen("ADJUST_TENSOR_WITH_NGBRS")) == 0)
		{
			sscanf(buf, "ADJUST_TENSOR_WITH_NGBRS=%d\n", &tmpInt); ADJUST_TENSOR_WITH_NGBRS = tmpInt;
		}
		else if (strncmp(buf, "DUMP_STRESS_TENSOR", strlen("DUMP_STRESS_TENSOR")) == 0)
		{
			sscanf(buf, "DUMP_STRESS_TENSOR=%d\n", &tmpInt); DUMP_STRESS_TENSOR = tmpInt;
		}
		else if (strncmp(buf, "m_eFluidModel", strlen("m_eFluidModel")) == 0)
		{
			sscanf(buf, "m_eFluidModel=%d\n", &tmpInt); m_eFluidModel = (EFluidModel)tmpInt;
		}
		else if (strncmp(buf, "m_eCompressibility", strlen("m_eCompressibility")) == 0)
		{
			sscanf(buf, "m_eCompressibility=%d\n", &tmpInt); m_eCompressibility = (ECompressibility)tmpInt;
		}
		else if (strncmp(buf, "m_maxPoissonPauseSteps", strlen("m_maxPoissonPauseSteps")) == 0)
		{
			sscanf(buf, "m_maxPoissonPauseSteps=%d\n", &tmpInt); m_maxPoissonPauseSteps = tmpInt;
		}
		else if (strncmp(buf, "m_airPressure", strlen("m_airPressure")) == 0)
		{
			sscanf(buf, "m_airPressure=%f\n", &tmpFloat); m_airPressure = tmpFloat;
		}
		else if (strncmp(buf, "m_bConvexHull4SrfcDetect", strlen("m_bConvexHull4SrfcDetect")) == 0)
		{
			sscanf(buf, "m_bConvexHull4SrfcDetect=%d\n", &tmpInt); m_bConvexHull4SrfcDetect = tmpInt;
		}
		else if (strncmp(buf, "m_bLocalFluidOrgDensity", strlen("m_bLocalFluidOrgDensity")) == 0)
		{
			sscanf(buf, "m_bLocalFluidOrgDensity=%d\n", &tmpInt); m_bLocalFluidOrgDensity = tmpInt;
		}
		else if (strncmp(buf, "m_eLaplacian", strlen("m_eLaplacian")) == 0)
		{
			sscanf(buf, "m_eLaplacian=%d\n", &tmpInt); m_eLaplacian = (ELaplacian)tmpInt;
		}
		else if (strncmp(buf, "NON_NEWTONIAN_FLUID", strlen("NON_NEWTONIAN_FLUID")) == 0)
		{
			sscanf(buf, "NON_NEWTONIAN_FLUID=%d\n", &tmpInt); NON_NEWTONIAN_FLUID = tmpInt;
		}
		else if (strncmp(buf, "m_eRotationDerivative", strlen("m_eRotationDerivative")) == 0)
		{
			sscanf(buf, "m_eRotationDerivative=%d\n", &tmpInt); m_eRotationDerivative = (ERotation)tmpInt;
		}
		else if (strncmp(buf, "m_rotationFactor", strlen("m_rotationFactor")) == 0)
		{
			sscanf(buf, "m_rotationFactor=%f\n", &tmpFloat); m_rotationFactor = tmpFloat;
		}
		else if (strncmp(buf, "VISCOSITY", strlen("VISCOSITY")) == 0)
		{
			sscanf(buf, "VISCOSITY=%d\n", &tmpInt); VISCOSITY = tmpInt;
		}
		else if (strncmp(buf, "m_viscosity", strlen("m_viscosity")) == 0)
		{
			sscanf(buf, "m_viscosity=%f\n", &tmpFloat); m_viscosity = tmpFloat;
		}
		else if (strncmp(buf, "IMPLICIT_STRESS_INTEGRATION", strlen("IMPLICIT_STRESS_INTEGRATION")) == 0)
		{
			sscanf(buf, "IMPLICIT_STRESS_INTEGRATION=%d\n", &tmpInt); IMPLICIT_STRESS_INTEGRATION = tmpInt;
		}
		else if (strncmp(buf, "IMPLICIT_TENSOR_INTEGRATION", strlen("IMPLICIT_TENSOR_INTEGRATION")) == 0)
		{
			sscanf(buf, "IMPLICIT_TENSOR_INTEGRATION=%d\n", &tmpInt); IMPLICIT_TENSOR_INTEGRATION = tmpInt;
		}
		else if (strncmp(buf, "m_numTensorSubIntegraions", strlen("m_numTensorSubIntegraions")) == 0)
		{
			sscanf(buf, "m_numTensorSubIntegraions=%d\n", &tmpInt); m_numTensorSubIntegraions = tmpInt;
		}
		else if (strncmp(buf, "m_eInterFluidModel", strlen("m_eInterFluidModel")) == 0)
		{
			sscanf(buf, "m_eInterFluidModel=%d\n", &tmpInt); m_eInterFluidModel = (EInterFluidModel)tmpInt;
		}
		else if (strncmp(buf, "m_bRepulsiveResponseOnly", strlen("m_bRepulsiveResponseOnly")) == 0)
		{
			sscanf(buf, "m_bRepulsiveResponseOnly=%d\n", &tmpInt); m_bRepulsiveResponseOnly = tmpInt;
		}
		else if (strncmp(buf, "m_bStickyInterFluids", strlen("m_bStickyInterFluids")) == 0)
		{
			sscanf(buf, "m_bStickyInterFluids=%d\n", &tmpInt); m_bStickyInterFluids = tmpInt;
		}
		else if (strncmp(buf, "m_interFluidStickyness", strlen("m_interFluidStickyness")) == 0)
		{
			sscanf(buf, "m_interFluidStickyness=%f\n", &tmpFloat); m_interFluidStickyness = tmpFloat;
		}
		else if (strncmp(buf, "m_interFluidDamping", strlen("m_interFluidDamping")) == 0)
		{
			sscanf(buf, "m_interFluidDamping=%f\n", &tmpFloat); m_interFluidDamping = tmpFloat;
		}
		else if (strncmp(buf, "m_interFluidTension", strlen("m_interFluidTension")) == 0)
		{
			sscanf(buf, "m_interFluidTension=%f\n", &tmpFloat); m_interFluidTension = tmpFloat;
		}
		else if (strncmp(buf, "UNIFORM_FIXED_PARTICLE_MASS", strlen("UNIFORM_FIXED_PARTICLE_MASS")) == 0)
		{
			sscanf(buf, "UNIFORM_FIXED_PARTICLE_MASS=%d\n", &tmpInt); UNIFORM_FIXED_PARTICLE_MASS = tmpInt;
		}
		else if (strncmp(buf, "m_prtlMass", strlen("m_prtlMass")) == 0)
		{
			sscanf(buf, "m_prtlMass=%f\n", &tmpFloat); m_prtlMass = tmpFloat;
		}
		else if (strncmp(buf, "m_prtlDensity", strlen("m_prtlDensity")) == 0)
		{
			sscanf(buf, "m_prtlDensity=%f\n", &tmpFloat); m_prtlDensity = tmpFloat;
		}
		else if (strncmp(buf, "m_prtlDistance", strlen("m_prtlDistance")) == 0)
		{
			sscanf(buf, "m_prtlDistance=%f\n", &tmpFloat); m_prtlDistance = tmpFloat;
		}
		else if (strncmp(buf, "m_minPrtlDistance", strlen("m_minPrtlDistance")) == 0)
		{
			sscanf(buf, "m_minPrtlDistance=%f\n", &tmpFloat); m_minPrtlDistance = tmpFloat;
		}
		else if (strncmp(buf, "m_prtlPosJitter", strlen("m_prtlPosJitter")) == 0)
		{
			sscanf(buf, "m_prtlPosJitter=%f\n", &tmpFloat); m_prtlPosJitter = tmpFloat;
		}
		else if (strncmp(buf, "m_interFluidDumping", strlen("m_interFluidDumping")) == 0)
		{
			sscanf(buf, "m_interFluidDumping=%f\n", &tmpFloat); m_interFluidDumping = tmpFloat;
		}
		else if (strncmp(buf, "m_interFluidFriction", strlen("m_interFluidFriction")) == 0)
		{
			sscanf(buf, "m_interFluidFriction=%f\n", &tmpFloat); m_interFluidFriction = tmpFloat;
		}
		else if (strncmp(buf, "m_distDumpingRate", strlen("m_distDumpingRate")) == 0)
		{
			sscanf(buf, "m_distDumpingRate=%f\n", &tmpFloat); m_distDumpingRate = tmpFloat;
		}
		else if (strncmp(buf, "m_energyDumpingRate", strlen("m_energyDumpingRate")) == 0)
		{
			sscanf(buf, "m_energyDumpingRate=%f\n", &tmpFloat); m_energyDumpingRate = tmpFloat;
		}
		else if (strncmp(buf, "m_frictionRate", strlen("m_frictionRate")) == 0)
		{
			sscanf(buf, "m_frictionRate=%f\n", &tmpFloat); m_frictionRate = tmpFloat;
		}
		else if (strncmp(buf, "m_stressDumpingRate", strlen("m_stressDumpingRate")) == 0)
		{
			sscanf(buf, "m_stressDumpingRate=%f\n", &tmpFloat); m_stressDumpingRate = tmpFloat;
		}
		else if (strncmp(buf, "m_ballDumpingRate", strlen("m_ballDumpingRate")) == 0)
		{
			sscanf(buf, "m_ballDumpingRate=%f\n", &tmpFloat); m_ballDumpingRate = tmpFloat;
		}
		else if (strncmp(buf, "m_soundSpeed", strlen("m_soundSpeed")) == 0)
		{
			sscanf(buf, "m_soundSpeed=%f\n", &tmpFloat); m_soundSpeed = tmpFloat;
		}
		else if (strncmp(buf, "m_velocityTuner", strlen("m_velocityTuner")) == 0)
		{
			sscanf(buf, "m_velocityTuner=%f\n", &tmpFloat); m_velocityTuner = tmpFloat;
		}
		else if (strncmp(buf, "m_interFluidVelTuner", strlen("m_interFluidVelTuner")) == 0)
		{
			sscanf(buf, "m_interFluidVelTuner=%f\n", &tmpFloat); m_interFluidVelTuner = tmpFloat;
		}
		else if (strncmp(buf, "m_courantFactor", strlen("m_courantFactor")) == 0)
		{
			sscanf(buf, "m_courantFactor=%f\n", &tmpFloat); m_courantFactor = tmpFloat;
		}
		else if (strncmp(buf, "m_smoothLengthRatio", strlen("m_smoothLengthRatio")) == 0)
		{
			sscanf(buf, "m_smoothLengthRatio=%f\n", &tmpFloat); m_smoothLengthRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_srfcDenThreshold", strlen("m_srfcDenThreshold")) == 0)
		{
			sscanf(buf, "m_srfcDenThreshold=%f\n", &tmpFloat); m_srfcDenThreshold = tmpFloat;
		}
		else if (strncmp(buf, "m_srfcWSThreshold", strlen("m_srfcWSThreshold")) == 0)
		{
			sscanf(buf, "m_srfcWSThreshold=%f\n", &tmpFloat); m_srfcWSThreshold = tmpFloat;
		}
		else if (strncmp(buf, "m_srfcNgbrNumThreshold", strlen("m_srfcNgbrNumThreshold")) == 0)
		{
			sscanf(buf, "m_srfcNgbrNumThreshold=%f\n", &tmpFloat); m_srfcNgbrNumThreshold = tmpFloat;
		}
		else if (strncmp(buf, "m_interSrfcNgbrNumThreshold", strlen("m_interSrfcNgbrNumThreshold")) == 0)
		{
			sscanf(buf, "m_interSrfcNgbrNumThreshold=%f\n", &tmpFloat); m_interSrfcNgbrNumThreshold = tmpFloat;
		}
		else if (strncmp(buf, "m_minNumNgbrForUpsample", strlen("m_minNumNgbrForUpsample")) == 0)
		{
			sscanf(buf, "m_minNumNgbrForUpsample=%f\n", &tmpFloat); m_minNumNgbrForUpsample = tmpFloat;
		}
		else if (strncmp(buf, "m_CGSolverTolerance", strlen("m_CGSolverTolerance")) == 0)
		{
			sscanf(buf, "m_CGSolverTolerance=%f\n", &tmpFloat); m_CGSolverTolerance = tmpFloat;
		}
		else if (strncmp(buf, "m_lowerPrtlMassLimitRatio", strlen("m_lowerPrtlMassLimitRatio")) == 0)
		{
			sscanf(buf, "m_lowerPrtlMassLimitRatio=%f\n", &tmpFloat); m_lowerPrtlMassLimitRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_upperPrtlMassLimitRatio", strlen("m_upperPrtlMassLimitRatio")) == 0)
		{
			sscanf(buf, "m_upperPrtlMassLimitRatio=%f\n", &tmpFloat); m_upperPrtlMassLimitRatio= tmpFloat;
		}
		else if (strncmp(buf, "m_downSampleDistRatio", strlen("m_downSampleDistRatio")) == 0)
		{
			sscanf(buf, "m_downSampleDistRatio=%f\n", &tmpFloat); m_downSampleDistRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_intolerablePrtlDist", strlen("m_intolerablePrtlDist")) == 0)
		{
			sscanf(buf, "m_intolerablePrtlDist=%f\n", &tmpFloat); m_intolerablePrtlDist = tmpFloat;
		}
		else if (strncmp(buf, "m_srfcThicknessRatio", strlen("m_srfcThicknessRatio")) == 0)
		{
			sscanf(buf, "m_srfcThicknessRatio=%f\n", &tmpFloat); m_srfcThicknessRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_upsampleMinRadiusRatio", strlen("m_upsampleMinRadiusRatio")) == 0)
		{
			sscanf(buf, "m_upsampleMinRadiusRatio=%f\n", &tmpFloat); m_upsampleMinRadiusRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_upsampleMaxRadiusRatio", strlen("m_upsampleMaxRadiusRatio")) == 0)
		{
			sscanf(buf, "m_upsampleMaxRadiusRatio=%f\n", &tmpFloat); m_upsampleMaxRadiusRatio = tmpFloat;
		}
		else if (strncmp(buf, "SAME_DOWN_UP_SAMPLE_RATE", strlen("SAME_DOWN_UP_SAMPLE_RATE")) == 0)
		{
			sscanf(buf, "SAME_DOWN_UP_SAMPLE_RATE=%d\n", &tmpInt); SAME_DOWN_UP_SAMPLE_RATE = tmpInt;
		}
		else if (strncmp(buf, "INTERLEAVE_DOWN_UP_SAMPLE", strlen("INTERLEAVE_DOWN_UP_SAMPLE")) == 0)
		{
			sscanf(buf, "INTERLEAVE_DOWN_UP_SAMPLE=%d\n", &tmpInt); INTERLEAVE_DOWN_UP_SAMPLE = tmpInt;
		}
		else if (strncmp(buf, "NOT_MERGE_COLLIDING_PRTLS", strlen("NOT_MERGE_COLLIDING_PRTLS")) == 0)
		{
			sscanf(buf, "NOT_MERGE_COLLIDING_PRTLS=%d\n", &tmpInt); NOT_MERGE_COLLIDING_PRTLS = tmpInt;
		}
		else if (strncmp(buf, "m_maxSrfcDetectPauseSteps", strlen("m_maxSrfcDetectPauseSteps")) == 0)
		{
			sscanf(buf, "m_maxSrfcDetectPauseSteps=%d\n", &tmpInt); m_maxSrfcDetectPauseSteps = tmpInt;
		}
		else if (strncmp(buf, "m_maxDownsamplePauseSteps", strlen("m_maxDownsamplePauseSteps")) == 0)
		{
			sscanf(buf, "m_maxDownsamplePauseSteps=%d\n", &tmpInt); m_maxDownsamplePauseSteps = tmpInt;
		}
		else if (strncmp(buf, "m_maxUpsamplePauseSteps", strlen("m_maxUpsamplePauseSteps")) == 0)
		{
			sscanf(buf, "m_maxUpsamplePauseSteps=%d\n", &tmpInt); m_maxUpsamplePauseSteps = tmpInt;
		}
		else if (strncmp(buf, "m_framesPerResample", strlen("m_framesPerResample")) == 0)
		{
			sscanf(buf, "m_framesPerResample=%d\n", &tmpInt); m_framesPerResample = tmpInt;
		}
		else if (strncmp(buf, "CONSERVE_KINETIC_ENERGY", strlen("CONSERVE_KINETIC_ENERGY")) == 0)
		{
			sscanf(buf, "CONSERVE_KINETIC_ENERGY=%d\n", &tmpInt); CONSERVE_KINETIC_ENERGY = tmpInt;
		}
		else if (strncmp(buf, "m_shearModulus", strlen("m_shearModulus")) == 0)
		{
			sscanf(buf, "m_shearModulus=%f\n", &tmpFloat); m_shearModulus = tmpFloat;
		}
		else if (strncmp(buf, "m_relaxationTime", strlen("m_relaxationTime")) == 0)
		{
			sscanf(buf, "m_relaxationTime=%f\n", &tmpFloat); m_relaxationTime = tmpFloat;
		}
		else if (strncmp(buf, "m_plasticYieldValue", strlen("m_plasticYieldValue")) == 0)
		{
			sscanf(buf, "m_plasticYieldValue=%f\n", &tmpFloat); m_plasticYieldValue = tmpFloat;
		}
		else if (strncmp(buf, "m_stressTensorTuner", strlen("m_stressTensorTuner")) == 0)
		{
			sscanf(buf, "m_stressTensorTuner=%f\n", &tmpFloat); m_stressTensorTuner = tmpFloat;
		}
		else if (strncmp(buf, "THERMAL_FLUID", strlen("THERMAL_FLUID")) == 0)
		{	// detect "THERMAL_FLUID" in order to be compatible with old data formate
			sscanf(buf, "THERMAL_FLUID=%d\n", &tmpInt); HEAT_TRANSFER = tmpInt;
		}
		else if (strncmp(buf, "HEAT_TRANSFER", strlen("HEAT_TRANSFER")) == 0)
		{
			sscanf(buf, "HEAT_TRANSFER=%d\n", &tmpInt); HEAT_TRANSFER = tmpInt;
		}
		else if (strncmp(buf, "m_subThermalSteps", strlen("m_subThermalSteps")) == 0)
		{
			sscanf(buf, "m_subThermalSteps=%d\n", &tmpInt); m_subThermalSteps = tmpInt;
		}
		else if (strncmp(buf, "m_heatConductivity", strlen("m_heatConductivity")) == 0)
		{
			sscanf(buf, "m_heatConductivity=%f\n", &tmpFloat); m_heatConductivity = tmpFloat;
		}
		else if (strncmp(buf, "m_minTemperature", strlen("m_minTemperature")) == 0)
		{
			sscanf(buf, "m_minTemperature=%f\n", &tmpFloat); m_minTemperature = tmpFloat;
		}
		else if (strncmp(buf, "m_maxTemperature", strlen("m_maxTemperature")) == 0)
		{
			sscanf(buf, "m_maxTemperature=%f\n", &tmpFloat); m_maxTemperature = tmpFloat;
		}
		else if (strncmp(buf, "m_prtlIniTemperature", strlen("m_prtlIniTemperature")) == 0)
		{
			sscanf(buf, "m_prtlIniTemperature=%f\n", &tmpFloat); m_prtlIniTemperature = tmpFloat;
		}
		else if (strncmp(buf, "m_bdryMaxHeatingDist", strlen("m_bdryMaxHeatingDist")) == 0)
		{
			sscanf(buf, "m_bdryMaxHeatingDist=%f\n", &tmpFloat); m_bdryMaxHeatingDist = tmpFloat;
		}
		else if (strncmp(buf, "m_bdryTemperature", strlen("m_bdryTemperature")) == 0)
		{
			sscanf(buf, "m_bdryTemperature=%f\n", &tmpFloat); m_bdryTemperature = tmpFloat;
		}
		else if (strncmp(buf, "m_minShearModulus", strlen("m_minShearModulus")) == 0)
		{
			sscanf(buf, "m_minShearModulus=%f\n", &tmpFloat); m_minShearModulus = tmpFloat;
		}
		else if (strncmp(buf, "m_maxShearModulus", strlen("m_maxShearModulus")) == 0)
		{
			sscanf(buf, "m_maxShearModulus=%f\n", &tmpFloat); m_maxShearModulus = tmpFloat;
		}
		else if (strncmp(buf, "m_eETptrCtrl", strlen("m_eETptrCtrl")) == 0)
		{
			sscanf(buf, "m_eETptrCtrl=%d\n", &tmpInt); m_eETptrCtrl = (ETptrCtrl)tmpInt;
		}
		else if (strncmp(buf, "m_minEdgeLenRatio", strlen("m_minEdgeLenRatio")) == 0)
		{
			sscanf(buf, "m_minEdgeLenRatio=%f\n", &tmpFloat); m_minEdgeLenRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_maxEdgeLenRatio", strlen("m_maxEdgeLenRatio")) == 0)
		{
			sscanf(buf, "m_maxEdgeLenRatio=%f\n", &tmpFloat); m_maxEdgeLenRatio = tmpFloat;
		}
		else if (strncmp(buf, "NOT_COUNT_FIXED_PRTLS", strlen("NOT_COUNT_FIXED_PRTLS")) == 0)
		{
			sscanf(buf, "NOT_COUNT_FIXED_PRTLS=%d\n", &tmpInt); NOT_COUNT_FIXED_PRTLS = tmpInt;
		}
		else if (strncmp(buf, "m_isoDensityRatio", strlen("m_isoDensityRatio")) == 0)
		{
			sscanf(buf, "m_isoDensityRatio=%f\n", &tmpFloat); m_isoDensityRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_isoDenRadiusRatio", strlen("m_isoDenRadiusRatio")) == 0)
		{
			sscanf(buf, "m_isoDenRadiusRatio=%f\n", &tmpFloat); m_isoDenRadiusRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_meshSizeOverPrtlDist", strlen("m_meshSizeOverPrtlDist")) == 0)
		{
			sscanf(buf, "m_meshSizeOverPrtlDist=%f\n", &tmpFloat); m_meshSizeOverPrtlDist = tmpFloat;
		}
		else if (strncmp(buf, "m_dbFrameIntvl", strlen("m_dbFrameIntvl")) == 0)
		{
			sscanf(buf, "m_dbFrameIntvl=%f\n", &tmpFloat);
			m_pSimuMonitor->m_dbFrameIntvl = tmpFloat;
		}
		else if (strncmp(buf, END_OF_GLOBAL_PARAS, strlen(END_OF_GLOBAL_PARAS)) == 0)
			return;
	}
}

void CSimuManager::ExportGlobalParameters(FILE* fp)
{
	fprintf(fp, "%s\n", START_OF_GLOBAL_PARAS);

	fprintf(fp, "m_eFluidClass=%d\n",				m_eFluidClass);
	// modeling options
	fprintf(fp, "m_numFramesPerSecond=%d\n",		m_numFramesPerSecond);
	fprintf(fp, "m_numStepDivPerFrame=%d\n",		m_numStepDivPerFrame);
	fprintf(fp, "m_onlyGravityTime=%d\n",			m_onlyGravityTime);
	fprintf(fp, "m_onlyBdryMovingTime=%d\n",		m_onlyBdryMovingTime);
	fprintf(fp, "APPLY_GRAVITY=%d\n",				APPLY_GRAVITY);
	fprintf(fp, "GRAVITY=(%f, %f, %f)\n",			GRAVITY->v[X], GRAVITY->v[Y], GRAVITY->v[Z]);
	fprintf(fp, "APPLY_SURFACE_TENSION=%d\n",		APPLY_SURFACE_TENSION);
	fprintf(fp, "m_surfaceTension=%f\n",			m_surfaceTension);
	fprintf(fp, "ADAPTIVE_TIME_STEP=%d\n",			ADAPTIVE_TIME_STEP);
	fprintf(fp, "ADJUST_VEL_WITH_NGBRS=%d\n",		ADJUST_VEL_WITH_NGBRS);
	fprintf(fp, "ADJUST_TENSOR_WITH_NGBRS=%d\n",	ADJUST_TENSOR_WITH_NGBRS);
	fprintf(fp, "DUMP_STRESS_TENSOR=%d\n",			DUMP_STRESS_TENSOR);
	fprintf(fp, "m_eFluidModel=%d\n",				m_eFluidModel);
	fprintf(fp, "m_eCompressibility=%d\n",			m_eCompressibility);
	fprintf(fp, "m_maxPoissonPauseSteps=%d\n",		m_maxPoissonPauseSteps);
	fprintf(fp, "m_airPressure=%f\n",				m_airPressure);
	fprintf(fp, "m_bConvexHull4SrfcDetect=%d\n",	m_bConvexHull4SrfcDetect);
	fprintf(fp, "m_bLocalFluidOrgDensity=%d\n",		m_bLocalFluidOrgDensity);
	fprintf(fp, "m_eLaplacian=%d\n",				m_eLaplacian);
	fprintf(fp, "NON_NEWTONIAN_FLUID=%d\n",			NON_NEWTONIAN_FLUID);
	fprintf(fp, "m_eRotationDerivative=%d\n",		m_eRotationDerivative);
	fprintf(fp, "m_rotationFactor=%f\n",			m_rotationFactor);
	fprintf(fp, "VISCOSITY=%d\n",					VISCOSITY);
	fprintf(fp, "m_viscosity=%f\n",					m_viscosity);
	fprintf(fp, "IMPLICIT_STRESS_INTEGRATION=%d\n",	IMPLICIT_STRESS_INTEGRATION);
	fprintf(fp, "IMPLICIT_TENSOR_INTEGRATION=%d\n",	IMPLICIT_TENSOR_INTEGRATION);
	fprintf(fp, "m_numTensorSubIntegraions=%d\n",	m_numTensorSubIntegraions);
	fprintf(fp, "m_eInterFluidModel=%d\n",			m_eInterFluidModel);
	fprintf(fp, "m_bRepulsiveResponseOnly=%d\n",	m_bRepulsiveResponseOnly);
	fprintf(fp, "m_bStickyInterFluids=%d\n",		m_bStickyInterFluids);
	fprintf(fp, "m_interFluidStickyness=%f\n",		m_interFluidStickyness);
	fprintf(fp, "m_interFluidDamping=%f\n",			m_interFluidDamping);
	fprintf(fp, "m_interFluidTension=%f\n",			m_interFluidTension);
	fprintf(fp, "UNIFORM_FIXED_PARTICLE_MASS=%d\n",	UNIFORM_FIXED_PARTICLE_MASS);

	fprintf(fp, "m_prtlMass=%f\n",					m_prtlMass);
	fprintf(fp, "m_prtlDensity=%f\n",				m_prtlDensity);
	fprintf(fp, "m_prtlDistance=%f\n",				m_prtlDistance);
	fprintf(fp, "m_minPrtlDistance=%f\n",			m_minPrtlDistance);
	fprintf(fp, "m_prtlPosJitter=%f\n",				m_prtlPosJitter);
	fprintf(fp, "m_interFluidDumping=%f\n",			m_interFluidDumping);
	fprintf(fp, "m_interFluidFriction=%f\n",		m_interFluidFriction);
	fprintf(fp, "m_distDumpingRate=%f\n",			m_distDumpingRate);
	fprintf(fp, "m_energyDumpingRate=%f\n",			m_energyDumpingRate);
	fprintf(fp, "m_frictionRate=%f\n",				m_frictionRate);
	fprintf(fp, "m_stressDumpingRate=%f\n",			m_stressDumpingRate);
	fprintf(fp, "m_ballDumpingRate=%f\n",			m_ballDumpingRate);
	fprintf(fp, "m_soundSpeed=%f\n",				m_soundSpeed);
	fprintf(fp, "m_velocityTuner=%f\n",				m_velocityTuner);
	fprintf(fp, "m_interFluidVelTuner=%f\n",		m_interFluidVelTuner);
	fprintf(fp, "m_courantFactor=%f\n",				m_courantFactor);
	fprintf(fp, "m_smoothLengthRatio=%f\n",			m_smoothLengthRatio);
	fprintf(fp, "m_srfcDenThreshold=%f\n",			m_srfcDenThreshold);
	fprintf(fp, "m_srfcWSThreshold=%f\n",			m_srfcWSThreshold);
	fprintf(fp, "m_srfcNgbrNumThreshold=%d\n",		m_srfcNgbrNumThreshold);
	fprintf(fp, "m_interSrfcNgbrNumThreshold=%d\n",	m_interSrfcNgbrNumThreshold);
	fprintf(fp, "m_minNumNgbrForUpsample=%d\n",		m_minNumNgbrForUpsample);
	fprintf(fp, "m_CGSolverTolerance=%f\n",			m_CGSolverTolerance);
	fprintf(fp, "m_lowerPrtlMassLimitRatio=%f\n",	m_lowerPrtlMassLimitRatio);
	fprintf(fp, "m_upperPrtlMassLimitRatio=%f\n",	m_upperPrtlMassLimitRatio);
	fprintf(fp, "m_downSampleDistRatio=%f\n",		m_downSampleDistRatio);
	fprintf(fp, "m_intolerablePrtlDist=%f\n",		m_intolerablePrtlDist);
	fprintf(fp, "m_srfcThicknessRatio=%f\n",		m_srfcThicknessRatio);
	fprintf(fp, "m_upsampleMinRadiusRatio=%f\n",	m_upsampleMinRadiusRatio);
	fprintf(fp, "m_upsampleMaxRadiusRatio=%f\n",	m_upsampleMaxRadiusRatio);
	fprintf(fp, "SAME_DOWN_UP_SAMPLE_RATE=%d\n",	SAME_DOWN_UP_SAMPLE_RATE);
	fprintf(fp, "INTERLEAVE_DOWN_UP_SAMPLE=%d\n",	INTERLEAVE_DOWN_UP_SAMPLE);
	fprintf(fp, "NOT_MERGE_COLLIDING_PRTLS=%d\n",	NOT_MERGE_COLLIDING_PRTLS);
	fprintf(fp, "m_maxSrfcDetectPauseSteps=%d\n",	m_maxSrfcDetectPauseSteps);
	fprintf(fp, "m_maxDownsamplePauseSteps=%d\n",	m_maxDownsamplePauseSteps);
	fprintf(fp, "m_maxUpsamplePauseSteps=%d\n",		m_maxUpsamplePauseSteps);
	fprintf(fp, "m_framesPerResample=%d\n",		m_framesPerResample);
	fprintf(fp, "CONSERVE_KINETIC_ENERGY=%d\n",		CONSERVE_KINETIC_ENERGY);
	// non-newtonian fluid parameters
	fprintf(fp, "m_shearModulus=%f\n",				m_shearModulus);
	fprintf(fp, "m_relaxationTime=%f\n",			m_relaxationTime);
	fprintf(fp, "m_stressTensorTuner=%f\n",			m_stressTensorTuner);
	fprintf(fp, "m_plasticYieldValue=%f\n",			m_plasticYieldValue);
	// thermal model parameters
	fprintf(fp, "HEAT_TRANSFER=%d\n",				HEAT_TRANSFER);
	fprintf(fp, "m_subThermalSteps=%d\n",			m_subThermalSteps);
	fprintf(fp, "m_heatConductivity=%f\n",			m_heatConductivity);
	fprintf(fp, "m_minTemperature=%f\n",			m_minTemperature);
	fprintf(fp, "m_maxTemperature=%f\n",			m_maxTemperature);
	fprintf(fp, "m_prtlIniTemperature=%f\n",		m_prtlIniTemperature);
	fprintf(fp, "m_bdryMaxHeatingDist=%f\n",		m_bdryMaxHeatingDist);
	fprintf(fp, "m_bdryTemperature=%f\n",			m_bdryTemperature);
	fprintf(fp, "m_minShearModulus=%f\n",			m_minShearModulus);
	fprintf(fp, "m_maxShearModulus=%f\n",			m_maxShearModulus);
	fprintf(fp, "m_eETptrCtrl=%d\n",				m_eETptrCtrl);
	// edge length threshold ratio
	fprintf(fp, "m_minEdgeLenRatio=%f\n",			m_minEdgeLenRatio);
	fprintf(fp, "m_maxEdgeLenRatio=%f\n",			m_maxEdgeLenRatio);

	// marching cube options
	fprintf(fp, "NOT_COUNT_FIXED_PRTLS=%d\n",		NOT_COUNT_FIXED_PRTLS);
	fprintf(fp, "m_isoDensityRatio=%f\n",			m_isoDensityRatio);
	fprintf(fp, "m_isoDenRadiusRatio=%f\n",			m_isoDenRadiusRatio);
	fprintf(fp, "m_meshSizeOverPrtlDist=%f\n",		m_meshSizeOverPrtlDist);

	fprintf(fp, "m_dbFrameIntvl=%f\n",				m_pSimuMonitor->m_dbFrameIntvl);

	fprintf(fp, "%s\n", END_OF_GLOBAL_PARAS);
}

void CSimuManager::ImportFluidData()
{
	char location[] = "CSimuManager::ImportFluidData()";

	switch (CSimuManager::m_eFluidClass)
	{
		case PrtlFluid:
			if (m_curPrtlFluid != NULL)
			{
				FILE* fp;
				// make the fluid info file
				CString fileName = CDataExImport::MakePOVRayDataFileName(m_strImportDir,
													m_nCurFrame, m_curPrtlFluid->m_deiName);
				// Open the fluid info file
				if ((fp=fopen(fileName, "r")) == NULL)
				{
					CSimuManager::IMPORT_FLUID_DATA = false;
					return;
				}
				else
				{
					char tmpStr[LEN_DATA_LINE];
					float tmp, x, y, z;
					fgets(tmpStr, LEN_DATA_LINE, fp);
					fscanf(fp, "\n\n");
					fscanf(fp, "#declare My_Focus_Position = <%f, %f, %f>;\n", &x, &y, &z);
					fscanf(fp, "#declare My_Camera_Position = <%f, %f, %f>;\n", &x, &y, &z);
					fscanf(fp, "\n");
					m_curPrtlFluid->ImportPrtlFluidData(m_strImportDir, m_nCurFrame, fp);
					fclose(fp);
				}
				break;
			}
			
			// make povray file name
			FILE* POVRayFp;
			CString fileName = CDataExImport::MakePOVRaySceneFileName(m_strImportDir,
																		m_nCurFrame);
			// Open povray file
			if ((POVRayFp=fopen(fileName, "r")) == NULL)
			{
				CSimuManager::IMPORT_FLUID_DATA = false;
				return;
			}
			// skip simulation statistics
			char tmpStr[LEN_DATA_LINE];
			float tmp, x, y, z;
			fgets(tmpStr, LEN_DATA_LINE, POVRayFp);
			fscanf(POVRayFp, "\n\n");
			fscanf(POVRayFp, "#declare My_Focus_Position = <%f, %f, %f>;\n", &x, &y, &z);
			fscanf(POVRayFp, "#declare My_Camera_Position = <%f, %f, %f>;\n", &x, &y, &z);
			fscanf(POVRayFp, "\n");
			// import boundary data
			for (int i=0; i < m_aryBoundaries.m_paNum; i++)
			{
				fscanf(POVRayFp, "\n");
				m_aryBoundaries[i]->ImportBoundaryData(POVRayFp);
			}
			fscanf(POVRayFp, "\n");
			fscanf(POVRayFp, "%s", tmpStr);
			// skip inclusions of prtl data file names
			for (int i=0; i < m_aryPrtlFluid.m_paNum; i++)
			{
				fscanf(POVRayFp, "%s", tmpStr);
				fscanf(POVRayFp, "%s", tmpStr);
				fscanf(POVRayFp, "%s", tmpStr);
				fscanf(POVRayFp, "%s", tmpStr);
				fscanf(POVRayFp, "%s", tmpStr);
				fscanf(POVRayFp, "%s", tmpStr);
				fscanf(POVRayFp, "%s", tmpStr);
			}
			fclose(POVRayFp);
			// for prtl fluids
			for (int i=0; i < m_aryPrtlFluid.m_paNum; i++)
			{
				CPrtlFluid* pFluid = m_aryPrtlFluid[i];
				pFluid->ImportPrtlFluidData(m_strImportDir, m_nCurFrame);
			}
			break;
	};

	m_nCurFrame += m_nImportFrameInterval;
}

void CSimuManager::ExportFluidData()
{
	char location[] = "CSimuManager::ExportFluidData()";

	if (IsTimeToExport())
	{
		m_fPreExportTime = m_pSimuMonitor->m_dbAnimationClock;

		// exported directory should have been made after calling MakeExportDirectory();

		CString envInfo = "";
		char tmpStr[LEN_DATA_LINE];
		sprintf(tmpStr, "/* average times: interact=%f s/f, motion=%f s/f, downsample=%f s/f, upsample=%f s/f, and total=%f s/f. */\n\n",
						m_dbTotalInteractTime/m_nCurFrame,
						m_dbTotalMotionTime/m_nCurFrame,
						((SimuValue)m_dbTotalDownsampleClock)/(CLOCKS_PER_SEC*m_nCurFrame),
						((SimuValue)m_dbTotalUpsampleClock)/(CLOCKS_PER_SEC*m_nCurFrame),
						m_dbTotalRunningTime/m_nCurFrame);
		envInfo += tmpStr;
		sprintf(tmpStr, "#declare My_Focus_Position = <%f, %f, %f>;\n",
						m_pSimuView->m_pFocusPosition->v[X],
						m_pSimuView->m_pFocusPosition->v[Y],
						m_pSimuView->m_pFocusPosition->v[Z]);
		envInfo += tmpStr;
		sprintf(tmpStr, "#declare My_Camera_Position = <%f, %f, %f>;\n",
						m_pSimuView->m_pEyePosition->v[X],
						m_pSimuView->m_pEyePosition->v[Y],
						m_pSimuView->m_pEyePosition->v[Z]);
		envInfo += tmpStr;
		envInfo += "\n";

		switch (CSimuManager::m_eFluidClass)
		{
			case PrtlFluid:
				if (m_curPrtlFluid != NULL)
				{
					FILE* fp;
					// make the fluid info file
					CString fileName = CDataExImport::MakePOVRayDataFileName(m_strExportDir,
													m_nCurFrame, m_curPrtlFluid->m_deiName);
					// Open the fluid info file
					if ((fp=fopen(fileName, "w")) == NULL)
					{
						sprintf(CSimuMsg::GetEmptyBuf(), "Can't open fluid info file %s to write.", fileName);
						CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
						return;
					}
					fprintf(fp, "%s", envInfo);
					m_curPrtlFluid->ExportPrtlFluidData(m_strExportDir, m_nCurFrame, fp);
					fclose(fp);
					break;
				}
				
				// make povray file name
				FILE* POVRayFp;
				CString fileName = CDataExImport::MakePOVRaySceneFileName(m_strExportDir,
																			m_nCurFrame);
				// Open povray file
				if ((POVRayFp=fopen(fileName, "w")) == NULL)
				{
					sprintf(CSimuMsg::GetEmptyBuf(), "Can't open file %s to write.", fileName);
					CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
					return;
				}
				// export simulation statistics
				fprintf(POVRayFp, "%s", envInfo);
				// export boundary data
				for (int i=0; i < m_aryBoundaries.m_paNum; i++)
				{
					fprintf(POVRayFp, "\n");
					m_aryBoundaries[i]->ExportBoundaryData(POVRayFp);
				}
				fprintf(POVRayFp, "\n");
				fprintf(POVRayFp, "#include \"Fluid_Animation_Env.pov\"\n");
				// export inclusions of prtl data file names
				for (int i=0; i < m_aryPrtlFluid.m_paNum; i++)
				{
					CPrtlFluid* pFluid = m_aryPrtlFluid[i];

					fprintf(POVRayFp, "\n");
					fprintf(POVRayFp, "#if (display_particles_%d)\n", pFluid->m_pfFluidId);
					CString fileNamePrtlSpheres = pFluid->MakePrtlDataFileName(m_nCurFrame);
					fprintf(POVRayFp, "\t#include \"%s\"\n", fileNamePrtlSpheres);
					fprintf(POVRayFp, "#else\n");
					CString fileNameSurfaceMesh = pFluid->MakeSurfaceMeshDataFileName(m_nCurFrame);
					fprintf(POVRayFp, "\t#include \"%s\"\n", fileNameSurfaceMesh);
					fprintf(POVRayFp, "\tobject { %s translate Translate_%s material { My_Fluid_Material_%d } }\n", pFluid->m_deiName, pFluid->m_deiName, pFluid->m_pfFluidId);
					fprintf(POVRayFp, "#end\n");
				}
				fclose(POVRayFp);
				// for prtl fluids
				for (int i=0; i < m_aryPrtlFluid.m_paNum; i++)
				{
					CPrtlFluid* pFluid = m_aryPrtlFluid[i];
					pFluid->ExportPrtlFluidData(m_strExportDir, m_nCurFrame);
				}
				break;
		};
		m_nCurFrame++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSimuManager diagnostics

#ifdef _DEBUG
void CSimuManager::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CSimuManager::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSimuManager message handlers
BOOL CSimuManager::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

int CSimuManager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ReadSimulationParameters();
	InitManagerParameters();

	// create a view to occupy the client area of the frame
	if (!m_pSimuView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

//	Intend to delete this piece of code, but it may be needed sometime later.
//	if (!m_wndStatusBar.Create(this) ||
//		!m_wndStatusBar.SetIndicators(indicators,
//		  sizeof(indicators)/sizeof(UINT)))
//	{
//		TRACE0("Failed to create status bar\n");
//		return -1;      // fail to create
//	}

	if (!m_pSimuMonitor->Create(CSimuMonitor::IDD, NULL))
	{
		TRACE0("Failed to create monitor dialog\n");
		return -1;
	}

	if (!m_dlgOptionDisplay->Create(CDlgOptionDisplay::IDD, NULL))
	{
		TRACE0("Failed to create display option dialog\n");
		return -1;
	}

	if (!m_dlgOptionSurface->Create(CDlgOptionSurface::IDD, NULL))
	{
		TRACE0("Failed to create surface option dialog\n");
		return -1;
	}

	if (!m_dlgOptionModeling->Create(CDlgOptionModeling::IDD, NULL))
	{
		TRACE0("Failed to create modeling option dialog\n");
		return -1;
	}

	if (!m_dlgOptionImExport->Create(CDlgOptionImExport::IDD, NULL))
	{
		TRACE0("Failed to create import/export option dialog\n");
		return -1;
	}

	if (!m_dlgOptionParticleSelect->Create(CDlgOptionParticleSelect::IDD, NULL))
	{
		TRACE0("Failed to create particle select option dialog\n");
		return -1;
	}
	
	if (!m_dlgOptionMeshTexture->Create(CDlgOptionMeshTexture::IDD, NULL))
	{
		TRACE0("Failed to create mesh texture option dialog\n");
		return -1;
	}

	if (!m_dlgOptionMesher->Create(CDlgOptionMesher::IDD, NULL))
	{
		TRACE0("Failed to create mesher option dialog\n");
		return -1;
	}

	if (!m_dlgOptionViewing->Create(CDlgOptionViewing::IDD, NULL))
	{
		TRACE0("Failed to create viewing option dialog\n");
		return -1;
	}

	if (!m_dlgOptionElementSelect->Create(CDlgOptionElementSelect::IDD, NULL))
	{
		TRACE0("Failed to create element select option dialog\n");
		return -1;
	}

	if (!m_dlgFileMoveCopy->Create(CDlgFileMoveCopy::IDD, NULL))
	{
		TRACE0("Failed to create batch move/copy file dialog\n");
		return -1;
	}

	InitializeOptions();

	OnViewMonitor();

	RECT rectMontrDlg, rectWorkArea;
	m_pSimuMonitor->GetWindowRect(&rectMontrDlg);
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWorkArea, 0);
	int width = rectWorkArea.right-(rectMontrDlg.right-rectMontrDlg.left);
	int height = rectWorkArea.bottom;
	// 30 is the height of menu bar and title
	// deduct the preserved height first
	height -= 30;
	// put the rendering window as big as possible but in square
	if (width > height)
	{
		width = height;
	}
	else
	{
		height = width;
	}
	// restore the preserved height
	height += 30;
	MoveWindow(0, 0, width, height);

	return 0;
}

void CSimuManager::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_pSimuView->SetFocus();
}

BOOL CSimuManager::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_pSimuView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// let the modeler have second crack at the command
	if (m_pSimuModeler->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CSimuManager::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CFrameWnd::OnTimer(nIDEvent);
	if (m_bRunningTimerEvent) return;	// prevent multiple executions of this function
	m_bRunningTimerEvent = true;

	if (KillTimer(SIMU_TIMER_ID) == 0)
	{
		CSimuMsg::ExitWithMessage("CSimuManager::OnTimer(UINT nIDEvent)",
									"Can not kill timer.");
		return;
	}
	m_pSimuMonitor->UpdateData();
	if (m_pSimuMonitor->m_bGo || m_pSimuMonitor->m_uFwdIntvls-- > 0)
	{
		if (IMPORT_FLUID_DATA)
		{
			ImportFluidData();
			if (!IMPORT_FLUID_DATA)
			{
				CREATE_SURFACE_MESH = false;
				EXPORT_FLUID_DATA = false;
				ResetAnimation();
			}
		}
		else
		{
			if (EXPORT_FLUID_DATA && m_bNeedExportDirectory)
			{
				m_bNeedExportDirectory = false;
				MakeExportDirectory(); // do this first because some information may need export
				ExportFluidParameters();
				if (IM_EXPORT_FLUID_SURFACE
				 && (m_eFluidModel == UPSAMPLE || m_eFluidModel == SPH))
				{
					CREATE_SURFACE_MESH = true;
					InitializeOptions();
				}
			}
			m_pSimuDriver->RunAnimationOneTimeStep();

			if (EXPORT_FLUID_DATA)
				ExportFluidData();

			m_pSimuMonitor->m_dbAnimationClock += m_pSimuMonitor->m_dbTimeStep;
		}
		m_pSimuMonitor->ShowSimulationInformation();
		m_pSimuMonitor->UpdateData(false);

		if (UPDATE_ENTIRE_DISPLAY)
			m_pSimuView->Invalidate(false);

		SetTimer(SIMU_TIMER_ID, 1, NULL); // instant timer
	}
	m_bRunningTimerEvent = false;
}

void CSimuManager::OnFileOutputbodydatato() 
{
	// TODO: Add your command handler code here
	
	CFileDialog* fileDlg = new CFileDialog(true);
	fileDlg->m_ofn.lpstrInitialDir	= m_strAppDataDir;
	fileDlg->m_ofn.lpstrFilter		= "*.inc";
	fileDlg->m_ofn.lpstrFile = m_strSimuEnvPOVFile.GetBuffer(LEN_DIR_NAME+LEN_FILE_NAME);
	if (fileDlg->DoModal() == IDOK)
	{
		m_strSimuEnvPOVFile.ReleaseBuffer();
		m_strSimuEnvPOVFile = fileDlg->GetFileName();
	}
	delete fileDlg;
}

void CSimuManager::OnViewMonitor() 
{
	// TODO: Add your command handler code here
	
	RECT rectMontrDlg, rectWorkArea;
	m_pSimuMonitor->GetWindowRect(&rectMontrDlg);
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWorkArea, 0);
	// put the monitor dialog at the upper-right of the screen
	m_pSimuMonitor->SetWindowPos(NULL,
								 rectWorkArea.right-(rectMontrDlg.right-rectMontrDlg.left),
								 0, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	m_pSimuMonitor->ShowWindow(SW_SHOW);
}

void CSimuManager::OnOptionsDisplay() 
{
	// TODO: Add your command handler code here
	
	m_dlgOptionDisplay->InitializeDisplayOptions();
	m_dlgOptionDisplay->UpdateData(false);
	m_dlgOptionDisplay->ShowWindow(SW_SHOW);
}

void CSimuManager::OnOptionsSurface() 
{
	// TODO: Add your command handler code here

	m_dlgOptionSurface->InitializeSurfaceOptions();
	m_dlgOptionSurface->UpdateData(false);
	m_dlgOptionSurface->ShowWindow(SW_SHOW);
}

void CSimuManager::OnOptionsModeling() 
{
	// TODO: Add your command handler code here

	m_dlgOptionModeling->InitializeModelingOptions();
	m_dlgOptionModeling->UpdateData(false);
	m_dlgOptionModeling->ShowWindow(SW_SHOW);
}

void CSimuManager::OnOptionsImportexport() 
{
	// TODO: Add your command handler code here

	m_dlgOptionImExport->InitializeImExportOptions();
	m_dlgOptionImExport->UpdateData(false);
	m_dlgOptionImExport->ShowWindow(SW_SHOW);
}

void CSimuManager::OnOptionsParticleselect() 
{
	// TODO: Add your command handler code here
	
	m_dlgOptionParticleSelect->InitializeParticleSelectOptions();
	m_dlgOptionParticleSelect->UpdateData(false);
	m_dlgOptionParticleSelect->ShowWindow(SW_SHOW);
}

void CSimuManager::OnOptionsMeshtexture() 
{
	// TODO: Add your command handler code here
	
	m_dlgOptionMeshTexture->InitializeMeshTextureOptions();
	m_dlgOptionMeshTexture->UpdateData(false);
	m_dlgOptionMeshTexture->ShowWindow(SW_SHOW);
}

void CSimuManager::OnOptionsMesher() 
{
	// TODO: Add your command handler code here

	m_dlgOptionMesher->InitializeMesherOptions();
	m_dlgOptionMesher->UpdateData(false);
	m_dlgOptionMesher->ShowWindow(SW_SHOW);
}

void CSimuManager::OnOptionsViewing() 
{
	// TODO: Add your command handler code here
	
	m_dlgOptionViewing->InitializeViewingOptions();
	m_dlgOptionViewing->UpdateData(false);
	m_dlgOptionViewing->ShowWindow(SW_SHOW);
}

void CSimuManager::OnOptionsElementselect() 
{
	// TODO: Add your command handler code here

	m_dlgOptionElementSelect->InitializeElementSelectOptions();
	m_dlgOptionElementSelect->UpdateData(false);
	m_dlgOptionElementSelect->ShowWindow(SW_SHOW);
}

void CSimuManager::OnFileBatchmovecopyfiles() 
{
	// TODO: Add your command handler code here

	m_dlgFileMoveCopy->ShowWindow(SW_SHOW);
}
