// FluidPrtlNonNew.h: interface for the CFluidPrtlNonNew class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLUIDPRTLNONNEW_H__30E3565B_83BD_4B45_80DD_6647DB943DEB__INCLUDED_)
#define AFX_FLUIDPRTLNONNEW_H__30E3565B_83BD_4B45_80DD_6647DB943DEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FluidPrtlPoisson.h"

class CFluidPrtlNonNew : public CFluidPrtlPoisson  
{
public:
	SimuValue m_fpnnStrTen[3][3];

	CFluidPrtlNonNew();
	virtual ~CFluidPrtlNonNew();

	static int m_fpnnMRow, m_fpnnMCol;
	static SimuValue** m_fpnnM;
	static SimuValue* m_fpnnX;
	void IntegratePrtlStressTensorByPlasticModel(SimuValue velGrad[3][3], SimuValue timeStep,
												SimuValue shearModulus, SimuValue decayRate,
												ERotation rotationDerivative);
	void IntegratePrtlStressTensorBySPHModel(SimuValue velGrad[3][3], SimuValue timeStep,
											 SimuValue shearModulus, SimuValue decayRate,
											 ERotation rotationDerivative);
	void IntegratePrtlVelocityByStressTensor(CVector3D &stress,
											 SimuValue timeStep, SimuValue smoothLen);
	void ComputeStressOnPrtl(CVector3D &stress, SimuValue smoothLen);

	virtual void InteroplatePrtlValues(TPointerArray<CFluidPrtl> &aryPossibleNgbrs, SimuValue radius);
	virtual void ResetPrtlValues();

	static void ComputeDeviatedStressTensor(SimuValue strTen[3][3]);
	static SimuValue ComputeSPHStressTensorRateElement(int i, int j,
														SimuValue decayRate,
														SimuValue shearModulus,
														SimuValue velGrad[3][3],
														SimuValue strTen[3][3]);
	static SimuValue ComputeUpperConvectedElement(int i, int j, SimuValue velGrad[3][3],
												  SimuValue tensor[3][3]);
	static SimuValue ComputeCorotationalElement(int i, int j, SimuValue velGrad[3][3],
												  SimuValue tensor[3][3]);

	static SimuValue StressFirstDerivativeWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue StressSecondDerivativeWeightFunction(SimuValue distance, SimuValue radius);
};

#endif // !defined(AFX_FLUIDPRTLNONNEW_H__30E3565B_83BD_4B45_80DD_6647DB943DEB__INCLUDED_)
