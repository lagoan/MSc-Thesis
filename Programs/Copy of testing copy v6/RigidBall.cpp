// RigidBall.cpp: implementation of the CRigidBall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "RigidBall.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CRigidBall::m_rbMaxId = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRigidBall::CRigidBall()
{
	InitializeRigidBall();

	m_rbFixed = true;
	m_rbMass = SIMU_POSITIVE_NUMBER;
	m_rbRadius = 0;
	m_rbRadiusSqre = 0;
}

CRigidBall::CRigidBall(SimuValue mass, SimuValue radius, CVector3D* ctrPos)
: CBdrySolidOval(radius, radius, radius, 0, ctrPos, NULL)
{
	InitializeRigidBall();

	m_rbFixed = true;
	m_rbMass = mass;
	m_rbRadius = radius;
	m_rbRadiusSqre = m_rbRadius*m_rbRadius;
}

CRigidBall::~CRigidBall()
{

}

void CRigidBall::InitializeRigidBall()
{
	m_bdryType = RIGID_BALL;

	m_rbMaxId++;
	char tmpName[LEN_BOUNDARY_NAME];
	sprintf(tmpName, "rigid_ball_%d", m_rbMaxId);
	m_deiName = tmpName;

	m_bdryColor->SetValue(CSimuManager::COLOR_MAGENTA);

	m_bdryDistDumpingRate	= 1;
	m_bdryEnergyDumpingRate	= 1;
	m_bdryFrictionRate		= 1;
	m_bdryStressDumpingRate	= 1;

	m_rbFrictionRate = 0.25;
	m_rbDistDumpingRate = 0.25;
	m_rbEnergyDumpingRate = 0.25;
}

void CRigidBall::DrawBoundary()
{
	const int slices = 100;
	const int stacks = 100;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	SimuColor3v(m_bdryColor->v);
	GLUquadricObj* qObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(m_ovalCtrPos[X], m_ovalCtrPos[Y], m_ovalCtrPos[Z]);
	gluSphere(qObj, m_rbRadius, slices, stacks);
	glPopMatrix();
	gluDeleteQuadric(qObj);
	glPopAttrib();
}

void CRigidBall::ExportBoundaryData(FILE* fp)
{
	fprintf(fp, "#declare %s__m_rbRadius = %f;\n", m_deiName, m_rbRadius);
	fprintf(fp, "#declare %s__m_ovalCtrPos = <%f, %f, %f>;\n", m_deiName,
							  m_ovalCtrPos[X],
							  m_ovalCtrPos[Y],
							  m_ovalCtrPos[Z]);
}

void CRigidBall::ImportBoundaryData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float tmpFloat, x, y, z;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_rbRadius = tmpFloat;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_ovalCtrPos[X] = x;
							m_ovalCtrPos[Y] = y;
							m_ovalCtrPos[Z] = z;
	m_deiName = "rigid_ball";
}

void CRigidBall::MoveBoundary(SimuValue curTime, SimuValue timeStep)
{
	if (m_rbFixed) return;
	m_bdryVel.AddedBy(CSimuManager::GRAVITY, timeStep);
	m_ovalCtrPos.AddedBy(&m_bdryVel, timeStep);
}

void CRigidBall::EnforceBoundaryConstraint(CPrtlFluid* pPrtlFluid)
{
	CVector3D deltaVelocity, deltaMomentum;
	deltaMomentum.SetValue((SimuValue)0);
	for (int i=0; i < pPrtlFluid->m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = pPrtlFluid->m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->m_vpVel->AddedBy(&m_bdryVel, -1);
		deltaVelocity.SetValue(pPrtl->m_vpVel);
		if (EnforceBoundaryConstraint(pPrtl, pPrtlFluid->m_pfTimeStep))
		{
			pPrtl->m_fpInCollision = true;
			deltaVelocity.AddedBy(pPrtl->m_vpVel, -1);
			deltaMomentum.AddedBy(&deltaVelocity, pPrtl->m_fpMass);
		}
		pPrtl->m_vpVel->AddedBy(&m_bdryVel);
	}
	m_bdryVel.AddedBy(&deltaMomentum, CSimuManager::m_ballDumpingRate/m_rbMass);
}

bool CRigidBall::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	char location[] = "CRigidBall::EnforceBoundaryConstraint(...)";

	CVector3D relaPos;
	relaPos.SetValueAsSubstraction(pPrtl->m_vpPos, &m_ovalCtrPos);
	SimuValue distSqre = relaPos.LengthSquare();
	if (distSqre >= m_rbRadiusSqre)
		return false;
	// find the intersection pos
	SimuValue* vel = pPrtl->m_vpVel->v;
	SimuValue coeff[3];
	coeff[0] = distSqre - m_rbRadiusSqre;
	coeff[1] = relaPos[X]*vel[X] + relaPos[Y]*vel[Y] + relaPos[Z]*vel[Z];
	coeff[1] *= 2;
	coeff[2] = pPrtl->m_vpVel->LengthSquare();
	SimuValue t[2];
	int result = CSimuUtility::SolveQuadraticEquation(coeff, t);
	if (result == 0)
		CSimuMsg::ExitWithMessage(location, "There must be at least one solution.");
	else if (result == 1)
		return false; // velocity tangentially pass by the ball
	// else result == 2
	if (fabs(t[0]) > fabs(t[1]))
		t[0] = t[1];
	CVector3D intersectPos;
	intersectPos.SetValue(&relaPos);
	intersectPos.AddedBy(pPrtl->m_vpVel, t[0]);
	CVector3D planeNormal; // reflection plane
	planeNormal.SetValue(&intersectPos);
	planeNormal.Normalize();
	SimuValue planeD = intersectPos.Length();
	SimuValue reflectValue = 2*fabs(planeD - relaPos.DotProduct(&planeNormal));
	// relaPos is the completely reflected position
	relaPos.AddedBy(&planeNormal, reflectValue);
	// compute relaPos under m_bdryEnergyDumpingRate
	relaPos.ScaledBy(m_bdryEnergyDumpingRate);
	relaPos.AddedBy(&intersectPos, 1 - m_bdryEnergyDumpingRate);
	// compute translated prtl position
	pPrtl->m_vpPos->SetValue(&relaPos);
	pPrtl->m_vpPos->AddedBy(&m_ovalCtrPos);
	// adjust velocity
	SimuValue verticalSpeed = planeNormal.DotProduct(pPrtl->m_vpVel);
	// get tangential velocity
	pPrtl->m_vpVel->AddedBy(&planeNormal, -verticalSpeed);
	// apply friction to tangential velocity
	pPrtl->m_vpVel->ScaledBy(1 - m_bdryFrictionRate);
	// add dumped vertical velocity
	pPrtl->m_vpVel->AddedBy(&planeNormal, -verticalSpeed*m_bdryEnergyDumpingRate);
	return true;
}

bool CRigidBall::PosBreakBoundary(CVector3D* pPos)
{
	SimuValue distSqre = pPos->GetDistanceSquareToVector(&m_ovalCtrPos);
	if (distSqre < m_rbRadiusSqre)
		return true;
	return false;
}

// return 0 if on or inside ball; otherwise, return positive value.
SimuValue CRigidBall::GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl)
{
	SimuValue distance = pPrtl->m_vpPos->GetDistanceToVector(&m_ovalCtrPos);
	if (distance > m_ovalRadiusOnX)
		return distance - m_ovalRadiusOnX;
	return 0;
}

void CRigidBall::EnforceBallMotionWithBoundary(CBoundary* pBdry)
{
	if (pBdry->m_bdryType == SQUARE_SLOPE)
	{
		CBdrySquareSlope* pSlope = (CBdrySquareSlope*)pBdry;
		CVector3D translatePos, stdPos, stdVel;
		translatePos.SetValueAsSubstraction(&m_ovalCtrPos, &pSlope->m_bssCtrPos);
		pSlope->RotateOrgVectToStdVect(translatePos, stdPos);
		pSlope->RotateOrgVectToStdVect(m_bdryVel, stdVel);
		stdPos[Z] -= m_rbRadius;
		if (stdPos[Z] < 0
		 && fabs(stdPos[Z]) < pSlope->m_bssSlopeThickness
		 && fabs(stdPos[X]) <= pSlope->m_bssSideLen
		 && fabs(stdPos[Y]) <= pSlope->m_bssSideLen)
		{
			if (stdVel[Z] >= 0)
			{
				// enforce position
				stdPos[Z] = -stdPos[Z]*m_rbDistDumpingRate;
				// enforce velocity
				stdVel[Z] = stdVel[Z]*m_rbEnergyDumpingRate;
			}
			else
			{
				// compute intersection with boundary along velocity
				CVector3D intersectPos;
				SimuValue t = fabs(stdPos[Z])/fabs(stdVel[Z]);
				intersectPos.SetValue(&stdPos);
				intersectPos.AddedBy(&stdVel, -t); // move backwards along velocity
				// enforce position
				CVector3D relativePos;
				relativePos.SetValueAsSubstraction(&stdPos, &intersectPos);
				relativePos[Z] = -relativePos[Z]*m_rbDistDumpingRate;
				relativePos[X] *= m_rbFrictionRate;
				relativePos[Y] *= m_rbFrictionRate;
				stdPos.SetValueAsAddition(&intersectPos, &relativePos);
				// enforce velocity
				stdVel[Z] = -stdVel[Z]*m_rbEnergyDumpingRate;
			}
			stdVel[X] *= m_rbFrictionRate;
			stdVel[Y] *= m_rbFrictionRate;
		}
		stdPos[Z] += m_rbRadius;
		pSlope->RotateStdVectToOrgVect(stdVel, m_bdryVel);
		pSlope->RotateStdVectToOrgVect(stdPos, translatePos);
		m_ovalCtrPos.SetValueAsAddition(&translatePos, &pSlope->m_bssCtrPos);
	}
}
