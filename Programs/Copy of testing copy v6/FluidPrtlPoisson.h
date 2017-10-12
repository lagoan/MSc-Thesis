// FluidPrtlPoisson.h: interface for the CFluidPrtlPoisson class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLUIDPRTLPOISSON_H__B1292408_F216_49FD_B881_58A567044D35__INCLUDED_)
#define AFX_FLUIDPRTLPOISSON_H__B1292408_F216_49FD_B881_58A567044D35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FluidPrtl.h"

class CFluidPrtlPoisson : public CFluidPrtl  
{
public:
	SimuValue m_fppConst;
	SimuValue m_fppDiagCoeff;
	SimuValue* m_fppNgbrCoeffs;
	int m_fppNumInsideNgbrs;

	CFluidPrtlPoisson();
	virtual ~CFluidPrtlPoisson();

	void ComputeLaplacianCoefficients(SimuValue smoothLen,
									  SimuValue orgDensity,
									  SimuValue timeStepSqre);
	void ComputeDensityCoefficients(SimuValue smoothLen);

protected:
	void ResetDataStructureForPoissonMatrix();
};

#endif // !defined(AFX_FLUIDPRTLPOISSON_H__B1292408_F216_49FD_B881_58A567044D35__INCLUDED_)
