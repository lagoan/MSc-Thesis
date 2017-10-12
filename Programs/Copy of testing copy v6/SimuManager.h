// SimuManager.h : interface of the CSimuManager class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUMANAGER_H__993A482C_CB30_11D5_9B85_0050BADE013B__INCLUDED_)
#define AFX_SIMUMANAGER_H__993A482C_CB30_11D5_9B85_0050BADE013B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuView.h"
#include "SimuModeler.h"
#include "SimuMonitor.h"
#include "SimuDriver.h"
#include "DlgOptionDisplay.h"
#include "DlgOptionSurface.h"
#include "DlgOptionModeling.h"
#include "DlgOptionImExport.h"
#include "DlgOptionParticleSelect.h"
#include "DlgOptionMeshTexture.h"
#include "DlgOptionMesher.h"
#include "DlgOptionViewing.h"
#include "DlgOptionElementSelect.h"
#include "DlgFileMoveCopy.h"
#include "SimuEnclosingBall.h"

class CSimuManager : public CFrameWnd
{
public:
	// temp debug variables
	static bool m_bSpikyGradient;

	// current fluid modeling classes
	enum EFluidClass
	{
		PrtlFluid,
	};
	static EFluidClass m_eFluidClass;

	// modeling options
	static int m_numFramesPerSecond;
	static int m_numStepDivPerFrame;
	static int m_maxPoissonPauseSteps;
	static int m_numTensorSubIntegraions;
	static int m_maxSrfcDetectPauseSteps;
	static int m_maxDownsamplePauseSteps;
	static int m_maxUpsamplePauseSteps;
	static SimuValue m_framesPerResample;
	static SimuValue m_onlyGravityTime;
	static SimuValue m_onlyBdryMovingTime;
	static bool		APPLY_GRAVITY;
	static bool		APPLY_SURFACE_TENSION;
	static SimuValue m_surfaceTension;
	static bool		ADAPTIVE_TIME_STEP;
	static bool		ADJUST_VEL_WITH_NGBRS;
	static bool		ADJUST_TENSOR_WITH_NGBRS;
	static bool		IMPLICIT_STRESS_INTEGRATION;
	static bool		IMPLICIT_TENSOR_INTEGRATION;
	static bool		DUMP_STRESS_TENSOR;
	enum EFluidModel
	{
		SPH,
		REMESH,
		RESAMPLE,
		REDISTRIBUTE,
		UPSAMPLE,
		SPRING
	};
	static EFluidModel m_eFluidModel;
	enum ECompressibility
	{
		NO_PRESSURE,
		GAS_STATE,
		POISSON
	};
	static ECompressibility	m_eCompressibility;
	static SimuValue m_airPressure;
	static bool m_bConvexHull4SrfcDetect;
	static bool m_bLocalFluidOrgDensity;
	enum ELaplacian
	{
		VELOCITY_DIVERGENCE,
		DENSITY_VARIATION
	};
	static ELaplacian m_eLaplacian;
	static bool		NON_NEWTONIAN_FLUID;
	enum EStressModel
	{
		PLASTIC,
		VISCOELASTIC,
		LINEAR
	};
	static EStressModel m_eStressModel;
	static ERotation m_eRotationDerivative;
	static SimuValue m_rotationFactor;
	static bool		VISCOSITY;
	static SimuValue m_viscosity;

	enum EInterFluidModel
	{
		REPULSIVE_FORCE,
		DETECT_BY_ISO_SRFC,
		DETECT_BY_CONVEX_HULL
	};
	static EInterFluidModel m_eInterFluidModel;
	static bool		m_bRepulsiveResponseOnly;
	static bool		m_bStickyInterFluids;
	static SimuValue m_interFluidStickyness;
	static SimuValue m_interFluidDamping;
	static SimuValue m_interFluidTension;

	static bool		UNIFORM_FIXED_PARTICLE_MASS;
	static SimuValue m_prtlMass;
	static SimuValue m_prtlDensity;
	static SimuValue m_prtlDistance;
	static SimuValue m_minPrtlDistance;
	static SimuValue m_prtlPosJitter;
	static SimuValue m_interFluidDumping;	// range: 0 to 1
	static SimuValue m_interFluidFriction;	// range: 0 to 1
	static SimuValue m_distDumpingRate;		// range: 0 to 1
	static SimuValue m_energyDumpingRate;	// range: 0 to 1
	static SimuValue m_frictionRate;		// range: 0 to 1
	static SimuValue m_stressDumpingRate;	// range: 0 to 1
	static SimuValue m_ballDumpingRate;		// range: 0 to 1
	static SimuValue m_soundSpeed;
	static SimuValue m_velocityTuner;
	static SimuValue m_interFluidVelTuner;
	static SimuValue m_courantFactor;
	static SimuValue m_smoothLengthRatio;
	static SimuValue m_srfcDenThreshold;
	static SimuValue m_srfcWSThreshold;
	static int m_srfcNgbrNumThreshold;
	static int m_interSrfcNgbrNumThreshold;
	static int m_minNumNgbrForUpsample;
	static SimuValue m_springConstant;
	static SimuValue m_CGSolverTolerance;
	static SimuValue m_lowerPrtlMassLimitRatio;
	static SimuValue m_upperPrtlMassLimitRatio;
	static SimuValue m_downSampleDistRatio;
	static SimuValue m_intolerablePrtlDist;
	static SimuValue m_srfcThicknessRatio;
	static SimuValue m_upsampleMinRadiusRatio;
	static SimuValue m_upsampleMaxRadiusRatio;
	static bool		SAME_DOWN_UP_SAMPLE_RATE;
	static bool		INTERLEAVE_DOWN_UP_SAMPLE;
	static bool		NOT_MERGE_COLLIDING_PRTLS;
	static bool		CONSERVE_KINETIC_ENERGY;
	// non-newtonian fluid parameters
	static SimuValue m_shearModulus;
	static SimuValue m_relaxationTime;
	static SimuValue m_plasticYieldValue;
	static SimuValue m_stressTensorTuner;
	// thermal model parameters
	static bool		HEAT_TRANSFER;
	static bool		NOT_CONSERVE_HEAT;
	static bool		OVERWRITE_TEXTURE;
	static int m_subThermalSteps;
	static SimuValue m_heatConductivity;
	static SimuValue m_minTemperature;
	static SimuValue m_maxTemperature;
	static SimuValue m_prtlIniTemperature;
	static SimuValue m_bdryMaxHeatingDist;
	static SimuValue m_bdryTemperature;
	static SimuValue m_minShearModulus;
	static SimuValue m_maxShearModulus;
	enum ETptrCtrl
	{
		LINEAR_MELTING_0
	};
	static ETptrCtrl m_eETptrCtrl;
	// edge length threshold ratio
	static SimuValue m_minEdgeLenRatio;
	static SimuValue m_maxEdgeLenRatio;

	// mesher options
	enum ERemeshing
	{
		MARCHING_TETRAHEDRA,
		AGGRESSIVE_ISOMORPHISM,
		CONSERVATIVE_ISOMORPHISM
	};
	static ERemeshing m_remeshingMode;
	static bool		DEBUG_MESHER;
	static bool		DRAW_MESHER_GRID;
	static bool		DRAW_MESHER_PRTL;
	static bool		DRAW_MESHER_TETRA;
	static bool		DRAW_MESHER_SRFC_TETRA;
	static bool		DRAW_INTR_SRFC_TRIANGLES;
	static bool		DRAW_INTR_SRFC_PARTICLES;
	static bool		DRAW_EXTR_SRFC_PARTICLES;
	static bool		DRAW_EXTR_SRFC_TRIANGLES;
	static bool		DRAW_INTR_SRFC_NORMALS;
	static bool		DRAW_ISOMORPHIC_LINES;

	// display options
	static bool		UPDATE_ENTIRE_DISPLAY;
	static bool		DRAW_HIGHLIT_ONLY;
	static bool		DRAW_PARTICLES;
	static bool		DRAW_PARTICLE_ID;
	static bool		COLOR_SURFACE_PARTICLES;
	static bool		DRAW_PARTICLE_DENSITIES_BY_COLOR;
	static bool		DRAW_PARTICLE_MASS_BY_COLOR;
	static bool		DRAW_PARTICLE_SPEED_BY_COLOR;
	static bool		DRAW_GRID_PARTICLES;
	static bool		DRAW_SURFACE_MESH;
	static bool		DRAW_EDGE_ONLY;
	static bool		DRAW_SRFC_PARTICLES;
	static bool		DRAW_SRFC_PRTL_NORMALS;
	static bool		DRAW_CURVATURE_NORMALS;
	static bool		DRAW_TRIANGLE_NORMALS;
	static bool		DRAW_TETRAHEDRA;
	static bool		DRAW_BOUNDARY;
	static bool		DRAW_DELAUNAY;
	static bool		DRAW_CONVEX_HULL;
	static SimuValue m_displayTuner;
	static SimuValue m_particleIdSize;
	// display variables
	static bool		DIFFER_SRFC_COLOR;
	static bool		DRAW_MC_BOUNDING_BOX;

	// viewing options
	static bool		DRAW_PARTICLES_BY_SPHERE;
	static bool		DRAW_X_Z_COORDINATES;
	static bool		ENABLE_LIGHT0;
	static bool		ENABLE_LIGHT1;
	static bool		DRAW_LIGHT0;
	static bool		DRAW_LIGHT1;
	static SimuValue m_sphereRadius;
	static int m_sphereSlices;
	static int m_sphereStacks;
	static CVector*	m_viewBoundary;

	// particle selecting options
	static CSimuEnclosingBall* m_enclosingBall;
	static bool		SHOW_SELECTED_PRTL_ONLY;
	static CVector3D* m_hilightColor;
	// element selecting options
	enum EElementType
	{
		SELECT_TETRAHEDRON,
		SELECT_TRIANGLE,
		SELECT_PARTICLE
	};
	static EElementType m_eSelectedElmntType;
	static bool		SHOW_SELECTED_ELMNT_ONLY;

	// marching cube options
	static bool		CREATE_SURFACE_MESH;
	static bool		USE_PRTL_NUM_DENSITY;
	static bool		NOT_COUNT_FIXED_PRTLS;
	static SimuValue m_isoDensityRatio; // ratio of iso-density over fluid density
	static SimuValue m_isoDenRadiusRatio;
	static SimuValue m_meshSizeOverPrtlDist;

	// mesh texture options
	static SimuValue m_ratioColorR;
	static SimuValue m_ratioColorG;
	static SimuValue m_ratioColorB;
	static SimuValue m_constColorR;
	static SimuValue m_constColorG;
	static SimuValue m_constColorB;
	static SimuValue m_ratioFilter;
	static SimuValue m_constFilter;

	// import/export options
	static CString	m_strAppDataDir;
	static CString	m_strImportDir;
	static CString	m_strImportParaFile;
	static UINT		m_nImportFrameInterval;
	// during import initialization: get import directory with a fluid body data file,
	// and create a fluid body with the data from the data file.
	// in the OnTimer(), import particle poisitions from serial data files
	// and draw the fluid body
	static bool		IMPORT_FLUID_DATA;
	static bool		EXPORT_FLUID_DATA;
	static bool		OVERWRITE_EXISTING_DATA;
	static bool		IM_EXPORT_FLUID_PARTICLES;
	static bool		IM_EXPORT_FLUID_SURFACE;

	// import/export variables
	static time_t	m_t2005_JUNE_29_5PM;
	enum IMEXPORT_FORMAT
	{
		BEFORE_2005_APR_11_6PM,
		BEFORE_2005_JUNE_29_5PM,
		AFTER_2005_JUNE_29_5PM
	};
	static enum IMEXPORT_FORMAT m_enumImExportFormat;
	static CString	m_strExportDir;
	static CString	m_strSimuInfoFile;
	static CString	m_strSimuEnvPOVFile;
	static bool		m_bNeedExportDirectory;
	// if the export mode is turned on, at the first frame, 
	// create an export directory,
	// and export fluid body data into a file in the directory
	static UINT		m_nCurFrame;
	static SimuValue m_fPreExportTime;
	// export temp debug data
	static CString m_strDataDebugFile;
	static FILE* m_dataDebugFile;
	static bool m_bWriteToDataDebugFile;

	// animation monitor variables
	static UINT		m_nCurStep;
	static SimuValue m_dbTotalRunningTime;
	static SimuValue m_dbTotalMotionTime;
	static SimuValue m_dbTotalInteractTime;
	static long m_dbTotalDownsampleClock;
	static long m_dbTotalUpsampleClock;

	// debug control variable
	static bool		m_bShowMsg;
	static bool		m_bSkipWarning;
	static bool		m_bInvalidValue;
	static SimuValue m_maxAngVelTenElmnt;

	static CVector3D*	GRAVITY;
	static CVector3D*	COLOR_WHITE;
	static CVector3D*	COLOR_RED;
	static CVector3D*	COLOR_GREEN;
	static CVector3D*	COLOR_BLUE;
	static CVector3D*	COLOR_YELLOW;
	static CVector3D*	COLOR_MAGENTA;
	static CVector3D*	COLOR_CYAN;
	static CVector3D*	COLOR_ORANGE;
	static CVector3D*	COLOR_SRFC;
	static CVector3D*	COLOR_INSIDE;
	static CVector3D*	COLOR_FIXED;
	
	CSimuManager();
protected: 
	DECLARE_DYNAMIC(CSimuManager)

// Attributes
public:
	CMapStringToString	m_cParaList;
	CSimuView*			m_pSimuView;
	CSimuModeler*		m_pSimuModeler;
	CSimuMonitor*		m_pSimuMonitor;
	CSimuDriver*		m_pSimuDriver;

	CDlgOptionDisplay*	m_dlgOptionDisplay;
	CDlgOptionSurface*	m_dlgOptionSurface;
	CDlgOptionModeling*	m_dlgOptionModeling;
	CDlgOptionImExport*	m_dlgOptionImExport;
	CDlgOptionParticleSelect*	m_dlgOptionParticleSelect;
	CDlgOptionMeshTexture*	m_dlgOptionMeshTexture;

	CDlgOptionMesher*	m_dlgOptionMesher;
	CDlgOptionViewing*	m_dlgOptionViewing;
	CDlgOptionElementSelect*	m_dlgOptionElementSelect;
	CDlgFileMoveCopy*	m_dlgFileMoveCopy;

	CPrtlFluid*			m_curPrtlFluid;

	TClassArray<CPrtlFluid> m_aryPrtlFluid;
	TClassArray<CBoundary> m_aryBoundaries;

// Operations
public:
	static time_t GetDirectoryCreationTime(CString dir);
	static time_t GetFileModificationTime(FILE* fp);
	static time_t GetSpecifiedTime(int year, int month, int day, int hour);
	static bool FirstTimeIsLaterThanSecondTime(time_t firstTime, time_t secondTime);

	static void OpenDataDebugFile();
	static void CloseDataDebugFile();
	static void ExportDebugData(char* location);

	static void SetTuner(bool bEnabled);
	static void SetTuner(bool bEnableVel, bool bEnableStress);
	static void SetTunerValue(SimuValue ratio); // imply enabled
	static void SetTunerValue(SimuValue ratioVel, SimuValue ratioStress);
	static void SetPrtlDistance(SimuValue prtlDistance);

	void TurnOnMemoryDebugFlags();
	void DisplayOperationCountingMessage(const char* elmntName,
										UINT countNum, UINT totalNum,
										UINT period=100, bool bDisabled=true);
	void InitNamedVector(CVector* vector);
	void PlaceDialogToTheRight(CDialog* pDialog);

	void RefreshParametersOnInterface();
	void DrawSimulationBodies();

	void ResetAnimation();
	void PrepareImportFluidData();
	void SearchNeighboringPrtls(CFluidPrtl* pPrtl, CPrtlFluid* curFluid,
								TGeomElemArray<CFluidPrtl> &pNgbrs);
	void CreateCurrentPrtlFluid();
	static CPrtlFluid* CreatePrtlFluid();
	static CFluidPrtl* CreateOneFluidPrtl();
	bool IsTimeToExport();

	void RefreshView();
	void InitializeOptions();
	static SimuValue ComputeTimeStep();
	void UpdateTimeStep();
	void SelectEnclosedParticles();
	void DeSelectEnclosedParticles();
	void DeSelectAllParticles();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuManager)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSimuManager();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
//	Intend to delete this variable, but it may be needed sometime later.
//	CStatusBar			m_wndStatusBar;

private:
	bool		m_bRunningTimerEvent;
	CString		m_strPOVAppName;
	CVector*	m_pMainWinRect;

	time_t		m_tNewImExportFormatTime;

	bool DelimiteNameValuePair(char* src, CString& name, CString& string);
	void ReadSimulationParameters();
	void InitManagerParameters();

	void OutputLiquidBodySurfaceIntoPOVFile();

	void MakeExportDirectory();
	void ImportFluidParameters();
	void ExportFluidParameters();
	void ImportGlobalParameters(FILE* fp);
	void ExportGlobalParameters(FILE* fp);
	void ImportFluidData();
	void ExportFluidData();
	void ImportFluidBodyPrtlPositions();
	void ExportFluidBodyPrtlPositions();

// Generated message map functions
protected:
	//{{AFX_MSG(CSimuManager)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFileOutputbodydatato();
	afx_msg void OnViewMonitor();
	afx_msg void OnSetEpsilon();
	afx_msg void OnOptionsDisplay();
	afx_msg void OnOptionsSurface();
	afx_msg void OnOptionsModeling();
	afx_msg void OnOptionsImportexport();
	afx_msg void OnOptionsParticleselect();
	afx_msg void OnOptionsMesher();
	afx_msg void OnOptionsViewing();
	afx_msg void OnOptionsElementselect();
	afx_msg void OnFileBatchmovecopyfiles();
	afx_msg void OnOptionsMeshtexture();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUMANAGER_H__993A482C_CB30_11D5_9B85_0050BADE013B__INCLUDED_)
