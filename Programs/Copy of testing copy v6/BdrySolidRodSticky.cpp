// BdrySolidRodSticky.cpp: implementation of the CBdrySolidRodSticky class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BdrySolidRodSticky.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBdrySolidRodSticky::CBdrySolidRodSticky()
{

}

CBdrySolidRodSticky::CBdrySolidRodSticky(SimuValue length, SimuValue radius,
										 SimuValue capHeight, bool bCylTopCap, bool bCylBtmCap,
										 CVector3D* topCtrPos, CVector3D* direction)
: CBdrySolidRod(length, radius, capHeight, bCylTopCap, bCylBtmCap, topCtrPos, direction)
{
}

CBdrySolidRodSticky::~CBdrySolidRodSticky()
{

}

// return negative number if inside rod; otherwise, non-negative number.
SimuValue CBdrySolidRodSticky::ComputeDistanceToRod(CVector3D &pPos)
{
	SimuValue distToRod[3];
	distToRod[0] = pPos[Z] - m_cylLength;
	distToRod[1] = - pPos[Z];
	SimuValue distToZAxis = sqrt(pPos[X]*pPos[X] + pPos[Y]*pPos[Y]);
	distToRod[2] = distToZAxis - m_cylRadius;
	SimuValue minDistToRod = SIMU_VALUE_MAX;
	bool bInsideRod = true;
	for (int i=0; i < 3; i++)
	{
		if (minDistToRod > distToRod[i] && distToRod[i] >= 0)
		{
			minDistToRod = distToRod[i];
			bInsideRod = false;
		}
	}
	if (bInsideRod)
		return SIMU_NEGATIVE_NUMBER;
	return minDistToRod;
}

bool CBdrySolidRodSticky::EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel,
													SimuValue timeStep)
{
	char location[] = "CBdrySolidRodSticky::EnforceBoundaryConstraint(pPos, pVel, timeStep)";

	SimuValue distToRod = ComputeDistanceToRod(pPos);
	if (distToRod >= 0)
	{
		if (distToRod <= CSimuManager::m_prtlDistance)
		{
			SimuValue weight = CSimuUtility::SplineWeightFunction(distToRod, CSimuManager::m_prtlDistance);
			AddAngularVelocity(m_bdryAngVel*weight, timeStep, pPos, pVel);
		}
		return false;
	}

	CVector3D intersectPos;
	CVector3D reflectPlaneNormal;
	SimuValue t[2];
	if (fabs(pVel[Z]) > SIMU_VALUE_EPSILON)
	{
		t[0] = (m_cylLength - pPos[Z])/pVel[Z];
		reflectPlaneNormal.SetElements(0, 0, 1);
		if (t[0] > 0)
		{
			t[0] = - pPos[Z]/pVel[Z];
			reflectPlaneNormal.SetElements(0, 0, -1);
		}
		if (t[0] > 0)
			CSimuMsg::ExitWithMessage(location, "No intersection on cylinder top or bottom.");
		intersectPos.SetValue(&pPos);
		intersectPos.AddedBy(&pVel, t[0]);
		SimuValue radius = sqrt(pow(intersectPos[X], 2) + pow(intersectPos[Y], 2));
		if (radius < m_cylRadius) // intersect top or bottom before the wall
		{
			reflectPlaneNormal.Normalize();
			ReflectPosOnPlane(pPos, reflectPlaneNormal, intersectPos);
			ReflectVelOnPlane(pVel, reflectPlaneNormal);
			AddAngularVelocity(m_bdryAngVel, timeStep, pPos, pVel);
			return true;
		}
		// else: intersect the wall before the top or bottom
	}
	// else: intersect the wall only
	SimuValue coeff[3];
	coeff[2] = pVel[X]*pVel[X] + pVel[Y]*pVel[Y];
	coeff[1] = 2*(pPos[X]*pVel[X] + pPos[Y]*pVel[Y]);
	coeff[0] = pPos[X]*pPos[X] + pPos[Y]*pPos[Y] - m_cylRadius*m_cylRadius;
	int result = CSimuUtility::SolveQuadraticEquation(coeff, t);
	if (result == 2)
	{
		if (t[0] > t[1])
			t[0] = t[1]; // 2 solutions are one > 0 and the other < 0
		if (t[0] > 0)
			CSimuMsg::ExitWithMessage(location, "Velocity is not towards inside rod.");
//		CSimuUtility::AssertSimuValue(t[0]);
		intersectPos.SetValue(&pPos);
		intersectPos.AddedBy(&pVel, t[0]);
		if (intersectPos[Z] < m_cylLength && intersectPos[Z] > 0)
		{
			reflectPlaneNormal.SetValue(&intersectPos);
			reflectPlaneNormal[Z] = 0;
			reflectPlaneNormal.Normalize();
			ReflectPosOnPlane(pPos, reflectPlaneNormal, intersectPos);
			ReflectVelOnPlane(pVel, reflectPlaneNormal);
			AddAngularVelocity(m_bdryAngVel, timeStep, pPos, pVel);
			return true;
		}
		else
			CSimuMsg::ExitWithMessage(location, "Not intersect with the wall.");
	}
	else if (result == 1)
	{
		AddAngularVelocity(m_bdryAngVel, timeStep, pPos, pVel);
		return false; //	CSimuMsg::ExitWithMessage(location, "On the wall.");
	}
	else // (result == 0)
		CSimuMsg::ExitWithMessage(location,
								"Either not inside or already intersect top or bottom.");
	return true;
}
