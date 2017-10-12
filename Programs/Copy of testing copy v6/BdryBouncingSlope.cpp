// BdryBouncingSlope.cpp: implementation of the CBdryBouncingSlope class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BdryBouncingSlope.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBdryBouncingSlope::CBdryBouncingSlope()
{

}

CBdryBouncingSlope::CBdryBouncingSlope(CVector3D* direction, CVector3D* ctrPos,
									   SimuValue sideLength, SimuValue rotationAngleAboutZAxis)
: CBdrySquareSlope(direction, ctrPos, sideLength, rotationAngleAboutZAxis)
{
}

CBdryBouncingSlope::~CBdryBouncingSlope()
{

}

bool CBdryBouncingSlope::EnforceSquareSlopeConstraint(CVector3D &stdPos, CVector3D &stdVel,
													  SimuValue timeStep)
{
	bool bCollisionOccurs = CBdrySquareSlope::EnforceSquareSlopeConstraint(
												stdPos, stdVel, timeStep);
	if (stdPos[Z] >= m_maxBouncingDist)
		return bCollisionOccurs;

	SimuValue maxBouncingForce = m_maxBouncingForce;
	if (!bCollisionOccurs)
	{
		maxBouncingForce *= (m_maxBouncingDist - stdPos[Z])/m_maxBouncingDist;
	}

	stdVel[Z] += maxBouncingForce*timeStep;

	return bCollisionOccurs;
}
