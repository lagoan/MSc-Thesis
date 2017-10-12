// BdryBowl.cpp: implementation of the CBdryBowl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BdryBowl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBdryBowl::CBdryBowl()
{
	m_bdryType = BOWL;
	m_deiName = "bowl";
}

CBdryBowl::CBdryBowl(CVector3D* direction, CVector3D* btmPos,
					 SimuValue height, SimuValue bigRadius)
{
	m_bdryType = BOWL;
	m_deiName = "bowl";

	m_bbBigR2 = pow(bigRadius, 2);
	m_bbBigRadius = bigRadius;
	m_bbHeight = height;
	m_bbRimRadius = sqrt(pow(m_bbBigRadius, 2) - pow(m_bbBigRadius - m_bbHeight, 2));
	m_bbBtmPos.SetValue(btmPos);

	m_ovalRadiusOnX = m_bbBigRadius;
	m_ovalRadiusOnY = m_bbBigRadius;
	m_ovalRadiusOnZ = m_bbBigRadius;
	m_ovalCtrPos.SetValue(&m_bbBtmPos);
	m_ovalCtrPos.AddedBy(direction, m_bbBigRadius);

	m_rtzaOrgDirection.SetValue(direction);
	m_rtzaOrgDirection.Normalize();
	m_rtzaRotationAngleAboutZAxis = 0;

	ComputeRotationVariables();
	PrepareRotationMatrix();
}

CBdryBowl::~CBdryBowl()
{

}

void CBdryBowl::DrawBoundary()
{
	const int numLayers = 10;
	const int numSegments = 20;
	SimuValue layerHeight = m_bbHeight/numLayers;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	SimuColor3v(m_bdryColor->v);
	for (int i=0; i < numLayers; i++)
	{
		SimuValue tmpHeight = (i+1)*layerHeight;
		SimuValue tmpRadius = sqrt(pow(m_ovalRadiusOnX, 2) - pow(m_ovalRadiusOnX- tmpHeight, 2));
		glPushMatrix();
		SimuTranslate(m_bbBtmPos[X], m_bbBtmPos[Y], m_bbBtmPos[Z]);
		SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
		CSimuUtility::DrawCircleOnZ0ByOpenGL(tmpRadius, tmpHeight, numSegments);
		glPopMatrix();
	}
	glPopAttrib();
}

void CBdryBowl::ExportBoundaryData(FILE* fp)
{
	fprintf(fp, "#declare %s__m_bbBigRadius = %f;\n", m_deiName, m_bbBigRadius);
	fprintf(fp, "#declare %s__m_bbRimRadius = %f;\n", m_deiName, m_bbRimRadius);
	fprintf(fp, "#declare %s__m_bbHeight = %f;\n", m_deiName, m_bbHeight);
	fprintf(fp, "#declare %s__m_bbBtmPos = <%f, %f, %f>;\n", m_deiName,
							  m_bbBtmPos[X],
							  m_bbBtmPos[Y],
							  m_bbBtmPos[Z]);
	ExportOvalData(m_deiName, fp);
	ExportRotationData(m_deiName, fp);
}

void CBdryBowl::ImportBoundaryData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float tmpFloat, x, y, z;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_bbBigRadius = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_bbRimRadius = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_bbHeight = tmpFloat;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_bbBtmPos[X] = x;
							m_bbBtmPos[Y] = y;
							m_bbBtmPos[Z] = z;
	ImportOvalData(fp);
	ImportRotationData(fp);
	m_deiName = "bowl";
}

bool CBdryBowl::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	char location[] = "CBdryBowl::EnforceBoundaryConstraint(...)";

	CVector3D translatePos, orgPos, orgVel;
	translatePos.SetValueAsSubstraction(pPrtl->m_vpPos, &m_ovalCtrPos);
	RotateOrgVectToStdVect(translatePos, orgPos);
	RotateOrgVectToStdVect(*pPrtl->m_vpVel, orgVel);
	bool bCollisionOccurs = false;
	if (orgPos[Z] > m_bbHeight - m_bbBigRadius)
		return bCollisionOccurs;
	if (orgPos.LengthSquare() <= m_bbBigR2)
		return bCollisionOccurs;
	CVector3D intersectPos;
	intersectPos.SetValue(&orgPos);
	intersectPos.Normalize(m_bbBigRadius);
	if (intersectPos[Z] > m_bbHeight - m_bbBigRadius)
		return bCollisionOccurs;
	bCollisionOccurs = true;

	// move orgPos back onto or inside oval
	SimuValue coeff[3];
	coeff[0] = orgPos.LengthSquare() - m_bbBigR2;
	coeff[1] = orgPos[X]*orgVel[X] + orgPos[Y]*orgVel[Y] + orgPos[Z]*orgVel[Z];
	coeff[1] *= 2;
	coeff[2] = orgVel.LengthSquare();
	SimuValue t[2];
	int result = CSimuUtility::SolveQuadraticEquation(coeff, t);
	if (result == 0)
	{ // orgVel has no intersection with oval
		SimuValue deltaDist = intersectPos.GetDistanceToVector(&orgPos);
		CVector3D backwardNormal;
		backwardNormal.SetValue(&orgPos);
		backwardNormal.Normalize(-1);
		intersectPos.AddedBy(&backwardNormal, m_bdryDistDumpingRate*deltaDist);
	}
	else
	{
		if (result == 2 && fabs(t[0]) > fabs(t[1]))
			t[0] = t[1]; // use the closer <t>
		// complete dumping onto boundary
		intersectPos.SetValue(&orgPos);
		intersectPos.AddedBy(&orgVel, t[0]);
	}
	orgPos.SetValue(&intersectPos);
	// adjust velocity
	CVector3D planeNormal;
	planeNormal.SetValue(&intersectPos);
	planeNormal.Normalize();
	// usually reflectValue should be >= 0, but due to fluid-fluid interaction
	// reflectValue could be < 0, which is allowed.
	SimuValue reflectValue = orgVel.DotProduct(&planeNormal);
	orgVel.SetValue(&orgPos);
	orgVel.AddedBy(&planeNormal, -reflectValue);
	// by now, orgVel is velocity horizontal to the plane
	orgVel.ScaledBy(m_bdryFrictionRate);
	// by now, orgVel is under friction effect
	// added velocity vertical to the plane
	orgVel.AddedBy(&planeNormal, -reflectValue*m_bdryEnergyDumpingRate);

	RotateStdVectToOrgVect(orgVel, *pPrtl->m_vpVel);
	RotateStdVectToOrgVect(orgPos, translatePos);
	pPrtl->m_vpPos->SetValueAsAddition(&translatePos, &m_ovalCtrPos);
	return bCollisionOccurs;
}
