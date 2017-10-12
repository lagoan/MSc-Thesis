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
	bool m_fpOnSurface;
	bool m_fpFixedMotion; // when it's true, particle's motion is predetermined.
	bool m_fpNoDelaunay;
	bool m_fpInCollision;
	bool m_fpNewPrtl;
	int m_fpId; // for symmetric neighbor search and sparse matrix of Poisson equation
	SimuValue m_fpMass;
	SimuValue m_fpOrgDensity;
	SimuValue m_fpDensity;
	SimuValue m_fpPressure;
	SimuValue m_fpHTRate;
	SimuValue m_fpTemperature;
	CVector3D m_fpPresGrad;
	CVector3D m_fpStress;

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
	void ComputePrtlDensity(SimuValue radius);
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

	virtual void InteroplatePrtlValues(TPointerArray<CFluidPrtl> &aryPossibleNgbrs, SimuValue radius);
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
};

#endif // !defined(AFX_FLUIDPRTL_H__990B39BF_385B_4E6F_98B6_F5B9D5E2F79E__INCLUDED_)
