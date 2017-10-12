// FluidMeshPrtl.cpp: implementation of the CFluidMeshPrtl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "FluidMeshPrtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFluidMeshPrtl::CFluidMeshPrtl()
{
}

CFluidMeshPrtl::~CFluidMeshPrtl()
{

}

void CFluidMeshPrtl::CopyFluidValuesFromPrtl(CFluidMeshPrtl* pPrtl)
{
	m_vpPos->SetValue(m_vpPos);
	m_vpVel->SetValue(m_vpVel);
	m_fpDensity = m_fpDensity;
	if (CSimuManager::NON_NEWTONIAN_FLUID)
	{
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
			m_fpnnStrTen[m][n] += m_fpnnStrTen[m][n];
	}
}

void CFluidMeshPrtl::InterpolatePrtlValuesByDistance(TPointerArray<CFluidPrtl>
													   &aryPossibleNgbrs, SimuValue radius,
													   SimuValue colocationEpsilon)
{
	char location[] = "CFluidMeshPrtl::InterpolatePrtlValuesByDistance(...)";

	ResetPrtlValues();

	SimuValue weight, totalWeight = 0;
	SimuValue radiusSqr = radius*radius;
	SimuValue tmpDistSqr;
	for (int k=0; k < aryPossibleNgbrs.m_paNum; k++)
	{
		CFluidMeshPrtl* dataPrtl = (CFluidMeshPrtl*)aryPossibleNgbrs[k];
		tmpDistSqr = m_vpPos->GetDistanceSquareToVector(dataPrtl->m_vpPos);
		if (tmpDistSqr > radiusSqr) continue;
		if (sqrt(tmpDistSqr) <= colocationEpsilon
		 || tmpDistSqr <= SIMU_VALUE_EPSILON)
		{
			CopyFluidValuesFromPrtl(dataPrtl);
			totalWeight = -1; // so as not to scale with totalWeight after breaking the loop
			break;
		}
		weight = 1/tmpDistSqr;
		m_vpVel->AddedBy(dataPrtl->m_vpVel, weight);
		if (CSimuManager::NON_NEWTONIAN_FLUID)
		{
			for (int m=0; m < 3; m++)
			for (int n=0; n < 3; n++)
				m_fpnnStrTen[m][n] += weight*dataPrtl->m_fpnnStrTen[m][n];
		}
		totalWeight += weight;
	}
	if (totalWeight > SIMU_VALUE_EPSILON)
	{
		m_vpVel->ScaledBy(1/totalWeight);
		if (CSimuManager::NON_NEWTONIAN_FLUID)
		{
			for (int m=0; m < 3; m++)
			for (int n=0; n < 3; n++)
				m_fpnnStrTen[m][n] /= totalWeight;
		}
	}
	else if (totalWeight == 0)
	{
		m_geHighlit = true;
		CSimuMsg::ExitWithMessage(location, "No data prtl is found within radius.", CSimuManager::m_bSkipWarning);
		m_geHighlit = false;
	}
}
