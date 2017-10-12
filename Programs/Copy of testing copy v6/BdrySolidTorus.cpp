// BdrySolidTorus.cpp: implementation of the CBdrySolidTorus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BdrySolidTorus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBdrySolidTorus::CBdrySolidTorus()
{
	m_bdryType = SOLID_TORUS;
}

CBdrySolidTorus::CBdrySolidTorus(SimuValue innerRadius, SimuValue outerRadius,
								 CVector3D* ctrPos, CVector3D* direction)
:CTorusOnXYPlane(innerRadius, outerRadius, ctrPos, direction)
{
	m_bdryType = SOLID_TORUS;
}

CBdrySolidTorus::~CBdrySolidTorus()
{

}

void CBdrySolidTorus::DrawBoundary()
{
	if (!m_bdryVisible) return;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	SimuColor3v(m_bdryColor->v);
	glPushMatrix();
	SimuTranslate(m_trsCtrPos[X], m_trsCtrPos[Y], m_trsCtrPos[Z]);
	SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
	DrawTorusOnXYPlane();
	glPopMatrix();
	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CBdrySolidTorus::ExportBoundaryData(FILE* fp)
{
	fprintf(fp, "#declare %s__m_trsInnerRadius = %f;\n", m_deiName, m_trsInnerRadius);
	fprintf(fp, "#declare %s__m_trsOuterRadius = %f;\n", m_deiName, m_trsOuterRadius);
	fprintf(fp, "#declare %s__m_trsCtrPos = <%f, %f, %f>;\n", m_deiName,
							  m_trsCtrPos[X],
							  m_trsCtrPos[Y],
							  m_trsCtrPos[Z]);
	ExportRotationData(m_deiName, fp);
}

void CBdrySolidTorus::ImportBoundaryData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float tmp, x, y, z;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmp); m_trsInnerRadius = tmp;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmp); m_trsOuterRadius = tmp;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_trsCtrPos[X] = x;
							m_trsCtrPos[Y] = y;
							m_trsCtrPos[Z] = z;
	ImportRotationData(fp);
	ComputeTorusVariables();
	m_deiName = "solid_torus";
}

bool CBdrySolidTorus::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	bool bShowMsg = false;
	if (PrtlBreakBoundary(pPrtl))
	{
//		bShowMsg = true;
//		CSimuMsg::PauseMessage("Before Enforce torus boundary");
	}
	else
		return false;
	CVector3D standardPos, standardVel;
	// transform prtl position and velocity back to standard torus posture
	pPrtl->m_vpPos->AddedBy(&m_trsCtrPos, -1);
	RotateOrgVectToStdVect(*pPrtl->m_vpPos, standardPos);
	RotateOrgVectToStdVect(*pPrtl->m_vpVel, standardVel);
	bool bCollisionOccurs = EnforceBoundaryConstraint(standardPos, standardVel, timeStep);
	RotateStdVectToOrgVect(standardVel, *pPrtl->m_vpVel);
	RotateStdVectToOrgVect(standardPos, *pPrtl->m_vpPos);
	pPrtl->m_vpPos->AddedBy(&m_trsCtrPos);
	if (bShowMsg)
		CSimuMsg::PauseMessage("After Enforce torus boundary");
	return bCollisionOccurs;
}

bool CBdrySolidTorus::PosBreakBoundary(CVector3D* pPos)
{
	CVector3D standardPos;
	// transform prtl position back to standard position
	pPos->AddedBy(&m_trsCtrPos, -1);
	RotateOrgVectToStdVect(*pPos, standardPos);
	pPos->AddedBy(&m_trsCtrPos);
	SimuValue radius = sqrt(standardPos[X]*standardPos[X] + standardPos[Y]*standardPos[Y]);
	radius -= m_trsMiddleRadius;
	SimuValue distanceToCtr = sqrt(standardPos[Z]*standardPos[Z] + radius*radius);
	if (distanceToCtr >= m_trsNeckRadius)
		return false;
	return true;
}

SimuValue CBdrySolidTorus::GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl)
{
	CVector3D orgPos;
	GetStandardPosition(*pPrtl->m_vpPos, orgPos);

	return GetDistanceToTorus(orgPos);
}

// return true if collision occurs; otherwise, false.
bool CBdrySolidTorus::EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel,
												SimuValue timeStep)
{
	SimuValue radiusToZAxis = sqrt(pPos[X]*pPos[X] + pPos[Y]*pPos[Y]);
	SimuValue horizontalValue = radiusToZAxis - m_trsMiddleRadius;
	SimuValue distanceToMidCircle = sqrt(pPos[Z]*pPos[Z] + horizontalValue*horizontalValue);
	if (distanceToMidCircle >= m_trsNeckRadius)
		return false;
	if (distanceToMidCircle <= SIMU_VALUE_EPSILON)
	{
		CSimuMsg::ExitWithMessage("CBdrySolidTorus::EnforceBoundaryConstraint(pPos, pVel)",
								"Ambiguous location in torus.");
		// move the position in the cutting plane onto the neck circle
		pPos[Z] = m_trsNeckRadius;
		horizontalValue = 0;
	}
	else
	{
		// move the position in the cutting plane onto the neck circle
		pPos[Z] *= m_trsNeckRadius/distanceToMidCircle;
		horizontalValue *= m_trsNeckRadius/distanceToMidCircle;
	}
	// move <pPos> onto torus
	SimuValue newRadiusToZAxis = horizontalValue + m_trsMiddleRadius;
	pPos[X] *= newRadiusToZAxis/radiusToZAxis;
	pPos[Y] *= newRadiusToZAxis/radiusToZAxis;
	// compute reflection normal for velocity
	CVector3D reflectionNormal;
	reflectionNormal[X] = horizontalValue*pPos[X]/newRadiusToZAxis;
	reflectionNormal[Y] = horizontalValue*pPos[Y]/newRadiusToZAxis;
	reflectionNormal[Z] = pPos[Z];
	reflectionNormal.Normalize();
	// reflect velocity
	SimuValue reflectValue = reflectionNormal.DotProduct(&pVel);
	if (reflectValue < 0)
	{
		reflectValue *= -(1 + m_bdryDistDumpingRate);
		pVel.AddedBy(&reflectionNormal, reflectValue);
	}
	return true;
}

void CBdrySolidTorus::GetStandardPosition(CVector3D &transformedPos,
										  CVector3D &standardPos)
{
	transformedPos.AddedBy(&m_trsCtrPos, -1);
	RotateOrgVectToStdVect(transformedPos, standardPos);
	transformedPos.AddedBy(&m_trsCtrPos, 1);
}

void CBdrySolidTorus::GetTransformedPosition(CVector3D &standardPos,
											 CVector3D &transformedPos)
{
	RotateStdVectToOrgVect(standardPos, transformedPos);
	transformedPos.AddedBy(&m_trsCtrPos, 1);
}

