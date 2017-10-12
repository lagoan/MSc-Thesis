// Boundary.cpp: implementation of the CBoundary class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Boundary.h"



SimuValue CBoundary::m_bdryMaxClsSpeed = 0;
TClassArray<CBoundary>* CBoundary::m_bdryBalls = NULL;
TClassArray<CBoundary>* CBoundary::m_bdryNotBalls = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoundary::CBoundary()
{
	m_bdryDistDumpingRate	= CSimuManager::m_distDumpingRate;
	m_bdryEnergyDumpingRate	= CSimuManager::m_energyDumpingRate;
	m_bdryFrictionRate		= CSimuManager::m_frictionRate;
	m_bdryStressDumpingRate	= CSimuManager::m_stressDumpingRate;

	m_bdryVisible = true;
	m_bdryAngVel = 0;
	m_bdryEnclosedPrtls.InitializeClassArray(false, SIMU_PARA_PRTL_NUM_ALLOC_SIZE);
	m_bdryOrgPrtlPos.InitializeClassArray(true, SIMU_PARA_PRTL_NUM_ALLOC_SIZE);

	m_bdryColor = new CVector3D();
	m_bdryColor->SetValue(CSimuManager::COLOR_GREEN);

	m_bdryThermalBody = false;
	m_bdryMaxHeatingDist = CSimuManager::m_bdryMaxHeatingDist;
	m_bdryTemperature = CSimuManager::m_bdryTemperature;
}

CBoundary::~CBoundary()
{
	delete m_bdryColor;
}

void CBoundary::AddBoundary(CBoundary* boundary)
{
	if (boundary->m_bdryType == RIGID_BALL)
		m_bdryBalls->AppendOnePointer(boundary);
	else
		m_bdryNotBalls->AppendOnePointer(boundary);
}

void CBoundary::MoveBoundaries(SimuValue curTime, SimuValue timeStep)
{
	for (int i=0; i < m_bdryBalls->m_paNum; i++)
	{
		CBoundary* pBdry = m_bdryBalls->m_paPtrs[i];
		pBdry->MoveBoundary(curTime, timeStep);
	}
	for (int i=0; i < m_bdryNotBalls->m_paNum; i++)
	{
		CBoundary* pBdry = m_bdryNotBalls->m_paPtrs[i];
		pBdry->MoveBoundary(curTime, timeStep);
	}
}

void CBoundary::DrawBoundaries()
{
	for (int i=0; i < m_bdryBalls->m_paNum; i++)
	{
		CBoundary* pBdry = m_bdryBalls->m_paPtrs[i];
		pBdry->DrawBoundary();
	}
	for (int i=0; i < m_bdryNotBalls->m_paNum; i++)
	{
		CBoundary* pBdry = m_bdryNotBalls->m_paPtrs[i];
		pBdry->DrawBoundary();
	}
}

void CBoundary::DrawBoundary()
{
}

#if 0
void CBoundary::ExportBoundaryData(FILE* fp)
{
	fprintf(fp, "#declare %s__m_bdryVisible = %d;\n", m_deiName, m_bdryVisible);
}

void CBoundary::ExportBoundaryParameters(FILE* fp)
{
	ExportBoundaryData(fp);
}

void CBoundary::ImportBoundaryData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	fscanf(fp, "#declare %s = %d;\n", tmpName, &m_bdryVisible);
}

void CBoundary::ImportBoundaryParameters(FILE* fp)
{
	ImportBoundaryData(fp);
}
#endif

void CBoundary::MoveBoundary(SimuValue curTime, SimuValue timeStep)
{
}

void CBoundary::EnforceBoundaryConstraint(CPrtlFluid* pPrtlFluid)
{
	for (int i=0; i < pPrtlFluid->m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = pPrtlFluid->m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		if (EnforceBoundaryConstraint(pPrtl, pPrtlFluid->m_pfTimeStep))
		{
			pPrtl->m_fpInCollision = true;
//			CSimuUtility::AssertSimuVector3D(pPrtl->m_vpPos);
		}
	}
}

// return true if in collision; otherwise, false.
bool CBoundary::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	return false;
}

bool CBoundary::PrtlBreakBoundary(CVirtualPrtl* pPrtl)
{
	return PosBreakBoundary(pPrtl->m_vpPos);
}

bool CBoundary::PosBreakBoundary(CVector3D* pPos)
{
	return false;
}

void CBoundary::AddEnclosedPrtl(CFluidPrtl* pPrtl)
{
}

SimuValue CBoundary::GetDistanceToOpenOutletPlane(CVirtualPrtl* pPrtl)
{
	return -1;
}

bool CBoundary::StretchPrtlOntoBoundary(CVirtualPrtl* pPrtl)
{
	return false;
}

void CBoundary::SetBdryParams(SimuValue bdryParams)
{
	m_bdryDistDumpingRate	= bdryParams;
	m_bdryEnergyDumpingRate	= bdryParams;
	m_bdryFrictionRate		= bdryParams;
	m_bdryStressDumpingRate	= bdryParams;
}

void CBoundary::HeatTransferOnBdry(CPrtlFluid* pPrtlFluid)
{
	for (int i=0; i < pPrtlFluid->m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = pPrtlFluid->m_pfPrtls[i];
		HeatTransferBetweenPrtlAndBdry(pPrtl, pPrtlFluid->m_pfHeatConductivity,
										pPrtl->m_pfSmoothLen, pPrtlFluid->m_pfTimeStep);
	}
}

void CBoundary::HeatTransferBetweenPrtlAndBdry(CFluidPrtl* pPrtl, SimuValue conductivity,
											   SimuValue smoothLen, SimuValue timeStep)
{
	if (CSimuManager::NOT_CONSERVE_HEAT)
	{
		if (m_bdryTemperature <= pPrtl->m_fpTemperature)
			return;
	}
	SimuValue distance = GetDistanceBetweenPrtlAndBdry(pPrtl);
	if (distance >= m_bdryMaxHeatingDist)
		return;
	if (distance < 0)
		distance = 0;
	SimuValue deltaHeat = m_bdryTemperature - pPrtl->m_fpTemperature;
	deltaHeat *= conductivity * pPrtl->SplineWeightFunction(distance, pPrtl->m_pfSmoothLen);
	deltaHeat /= pPrtl->m_fpDensity;
	pPrtl->m_fpTemperature += deltaHeat*timeStep;
}

SimuValue CBoundary::GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl)
{
	return m_bdryMaxHeatingDist; // this return value would result in no heat transfer.
}

void CBoundary::ReflectPosOnPlane(CVector3D &pPos, CVector3D &planeNormal,
								  CVector3D &intersectPos)
{
	CVector3D relativePos;
	relativePos.SetValueAsSubstraction(&pPos, &intersectPos);
	SimuValue reflectValue = relativePos.DotProduct(&planeNormal);
	// make <relativePos> sliding distance
	relativePos.AddedBy(&planeNormal, -reflectValue);
	// scale sliding distance by friction rate
	relativePos.ScaledBy(m_bdryFrictionRate);
	// add bouncing distance to <relativePos>
	relativePos.AddedBy(&planeNormal, -reflectValue*m_bdryDistDumpingRate);
	pPos.SetValueAsAddition(&intersectPos, &relativePos);
}

// return true if <pVel> penetrates the plane; otherwise, false.
bool CBoundary::ReflectVelOnPlane(CVector3D &pVel, CVector3D &planeNormal)
{
	SimuValue reflectValue = pVel.DotProduct(&planeNormal);
	if (reflectValue >= 0)
		return false;
	if (CBoundary::m_bdryMaxClsSpeed < fabs(reflectValue))
	{
		CBoundary::m_bdryMaxClsSpeed = fabs(reflectValue);
	}
	// make <pVel> as sliding velocity
	pVel.AddedBy(&planeNormal, -reflectValue);
	// scale sliding velocity by friction rate
	pVel.ScaledBy(m_bdryFrictionRate);
	// add bouncing velocity to <pVel>
	pVel.AddedBy(&planeNormal, -reflectValue*CSimuManager::m_energyDumpingRate);
	return true;
}
