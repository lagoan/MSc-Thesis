// PrtlFluid.h: interface for the CPrtlFluid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLFLUID_H__A2EC1C03_54F8_4C1B_A455_B3343360717F__INCLUDED_)
#define AFX_PRTLFLUID_H__A2EC1C03_54F8_4C1B_A455_B3343360717F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cfloat>
#include <limits>

#include "LinearTemperaTexture.h"
//#include "DataExImport.h"
#include "TPrtlGrid.h"
#include "TGeomElemArray.h"
#include "FluidPrtl.h"
#include "FluidPrtlPoisson.h"
#include "FluidPrtlNonNew.h"
#include "Boundary.h"
//#include "SimuFluidNozzle.h"
//#include "RigidBall.h"
#include "ParticleSearch.h"
#include "SimuMarchingCube.h"
#include "SimuCGSolver3D.h"
#include "system/SimulationProperties.h"

#include "DelaunayTetrahedron.h"

#include "primitives/ovKDTree.h"
#include "scene/Node.h"
#include "system/types.h"


class CSimuManager;


struct FluidInformation 
{
	
	// for graphs
	
	SimuValue time;
	
	SimuValue averageTemperature;
	SimuValue averageNeighbourCount;
	SimuValue averageShearModulus;
	SimuValue averageDecayRate;
	SimuValue averagePressure;
	SimuValue averageBubblePressure;
	SimuValue averageDensity;
	SimuValue averageVelocityTuner;
	
	SimuValue minTemperature;
	SimuValue minNeighbourCount;
	SimuValue minShearModulus;
	SimuValue minDecayRate;
	SimuValue minPressure;
	SimuValue minBubblePressure;
	SimuValue minDensity;
	SimuValue minVelocityTuner;
	
	SimuValue maxTemperature;
	SimuValue maxNeighbourCount;
	SimuValue maxShearModulus;
	SimuValue maxDecayRate;
	SimuValue maxPressure;
	SimuValue maxBubblePressure;
	SimuValue maxDensity;
	SimuValue maxVelocityTuner;
	
	SimuValue minPos[3];
	SimuValue maxPos[3];
	
	FluidInformation() {
		reset();
		
		minTemperature		= DBL_MAX;
		minNeighbourCount	= DBL_MAX;
		minShearModulus		= DBL_MAX;
		minDecayRate		= DBL_MAX;
		minPressure			= DBL_MAX;
		minBubblePressure	= DBL_MAX;
		minDensity			= DBL_MAX;
		minVelocityTuner	= DBL_MAX;
		
		maxTemperature		= DBL_MIN;
		maxNeighbourCount	= DBL_MIN;
		maxShearModulus		= DBL_MIN;
		maxDecayRate		= DBL_MIN;
		maxPressure			= DBL_MIN;
		maxBubblePressure	= DBL_MIN;
		maxDensity			= DBL_MIN;
		maxVelocityTuner	= DBL_MIN;

		
	};
	
	void reset() {
		time					= 0;
		averageTemperature		= 0;
		averageNeighbourCount	= 0;
		averageShearModulus		= 0;
		averageDecayRate		= 0;
		averagePressure			= 0;
		averageBubblePressure	= 0;
		averageDensity			= 0;
		averageVelocityTuner	= 0;
		
		minPos[0] = minPos[1] = minPos[2] = DBL_MAX; 
		maxPos[0] = maxPos[1] = maxPos[2] = DBL_MIN;


	};

	FluidInformation operator= (const FluidInformation &info) {
		time = info.time;
	
		averageTemperature = info.averageTemperature;
		averageNeighbourCount = info.averageNeighbourCount;
		averageShearModulus = info.averageShearModulus;
		averageDecayRate = info.averageDecayRate;
		averagePressure = info.averagePressure;
		averageBubblePressure = info.averageBubblePressure;
		averageDensity = info.averageDensity;
		averageVelocityTuner = info.averageVelocityTuner;
		
		minTemperature = info.minTemperature;
		minNeighbourCount = info.minNeighbourCount;
		minShearModulus = info.minShearModulus;
		minDecayRate = info.minDecayRate;
		minPressure = info.minPressure;
		minBubblePressure = info.minBubblePressure;
		minDensity = info.minDensity;
		minVelocityTuner = info.minVelocityTuner;
		
		maxTemperature = info.maxTemperature;
		maxNeighbourCount = info.maxNeighbourCount;
		maxShearModulus = info.maxShearModulus;
		maxDecayRate = info.maxDecayRate;
		maxPressure = info.maxPressure;
		maxBubblePressure = info.maxBubblePressure;
		maxDensity = info.maxDensity;
		maxVelocityTuner = info.maxVelocityTuner;
		 

		for (int i=0; i<3; i++) { 
			minPos[i] = info.minPos[i];
			maxPos[i] = info.minPos[i];
		}
		return *this;
	}
	
};

class CPrtlFluid  /*: public CDataExImport */
{
public:
	static int m_pfMaxFluidId;
	int m_pfFluidId;

	CSimuManager* m_pfSimuManager;
	Node *node;
	bool m_pfPrtlEvolutionIsPrepared; // for not forgetting call PreparePrtlEvolution() in SimuModeler.cpp
	
	bool m_pfSolidFirstTime;
	bool m_pbApplyGravity;
	bool m_pbFixedParticles;
	GLint glName;
	ObjectType objectType;
	
	double densityGeometricMean;
	
	// CO2 bubble controls
	bool addBubblesFromCO2;
	AddBubblesByTotalOrRate addBubblesByTotalOrRate;
	int	co2BubblesPerParticle;
	bool generateCO2;
	SimuValue co2EnoughForBubble;
	SimuValue m_CO2PeakGeneration;
	SimuValue m_CO2TemperatureMean;
	SimuValue m_CO2TemperatureRange;
	SimuValue m_CO2AreaUnderRateCurve;


	SimuValue m_pfDensity;
	SimuValue m_pfPrtlDist;
	SimuValue m_pfPrtlMass;
	SimuValue m_pfTotalMass;
	SimuValue m_pfTotalFreePrtlMass;
	//SimuValue m_pfSmoothLen;
	//SimuValue m_pfSmoothLenSqr;
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

	enum UPSAMPLE_POINT_STATUS
	{
		TOO_CLOSE_TO_EXISTING_PRTL,
		TOO_SMALL_MASS,
		OUTSIDE_FLUID_SURFACE,
		ON_FLUID_SURFACE,
		INSIDE_FLUID
	};
	
	bool m_pfuResampled;
	int m_pfuUpsamplePauseSteps;
	SimuValue m_pfuUpsampleMinRadiusSqr;
	TGeomElemArray<CFluidPrtl> m_pfuUpsampledPrtls;
	CDelaunayTetrahedron m_pfuDTConstructor;
	SimuValue m_pfuLowerPrtlMassLimit;
	SimuValue m_pfuUpperPrtlMassLimit;
	
	//TClassArray<CBoundary> m_pfBoundaries;

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
	
	SimuValue m_pfuSrfcThickness;

	QString m_pfSimuInfo;
	CVector3D m_pfColor;
	CVector3D m_pfSrfcColor;


	CVector3D initialVelocity;
	
	// for gradient
	SimuValue maxGradient;
	SimuValue minGradient;
	bool gradientMaxMinSet;
	
	// for solids
	//std::list<Nef_polyhedron> nefPolyhedronList;
	
	struct FacetComposedOf {
		int vertices[3];
	};

	std::vector<FacetComposedOf> facets;
	std::vector<int> vertexBelongstoFacet;

	//FluidInformation information;
	
	
	CPrtlFluid();
	virtual ~CPrtlFluid();

	virtual void RenderPrtlFluid();
	void RenderFluidPrtls();
	void ComputeParticleColor(CVector3D &color, CFluidPrtl* pPrtl);
	void RenderHighlitPrtls();
	void DrawBoundaries();

	void LoadPrtlFluidVariablesFrom(CPrtlFluid* anotherFluid);
	bool IsPosOnSrfcByItsConvexHull(CVector3D* pos, TGeomElemArray<CFluidPrtl> &ngbrPrtls);
	bool IsPosOnSurfaceByColorField(CFluidPrtl* pPrtl);

	void getTrueCurrentSurface();
	void addBubblesBasedOnCO2();
	void createBubbleArround(CFluidPrtl *pPrtl);
	CFluidPrtl* createBubbleAtPos(CVector3D* pos, TGeomElemArray<CFluidPrtl> &ngbrPrtls, SimuValue smoothLength);
	void resetBubbleSizes();

	SimuValue GetShearModulusFromBakingTemperature(SimuValue temperature);
	SimuValue calculateShearRate(SimuValue velGrad[3][3]);

	/*
	void ExportPrtlFluidVariables(FILE* fp);
	void ImportPrtlFluidVariables(FILE* fp);
	void ExportPrtlFluidData(QString exportDir, unsigned int nCurFrame);
	void ImportPrtlFluidData(QString importDir, unsigned int nCurFrame);

	void ExportPrtlFluidParameters(FILE* fp);
	void ImportPrtlFluidParameters(FILE* fp);
	void ExportPrtlFluidData(QString exportDir, unsigned int nCurFrame, FILE* POVRayFp);
	void ImportPrtlFluidData(QString importDir, unsigned int nCurFrame, FILE* POVRayFp);
	void ExportSimulationInfo(QString exportDir, const char* info);
	void ImportSimulationInfo(QString importDir, unsigned int nCurFrame);
	void ExportPOVRayEnvData(unsigned int nCurFrame, FILE* POVRayFp);
	void ImportPOVRayEnvData(FILE* POVRayFp);
	void ExportPrtlData(QString exportDir, unsigned int nCurFrame);
	void ImportPrtlData(QString importDir, unsigned int nCurFrame);
	void ExportPrtlData(FILE* fp);
	void ImportPrtlData(FILE* fp);
	void ExportPrtlsAsSpheresInPOVRay(FILE* fp);
	void ExportTwoTypePrtlsAsSpheresInPOVRay(FILE* fp);
	void ImportPrtlsAsSpheresInPOVRay(FILE* fp);
	void ImportTwoTypePrtlsAsSpheresInPOVRay(FILE* fp);
	virtual void ExportSurfaceMeshIntoPOVRayMesh2ObjectFile(QString &txFileName, FILE* fp);
	virtual void ImportSurfaceMeshFromPOVRayMesh2ObjectFile(FILE* fp);
	*/

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
	//CParticleSearch m_pfPrtlSearch;
	ovKDTree kdTree;
	void RegisterPrtlsForNeighborSearchIfNotYet(bool mixFluidWithBubble = true, bool fullSort = false);
	void SearchPrtlNeighbors(bool mixed);
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
	SimuValue ComputePrtlDensities(bool initialD = false);
	void AssignOrgPrtlDensities();
	void AssignIniTemperatureForPrtls();
	void FixPrtlMotionWithBoundaries();

	virtual void PreparePrtlEvolution();
	virtual void DetectSrfcPrtls();
	void SetSrfcPrtl(CFluidPrtl* pPrtl);
	void SetInsdPrtl(CFluidPrtl* pPrtl);
	virtual void EvolveFluidParticles(SimuValue simuTime, bool mixed);
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

	void HeatTransferFromAmbient();
	void CalculateCO2Generation();
	SimuValue CalculateCO2GenerationAtTemperature(SimuValue temp);
	SimuValue CalulateAreaUnderCO2GenerationCurve(double alpha, double beta);

	//void InteractWithSolidsBasedOnConvexHull();

	void AttractedToNgbrPrtls(CFluidPrtl* pPrtl, TGeomElemArray<CFluidPrtl> &ngbrPrtls);
	SimuValue CollisionResponse(CFluidPrtl* pPrtl, TGeomElemArray<CFluidPrtl> &ngbrPrtls,
								SimuValue smoothLen,
								SimuValue prtlMassRatio, SimuValue vrtlPrtlMassRatio,
								CVector3D* srfcNormal, SimuValue distToSrfc);

	SimuValue respondToSolidCollision(CFluidPrtl* pPrtl, 
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

	SimuValue GetRelaxationTimeFromTemperature(SimuValue temperature);
	SimuValue getApparentViscosity(SimuValue temperature, SimuValue velGrad[3][3]);

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

	void adaptParticleSmoothLength();
	void setPreviousPositions();
	void InteractWithSolids();

private:
	CFluidPrtl* m_pfHighlitPrtl;
	TPointerArray<CFluidPrtl>* m_pfHighlitPrtlAry;

	static time_t	m_tNewFormatForPrtlDataImExport0;
	static time_t	m_tNewFormatForPrtlDataImExport1;
	
	/////////
	
	void UpsampleFluidParticles();
	int IsPointValidForUpsample(CPoint3D* upsampleP,
								TGeomElemArray<CFluidPrtl> &ngbrPrtls);

	CFluidPrtl* AddingOnePrtlAtUpsamplePos(CPoint3D* upsampleP,
										   TGeomElemArray<CFluidPrtl> &ngbrPrtls);
	bool IsPointBreakingFluidInterfaces(CPoint3D* upsampleP);
	
	void calculateBubbleInducedPressure();
};

#endif // !defined(AFX_PRTLFLUID_H__A2EC1C03_54F8_4C1B_A455_B3343360717F__INCLUDED_)
