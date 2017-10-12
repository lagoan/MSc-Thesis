// BdrySolidBox.cpp: implementation of the CBdrySolidBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BdrySolidBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CBdrySolidBox::m_bsbMaxId = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBdrySolidBox::CBdrySolidBox()
{
	InitializeSolidBox();

	m_bsbHalfX = 0;
	m_bsbHalfY = 0;
}

CBdrySolidBox::CBdrySolidBox(SimuValue heightOnZ, SimuValue widthOnX, SimuValue widthOnY,
							 SimuValue angleAboutZ, CVector3D* btmCtrPos, CVector3D* direction)
: CBoxOnXYPlane(heightOnZ, widthOnX, widthOnY, angleAboutZ, btmCtrPos, direction)
{
	InitializeSolidBox();

	m_bsbHalfX = widthOnX/2;
	m_bsbHalfY = widthOnY/2;
}

CBdrySolidBox::~CBdrySolidBox()
{

}

void CBdrySolidBox::InitializeSolidBox()
{
	m_bdryType = SOLID_BOX;
	m_bdryColor->SetValue(CSimuManager::COLOR_CYAN);

	m_bsbMaxId++;
	char tmpName[LEN_BOUNDARY_NAME];
	sprintf(tmpName, "solid_box_%d", m_bsbMaxId);
	m_deiName = tmpName;
}

void CBdrySolidBox::DrawBoundary()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	SimuColor3v(m_bdryColor->v);
	glPushMatrix();
	SimuTranslate(m_boxBtmCtrPos[X], m_boxBtmCtrPos[Y], m_boxBtmCtrPos[Z]);
	SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
	SimuRotate(m_rtzaRotationAngleAboutZAxis, 0, 0, 1);
	// draw box at the standard position
	glBegin(GL_QUADS);
	// left face
	SimuNormal3d(-1, 0, 0);
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	// right face
	SimuNormal3d(1, 0, 0);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	// front face
	SimuNormal3d(0, 0, 1);
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	// back face
	SimuNormal3d(0, 0, -1);
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	// bottom face
	SimuNormal3d(0, -1, 0);
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	// top face
	SimuNormal3d(0, 1, 0);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	glEnd();
	// draw direction
	SimuColor3v(CSimuManager::COLOR_RED->v);
	glBegin(GL_LINES);
	SimuVertex3d(0, 0, m_boxHeightOnZ);
	SimuVertex3d(0, 0, 100+m_boxHeightOnZ);
	glEnd();
	glEnd();
	glPopMatrix();
	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CBdrySolidBox::ExportBoundaryData(FILE* fp)
{
	CBoundary::ExportBoundaryData(fp);
	ExportBoxData(m_deiName, fp);
}

void CBdrySolidBox::ImportBoundaryData(FILE* fp)
{
	CBoundary::ImportBoundaryData(fp);
	ImportBoxData(fp);
	m_deiName = "solid_box";
}

void CBdrySolidBox::MoveBoundary(SimuValue curTime, SimuValue timeStep)
{
	m_boxTopCtrPos.AddedBy(&m_bdryVel, timeStep);
	m_boxBtmCtrPos.AddedBy(&m_bdryVel, timeStep);

	m_rtzaRotationAngleAboutZAxis += m_bdryAngVel*timeStep;
	PrepareRotationMatrix();

	for (int i=0; i < m_bdryEnclosedPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_bdryEnclosedPrtls[i];
		RotateStdVectToOrgVect(*m_bdryOrgPrtlPos[i], *pPrtl->m_vpPos);
		pPrtl->m_vpPos->AddedBy(&m_boxBtmCtrPos);
		pPrtl->m_vpPos->AddedBy(&m_bdryVel, timeStep);
	}

	m_rtzaRotationAngleAboutZAxis += m_bdryAngVel*timeStep;
	PrepareRotationMatrix();

	SimuValue timeStepReciprocal = 1/timeStep;
	CVector3D nextPos;
	for (int i=0; i < m_bdryEnclosedPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_bdryEnclosedPrtls[i];
		RotateStdVectToOrgVect(*m_bdryOrgPrtlPos[i], nextPos);
		nextPos.AddedBy(&m_boxBtmCtrPos);
		pPrtl->m_vpVel->SetValueAsSubstraction(&nextPos, pPrtl->m_vpPos);
		pPrtl->m_vpVel->ScaledBy(timeStepReciprocal);
		pPrtl->m_vpVel->AddedBy(&m_bdryVel);
	}

	m_rtzaRotationAngleAboutZAxis -= m_bdryAngVel*timeStep;
	PrepareRotationMatrix();
}

// return true if collision occurs; otherwise false.
bool CBdrySolidBox::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	CVector3D orgPos, orgVel;
	// transform prtl position and velocity back to original box position
	pPrtl->m_vpPos->AddedBy(&m_boxBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPrtl->m_vpPos, orgPos);
	RotateOrgVectToStdVect(*pPrtl->m_vpVel, orgVel);
	bool bCollisionOccurs = EnforceBoundaryConstraint(orgPos, orgVel, timeStep);
	RotateStdVectToOrgVect(orgVel, *pPrtl->m_vpVel);
	RotateStdVectToOrgVect(orgPos, *pPrtl->m_vpPos);
	pPrtl->m_vpPos->AddedBy(&m_boxBtmCtrPos);
	return bCollisionOccurs;
}

bool CBdrySolidBox::PosBreakBoundary(CVector3D* pPos)
{
	CVector3D stdPos;
	// transform particle position back to original box position
	pPos->AddedBy(&m_boxBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPos, stdPos);
	pPos->AddedBy(&m_boxBtmCtrPos);
	return IsInsideBox(stdPos);
}

void CBdrySolidBox::AddEnclosedPrtl(CFluidPrtl* pPrtl)
{
	m_bdryEnclosedPrtls.AppendOnePointer(pPrtl);
	CVector3D* orgPos = m_bdryOrgPrtlPos.CreateOneElement();
	// get original position in box
	pPrtl->m_vpPos->AddedBy(&m_boxBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPrtl->m_vpPos, *orgPos);
	pPrtl->m_vpPos->AddedBy(&m_boxBtmCtrPos, 1);
}

// return 0 if on or inside box; otherwise, return positive value.
SimuValue CBdrySolidBox::GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl)
{
	CVector3D orgPos;
	GetStandardPosition(*pPrtl->m_vpPos, orgPos);

	return GetDistanceToBox(orgPos);
}

// return true if <pPos> is inside rod; otherwise, false.
// <pPos> is tested against the standard box position
bool CBdrySolidBox::IsInsideBox(CVector3D &pos)
{
	if (pos[X] <= -m_bsbHalfX || pos[X] >= m_bsbHalfX)
		return false;
	if (pos[Y] <= -m_bsbHalfY || pos[Y] >= m_bsbHalfY)
		return false;
	if (pos[Z] <= 0 || pos[Z] >= m_boxHeightOnZ)
		return false;
	return true;
}

// pPos and pVel are in standard box posture: sitting on Z= 0 plane
bool CBdrySolidBox::EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel,
											  SimuValue timeStep)
{
	char location[] = "CBdrySolidBox::EnforceBoundaryConstraint(...)";

	if (!IsInsideBox(pPos))
		return false;

	SimuValue tForSide[BOX_SIX_SIDES];
	if (fabs(pVel[X]) > SIMU_VALUE_EPSILON)
	{
		tForSide[BOX_SIDE_POS_X] = (m_bsbHalfX - pPos[X])/pVel[X];
		tForSide[BOX_SIDE_NEG_X] = (-m_bsbHalfX - pPos[X])/pVel[X];
	}
	else
	{
		tForSide[BOX_SIDE_POS_X] = SIMU_VALUE_MAX;
		tForSide[BOX_SIDE_NEG_X] = SIMU_VALUE_MAX;
	}
	if (fabs(pVel[Y]) > SIMU_VALUE_EPSILON)
	{
		tForSide[BOX_SIDE_POS_Y] = (m_bsbHalfY - pPos[Y])/pVel[Y];
		tForSide[BOX_SIDE_NEG_Y] = (-m_bsbHalfY - pPos[Y])/pVel[Y];
	}
	else
	{
		tForSide[BOX_SIDE_POS_Y] = SIMU_VALUE_MAX;
		tForSide[BOX_SIDE_NEG_Y] = SIMU_VALUE_MAX;
	}
	if (fabs(pVel[Z]) > SIMU_VALUE_EPSILON)
	{
		tForSide[BOX_SIDE_POS_Z] = (m_boxHeightOnZ - pPos[Z])/pVel[Z];
		tForSide[BOX_SIDE_NEG_Z] = (0 - pPos[Z])/pVel[Z];
	}
	else
	{
		tForSide[BOX_SIDE_POS_Z] = SIMU_VALUE_MAX;
		tForSide[BOX_SIDE_NEG_Z] = SIMU_VALUE_MAX;
	}

	SimuValue minTforSide = tForSide[BOX_SIDE_POS_X];
	UINT minTforSideId = BOX_SIDE_POS_X;
	UINT sideId = BOX_SIDE_POS_X + 1;
	while (sideId < BOX_SIX_SIDES)
	{
		if (fabs(minTforSide) > fabs(tForSide[sideId]))
		{
			minTforSide = tForSide[sideId];
			minTforSideId = sideId;
		}
		sideId = sideId + 1;
	}
	CVector3D sideNormal;
	switch (minTforSideId)
	{
		case BOX_SIDE_POS_X:
			sideNormal.SetElements(1, 0, 0);
			break;
		case BOX_SIDE_NEG_X:
			sideNormal.SetElements(-1, 0, 0);
			break;
		case BOX_SIDE_POS_Y:
			sideNormal.SetElements(0, 1, 0);
			break;
		case BOX_SIDE_NEG_Y:
			sideNormal.SetElements(0, -1, 0);
			break;
		case BOX_SIDE_POS_Z:
			sideNormal.SetElements(0, 0, 1);
			break;
		case BOX_SIDE_NEG_Z:
			sideNormal.SetElements(0, 0, -1);
			break;
		default:
			CSimuMsg::ExitWithMessage(location, "Unknown box side id.");
	}
	CVector3D intersectPos;
	intersectPos.SetValue(&pPos);
	intersectPos.AddedBy(&pVel, minTforSide);

	ReflectPosOnPlane(pPos, sideNormal, intersectPos);
	ReflectVelOnPlane(pVel, sideNormal);

	return false;
}
