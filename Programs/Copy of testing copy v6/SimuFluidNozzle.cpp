// SimuFluidNozzle.cpp: implementation of the CSimuFluidNozzle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuFluidNozzle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuFluidNozzle::CSimuFluidNozzle()
{
	m_bdryType = NOZZLE;
}

CSimuFluidNozzle::CSimuFluidNozzle(SimuValue length, SimuValue radius,
								   CVector3D* topCtrPos, CVector3D* direction)
: CSimuBoundaryCylinder(length, radius, topCtrPos, direction)
{
	m_bdryType = NOZZLE;

	m_bcOpenTop = true;

	m_fnTurnOff = true;
	m_fnExitSpeed = 0;
	m_fnLastEmptyLength = 0;
	m_fnLastInjectionTime = 0;
	m_fnFrictionEffectiveTime = SIMU_NEGATIVE_NUMBER; // initially, no friction
	m_fnStopFlowingTime = SIMU_NEGATIVE_NUMBER; // initially, don not stop squeeze

	m_fnTotalLayers = 0;
}

CSimuFluidNozzle::~CSimuFluidNozzle()
{
}

void CSimuFluidNozzle::DrawFluidNozzle()
{
	DrawBoundary();
}

void CSimuFluidNozzle::ExportBoundaryParameters(FILE* fp)
{
	fprintf(fp, "exit speed=%f\n", m_fnExitSpeed);
	fprintf(fp, "turn off=%d\n", m_fnTurnOff);
	fprintf(fp, "friction effective time=%f\n", m_fnFrictionEffectiveTime);
	CSimuBoundaryCylinder::ExportBoundaryData(fp);
}

void CSimuFluidNozzle::ImportBoundaryParameters(FILE* fp)
{
	int tmpInt; float tmpFloat;
	fscanf(fp, "exit speed=%f\n", &tmpFloat); m_fnExitSpeed = tmpFloat;
	fscanf(fp, "turn off=%d\n", &tmpInt); m_fnTurnOff = tmpInt;
	fscanf(fp, "friction effective time=%f\n", &tmpFloat); m_fnFrictionEffectiveTime = tmpFloat;
	CSimuBoundaryCylinder::ImportBoundaryData(fp);
}

void CSimuFluidNozzle::EnforceBoundaryConstraint(CPrtlFluid* pPrtlFluid)
{
	for (int i=0; i < pPrtlFluid->m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = pPrtlFluid->m_pfPrtls[i];
		EnforceBoundaryConstraint(pPrtl, pPrtlFluid->m_pfTimeStep);
	}
}

bool CSimuFluidNozzle::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	CVector3D tmpPos, tmpVel;
	// transform prtl position and velocity back to original cylinder position
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPrtl->m_vpPos, tmpPos);
	RotateOrgVectToStdVect(*pPrtl->m_vpVel, tmpVel);
	if (CSimuBoundaryCylinder::EnforceBoundaryConstraint(tmpPos, tmpVel, timeStep))
		m_fnPrtlInsideNozzle = true;
	else
		m_fnPrtlInsideNozzle = false;
	RotateStdVectToOrgVect(tmpVel, *pPrtl->m_vpVel);
	RotateStdVectToOrgVect(tmpPos, *pPrtl->m_vpPos);
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
	return m_fnPrtlInsideNozzle;
}

void CSimuFluidNozzle::CheckFrictionStopTime(SimuValue simuTime)
{
	if (m_fnFrictionEffectiveTime != SIMU_NEGATIVE_NUMBER
	 && m_fnFrictionEffectiveTime <= simuTime)
		m_bdryFrictionRate = CSimuManager::m_frictionRate;
}

// freeze prtls inside nozzle from m_fnStopFlowingTime forward
void CSimuFluidNozzle::StopFlowing(SimuValue simuTime, CPrtlFluid* pPrtlFluid)
{
	if (m_bcOpenTop
	 && m_fnStopFlowingTime != SIMU_NEGATIVE_NUMBER
	 && m_fnStopFlowingTime <= simuTime)
	{
		CVector3D tmpVel;
		for (int i=0; i < pPrtlFluid->m_pfPrtls.m_paNum; i++)
		{
 			CFluidPrtl* pPrtl = pPrtlFluid->m_pfPrtls[i];
			if (IsPosInCylinder(pPrtl->m_vpPos))
			{
				// transform prtl velocity back to original cylinder position
				RotateOrgVectToStdVect(*pPrtl->m_vpVel, tmpVel);
				if (tmpVel[Z] > 0)
					tmpVel[Z] = 0; // cancel the velocity pointing outward open top
				RotateStdVectToOrgVect(tmpVel, *pPrtl->m_vpVel);
			}
		}
	}
}

void CSimuFluidNozzle::InjectFluidParticles(SimuValue curTime, CPrtlFluid* pPrtlFluid)
{
	if (m_fnTurnOff) return;

	SimuValue timeElapse = curTime - m_fnLastInjectionTime;
	m_fnLastEmptyLength += timeElapse*m_fnExitSpeed;
	m_fnLastInjectionTime = curTime;
	if (m_fnLastEmptyLength < pPrtlFluid->m_pfPrtlDist)
		return;
	CVector3D tmpPos;
	SimuValue prtlDistance = pPrtlFluid->m_pfPrtlDist;
	SimuValue nozzleLength = m_cylLength;
	SimuValue nozzleRadius = m_cylRadius;
	int verticalLayers = (int)floor(m_fnLastEmptyLength/prtlDistance);
	int horizontalLayers = 1+2*(int)ceil(m_cylRadius/prtlDistance);
	SimuValue minX = -prtlDistance*(horizontalLayers-1)/2;
	SimuValue minY = -prtlDistance*(horizontalLayers-1)/2;
	SimuValue maxZ = m_fnLastEmptyLength - prtlDistance;
	// prepare for next injection
	m_fnLastEmptyLength -= verticalLayers*prtlDistance;
	UINT d;
	SimuValue radiusLayer = prtlDistance/3;
	for (int i=0; i < verticalLayers; i++)
	{
		m_fnTotalLayers ++;
		for (int j=0; j < horizontalLayers; j++)
		{
			for (int k=0; k < horizontalLayers; k++)
			{
				tmpPos[Z] = maxZ - i*prtlDistance;
				tmpPos[X] = minX + j*prtlDistance;
				tmpPos[Y] = minY + k*prtlDistance;
				SimuValue tmpDist = sqrt(pow(tmpPos[X], 2)+pow(tmpPos[Y], 2));
				if (tmpDist > nozzleRadius + radiusLayer)
					continue;
				// enforce the height
//				if (tmpPos[Z] > nozzleLength)
//					tmpPos[Z] = nozzleLength;
//				if (tmpPos[Z] < 0)
//					tmpPos[z] = 0;
				// enforce the radius
				if (tmpDist > nozzleRadius)
				{
					tmpPos[X] *= nozzleRadius/tmpDist;
					tmpPos[Y] *= nozzleRadius/tmpDist;
				}
				CFluidPrtl* newPrtl = pPrtlFluid->CreateOneFluidPrtl();
				pPrtlFluid->m_pfPrtls.AppendOnePointer(newPrtl);
				RotateStdVectToOrgVect(tmpPos, *newPrtl->m_vpPos);
				newPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
				newPrtl->m_vpVel->SetValue(&m_rtzaOrgDirection, m_fnExitSpeed);
				newPrtl->m_fpMass = pPrtlFluid->m_pfPrtlMass;
				if (tmpDist > nozzleRadius - radiusLayer
				 || i == 0 || i == verticalLayers-1)
				{
					newPrtl->m_fpOnSurface = true;
					newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_SRFC);
				}
				else
					newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_INSIDE);
			}
		}
	}
}
