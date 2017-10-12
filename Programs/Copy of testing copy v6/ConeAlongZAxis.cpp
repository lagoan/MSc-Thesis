// ConeAlongZAxis.cpp: implementation of the CConeAlongZAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "ConeAlongZAxis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConeAlongZAxis::CConeAlongZAxis()
{
	InitializeCone();
}

CConeAlongZAxis::CConeAlongZAxis(SimuValue coneLen, SimuValue coneTopRadius,
								 SimuValue coneBtmRadius,
								 CVector3D* btmCtrPos, CVector3D* direction)
: CRotationToZAxis(direction)
{
	m_coneLength = coneLen;
	m_coneTopRadius = coneTopRadius;
	m_coneBtmRadius = coneBtmRadius;
	m_coneUpperLength = m_coneTopRadius*m_coneLength/(m_coneBtmRadius - m_coneTopRadius);
	m_coneSlopeRatio = (m_coneBtmRadius - m_coneTopRadius)/m_coneLength;
	m_coneCosHalfAngleOfTip = m_coneUpperLength/sqrt(pow(m_coneUpperLength, 2)+pow(m_coneTopRadius, 2));
	m_coneHalfAngleOfTip = acos(m_coneCosHalfAngleOfTip);

	m_coneBtmCtrPos.SetValue(btmCtrPos);
	m_coneTopCtrPos.SetValue(btmCtrPos);
	m_coneTopCtrPos.AddedBy(direction, m_coneLength);
	m_coneStandardTipPos[X] = 0;
	m_coneStandardTipPos[Y] = 0;
	m_coneStandardTipPos[Z] = m_coneLength + m_coneUpperLength;

	InitializeCone();
}

CConeAlongZAxis::~CConeAlongZAxis()
{

}

void CConeAlongZAxis::InitializeCone()
{
	m_coneSlices = 200;
	m_coneStacks = 10;
}

void CConeAlongZAxis::DrawConeAlongZAxis()
{
	const bool bDrawConeWall = true;
	if (bDrawConeWall)
	{
		const SimuValue thickness = 0;
		SimuValue layerHeight = m_coneLength/(m_coneStacks-1);
		for (int i=0; i < m_coneStacks; i++)
		{
			SimuValue tmpHeight = i*layerHeight;
			SimuValue lenToTip = m_coneUpperLength + (m_coneLength - tmpHeight);
			SimuValue tmpRadius = lenToTip * m_coneSlopeRatio;
			glPushMatrix();
			SimuTranslate(m_coneBtmCtrPos[X], m_coneBtmCtrPos[Y], m_coneBtmCtrPos[Z]);
			SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
			CSimuUtility::DrawCircleOnZ0ByOpenGL(tmpRadius, tmpHeight, m_coneSlices);
			glPopMatrix();
		}
	}

	// draw direction
	SimuColor3v(CSimuManager::COLOR_RED->v);
	glPushMatrix();
	SimuTranslate(m_coneBtmCtrPos[X], m_coneBtmCtrPos[Y], m_coneBtmCtrPos[Z]);
	SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
	SimuRotate(m_rtzaRotationAngleAboutZAxis, 0, 0, 1);
	glBegin(GL_LINES);
	SimuVertex3d(0, 0, m_coneLength);
	SimuVertex3d(0, 0, m_coneLength*2);
	glEnd();
	glPopMatrix();
}

void CConeAlongZAxis::ExportConeData(CString objName, FILE* fp)
{
	fprintf(fp, "#declare %s__m_coneLength = %f;\n", objName, m_coneLength);
	fprintf(fp, "#declare %s__m_coneTopRadius = %f;\n", objName, m_coneTopRadius);
	fprintf(fp, "#declare %s__m_coneBtmRadius = %f;\n", objName, m_coneBtmRadius);
	fprintf(fp, "#declare %s__m_coneUpperLength = %f;\n", objName, m_coneUpperLength);
	fprintf(fp, "#declare %s__m_coneSlopeRatio = %f;\n", objName, m_coneSlopeRatio);
	fprintf(fp, "#declare %s__m_coneTopCtrPos = <%f, %f, %f>;\n", objName,
							  m_coneTopCtrPos[X],
							  m_coneTopCtrPos[Y],
							  m_coneTopCtrPos[Z]);
	fprintf(fp, "#declare %s__m_coneBtmCtrPos = <%f, %f, %f>;\n", objName,
							  m_coneBtmCtrPos[X],
							  m_coneBtmCtrPos[Y],
							  m_coneBtmCtrPos[Z]);
	ExportRotationData(objName, fp);
}

void CConeAlongZAxis::ImportConeData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float tmpFloat, x, y, z;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_coneLength = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_coneTopRadius = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_coneBtmRadius = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_coneUpperLength = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_coneSlopeRatio = tmpFloat;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_coneTopCtrPos[X] = x;
							m_coneTopCtrPos[Y] = y;
							m_coneTopCtrPos[Z] = z;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_coneBtmCtrPos[X] = x;
							m_coneBtmCtrPos[Y] = y;
							m_coneBtmCtrPos[Z] = z;

	ImportRotationData(fp);
}

void CConeAlongZAxis::GetStandardPosition(CVector3D &transformedPos, CVector3D &standardPos)
{
	transformedPos.AddedBy(&m_coneBtmCtrPos, -1);
	RotateOrgVectToStdVect(transformedPos, standardPos);
	transformedPos.AddedBy(&m_coneBtmCtrPos, 1);
}

void CConeAlongZAxis::GetTransformedPosition(CVector3D &standardPos, CVector3D &transformedPos)
{
	RotateStdVectToOrgVect(standardPos, transformedPos);
	transformedPos.AddedBy(&m_coneBtmCtrPos, 1);
}

SimuValue CConeAlongZAxis::GetRadius(SimuValue heightOnZ)
{
	SimuValue lenToTip = m_coneUpperLength + (m_coneLength - heightOnZ);
	SimuValue radius = lenToTip * m_coneSlopeRatio;
	return radius;
}
