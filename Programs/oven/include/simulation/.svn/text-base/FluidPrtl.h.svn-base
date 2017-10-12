// FluidPrtl.h: interface for the CFluidPrtl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLUIDPRTL_H__990B39BF_385B_4E6F_98B6_F5B9D5E2F79E__INCLUDED_)
#define AFX_FLUIDPRTL_H__990B39BF_385B_4E6F_98B6_F5B9D5E2F79E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VirtualPrtl.h"
#include "TGeomElemArray.h"

class CFluidPrtl : virtual public CVirtualPrtl  
{
public:
	
	SimuValue velocityGradient[3][3];
	
	bool m_fpOnSurface;
	bool m_fpFixedMotion; // when it's true, particle's motion is predetermined.
	bool m_fpNoDelaunay;
	bool m_fpInCollision;
	bool m_fpNewPrtl;
	bool m_pbFixedParticle;
	bool m_pbTrueCurrentSurface;
	SimuValue m_pbTrueCurrentSurfaceTime;
	SimuValue m_pfSize;
	SimuValue m_pfCO2;
	SimuValue m_pfCO2Sum;
	bool startGravity;
	int m_fpTotalNeighbors;
	SimuValue lastTemperatureWhenBubbleWasAdded;
	int m_pointBelongsToFacet;
	//SimuValue changeInCO2Generation;
	
	int initialNeighborCount;
	bool firstSmoothLengthReset;
	
	SimuValue m_fpShearModulus;
	SimuValue m_decayRate;
	
	SimuValue velocityTuner;
	SimuValue colorGradientLength;


	int m_fpId; // for symmetric neighbor search and sparse matrix of Poisson equation
	SimuValue m_fpMass;
	SimuValue m_fpOrgDensity;
	SimuValue m_fpDensity;
	SimuValue m_fpPressure;
	SimuValue m_fpHTRate;
	SimuValue m_fpTemperature;
	SimuValue m_fpFinalDensity;
	
	CVector3D m_fpPresGrad;
	CVector3D m_fpStress;

	SimuValue m_pfSmoothLen;
	SimuValue m_fpInitialDensity;
	SimuValue m_fpInitialSmoothingLength;

	// test if particle should be treated as fluid particle or bubble
	bool m_bpIsBubble;
	SimuValue m_initBubbleSize;
	SimuValue m_initBubbleTemperature;
	SimuValue m_initBubblePressure;
	SimuValue m_bubbleDensityChange;
	SimuValue m_bubbleSizeChange;
	SimuValue m_bubbleMass;
	SimuValue m_bubbleDensity;
	bool presssureInitialized;
	SimuValue initialPressure;
	bool visible;
	SimuValue m_bubblePressure;
	CVector3D m_fpBubblePresGrad;
	
	bool continueGrowing;
	
	//SimuValue m_myConstant;
	SimuValue m_viscosity;	

	// for solid fluid colussions
	//bool crash;
	//TGeomElemArray<CFluidPrtl> m_fpCrashNgbrs;

	TGeomElemArray<CFluidPrtl> m_fpNgbrs;

	CFluidPrtl();
	virtual ~CFluidPrtl();

	void SetAsFixedMotion(SimuValue airPressure);
	void AddVelocity(CVector3D* deltaVel);
	void AddVelocity(CVector3D* deltaVel, SimuValue scalar);
	void DeregisterFromNgbrPrtls();
	SimuValue ComputeDistWeightSum(SimuValue radius);
	SimuValue ComputeDistWeightSumAndGrad(CVector3D &grad, SimuValue radius);
	void ComputePrtlNumDenAndGradLen(SimuValue radius, SimuValue &pnd, SimuValue &gradLen);
	void ComputePrtlNumDenGradLen(SimuValue radius, SimuValue &gradLen);
	void ComputePrtlDensity(bool initialD);
	void ComputePrtlPressureGradient(SimuValue radius);
	void ComputePrtlPressureGradientFromOneNgbr(CFluidPrtl* ngbrPrtl, CVector3D &presGrad,
												SimuValue radius);
	SimuValue ComputeHeatChange(SimuValue radius);
	SimuValue ComputeHeatChangeFromOneNgbr(CFluidPrtl* ngbrPrtl, SimuValue radius);
	void ComputeViscosity(CVector3D &viscosity, SimuValue radius);
	void ComputeViscosityFromOneNgbr(CFluidPrtl* ngbrPrtl, CVector3D &interVisc,
									 SimuValue radius);
	void ComputeVelocityGradient(SimuValue velGrad[3][3], SimuValue radius);
	SimuValue ComputeMaxRelativeVelMagnitude();
	SimuValue ComputeMaxRelativePresGradMagnitude();

	SimuValue ComputeHeatFromAir();

	virtual void InterpolatePrtlValues(TPointerArray<CFluidPrtl> &aryPossibleNgbrs, SimuValue radius);
	virtual void ResetPrtlValues();

	static SimuValue GetClosestNgbrPrtl(CFluidPrtl* pPrtl, CFluidPrtl* &closestPrtl,
										TGeomElemArray<CFluidPrtl> &ngbrPrtls);
	static SimuValue ComputeWeightedPrtlNumDenAndGrad(CVector3D &pos, CVector3D &grad,
													  TPointerArray<CFluidPrtl> &ngbrPrtls,
													  SimuValue radius);
	static void CreateRandomPrtls(CVector3D* ctrPos, SimuValue radius, int numTotalPrtls,
								  TGeomElemArray<CFluidPrtl> &ranPrtls);
	static SimuValue ComputeVERatio(CVector3D* presGrad, CVector3D* stress);
	static SimuValue ComputeWeightLaplacian(CVector3D* ctrPos, SimuValue radius,
											TGeomElemArray<CFluidPrtl> &ngbrPrtls);
	static void ComputeWeightGradient(CVector3D* ctrPos, SimuValue radius,
									TGeomElemArray<CFluidPrtl> &ngbrPrtls, CVector3D* grad);

	
	void resetSmoothingLength(double g);
	
	SimuValue computeDecayRate();
	SimuValue computeVaporPressure(SimuValue temp);
	void ComputeVelocityGradient();
	//CFluidPrtl operator= (const CFluidPrtl &p);
	void ComputePrtlBubblePressureGradient();
	void ComputePrtlBubblePressureGradientFromOneNgbr(CFluidPrtl* ngbrPrtl, CVector3D &presGrad);
	
	SimuValue SplineWeightFunction(SimuValue distance, SimuValue otherRadius);
	SimuValue SplineGradientWeightFunction(SimuValue distance, SimuValue otherRadius);
	SimuValue SplineSecondDerivativeFunction(SimuValue distance, SimuValue otherRadius);
	SimuValue TESTDerivativeWeightFunction(SimuValue distance, SimuValue otherRadius);
    SimuValue SpikyGradientWeightFunction(SimuValue distance, SimuValue otherRadius);
    SimuValue ViscocitySecondWeightFunction(SimuValue distance, SimuValue otherRadius);
    SimuValue ViscSecondDerivativeWeightFunction(SimuValue distance, SimuValue radius);
	SimuValue SplineGradientWeightInitialFunction(SimuValue d);
};

/*
inline CFluidPrtl CFluidPrtl::operator= (const CFluidPrtl &pPrtl)
{
	
	m_fpOnSurface				= pPrtl.m_fpOnSurface;
	m_fpFixedMotion				= pPrtl.m_fpFixedMotion; 
	m_fpNoDelaunay				= pPrtl.m_fpNoDelaunay;
	m_fpInCollision				= pPrtl.m_fpInCollision;
	m_fpNewPrtl					= pPrtl.m_fpNewPrtl;
	m_pbFixedParticle			= pPrtl.m_pbFixedParticle;
	m_pbTrueCurrentSurface		= pPrtl.m_pbTrueCurrentSurface;
	m_pfSize					= pPrtl.m_pfSize;
	m_pfCO2						= pPrtl.m_pfCO2;
	m_pfCO2Sum					= pPrtl.m_pfCO2Sum;

	m_fpTotalNeighbors			= pPrtl.m_fpTotalNeighbors;


	m_fpId						= pPrtl.m_fpId; 
	m_fpMass					= pPrtl.m_fpMass;
	m_fpOrgDensity				= pPrtl.m_fpOrgDensity;
	m_fpDensity					= pPrtl.m_fpDensity;
	m_fpPressure				= pPrtl.m_fpPressure;
	m_fpHTRate					= pPrtl.m_fpHTRate;
	m_fpTemperature				= pPrtl.m_fpTemperature;
	m_fpPresGrad.v[0]			= pPrtl.m_fpPresGrad.v[0];
	m_fpPresGrad.v[1]			= pPrtl.m_fpPresGrad.v[1];
	m_fpPresGrad.v[2]			= pPrtl.m_fpPresGrad.v[2];
	m_fpStress.v[0]				= pPrtl.m_fpStress.v[0];
	m_fpStress.v[1]				= pPrtl.m_fpStress.v[1];
	m_fpStress.v[2]				= pPrtl.m_fpStress.v[2];


	m_bpIsBubble				= pPrtl.m_bpIsBubble;
	return *this;

}
*/

#endif // !defined(AFX_FLUIDPRTL_H__990B39BF_385B_4E6F_98B6_F5B9D5E2F79E__INCLUDED_)
