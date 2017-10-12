// PrtlFluid.h: interface for the CPrtlFluid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLFLUID_H__A2EC1C03_54F8_4C1B_A455_B3343360717F__INCLUDED_)
#define AFX_PRTLFLUID_H__A2EC1C03_54F8_4C1B_A455_B3343360717F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LinearTemperaTexture.h"
#include "DataExImport.h"
#include "TPrtlGrid.h"
#include "TGeomElemArray.h"
#include "FluidPrtl.h"
#include "FluidPrtlPoisson.h"
#include "FluidPrtlNonNew.h"
#include "Boundary.h"
#include "SimuFluidNozzle.h"
#include "RigidBall.h"
#include "ParticleSearch.h"
#include "SimuMarchingCube.h"
#include "SimuCGSolver3D.h"

class CSimuManager;

class CPrtlFluid  : public CDataExImport
{
public:
	static int m_pfMaxFluidId;
	int m_pfFluidId;

	CSimuManager* m_pfSimuManager;
	bool m_pfPrtlEvolutionIsPrepared; // for not forgetting call PreparePrtlEvolution() in SimuModeler.cpp
	bool m_pbApplyGravity;

	SimuValue m_pfDensity;
	SimuValue m_pfPrtlDist;
	SimuValue m_pfPrtlMass;
	SimuValue m_pfTotalMass;
	SimuValue m_pfTotalFreePrtlMass;
	SimuValue m_pfSmoothLen;
	SimuValue m_pfSmoothLenSqr;
	SimuValue m_pfTimeStep;
	SimuValue m_pfMaxTimeStep;
	SimuValue m_pfRandomSeed;
	SimuValue m_pfAvgVERatio;

	SimuValue m_pfUniWeightSum;
	SimuValue m_pfUniPrtlVolume;
	SimuValue m_pfUniPrtlNumDensity;

	// for fluid-fluid interaction
	SimuValue m_pfPrtlMassRatio;
	SimuValue m_pfInterFluidDamping;
	SimuValue m_pfInterFluidRadius; // for repulsive force
	SimuValue m_pfKEVariation;	// for debug use only

	SimuValue m_pfAirPressure;
	SimuValue m_pfShearModulus;
	SimuValue m_pfRelaxationTime;
	ERotation m_pfRotationDerivative;

	int m_pfSubThermalSteps;
	SimuValue m_pfHeatConductivity;
	SimuValue m_pfIniTemperature;
	SimuValue m_pfMinShearModulus;
	SimuValue m_pfMaxShearModulus;
	// linear melting 0
	SimuValue m_pfCoeffTemperature;
	SimuValue m_pfCoeffShearModulus;
	SimuValue m_pfConstSummation;
	// for surface texture
	CLinearTemperaTexture m_pfLinrTempTxtr;

	// variables for animation control
	SimuValue m_pfOnlyBdryMovingTime;
	SimuValue m_pfOnlyGravityTime;

	TClassArray<CBoundary> m_pfBoundaries;

	TGeomElemArray<CFluidPrtl> m_pfPrtls;
	SimuValue m_pfAvgNumNgbrs;

	int m_pfPoissonPauseSteps;
	int m_pfSrfcDetectPauseSteps;

	// variables for iso-surface
	SimuValue m_pfIsoDensituRadius;
	SimuValue m_pfIsoDensityRatio;
	// variables for marching cube
	TPrtlGrid<CFluidPrtl> m_pfMCGrid;
	CSimuMarchingCube m_pfMarchingCube;

	SimuValue m_pfInteractTime;
	SimuValue m_pfMotionTime;
	SimuValue m_pfSurfaceTime;

	CString m_pfSimuInfo;
	CVector3D m_pfColor;
	CVector3D m_pfSrfcColor;

	CPrtlFluid();
	virtual ~CPrtlFluid();

	virtual void RenderPrtlFluid();
	void RenderFluidPrtls();
	void ComputeParticleColor(CVector3D &color, CFluidPrtl* pPrtl);
	void RenderHighlitPrtls();
	void DrawBoundaries();

	void LoadPrtlFluidVariablesFrom(CPrtlFluid* anotherFluid);
	void ExportPrtlFluidVariables(FILE* fp);
	void ImportPrtlFluidVariables(FILE* fp);
	void ExportPrtlFluidData(CString exportDir, UINT nCurFrame);
	void ImportPrtlFluidData(CString importDir, UINT nCurFrame);

	void ExportPrtlFluidParameters(FILE* fp);
	void ImportPrtlFluidParameters(FILE* fp);
	void ExportPrtlFluidData(CString exportDir, UINT nCurFrame, FILE* POVRayFp);
	void ImportPrtlFluidData(CString importDir, UINT nCurFrame, FILE* POVRayFp);
	void ExportSimulationInfo(CString exportDir, const char* info);
	void ImportSimulationInfo(CString importDir, UINT nCurFrame);
	void ExportPOVRayEnvData(UINT nCurFrame, FILE* POVRayFp);
	void ImportPOVRayEnvData(FILE* POVRayFp);
	void ExportPrtlData(CString exportDir, UINT nCurFrame);
	void ImportPrtlData(CString importDir, UINT nCurFrame);
	void ExportPrtlData(FILE* fp);
	void ImportPrtlData(FILE* fp);
	void ExportPrtlsAsSpheresInPOVRay(FILE* fp);
	void ExportTwoTypePrtlsAsSpheresInPOVRay(FILE* fp);
	void ImportPrtlsAsSpheresInPOVRay(FILE* fp);
	void ImportTwoTypePrtlsAsSpheresInPOVRay(FILE* fp);
	virtual void ExportSurfaceMeshIntoPOVRayMesh2ObjectFile(CString &txFileName, FILE* fp);
	virtual void ImportSurfaceMeshFromPOVRayMesh2ObjectFile(FILE* fp);
	void ConstructFluidSurfaceMeshByMarchingCube();
	void InterpolateSurfaceMeshPointTemperatures();
	SimuValue InterpolateTemperatureAtPoint(CSimuPoint3D* pos, SimuValue radius);
	SimuValue InterpolateTemperatureAtPoint(CVector3D* pos, SimuValue radius);

	void SetTimeStep(SimuValue timeStep);

	virtual CFluidPrtl* CreateOneFluidPrtl();
	void AddBoundary(CBoundary* boundary);

	// particle search
	bool m_pfNeedToRegisterMovedPrtls;
	bool m_pfNeedToRefreshPrtlNeighbors;
	CParticleSearch m_pfPrtlSearch;
	void RegisterPrtlsForNeighborSearchIfNotYet();
	void SearchPrtlNeighbors();
	void ConfirmSymmetricPrtlNeighbors();

	void JitterPrtlPositions(SimuValue maxJitterDist);
	void ComputeUniformWeightSum();
	void InitializePrtlDensities(SimuValue prtlDensity);
	void InitializePrtlVelocities(CVector3D &velocity);
	void InitializeSurfacePrtlPressures();
	void ApplyFluidColorOntoPrtls();

	void AssignInitialPrtlMass();
	void ComputeVariablePrtlMasses();
	bool m_pfNeedToUpdatePrtlDensities;
	SimuValue ComputePrtlDensities();
	void AssignOrgPrtlDensities();
	void AssignIniTemperatureForPrtls();
	void FixPrtlMotionWithBoundaries();

	virtual void PreparePrtlEvolution();
	virtual void DetectSrfcPrtls();
	void SetSrfcPrtl(CFluidPrtl* pPrtl);
	void SetInsdPrtl(CFluidPrtl* pPrtl);
	virtual void EvolveFluidParticles(SimuValue simuTime);
	void MovePrtlsByGravity();
	void ComputeAvgVERatio();
	void IntegratePrtlMotions(SimuValue simuTime);
	void ResetHeatTransferRateOnPrtls();
	void ComputeTemperatureFromHeatTransferRateOnPrtls(SimuValue thermalTimeStep);
	void HeatTransferWithOtherFluids(CPrtlFluid* pOtherFluid);
	void InteractWithOtherFluids();
	void InteractWithOtherFluidsByRepulsiveForce();
	void InteractWithOtherFluidsBasedOnIsoSrfc();
	void InteractWithOtherFluidsBasedOnConvexHullWithOldWgt();
	void InteractWithOtherFluidsBasedOnConvexHull();
	void AttractedToNgbrPrtls(CFluidPrtl* pPrtl, TGeomElemArray<CFluidPrtl> &ngbrPrtls);
	SimuValue CollisionResponse(CFluidPrtl* pPrtl, TGeomElemArray<CFluidPrtl> &ngbrPrtls,
								SimuValue smoothLen,
								SimuValue prtlMassRatio, SimuValue vrtlPrtlMassRatio,
								CVector3D* srfcNormal, SimuValue distToSrfc);
	bool ModifyPrtlVelocity(CFluidPrtl* pPrtl, CVector3D* fluidVelocity,
							CVector3D* srfcNormal, SimuValue wgtPrtl, SimuValue wgtFluid);
	SimuValue ModifyPrtlVelocity(CFluidPrtl* pPrtl, CVector3D* srfcNormal,
								 CVector3D* resistVelocity,
								 SimuValue curSMWgt, SimuValue curRTWgt,
								 SimuValue othSMWgt, SimuValue othRTWgt);
	SimuValue m_pfSrfcIsoDensity;
	void ComputeSrfcIsoDensity();
	void ComputeVelocityAtPos(CVector3D &pos, CVector3D &vel, SimuValue radius);
	SimuValue ComputeIsoDensityAndGradAndVelAtPos(CVector3D &pos, CVector3D &grad,
													CVector3D &vel, SimuValue radius);
	void MoveFluidBoundary(SimuValue simuTime);
	void EnforceFluidBoundary(SimuValue simuTime);
	void DumpStressTensorOnPrtlsInCollision();
	void ApplySurfaceTension();
	void IntegratePrtlVelocitiesWithGasStateEquation();
	void IntegratePrtlTemperatures();
	void IntegratePrtlVelocitiesWithViscosity();
	void IntegratePrtlVelocitiesWithNonNewtonianStress();
	void ImplicitIntegratePrtlVelocitiesWithNonNewtonianStress();
	void IntegratePrtlMotionsWithPoissonEquation();
	virtual void ComposeSimuInfoAfterOneTimeStep(int numTimeSteps);
	void ShowSimuInfoAfterOneTimeStep(int numTimeSteps);

	// used for particle select options 
	void SelectParticlesEnclosedBySphere(CVector3D &center, SimuValue radius);
	void DeSelectParticlesEnclosedBySphere(CVector3D &center, SimuValue radius);
	void DeSelectAllParticles();

	void ComputePrtlPressuresUsingGasStateEquation();

	// conjugate gradient solver for Poisson equation of pressure
	SimuValue m_pfCGTolerance;
	CSimuCGSolver m_pfCGSolver;
	int m_pfInterations; // for debug use only
	void EnforceIncompressibilityUsingPoissonEquation();
	bool SetupSymmetricSparseMatrix();

	CSimuCGSolver3D m_pfCGSolver3D; // for implicit integration of stress tensor
	int m_pfInterations3D; // for debug use only
	TClassArray<CVector3D> m_pfDeltaPos;
	void SetupSymmetricSparseMatrixForImplicitStressIntegration();
	void ComputeStressDerivativeOnPrtl(CFluidPrtlNonNew* pPrtl, CFluidPrtlNonNew* pNgbrPrtl,
										SimuValue matrixFD[9]);

	void ComputePrtlPressureGradients();
	void IntegratePrtlVelocitiesByPresGrad();
	void IntegratePrtlVelocitiesByGravity();
	void AdjustPrtlVelocitiesWithNeighbors();
	void IntegratePrtlPositions();

	// non-newtonian stress tensor computation
	SimuValue m_pfMaxVortex; // debug use only
	SimuValue m_pfMaxAngTen; // debug use only
	SimuValue m_pfMaxTensor; // debug use only
	void IntegratePrtlStressTensorsByPlasticModel();
	void IntegratePrtlStressTensorsByElasticModel();
	void IntegratePrtlStressTensorsByLinearModel();
	SimuValue GetShearModulusFromTemperature(SimuValue temperature);
	SimuValue GetShearModulusFromLinearMelting0(SimuValue temperature, SimuValue coeffT,
												SimuValue coeffSM, SimuValue constant);
	void AdjustPrtlStressTensorsWithNgbrs();
	void IntegratePrtlVelocitiesByStressTensor();

	void ComputePosOnIsoSrfc(SimuValue isoValue, SimuValue value0, SimuValue value1,
							 CVector3D* srfcPos, CVector3D* pos0, CVector3D* pos1);
	SimuValue ComputeDensityAtPos(CVector3D &pos, SimuValue radius);
	SimuValue ComputePrtlNumDensityAtPos(CVector3D &pos, SimuValue radius);
	SimuValue ComputeIsoDensityAtPos(CVector3D &pos, SimuValue radius);
	SimuValue ComputeGasStatePressure(SimuValue curDensity, SimuValue orgDensity);
	void ComputeDensityGradient(CVector3D &pos, CVector3D &dir, SimuValue radius);
	SimuValue ComputeWeightedPrtlNumDenAndGrad(CVector3D &pos, CVector3D &grad,
												SimuValue radius);

	static SimuValue ComputeKineticEnergy(TPointerArray<CFluidPrtl> &aryPrtls);

private:
	CFluidPrtl* m_pfHighlitPrtl;
	TPointerArray<CFluidPrtl>* m_pfHighlitPrtlAry;

	static time_t	m_tNewFormatForPrtlDataImExport0;
	static time_t	m_tNewFormatForPrtlDataImExport1;
};

#endif // !defined(AFX_PRTLFLUID_H__A2EC1C03_54F8_4C1B_A455_B3343360717F__INCLUDED_)
