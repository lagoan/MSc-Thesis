// BdryHollowCone.cpp: implementation of the CBdryHollowCone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BdryHollowCone.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CBdryHollowCone::m_hcMaxId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBdryHollowCone::CBdryHollowCone()
{
	InitializeHollowCone();
}

CBdryHollowCone::CBdryHollowCone(SimuValue coneLen, SimuValue coneTopRadius,
								 SimuValue coneBtmRadius,
								 CVector3D* btmCtrPos, CVector3D* direction)
: CConeAlongZAxis(coneLen, coneTopRadius, coneBtmRadius, btmCtrPos, direction)
{
	InitializeHollowCone();
}

CBdryHollowCone::~CBdryHollowCone()
{

}

void CBdryHollowCone::InitializeHollowCone()
{
	m_bdryType = HOLLOW_CONE;

	m_hcMaxId ++;
	char tmpName[LEN_BOUNDARY_NAME];
	sprintf(tmpName, "hollow_cone_%d", m_hcMaxId);
	m_deiName = tmpName;
}

void CBdryHollowCone::DrawBoundary()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	SimuColor3v(m_bdryColor->v);
	CConeAlongZAxis::DrawConeAlongZAxis();
	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CBdryHollowCone::ExportBoundaryData(FILE* fp)
{
	CBoundary::ExportBoundaryData(fp);

	ExportConeData(m_deiName, fp);
}

void CBdryHollowCone::ExportBoundaryParameters(FILE* fp)
{
	ExportBoundaryData(fp);
}

void CBdryHollowCone::ImportBoundaryData(FILE* fp)
{
	CBoundary::ImportBoundaryData(fp);

	ImportConeData(fp);
}

void CBdryHollowCone::ImportBoundaryParameters(FILE* fp)
{
	ImportBoundaryData(fp);
}

// return true if collision occurs; otherwise false.
bool CBdryHollowCone::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	CVector3D orgPos, orgVel;
	// transform prtl position and velocity back to original cylinder position
	pPrtl->m_vpPos->AddedBy(&m_coneBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPrtl->m_vpPos, orgPos);
	RotateOrgVectToStdVect(*pPrtl->m_vpVel, orgVel);
	bool bCollisionOccurs = EnforceBoundaryConstraint(orgPos, orgVel, timeStep);
	CSimuUtility::AssertSimuVector3D(&orgPos);
	if (CSimuManager::m_bInvalidValue)
	{
		RotateOrgVectToStdVect(*pPrtl->m_vpPos, orgPos);
		RotateOrgVectToStdVect(*pPrtl->m_vpVel, orgVel);
		EnforceBoundaryConstraint(orgPos, orgVel, timeStep);
	}
	RotateStdVectToOrgVect(orgVel, *pPrtl->m_vpVel);
	RotateStdVectToOrgVect(orgPos, *pPrtl->m_vpPos);
	pPrtl->m_vpPos->AddedBy(&m_coneBtmCtrPos);
	return bCollisionOccurs;
}

bool CBdryHollowCone::EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel,
												SimuValue timeStep)
{
	char location[] = "CBdryHollowCone::EnforceBoundaryConstraint(...)";

	if (pPos[Z] >= m_coneLength) // prtl is on or over cylinder top.
	{
		return false;
	}
	else if (pPos[Z] <= 0) // prtl is on or below cylinder bottom.
	{
		return false;
	}
	SimuValue distToZAxis = sqrt(pPos[X]*pPos[X] + pPos[Y]*pPos[Y]);
	SimuValue radius = GetRadius(pPos[Z]);
	const SimuValue coneThickness = CSimuManager::m_prtlDistance;
	if (distToZAxis >= radius + coneThickness) // prtl is on or outside cone wall.
		return false;

	// collision occurs
	CVector3D tmpVector;
	tmpVector.SetValueAsSubstraction(&pPos, &m_coneStandardTipPos);
	SimuValue tmpLen = tmpVector.Length();
	tmpVector.ScaledBy(1/tmpLen);
	if (tmpVector[Z] >= 0)
		CSimuMsg::ExitWithMessage(location, "particle should be below upper tip position.");
	else if (-tmpVector[Z] < m_coneCosHalfAngleOfTip)
	{	// particle is outside upper cone
		SimuValue curAngle = acos(-tmpVector[Z]);
		SimuValue reflectAngle = (curAngle - m_coneHalfAngleOfTip)*m_bdryDistDumpingRate;
		SimuValue newAngle = m_coneHalfAngleOfTip - reflectAngle;
		if (newAngle < 0)
		{
			newAngle = 0;
			CSimuMsg::ExitWithMessage(location, "particle is too far away from upper cone.");
		}
		CVector3D tmpNormal;
		tmpNormal.SetValue(&tmpVector);
		tmpVector[Z] = 0;
		tmpVector.Normalize(tmpLen*sin(newAngle));
		tmpVector[Z] = -tmpLen*cos(newAngle);
		pPos.SetValueAsAddition(&m_coneStandardTipPos, &tmpVector);
		// reflect velocity
		tmpVector.Normalize();
		tmpNormal.AddedBy(&tmpVector, -1);
		tmpNormal.Normalize();
		SimuValue reflectValue = pVel.DotProduct(&tmpNormal);
		if (reflectValue > 0)
			pVel.AddedBy(&tmpNormal, -reflectValue*(1+m_bdryEnergyDumpingRate));
		else
			pVel.AddedBy(&tmpNormal, -reflectValue*(1-m_bdryEnergyDumpingRate));
	}
	return true;
}
