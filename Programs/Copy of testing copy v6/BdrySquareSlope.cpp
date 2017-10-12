// BdrySquareSlope.cpp: implementation of the CBdrySquareSlope class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BdrySquareSlope.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CBdrySquareSlope::m_bssMaxId = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBdrySquareSlope::CBdrySquareSlope()
{
	InitializeSquareSlope();

	m_bssSideLen = 1;
}

CBdrySquareSlope::CBdrySquareSlope(CVector3D* direction, CVector3D* ctrPos,
								   SimuValue sideLength, SimuValue rotationAngleAboutZAxis)
: CRotationToZAxis(direction, rotationAngleAboutZAxis)
{
	InitializeSquareSlope();

	m_bssSideLen = sideLength;
	m_bssCtrPos.SetValue(ctrPos);
	m_bssSlopeStadDirection.SetElements(0, 0, 1);
}

CBdrySquareSlope::~CBdrySquareSlope()
{

}

void CBdrySquareSlope::InitializeSquareSlope()
{
	m_bdryType = SQUARE_SLOPE;

	m_bssMaxId++;
	char tmpName[LEN_BOUNDARY_NAME];
	sprintf(tmpName, "square_slope_%d", m_bssMaxId);
	m_deiName = tmpName;

	m_bssSlopeThickness = 5*CSimuManager::m_prtlDistance;
}

void CBdrySquareSlope::DrawBoundary()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	SimuTranslate(m_bssCtrPos[X], m_bssCtrPos[Y], m_bssCtrPos[Z]);
	SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
	SimuRotate(m_rtzaRotationAngleAboutZAxis, 0, 0, 1);
	// draw direction
	SimuColor3v(CSimuManager::COLOR_RED->v);
	glBegin(GL_LINES);
	SimuVertex3d(0, 0, 0);
	SimuVertex3d(0, 0, 100);
	glEnd();
	// draw square
	SimuColor3v(m_bdryColor->v);
	glBegin(GL_QUADS);
	SimuNormal3d(0, 0, 1);
	SimuVertex3d(-m_bssSideLen, -m_bssSideLen, 0);
	SimuVertex3d(-m_bssSideLen, m_bssSideLen, 0);
	SimuVertex3d(m_bssSideLen, m_bssSideLen, 0);
	SimuVertex3d(m_bssSideLen, -m_bssSideLen, 0);
	glEnd();
	glPopAttrib();
	glPopMatrix();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CBdrySquareSlope::ExportBoundaryData(FILE* fp)
{
	fprintf(fp, "#declare %s__m_bssSlopeThickness = %f;\n", m_deiName, m_bssSlopeThickness);
	fprintf(fp, "#declare %s__m_bssSideLen = %f;\n", m_deiName, m_bssSideLen);
	fprintf(fp, "#declare %s__m_bssCtrPos = <%f, %f, %f>;\n", m_deiName,
							  m_bssCtrPos[X],
							  m_bssCtrPos[Y],
							  m_bssCtrPos[Z]);
	ExportRotationData(m_deiName, fp);
}

void CBdrySquareSlope::ImportBoundaryData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float tmpFloat, x, y, z;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_bssSlopeThickness = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_bssSideLen = tmpFloat;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_bssCtrPos[X] = x;
							m_bssCtrPos[Y] = y;
							m_bssCtrPos[Z] = z;
	ImportRotationData(fp);
	m_deiName = "slope_square";
}

bool CBdrySquareSlope::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	CVector3D translatePos, stdPos, stdVel;
	translatePos.SetValueAsSubstraction(pPrtl->m_vpPos, &m_bssCtrPos);
	RotateOrgVectToStdVect(translatePos, stdPos);
	RotateOrgVectToStdVect(*pPrtl->m_vpVel, stdVel);
	bool bCollisionOccurs = EnforceSquareSlopeConstraint(stdPos, stdVel, timeStep);
//	CSimuUtility::AssertSimuVector3D(&stdPos);
	if (CSimuManager::m_bInvalidValue)
	{
		translatePos.SetValueAsSubstraction(pPrtl->m_vpPos, &m_bssCtrPos);
		RotateOrgVectToStdVect(translatePos, stdPos);
		RotateOrgVectToStdVect(*pPrtl->m_vpVel, stdVel);
		EnforceSquareSlopeConstraint(stdPos, stdVel, timeStep);
	}
	RotateStdVectToOrgVect(stdVel, *pPrtl->m_vpVel);
	RotateStdVectToOrgVect(stdPos, translatePos);
	pPrtl->m_vpPos->SetValueAsAddition(&translatePos, &m_bssCtrPos);
	return bCollisionOccurs;
}

bool CBdrySquareSlope::PosBreakBoundary(CVector3D* pPos)
{
	CVector3D stdPos;
	// transform particle position back to original box position
	pPos->AddedBy(&m_bssCtrPos, -1);
	RotateOrgVectToStdVect(*pPos, stdPos);
	pPos->AddedBy(&m_bssCtrPos);
	if (stdPos[Z] < 0
	 && fabs(stdPos[Z]) < m_bssSlopeThickness
	 && fabs(stdPos[X]) <= m_bssSideLen
	 && fabs(stdPos[Y]) <= m_bssSideLen)
		return true;
	return false;
}

bool CBdrySquareSlope::EnforceSquareSlopeConstraint(CVector3D &stdPos, CVector3D &stdVel,
													SimuValue timeStep)
{
	bool bCollisionOccurs = false;
	if (stdPos[Z] < 0
	 && fabs(stdPos[Z]) < m_bssSlopeThickness
	 && fabs(stdPos[X]) <= m_bssSideLen
	 && fabs(stdPos[Y]) <= m_bssSideLen)
	{
		bCollisionOccurs = true;
		if (CBoundary::m_bdryMaxClsSpeed < fabs(stdVel[Z]))
		{
			CBoundary::m_bdryMaxClsSpeed = fabs(stdVel[Z]);
		}
		if (fabs(stdVel[Z]) <= SIMU_VALUE_EPSILON)
		{
			// enforce position
			stdPos[Z] = -stdPos[Z]*m_bdryDistDumpingRate;
			// enforce velocity
			stdVel[Z] = stdVel[Z]*m_bdryEnergyDumpingRate;
		}
		else // fabs(stdVel[Z]) > SIMU_VALUE_EPSILON
		{
			// compute intersection with boundary along velocity
			CVector3D intersectPos;
			SimuValue t = fabs(stdPos[Z])/fabs(stdVel[Z]);
			intersectPos.SetValue(&stdPos);
			intersectPos.AddedBy(&stdVel, -t); // move backwards along velocity
			// enforce position
			CVector3D relativePos;
			relativePos.SetValueAsSubstraction(&stdPos, &intersectPos);
			relativePos[Z] = -relativePos[Z]*m_bdryDistDumpingRate;
			relativePos[X] *= m_bdryFrictionRate;
			relativePos[Y] *= m_bdryFrictionRate;
			stdPos.SetValueAsAddition(&intersectPos, &relativePos);
			// enforce velocity
			stdVel[Z] = -stdVel[Z]*m_bdryEnergyDumpingRate;
		}
		stdVel[X] *= m_bdryFrictionRate;
		stdVel[Y] *= m_bdryFrictionRate;
	}
	return bCollisionOccurs;
}

SimuValue CBdrySquareSlope::GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl)
{
	CVector3D translatePos, stdPos;
	translatePos.SetValueAsSubstraction(pPrtl->m_vpPos, &m_bssCtrPos);
	RotateOrgVectToStdVect(translatePos, stdPos);
	return stdPos[Z];
}
