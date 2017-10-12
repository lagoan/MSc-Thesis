// FluidPrtlPoisson.cpp: implementation of the CFluidPrtlPoisson class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "FluidPrtlPoisson.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFluidPrtlPoisson::CFluidPrtlPoisson()
{
	m_fppConst = 0;
	m_fppDiagCoeff = 0;
	m_fppNgbrCoeffs = NULL;
	m_fppNumInsideNgbrs = 0;
}

CFluidPrtlPoisson::~CFluidPrtlPoisson()
{
	if (m_fppNgbrCoeffs != NULL)
		delete m_fppNgbrCoeffs;
}

void CFluidPrtlPoisson::ComputeLaplacianCoefficients(SimuValue smoothLen,
													 SimuValue orgDensity,
													 SimuValue timeStepSqre)
{
	// surface particle doesn't have a row in Poisson equation,
	// therefore, no need to compute Laplacian coefficients
	if (m_fpOnSurface) return;

	if (m_fpNgbrs.m_paNum <= 0)
	{
		// CSimuMsg::ExitWithMessage("CFluidPrtlPoisson::ComputeLaplacianCoefficients(...)",
		//							"Ngbr prtls are not found.");
		m_fpOnSurface = true;
		return;
	}

	ResetDataStructureForPoissonMatrix();

	SimuValue tmpDist;
	SimuValue tmpCoeff;
	SimuValue totalWeight = 0;
	SimuValue radius = smoothLen;
	for(int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		CFluidPrtlPoisson* pNgbrPrtl = (CFluidPrtlPoisson*)m_fpNgbrs[i];
		tmpDist = m_vpPos->GetDistanceToVector(pNgbrPrtl->m_vpPos);
		// using equation (16) from [Shao et al. 2003]
		tmpCoeff = tmpDist*CSimuUtility::SplineGradientWeightFunction(tmpDist, radius);
		tmpCoeff /= pow(tmpDist, 2) + pow(0.1*radius, 2);
		tmpCoeff *= 4*(m_fpMass + pNgbrPrtl->m_fpMass);
		tmpCoeff /= pow(m_fpDensity + pNgbrPrtl->m_fpDensity, 2);
		// testing equation
		tmpCoeff = -CSimuUtility::SplineWeightFunction(tmpDist, radius);
		tmpCoeff *= 0.5*(m_fpMass + pNgbrPrtl->m_fpMass);
		tmpCoeff /= pow(m_fpDensity + pNgbrPrtl->m_fpDensity, 2);
		totalWeight += tmpCoeff;

		m_fppDiagCoeff += tmpCoeff;
		if (pNgbrPrtl->m_fpOnSurface)
		{
			m_fppConst += tmpCoeff*pNgbrPrtl->m_fpPressure;
		}
		else
		{
			m_fppNumInsideNgbrs++;
			m_fppNgbrCoeffs[i] = -tmpCoeff;
		}
	}
	if (totalWeight > 0)
	{
		m_fppDiagCoeff /= totalWeight;
		m_fppConst /= totalWeight;
		for (int i=0; i < m_fpNgbrs.m_paNum; i++)
		{
			m_fppNgbrCoeffs[i] /= totalWeight;
		}
	}
	if (CSimuManager::m_bLocalFluidOrgDensity)
	{
//		CSimuUtility::AssertSimuValue(m_fpOrgDensity);
		SimuValue deltaDensity = m_fpOrgDensity - m_fpDensity;
		if (fabs(deltaDensity) >= SIMU_VALUE_EPSILON)
			m_fppConst += deltaDensity/timeStepSqre;
	}
	else
		m_fppConst += (orgDensity - m_fpDensity)/timeStepSqre;
}

void CFluidPrtlPoisson::ComputeDensityCoefficients(SimuValue smoothLen)
{
	ResetDataStructureForPoissonMatrix();

	for(int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		CFluidPrtlPoisson* pNgbrPrtl = (CFluidPrtlPoisson*)m_fpNgbrs[i];
		SimuValue tmpDist = m_vpPos->GetDistanceToVector(pNgbrPrtl->m_vpPos);
		m_fppNgbrCoeffs[i] = CSimuUtility::SplineWeightFunction(tmpDist, smoothLen);
	}
	m_fppDiagCoeff = CSimuUtility::SplineWeightFunction(0, smoothLen);;
	m_fppConst = m_fpDensity;
	m_fppNumInsideNgbrs = m_fpNgbrs.m_paNum;
}

void CFluidPrtlPoisson::ResetDataStructureForPoissonMatrix()
{
	// allocate memory for off-diagonal coefficients
	if (m_fppNgbrCoeffs != NULL)
	{
		delete m_fppNgbrCoeffs;
		m_fppNgbrCoeffs = NULL;
	}

	// reset coefficients
	m_fppConst = 0;
	m_fppDiagCoeff = 0;
	m_fppNgbrCoeffs = new SimuValue[m_fpNgbrs.m_paNum];
	memset(m_fppNgbrCoeffs, SIMU_VALUE_MAX, m_fpNgbrs.m_paNum*sizeof(SimuValue));
	m_fppNumInsideNgbrs = 0;
}
