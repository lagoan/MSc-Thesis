// FluidPrtlPoisson.cpp: implementation of the CFluidPrtlPoisson class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "FluidPrtlPoisson.h"



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
	if (m_pbTrueCurrentSurface) return;

	if (m_fpNgbrs.m_paNum <= 0)
	{
		// CSimuMsg::ExitWithMessage("CFluidPrtlPoisson::ComputeLaplacianCoefficients(...)",
		//							"Ngbr prtls are not found.");
		m_pbTrueCurrentSurface = true;
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
		tmpCoeff = tmpDist * SplineGradientWeightFunction(tmpDist, pNgbrPrtl->m_pfSmoothLen);
		tmpCoeff /= pow((double)tmpDist, (double)2.0f) + pow((double)0.1*radius, (double)2.0f);
		tmpCoeff *= 4*(m_fpMass + pNgbrPrtl->m_fpMass);
		tmpCoeff /= pow((double)m_fpDensity + pNgbrPrtl->m_fpDensity, (double)2.0f);
		
#if 0
		// testing equation
		tmpCoeff = - SplineWeightFunction(tmpDist, pNgbrPrtl->m_pfSmoothLen);
		tmpCoeff *= 0.5*(m_fpMass + pNgbrPrtl->m_fpMass);
		tmpCoeff /= pow(m_fpDensity + pNgbrPrtl->m_fpDensity, 2.0f);
#endif	
		totalWeight += tmpCoeff;

		m_fppDiagCoeff += tmpCoeff;
		if (pNgbrPrtl->m_pbTrueCurrentSurface)
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
		//SimuValue deltaDensity = m_fpOrgDensity - m_fpDensity + m_bubbleDensityChange;
		SimuValue deltaDensity = m_fpOrgDensity - m_fpDensity; // buenero

#if 1
		if (m_bpIsBubble) {
		//if (m_bpIsBubble && !m_pbTrueCurrentSurface) {
			
			//deltaDensity = (m_fpDensity - m_fpOrgDensity);

			deltaDensity = (m_bubbleDensity - m_fpOrgDensity);
			
			// before we had this
			//m_fpOrgDensity = m_fpDensity;
		}
#endif
		
		//SimuValue deltaDensity = -m_fpOrgDensity + m_fpDensity;
		//if (m_bpIsBubble) {
			//std::cout << "m_fpDensity " << m_fpDensity << " m_bubbleDensityChange " << m_bubbleDensityChange << " total " << m_fpDensity + m_bubbleDensityChange << "\n";
			//std::cout << "m_fpDensity " << m_fpDensity << "\n";
		//}

		if (fabs(deltaDensity) >= SIMU_VALUE_EPSILON) {
			m_fppConst +=  deltaDensity / timeStepSqre;
		}
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
		m_fppNgbrCoeffs[i] = SplineWeightFunction(tmpDist, pNgbrPrtl->m_pfSmoothLen);
	}
	m_fppDiagCoeff = SplineWeightFunction(0, m_pfSmoothLen);
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
	memset(m_fppNgbrCoeffs, SIMU_VALUE_MAX, m_fpNgbrs.m_paNum * sizeof(SimuValue));
	m_fppNumInsideNgbrs = 0;
}
