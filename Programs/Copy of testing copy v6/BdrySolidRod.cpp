// BdrySolidRod.cpp: implementation of the CBdrySolidRod class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BdrySolidRod.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CBdrySolidRod::m_bsrMaxId = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBdrySolidRod::CBdrySolidRod()
{
	InitializeSolidRod();
}

CBdrySolidRod::CBdrySolidRod(SimuValue length, SimuValue radius,
							 SimuValue capHeight, bool bCylTopCap, bool bCylBtmCap,
							 CVector3D* topCtrPos, CVector3D* direction)
: CCylinderAlongZAxis(length, radius, capHeight, bCylTopCap, bCylBtmCap, topCtrPos, direction)
{
	InitializeSolidRod();
}

CBdrySolidRod::~CBdrySolidRod()
{

}

void CBdrySolidRod::InitializeSolidRod()
{
	m_bdryType = SOLID_ROD;
	m_bdryColor->SetValue(CSimuManager::COLOR_CYAN);

	m_bsrMaxId++;
	char tmpName[LEN_BOUNDARY_NAME];
	sprintf(tmpName, "solid_rod_%d", m_bsrMaxId);
	m_deiName = tmpName;
}

void CBdrySolidRod::DrawBoundary()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	SimuColor3v(m_bdryColor->v);
	CCylinderAlongZAxis::DrawCylinderAlongZAxis();
	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CBdrySolidRod::ExportBoundaryData(FILE* fp)
{
	ExportCylinderData(m_deiName, fp);
}

void CBdrySolidRod::ImportBoundaryData(FILE* fp)
{
	ImportCylinderData(fp);
	m_deiName = "solid_rod";
}

void CBdrySolidRod::MoveBoundary(SimuValue curTime, SimuValue timeStep)
{
	m_cylTopCtrPos.AddedBy(&m_bdryVel, timeStep);
	m_cylBtmCtrPos.AddedBy(&m_bdryVel, timeStep);

	m_rtzaRotationAngleAboutZAxis += m_bdryAngVel*timeStep;
	PrepareRotationMatrix();

	CVector3D stdVel;
	for (int i=0; i < m_bdryEnclosedPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_bdryEnclosedPrtls[i];
		// compute rotated position
		RotateStdVectToOrgVect(*m_bdryOrgPrtlPos[i], *pPrtl->m_vpPos);
		// add on translation
		pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
		// computat rotate vel
		ComputeAngularVelocity(m_bdryAngVel, timeStep, *m_bdryOrgPrtlPos[i], stdVel);
		RotateStdVectToOrgVect(stdVel, *pPrtl->m_vpVel);
		// add on translation vel
		pPrtl->m_vpVel->AddedBy(&m_bdryVel);
	}
}

// return true if collision occurs; otherwise false.
bool CBdrySolidRod::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	CVector3D orgPos, orgVel;
	// transform prtl position and velocity back to original cylinder position
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPrtl->m_vpPos, orgPos);
	RotateOrgVectToStdVect(*pPrtl->m_vpVel, orgVel);
	// the function here reflect prtl by pos intersected by pVel on boundary
	bool bCollisionOccurs = EnforceBoundaryConstraint(orgPos, orgVel, timeStep);
	// the function here reflect prtl by the closest pos on boundary
//	bool bCollisionOccurs = ReflectAtClosestPosOnBdry(orgPos, orgVel, timeStep);
	CSimuUtility::AssertSimuVector3D(&orgPos);
	if (CSimuManager::m_bInvalidValue)
	{
		RotateOrgVectToStdVect(*pPrtl->m_vpPos, orgPos);
		RotateOrgVectToStdVect(*pPrtl->m_vpVel, orgVel);
		EnforceBoundaryConstraint(orgPos, orgVel, timeStep);
	}
	RotateStdVectToOrgVect(orgVel, *pPrtl->m_vpVel);
	RotateStdVectToOrgVect(orgPos, *pPrtl->m_vpPos);
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
	return bCollisionOccurs;
}

bool CBdrySolidRod::PosBreakBoundary(CVector3D* pPos)
{
	CVector3D stdPos;
	// transform particle position back to original cylinder position
	pPos->AddedBy(&m_cylBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPos, stdPos);
	pPos->AddedBy(&m_cylBtmCtrPos);
	return IsInsideRod(stdPos);
}

void CBdrySolidRod::AddEnclosedPrtl(CFluidPrtl* pPrtl)
{
	m_bdryEnclosedPrtls.AppendOnePointer(pPrtl);
	CVector3D* orgPos = m_bdryOrgPrtlPos.CreateOneElement();
	// get original position in box
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPrtl->m_vpPos, *orgPos);
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos, 1);
}

SimuValue CBdrySolidRod::GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl)
{
	CVector3D orgPos;
	GetStandardPosition(*pPrtl->m_vpPos, orgPos);

	return GetDistanceToCylinder(orgPos);
}

// return true if <pPos> is inside rod; otherwise, false.
// <pPos> is tested against the standard cylinder position
bool CBdrySolidRod::IsInsideRod(CVector3D &pPos)
{
	if (pPos[Z] >= m_cylLength) // prtl is on or over cylinder top.
	{
		return false;
	}
	else if (pPos[Z] <= 0) // prtl is on or below cylinder bottom.
	{
		return false;
	}

	SimuValue distToZAxis = sqrt(pPos[X]*pPos[X] + pPos[Y]*pPos[Y]);
	if (distToZAxis >= m_cylRadius) // prtl is on or outside cylinder wall.
		return false;
	else
		return true;
}

// pPos and pVel are the vectors in the transferred coordinates.
// return true if collision occurs; otherwise, false.
bool CBdrySolidRod::ReflectAtClosestPosOnBdry(CVector3D &pPos, CVector3D &pVel,
											  SimuValue timeStep)
{
	SimuValue distToRod = GetDistanceToCylinder(pPos);
	if (distToRod > 0)
	{
		if (fabs(m_bdryAngVel) > SIMU_VALUE_EPSILON // no rotation
		 && distToRod <= CSimuManager::m_prtlDistance*0.5)
		{
			SimuValue weight = 1 - distToRod/(CSimuManager::m_prtlDistance*0.5);
			AddAngularVelocity(weight*m_bdryAngVel*(1-m_bdryFrictionRate), timeStep, pPos, pVel);
		}
		return false;
	}

	// collision occurs
	CVector3D reflectNormal;
	CVector3D intersectPos;
	SimuValue distToZAxis = sqrt(pPos[X]*pPos[X] + pPos[Y]*pPos[Y]);
	SimuValue distToTop = fabs(pPos[Z] - m_cylLength);
	SimuValue distToBtm = fabs(pPos[Z]);
	SimuValue distToWall = fabs(distToZAxis - m_cylRadius);
	if (distToWall <= distToTop && distToWall <= distToBtm)
	{
		// reflect prtl on wall
		reflectNormal[X] = pPos[X];
		reflectNormal[Y] = pPos[Y];
		reflectNormal[Z] = 0;
		reflectNormal.Normalize();
		// use closest pos on wall as intersection pos
		intersectPos.SetValue(&pPos);
		intersectPos.AddedBy(&reflectNormal, distToWall);
	}
	else if (distToTop <= distToBtm)
	{
		// use closest pos on top as intersection pos
		intersectPos.SetValue(&pPos);
		intersectPos[Z] = m_cylLength;
		reflectNormal.SetElements(0, 0, 1);
	}
	else // (distToTop > distToBtm)
	{
		// use closest pos on btm as intersection pos
		intersectPos.SetValue(&pPos);
		intersectPos[Z] = 0;
		reflectNormal.SetElements(0, 0, -1);
	}
	ReflectPosOnPlane(pPos, reflectNormal, intersectPos);
	ReflectVelOnPlane(pVel, reflectNormal);
	AddAngularVelocity(m_bdryAngVel*(1-m_bdryFrictionRate), timeStep, pPos, pVel);
	return true;
}

// pPos and pVel are the vectors in the transferred coordinates.
// return true if collision occurs; otherwise, false.
bool CBdrySolidRod::EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel,
											  SimuValue timeStep)
{
	SimuValue distToRod = GetDistanceToCylinder(pPos);
	if (distToRod > 0)
	{
		return false;
		if (fabs(m_bdryAngVel) > SIMU_VALUE_EPSILON // no rotation
		 && distToRod <= CSimuManager::m_prtlDistance*0.5)
		{
			SimuValue weight = 1 - distToRod/(CSimuManager::m_prtlDistance*0.5);
			AddAngularVelocity(weight*m_bdryAngVel*(1-m_bdryFrictionRate), timeStep, pPos, pVel);
		}
	}

	char location[] = "CBdrySolidRod::EnforceBoundaryConstraint(pPos, pVel)";

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
			AddAngularVelocity(m_bdryAngVel*(1-m_bdryFrictionRate), timeStep, pPos, pVel);
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
		// this code assumes that the velocity is towards inside rod
		// and moves prtl backwards
		if (t[0] > t[1])
			t[0] = t[1]; // 2 solutions are one > 0 and the other < 0
		if (t[0] > 0)
			CSimuMsg::ExitWithMessage(location, "Velocity is not towards inside rod.");

		// this code moves prtl to the closest intersection on rod
//		if (fabs(t[0]) > fabs(t[1]))
//			t[0] = t[1];
		CSimuUtility::AssertSimuValue(t[0]);
		intersectPos.SetValue(&pPos);
		intersectPos.AddedBy(&pVel, t[0]);
		if (intersectPos[Z] < m_cylLength && intersectPos[Z] > 0)
		{
			reflectPlaneNormal.SetValue(&intersectPos);
			reflectPlaneNormal[Z] = 0;
			reflectPlaneNormal.Normalize();
			ReflectPosOnPlane(pPos, reflectPlaneNormal, intersectPos);
			ReflectVelOnPlane(pVel, reflectPlaneNormal);
			AddAngularVelocity(m_bdryAngVel*(1-m_bdryFrictionRate), timeStep, pPos, pVel);
			return true;
		}
//		else
//			CSimuMsg::ExitWithMessage(location, "Not intersect with the wall.");
	}
	else if (result == 1)
		return false; //	CSimuMsg::ExitWithMessage(location, "On the wall.");
	else // (result == 0)
		CSimuMsg::ExitWithMessage(location,
								"Either not inside or already intersect top or bottom.");
//	CSimuMsg::ExitWithMessage(location,
//							"Non-reachable code since all cases are covered by 'if-block' right before.");
	return true;
}

// return true if collision occurs; otherwise, false.
bool CBdrySolidRod::EnforceCylinderWallBoundary(CVector3D &pPos, CVector3D &pVel)
{
	bool bCollisionOccurs = false;
	SimuValue tmpZ = pPos[Z];
	pPos[Z] = 0;	// now, pPos is vertical to z axis
	SimuValue tmpDist = m_cylRadius - pPos.Length();
	if (tmpDist > 0) // inside cylinder wall
	{
		// reflect position w.r.t the tagent plane
		SimuValue tmpLen = m_cylRadius + tmpDist*m_bdryDistDumpingRate;
		pPos.Normalize(tmpLen);
		// reflect velocity
		// tmpX and tmpY are normalized values for computing reflection value
		SimuValue tmpX = pPos[X]/tmpLen;
		SimuValue tmpY = pPos[Y]/tmpLen;
		// reflect value is the dot product of velocity in x-y plane with reflect plane normal
		SimuValue reflectValue = pVel[X]*tmpX + pVel[Y]*tmpY;
		if (reflectValue < 0)
		{
			pVel[X] += -tmpX*(1+m_bdryEnergyDumpingRate)*reflectValue;
			pVel[Y] += -tmpY*(1+m_bdryEnergyDumpingRate)*reflectValue;
			pVel[Z] *= m_bdryFrictionRate;
		}
		bCollisionOccurs = true;
	}
	pPos[Z] = tmpZ;
	return bCollisionOccurs;
}
