// SimuBoundaryCylinder.cpp: implementation of the CSimuBoundaryCylinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuBoundaryCylinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CSimuBoundaryCylinder::m_bcMaxId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuBoundaryCylinder::CSimuBoundaryCylinder()
{
	InitializeCylinder();
}

CSimuBoundaryCylinder::CSimuBoundaryCylinder(SimuValue length, SimuValue radius,
											 CVector3D* topCtrPos, CVector3D* direction)
: CCylinderAlongZAxis(length, radius, 0, false, false, topCtrPos, direction)
{
	InitializeCylinder();
}

CSimuBoundaryCylinder::~CSimuBoundaryCylinder()
{
}

void CSimuBoundaryCylinder::InitializeCylinder()
{
	m_bdryType = CYLINDER;

	m_bcMaxId ++;
	char tmpName[LEN_BOUNDARY_NAME];
	sprintf(tmpName, "cylinder_%d", m_bcMaxId);
	m_deiName = tmpName;

	m_bcOpenTop = true;
	m_bcOpenBtm = false;

	m_bcOpenOutletWidth = CSimuManager::m_prtlDistance*2;
}

void CSimuBoundaryCylinder::DrawBoundary()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	SimuColor3v(m_bdryColor->v);
	// draw direction
	const bool bDrawDirection = false;
	if (bDrawDirection)
	{
		glBegin(GL_LINES);
		SimuVertex3d(m_cylTopCtrPos[X], m_cylTopCtrPos[Y], m_cylTopCtrPos[Z]);
		SimuVertex3d(m_cylTopCtrPos[X] + m_cylLength*m_rtzaOrgDirection[X],
					m_cylTopCtrPos[Y] + m_cylLength*m_rtzaOrgDirection[Y],
					m_cylTopCtrPos[Z] + m_cylLength*m_rtzaOrgDirection[Z]);
		glEnd();
	}

	GLUquadricObj* qObj = gluNewQuadric();
	const int numSlices = 60;
	const int numLoops = 20;
	const int numStacks = 20;
	const SimuValue wallThickness = 0;
	const SimuValue drawnRadius = m_cylRadius + wallThickness;
	// draw top circle
	glPushMatrix();
	SimuTranslate(m_cylTopCtrPos[X], m_cylTopCtrPos[Y], m_cylTopCtrPos[Z]);
	SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
	if (m_bcOpenTop)
	{
//		gluDisk(qObj, m_cylRadius, m_cylRadius+m_bcOpenOutletWidth, numSlices, numLoops);
	}
	else
		gluDisk(qObj, 0, drawnRadius, numSlices, numLoops);
	glPopMatrix();
	// draw bottom circle
	glPushMatrix();
	SimuTranslate(m_cylBtmCtrPos[X], m_cylBtmCtrPos[Y], m_cylBtmCtrPos[Z]);
	SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
	if (m_bcOpenBtm)
	{
//		gluDisk(qObj, drawnRadius, m_cylRadius+m_bcOpenOutletWidth, numSlices, numLoops);
	}
	else
		gluDisk(qObj, 0, drawnRadius, numSlices, numLoops);
	glPopMatrix();
	const bool bDrawCylinderWall = true;
	if (bDrawCylinderWall)
	{
		glPushMatrix();
		SimuTranslate(m_cylBtmCtrPos[X], m_cylBtmCtrPos[Y], m_cylBtmCtrPos[Z]);
		SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
		CSimuUtility::DrawCircleOnZ0ByOpenGL(drawnRadius, 0, numSlices);
		CSimuUtility::DrawCircleOnZ0ByOpenGL(drawnRadius, m_cylLength, numSlices);
		CSimuUtility::DrawCylinderOnZ0ByOpenGL(drawnRadius, m_cylLength, numSlices);
//		gluCylinder(qObj, drawnRadius, drawnRadius, m_cylLength, numSlices, numStacks);
		glPopMatrix();
	}
	gluDeleteQuadric(qObj);

	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CSimuBoundaryCylinder::ExportBoundaryData(FILE* fp)
{
	CBoundary::ExportBoundaryData(fp);

	fprintf(fp, "#declare %s__m_cylLength = %f;\n", m_deiName, m_cylLength);
	fprintf(fp, "#declare %s__m_cylRadius = %f;\n", m_deiName, m_cylRadius);
	fprintf(fp, "#declare %s__m_bcOpenOutletWidth = %f;\n", m_deiName, m_bcOpenOutletWidth);
	fprintf(fp, "#declare %s__m_cylTopCtrPos = <%f, %f, %f>;\n", m_deiName,
							  m_cylTopCtrPos[X],
							  m_cylTopCtrPos[Y],
							  m_cylTopCtrPos[Z]);
	fprintf(fp, "#declare %s__m_cylBtmCtrPos = <%f, %f, %f>;\n", m_deiName,
							  m_cylBtmCtrPos[X],
							  m_cylBtmCtrPos[Y],
							  m_cylBtmCtrPos[Z]);
	ExportRotationData(m_deiName, fp);
}

void CSimuBoundaryCylinder::ExportBoundaryParameters(FILE* fp)
{
	fprintf(fp, "top open=%d\n", m_bcOpenTop);
	fprintf(fp, "btm open=%d\n", m_bcOpenBtm);
	fprintf(fp, "friction rate=%f\n", m_bdryFrictionRate);
	ExportBoundaryData(fp);
}

void CSimuBoundaryCylinder::ImportBoundaryData(FILE* fp)
{
	CBoundary::ImportBoundaryData(fp);

	char tmpName[LEN_FLUID_NAME];
	float tmp, x, y, z;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmp); m_cylLength = tmp;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmp); m_cylRadius = tmp;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmp); m_bcOpenOutletWidth = tmp;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_cylTopCtrPos[X] = x;
							m_cylTopCtrPos[Y] = y;
							m_cylTopCtrPos[Z] = z;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_cylBtmCtrPos[X] = x;
							m_cylBtmCtrPos[Y] = y;
							m_cylBtmCtrPos[Z] = z;
	ImportRotationData(fp);
	m_deiName = tmpName;
}

void CSimuBoundaryCylinder::ImportBoundaryParameters(FILE* fp)
{
	int tmpInt; float tmpFloat;
	fscanf(fp, "top open=%d\n", &tmpInt); m_bcOpenTop = tmpInt;
	fscanf(fp, "btm open=%d\n", &tmpInt); m_bcOpenBtm = tmpInt;
	fscanf(fp, "friction rate=%f\n", &tmpFloat); m_bdryFrictionRate = tmpFloat;
	ImportBoundaryData(fp);
}

// return true if collision occurs; otherwise false.
bool CSimuBoundaryCylinder::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	CVector3D tmpPos, tmpVel;
	// transform prtl position and velocity back to original cylinder position
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPrtl->m_vpPos, tmpPos);
	RotateOrgVectToStdVect(*pPrtl->m_vpVel, tmpVel);
	bool bCollisionOccurs = EnforceBoundaryConstraint(tmpPos, tmpVel, timeStep);
	RotateStdVectToOrgVect(tmpVel, *pPrtl->m_vpVel);
	RotateStdVectToOrgVect(tmpPos, *pPrtl->m_vpPos);
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
	return bCollisionOccurs;
}

void CSimuBoundaryCylinder::EnforceBoundaryConstraint(CFluidMeshPrtl* pPrtl)
{
	CVector3D tmpPos;
	// transform prtl position back to original cylinder position
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPrtl->m_vpPos, tmpPos);
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
	if (IsInCylinder(tmpPos))
		return;
	CVector3D tmpVel;
	// transform prtl velocity back to original cylinder position
	RotateOrgVectToStdVect(*pPrtl->m_vpVel, tmpVel);
	while (1)
	{
		int enforceTop, enforceBtm;
		enforceTop = EnforceCylinderTopBoundary(tmpPos, tmpVel);
		if (enforceTop == 0)
			return; // over open top and pPos and vel are not changed
		enforceBtm = EnforceCylinderBtmBoundary(tmpPos, tmpVel);
		if (enforceBtm == 0)
			return; // below open bottom and pPos and vel are not changed
		if (enforceTop == 1 || enforceBtm == 1)
		{// this means close top or bottom is enforced and <tmpPos> is on top or bottom now.
			EnforceCylinderWallBoundary(tmpPos, tmpVel);
			break;
		}
		if (!pPrtl->m_mpOnSurface)
			break; // if inside prtl is outside boundary, leaving it alone would result in bad tet and thus trigger remeshing.
		CVector3D revNormal;
		RotateOrgVectToStdVect(pPrtl->m_mpNormal, revNormal);
		revNormal.ScaledBy(-1);
		if (IsBreakingCylinderWall(tmpPos, revNormal))
			EnforceCylinderWallBoundary(tmpPos, tmpVel);
		else if (IsBreakingCylinderOutlet(tmpPos, revNormal))
			EnforceCylinderOutletBoundary(tmpPos, tmpVel);
		break;
	}
	RotateStdVectToOrgVect(tmpVel, *pPrtl->m_vpVel);
	RotateStdVectToOrgVect(tmpPos, *pPrtl->m_vpPos);
	pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
}

bool CSimuBoundaryCylinder::PosBreakBoundary(CVector3D* pPos)
{
	CVector3D stdPos;
	// transform particle position back to original cylinder position
	pPos->AddedBy(&m_cylBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pPos, stdPos);
	pPos->AddedBy(&m_cylBtmCtrPos);
	return !IsInCylinder(stdPos);
}

bool CSimuBoundaryCylinder::IsPosInCylinder(CVector3D *pos)
{
	CVector3D orgPos;
	// transform particle position back to original cylinder position
	pos->AddedBy(&m_cylBtmCtrPos, -1);
	RotateOrgVectToStdVect(*pos, orgPos);
	pos->AddedBy(&m_cylBtmCtrPos);

	if (orgPos[Z] > m_cylLength) // prtl is over cylinder top.
		return false;
	if (orgPos[Z] < 0) // prtl is below cylinder bottom.
		return false;

	SimuValue distToZAxis = sqrt(orgPos[X]*orgPos[X] + orgPos[Y]*orgPos[Y]);
	if (distToZAxis <= m_cylRadius)
		return true;
	return false;
}

// <pPos> is tested against the standard cylinder position
bool CSimuBoundaryCylinder::IsInCylinder(CVector3D &pPos)
{
	SimuValue distToZAxis = sqrt(pPos[X]*pPos[X] + pPos[Y]*pPos[Y]);

	if (distToZAxis <= m_cylRadius)
	{
		if (pPos[Z] > m_cylLength) // prtl is over cylinder top.
		{
			return m_bcOpenTop? true : false;
		}
		else if (pPos[Z] < 0) // prtl is below cylinder bottom.
		{
			return m_bcOpenBtm? true : false;
		}
		else
			return true;
	}
	else if (distToZAxis < m_cylRadius+m_bcOpenOutletWidth)
	{
		if (pPos[Z] > m_cylLength) // prtl is over cylinder top.
		{
			return m_bcOpenTop? true : false;
		}
		else if (pPos[Z] < 0) // prtl is below cylinder bottom.
		{
			return m_bcOpenBtm? true : false;
		}
		else
			return false;
	}
	else // (distToZAxis >= m_cylRadius+m_bcOpenOutletWidth)
	{	// the point must be limited not be able to move across the cyliner wall
		// the thickness of the cylinder wall is m_bcOpenOutletWidth
		return true;
	}
}

bool CSimuBoundaryCylinder::IsBreakingCylinderWall(CVector3D &pPos, CVector3D &revNormal)
{
	SimuValue coeff[3], t[2];
	coeff[0] = pPos[X]*pPos[X] + pPos[Y]*pPos[Y] - m_cylRadius*m_cylRadius;
	if (coeff[0] <= 0)
		return false; // <pPos> is inside the infinite cylinder and cannot break wall.
	coeff[1] = 2 * (pPos[X]*revNormal[X] + revNormal[Y]*pPos[Y]);
	coeff[2] = revNormal[X]*revNormal[X] + revNormal[Y]*revNormal[Y];
	int result = CSimuUtility::SolveQuadraticEquation(coeff, t);
	if (result == 0)
		return false; // not intersection, which means outside the infinite cylinder
	if (result == 2)
	{ // since <pPos> is outside the infinite cylinder, both t[2] are either <=0 or > 0
		if (t[0] <= 0 && t[1] <= 0)
			return false; // both intersections are not inside the surface
		else if (t[0] > 0 && t[1] > 0 && t[0] > t[1])
			t[0] = t[1]; // drop old t[0] since it's farther away from <pPos>
	}
	// in combination with (result == 1), t[0] now corresponds to the intersection.
	CVector3D intersectPos;
	intersectPos.SetValue(&pPos);
	intersectPos.AddedBy(&revNormal, t[0]);
	if (intersectPos[Z] > m_cylLength)
		return false; // intersection is over the top
	if (intersectPos[Z] < 0)
		return false; // intersection is below the bottom
	return true;
}

bool CSimuBoundaryCylinder::IsBreakingCylinderOutlet(CVector3D &pPos, CVector3D &revNormal)
{
	if (pPos[Z] >= m_cylLength || pPos[Z] <= 0)
		return false;

	SimuValue distToZAxis = sqrt(pPos[X]*pPos[X] + pPos[Y]*pPos[Y]);

	if (m_bcOpenTop && pPos[Z] > m_cylLength/2
	 && distToZAxis > m_cylRadius
	 && distToZAxis <= m_cylRadius+m_bcOpenOutletWidth)
		return true;

	if (m_bcOpenBtm && pPos[Z] <= m_cylLength/2
	 && distToZAxis > m_cylRadius
	 && distToZAxis <= m_cylRadius+m_bcOpenOutletWidth)
		return true;

	return false;
}

// return true if collision occurs; otherwise false.
bool CSimuBoundaryCylinder::EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel,
													  SimuValue timeStep)
{
	if (EnforceCylinderTopBoundary(pPos, pVel) == 0)
		return false;
	if (EnforceCylinderBtmBoundary(pPos, pVel) == 0)
		return false;
	return EnforceCylinderWallBoundary(pPos, pVel);
}

// return 0 if over cylinder top, which means no need to check other constraints;
// otherwise, return 1 if <pPos> is enforced onto cylinder top;
// otherwise, return 2, which means <pPos> is not over cylinder top.
int CSimuBoundaryCylinder::EnforceCylinderTopBoundary(CVector3D &pPos, CVector3D &pVel)
{
	if (pPos[Z] > m_cylLength) // position is over cylinder top.
	{
		if (m_bcOpenTop) return 0; // outside boundary
		// enforce cylinder top constraint
		SimuValue tmpDist = pPos[Z] - m_cylLength;
		// reflect the particle position to be below cylinder top
		pPos[Z] -= tmpDist*(1+m_bdryDistDumpingRate);
		// reflect the velocity to move downward
		if (pVel[Z] > 0) pVel[Z] *= -m_bdryEnergyDumpingRate;
		return 1;
	}
	return 2;
}

// return 0 if below cylinder bottom, which means no need to check other constraints
// otherwise, return 1 if <pPos> is enforced onto cylinder bottom;
// otherwise, return 2, which means <pPos> is not below cylinder bottom.
int CSimuBoundaryCylinder::EnforceCylinderBtmBoundary(CVector3D &pPos, CVector3D &pVel)
{
	if (pPos[Z] < 0) // position is below cylinder bottom.
	{
		if (m_bcOpenBtm) return 0;
		// enforce cylinder bottom constraint
		SimuValue tmpDist = 0 - pPos[Z];
		// reflect the particle position to be above the bottom plane
		pPos[Z] += tmpDist*(1+m_bdryDistDumpingRate);
		// reflect the velocity to move upward
		if (pVel[Z] < 0) pVel[Z] *= -m_bdryEnergyDumpingRate;
		return 1;
	}
	return 2;
}

// return true if collision occurs; otherwise, false.
bool CSimuBoundaryCylinder::EnforceCylinderWallBoundary(CVector3D &pPos, CVector3D &pVel)
{
	bool bCollisionOccurs = false;
	SimuValue tmpZ = pPos[Z];
	pPos[Z] = 0;	// now, pPos is on z=0 plane
	SimuValue tmpDist = pPos.Length() - m_cylRadius;
	if (tmpDist > 0
	 && tmpDist < CSimuManager::m_prtlDistance) // outside cylinder wall, but not too far
	{
		// reflect position w.r.t the tagent plane
		SimuValue tmpLen = m_cylRadius - tmpDist*m_bdryDistDumpingRate;
		pPos.Normalize(tmpLen);
		// reflect velocity
		// tmpX and tmpY are normalized values for computing reflection value
		SimuValue tmpX = pPos[X]/tmpLen;
		SimuValue tmpY = pPos[Y]/tmpLen;
		// reflect value is the dot product of velocity in x-y plane with reflect plane normal
		SimuValue reflectValue = pVel[X]*tmpX + pVel[Y]*tmpY;
		if (reflectValue > 0)
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

void CSimuBoundaryCylinder::EnforceCylinderOutletBoundary(CVector3D &pPos, CVector3D &pVel)
{
	if (m_bcOpenTop
	 && pPos[Z] < m_cylLength
	 && pPos[Z] > m_cylLength/2)
	{
		// enforce cylinder top outlet constraint
		SimuValue distToOutlet = m_cylLength - pPos[Z];
		// reflect the particle position to be above the top plane
		pPos[Z] += distToOutlet*(1+m_bdryDistDumpingRate);
		// reflect the velocity to move upward
		if (pVel[Z] < 0) pVel[Z] *= -m_bdryEnergyDumpingRate;
	}
	else if (m_bcOpenBtm
	 && pPos[Z] <= m_cylLength/2
	 && pPos[Z] > 0)
	{
		// enforce cylinder bottom outlet constraint
		SimuValue distToOutlet = pPos[Z];
		// reflect the particle position to be below the bottom plane
		pPos[Z] -= distToOutlet*(1+m_bdryDistDumpingRate);
		// reflect the velocity to move downward
		if (pVel[Z] > 0) pVel[Z] *= -m_bdryEnergyDumpingRate;
	}
}
