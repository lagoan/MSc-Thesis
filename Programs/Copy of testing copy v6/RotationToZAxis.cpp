// RotationToZAxis.cpp: implementation of the CRotationToZAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "RotationToZAxis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

time_t CRotationToZAxis::m_t2005_JUNE_13_8AM = CSimuManager::GetSpecifiedTime(2005, 5, 13, 8);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRotationToZAxis::CRotationToZAxis()
{
	m_rtzaRotationAngle = 0;
	m_rtzaRotationAngleAboutZAxis = 0;
	for (int i=0; i < 3; i++)
	for (int j=0; j < 3; j++)
	{
		m_rtzaRotateM[i][j] = 0;
		m_rtzaInvRotM[i][j] = 0;
	}
	m_rtzaAxisZDir.SetElements(0, 0, 1);
}

CRotationToZAxis::CRotationToZAxis(CVector3D* orgDirection)
{
	InitializeVariables(orgDirection, 0);
}

CRotationToZAxis::CRotationToZAxis(CVector3D* orgDirection,
								   SimuValue rotationAngleAboutZAxis)
{
	InitializeVariables(orgDirection, rotationAngleAboutZAxis);
}

CRotationToZAxis::~CRotationToZAxis()
{

}

void CRotationToZAxis::ExportRotationData(CString objName, FILE* fp)
{
	fprintf(fp, "#declare %s__m_rtzaRotationAngleAboutZAxis = %f;\n",
							objName, m_rtzaRotationAngleAboutZAxis);
	fprintf(fp, "#declare %s__m_rtzaOrgDirection = <%f, %f, %f>;\n", objName,
							  m_rtzaOrgDirection[X],
							  m_rtzaOrgDirection[Y],
							  m_rtzaOrgDirection[Z]);
	for (int i=0; i < 3; i++)
	for (int j=0; j < 3; j++)
		fprintf(fp, "#declare %s__m_rtzaRotateM%d%d = %f;\n",
						objName, i, j, m_rtzaRotateM[i][j]);
	fprintf(fp, "#declare %s__m_rtzaRotateM30 = %f;\n", objName, 0);
	fprintf(fp, "#declare %s__m_rtzaRotateM31 = %f;\n", objName, 0);
	fprintf(fp, "#declare %s__m_rtzaRotateM32 = %f;\n", objName, 0);

	for (int i=0; i < 3; i++)
	for (int j=0; j < 3; j++)
		fprintf(fp, "#declare %s__m_rtzaInvRotM%d%d = %f;\n",
						objName, i, j, m_rtzaInvRotM[i][j]);
	fprintf(fp, "#declare %s__m_rtzaInvRotM30 = %f;\n", objName, 0);
	fprintf(fp, "#declare %s__m_rtzaInvRotM31 = %f;\n", objName, 0);
	fprintf(fp, "#declare %s__m_rtzaInvRotM32 = %f;\n", objName, 0);
}

void CRotationToZAxis::ImportRotationData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float tmpFloat, x, y, z;
	time_t fileTime = CSimuManager::GetFileModificationTime(fp);
	if (CSimuManager::FirstTimeIsLaterThanSecondTime(fileTime, m_t2005_JUNE_13_8AM))
		fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_rtzaRotationAngleAboutZAxis = tmpFloat;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_rtzaOrgDirection[X] = x;
							m_rtzaOrgDirection[Y] = y;
							m_rtzaOrgDirection[Z] = z;
	for (int i=0; i < 12; i++)
	{
		fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat);
	}
	for (int i=0; i < 12; i++)
	{
		fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat);
	}

	m_rtzaOrgDirection.Normalize();
	ComputeRotationVariables();
	PrepareRotationMatrix();
}

void CRotationToZAxis::RotateStdVectToOrgVect(CVector3D &oldV, CVector3D &newV)
{
	for (int i=0; i < 3; i++)
	{
		newV[i] = 0;
		for (int j=0; j < 3; j++)
			newV[i] += m_rtzaRotateM[i][j]*oldV[j];
	}
}

void CRotationToZAxis::RotateOrgVectToStdVect(CVector3D &oldV, CVector3D &newV)
{
	for (int i=0; i < 3; i++)
	{
		newV[i] = 0;
		for (int j=0; j < 3; j++)
			newV[i] += m_rtzaInvRotM[i][j]*oldV[j];
	}
}

void CRotationToZAxis::AddAngularVelocity(SimuValue angVel,
										  SimuValue timeStep,
										  CVector3D &pPos,
										  CVector3D &pVel)
{
	if (fabs(angVel) <= SIMU_VALUE_EPSILON)
		return;

	CVector3D pAngVel;
	ComputeAngularVelocity(angVel, timeStep, pPos, pAngVel);
	pPos.AddedBy(&pAngVel, timeStep);
	pVel.AddedBy(&pAngVel);
}

// assume right-hand rule: index finger swipes from ZDir to pPos,
// the thumb points to the angular velocity direction
void CRotationToZAxis::ComputeAngularVelocity(SimuValue angVel,
											  SimuValue timeStep,
											  CVector3D &pPos,
											  CVector3D &pAngVel)
{
	SimuValue savedZ = pPos[Z];
	pPos[Z] = 0;
	SimuValue deltaAngle = angVel*timeStep;
	SimuValue angRadius = pPos.Length();
	SimuValue tmpCos = angRadius*cos(deltaAngle*SIMU_CONST_DEGREE_RADIAN_RATIO);
	SimuValue tmpSin = angRadius*sin(deltaAngle*SIMU_CONST_DEGREE_RADIAN_RATIO);
	// compute cross-product of (0, 0, 1) and pPos
	pAngVel[X] = - pPos[Y];
	pAngVel[Y] = pPos[X];
	pAngVel[Z] = 0;
	pAngVel.Normalize(tmpSin);
	pAngVel.AddedBy(&pPos, tmpCos/angRadius);
	pAngVel.AddedBy(&pPos, -1);
	pAngVel.ScaledBy(1/timeStep);
	// restore value Z
	pPos[Z] = savedZ;
}

void CRotationToZAxis::InitializeVariables(CVector3D* orgDirection,
										   SimuValue rotationAngleAboutZAxis)
{
	m_rtzaAxisZDir.SetElements(0, 0, 1);
	if (orgDirection == NULL)
	{
		m_rtzaOrgDirection.SetElements(0, 0, 1);
	}
	else
	{
		m_rtzaOrgDirection.SetValue(orgDirection);
	}
	m_rtzaOrgDirection.Normalize();
	m_rtzaRotationAngleAboutZAxis = rotationAngleAboutZAxis;

	ComputeRotationVariables();
	PrepareRotationMatrix();
}

void CRotationToZAxis::ComputeRotationVariables()
{
	// m_rtzaRotationV = cross-product of (0, 0, 1) and orgDirection
	m_rtzaRotationV.SetValueAsCrossProduct(&m_rtzaAxisZDir, &m_rtzaOrgDirection);
	SimuValue tmpLen = m_rtzaRotationV.Length();
	if (tmpLen > SIMU_VALUE_EPSILON)
	{
		m_rtzaRotationAngle = asin(tmpLen/m_rtzaOrgDirection.Length());
		m_rtzaRotationAngle *= 180/PI;
		if (m_rtzaOrgDirection[Z] < 0)
			m_rtzaRotationAngle = 180 - m_rtzaRotationAngle;
	}
	else
	{
		m_rtzaRotationV.SetElements(0, 0, 1);
		m_rtzaRotationAngle = 0;
	}
	m_rtzaRotationV.Normalize();
}

void CRotationToZAxis::PrepareRotationMatrix()
{
	SimuValue s = sin(m_rtzaRotationAngle*PI/180);
	SimuValue c = cos(m_rtzaRotationAngle*PI/180);
	SimuValue t = 1 - c;
	SimuValue vX = m_rtzaRotationV[X];
	SimuValue vY = m_rtzaRotationV[Y];
	SimuValue vZ = m_rtzaRotationV[Z];

	SimuValue rotateM[3][3];
	rotateM[0][0] = t*vX*vX + c;
	rotateM[0][1] = t*vX*vY - s*vZ;
	rotateM[0][2] = t*vX*vZ + s*vY;

	rotateM[1][0] = t*vX*vY + s*vZ;
	rotateM[1][1] = t*vY*vY + c;
	rotateM[1][2] = t*vY*vZ - s*vX;

	rotateM[2][0] = t*vX*vZ - s*vY;
	rotateM[2][1] = t*vY*vZ + s*vX;
	rotateM[2][2] = t*vZ*vZ + c;

	SimuValue invRotM[3][3];
	invRotM[0][0] = t*vX*vX + c;
	invRotM[0][1] = t*vX*vY + s*vZ;
	invRotM[0][2] = t*vX*vZ - s*vY;

	invRotM[1][0] = t*vX*vY - s*vZ;
	invRotM[1][1] = t*vY*vY + c;
	invRotM[1][2] = t*vY*vZ + s*vX;

	invRotM[2][0] = t*vX*vZ + s*vY;
	invRotM[2][1] = t*vY*vZ - s*vX;
	invRotM[2][2] = t*vZ*vZ + c;

	s = sin(m_rtzaRotationAngleAboutZAxis*PI/180);
	c = cos(m_rtzaRotationAngleAboutZAxis*PI/180);
	SimuValue matrixAboutZ[3][3];
	matrixAboutZ[0][0] = c;
	matrixAboutZ[0][1] = -s;
	matrixAboutZ[0][2] = 0;
	matrixAboutZ[1][0] = s;
	matrixAboutZ[1][1] = c;
	matrixAboutZ[1][2] = 0;
	matrixAboutZ[2][0] = 0;
	matrixAboutZ[2][1] = 0;
	matrixAboutZ[2][2] = 1;

	for (int i=0; i < 3; i++)
	for (int j=0; j < 3; j++)
	{
		m_rtzaRotateM[i][j] = 0;
		for (int k=0; k < 3; k++)
			m_rtzaRotateM[i][j] += rotateM[i][k]*matrixAboutZ[k][j];
	}

	s = sin(-m_rtzaRotationAngleAboutZAxis*PI/180);
	c = cos(-m_rtzaRotationAngleAboutZAxis*PI/180);
	matrixAboutZ[0][0] = c;
	matrixAboutZ[0][1] = -s;
	matrixAboutZ[0][2] = 0;
	matrixAboutZ[1][0] = s;
	matrixAboutZ[1][1] = c;
	matrixAboutZ[1][2] = 0;
	matrixAboutZ[2][0] = 0;
	matrixAboutZ[2][1] = 0;
	matrixAboutZ[2][2] = 1;

	for (int i=0; i < 3; i++)
	for (int j=0; j < 3; j++)
	{
		m_rtzaInvRotM[i][j] = 0;
		for (int k=0; k < 3; k++)
			m_rtzaInvRotM[i][j] += matrixAboutZ[i][k]*invRotM[k][j];
	}
}
