// PrtlInOval.cpp: implementation of the CPrtlInOval class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "PrtlInOval.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtlInOval::CPrtlInOval(SimuValue radiusOnX, SimuValue radiusOnY, SimuValue radiusOnZ,
						 SimuValue angleAboutZ, CVector3D* ctrPos, CVector3D* direction)
:COvalAlong3DAxes(radiusOnX, radiusOnY, radiusOnZ, angleAboutZ, ctrPos, direction)
{

}

CPrtlInOval::~CPrtlInOval()
{

}

void CPrtlInOval::CreatePrtlInOval(CPrtlFluid* prtlFluid, SimuValue srfcThickness,
								   SimuValue hollowOvalRatio)
{
	CVector3D iniVel;
	iniVel.SetValue((SimuValue)0);
	CreatePrtlInOval(prtlFluid, srfcThickness, hollowOvalRatio, &iniVel);
}

void CPrtlInOval::CreatePrtlInOval(CPrtlFluid* prtlFluid, SimuValue srfcThickness,
								   SimuValue hollowOvalRatio, CVector3D* iniVelocity)
{
	SimuValue prtlDistance = prtlFluid->m_pfPrtlDist;
	int prtlLayerX = 1+(int)ceil(m_ovalRadiusOnX*2/prtlDistance);
	int prtlLayerY = 1+(int)ceil(m_ovalRadiusOnY*2/prtlDistance);
	int prtlLayerZ = 1+(int)ceil(m_ovalRadiusOnZ*2/prtlDistance);
	SimuValue minX = -prtlDistance*(prtlLayerX-1)/2;
	SimuValue minY = -prtlDistance*(prtlLayerY-1)/2;
	SimuValue minZ = -prtlDistance*(prtlLayerZ-1)/2;
	SimuValue RX2 = m_ovalRadiusOnX*m_ovalRadiusOnX;
	SimuValue RY2 = m_ovalRadiusOnY*m_ovalRadiusOnY;
	SimuValue RZ2 = m_ovalRadiusOnZ*m_ovalRadiusOnZ;
	SimuValue normalizedThickness = srfcThickness*3/(m_ovalRadiusOnX+m_ovalRadiusOnY+m_ovalRadiusOnZ);
	CVector3D tmpPos;
	CFluidPrtl* newPrtl;
	for (int i=0; i < prtlLayerX; i++)
	{
		for (int j=0; j < prtlLayerY; j++)
		{
			for (int k=0; k < prtlLayerZ; k++)
			{
				tmpPos[X] = minX + i*prtlDistance;
				tmpPos[Y] = minY + j*prtlDistance;
				tmpPos[Z] = minZ + k*prtlDistance;
				CSimuUtility::JitterVectorValue(tmpPos, CSimuManager::m_prtlPosJitter*prtlDistance);
				SimuValue normalDist = pow(tmpPos[X], 2)/RX2+pow(tmpPos[Y], 2)/RY2+pow(tmpPos[Z], 2)/RZ2;
				normalDist = sqrt(normalDist);
				if (normalDist > 1 + normalizedThickness)
					continue;
				if (normalDist < hollowOvalRatio - normalizedThickness)
					continue;
				newPrtl = prtlFluid->CreateOneFluidPrtl();
				// enforce the oval radius
				if (normalDist > 1 - normalizedThickness)
				{
					newPrtl->m_fpOnSurface = true;
					newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_SRFC);
					tmpPos.NormalizeOntoOval(m_ovalRadiusOnX, m_ovalRadiusOnY, m_ovalRadiusOnZ);
				}
				if (hollowOvalRatio > normalizedThickness
					&& normalDist < hollowOvalRatio + normalizedThickness)
				{
					newPrtl->m_fpOnSurface = true;
					newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_SRFC);
					tmpPos.NormalizeOntoOval(m_ovalRadiusOnX*hollowOvalRatio,
											 m_ovalRadiusOnY*hollowOvalRatio,
											 m_ovalRadiusOnZ*hollowOvalRatio);
				}
				else
				{
					newPrtl->m_fpOnSurface = false;
					newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_INSIDE);
				}
				newPrtl->m_fpMass = prtlFluid->m_pfPrtlMass;
				RotateStdVectToOrgVect(tmpPos, *newPrtl->m_vpPos);
				newPrtl->m_vpPos->AddedBy(&m_ovalCtrPos);
				newPrtl->m_vpVel->SetValue(iniVelocity);
				newPrtl->m_fpFixedMotion = false;
				prtlFluid->m_pfPrtls.AppendOnePointer(newPrtl);
			}
		}
	}
}
