// TorusOnXYPlane.cpp: implementation of the CTorusOnXYPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "TorusOnXYPlane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTorusOnXYPlane::CTorusOnXYPlane()
{
}

CTorusOnXYPlane::CTorusOnXYPlane(SimuValue innerRadius, SimuValue outerRadius,
								 CVector3D* ctrPos, CVector3D* direction)
: CRotationToZAxis(direction)
{
	m_trsInnerRadius = innerRadius;
	m_trsOuterRadius = outerRadius;
	m_trsCtrPos.SetValue(ctrPos);
	ComputeTorusVariables();
}

CTorusOnXYPlane::~CTorusOnXYPlane()
{

}

void CTorusOnXYPlane::DrawTorusOnXYPlane()
{
	const int numSegments = 20;
	CSimuUtility::DrawCircleOnZ0ByOpenGL(m_trsInnerRadius, 0, numSegments);
	CSimuUtility::DrawCircleOnZ0ByOpenGL(m_trsOuterRadius, 0, numSegments);
	glPushMatrix();
	CSimuUtility::DrawCircleOnZ0ByOpenGL(m_trsMiddleRadius, m_trsNeckRadius, numSegments);
	glPopMatrix();
	glPushMatrix();
	CSimuUtility::DrawCircleOnZ0ByOpenGL(m_trsMiddleRadius, -m_trsNeckRadius, numSegments);
	glPopMatrix();
	// draw 4 neck circles
	glPushMatrix();
	SimuTranslate(m_trsMiddleRadius, 0, 0);
	SimuRotate(90, 1, 0, 0);
	CSimuUtility::DrawCircleOnZ0ByOpenGL(m_trsNeckRadius, 0, numSegments);
	glPopMatrix();
	glPushMatrix();
	SimuTranslate(-m_trsMiddleRadius, 0, 0);
	SimuRotate(90, 1, 0, 0);
	CSimuUtility::DrawCircleOnZ0ByOpenGL(m_trsNeckRadius, 0, numSegments);
	glPopMatrix();
	glPushMatrix();
	SimuTranslate(0, m_trsMiddleRadius, 0);
	SimuRotate(90, 0, 1, 0);
	CSimuUtility::DrawCircleOnZ0ByOpenGL(m_trsNeckRadius, 0, numSegments);
	glPopMatrix();
	glPushMatrix();
	SimuTranslate(0, -m_trsMiddleRadius, 0);
	SimuRotate(90, 0, 1, 0);
	CSimuUtility::DrawCircleOnZ0ByOpenGL(m_trsNeckRadius, 0, numSegments);
	glPopMatrix();
}

void CTorusOnXYPlane::ComputeTorusVariables()
{
	m_trsMiddleRadius = 0.5*(m_trsInnerRadius + m_trsOuterRadius);
	m_trsNeckRadius = 0.5*(m_trsOuterRadius - m_trsInnerRadius);
}

// return 0 if on or inside torus; otherwise, return positive value.
SimuValue CTorusOnXYPlane::GetDistanceToTorus(CVector3D &standardPos)
{
	SimuValue distToZAxis = sqrt(standardPos[X]*standardPos[X] + standardPos[Y]*standardPos[Y]);
	SimuValue horizDist = distToZAxis - m_trsMiddleRadius;
	// in the cutting plane, the vertical distance is standardPos[Z].
	SimuValue distToMidCircle = sqrt(standardPos[Z]*standardPos[Z] + horizDist*horizDist);
	if (distToMidCircle > m_trsNeckRadius)
		return distToMidCircle - m_trsNeckRadius;
	return 0;
}
