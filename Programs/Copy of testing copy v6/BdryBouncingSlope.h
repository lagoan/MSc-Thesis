// BdryBouncingSlope.h: interface for the CBdryBouncingSlope class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDRYBOUNCINGSLOPE_H__E1225C1A_6A72_4086_8C7D_EFC6EB704604__INCLUDED_)
#define AFX_BDRYBOUNCINGSLOPE_H__E1225C1A_6A72_4086_8C7D_EFC6EB704604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BdrySquareSlope.h"

class CBdryBouncingSlope : public CBdrySquareSlope  
{
public:
	SimuValue m_maxBouncingDist;
	SimuValue m_maxBouncingForce;

	CBdryBouncingSlope();
	CBdryBouncingSlope(CVector3D* direction, CVector3D* ctrPos,
					   SimuValue sideLength, SimuValue rotationAngleAboutZAxis);
	virtual ~CBdryBouncingSlope();

	virtual bool EnforceSquareSlopeConstraint(CVector3D &stdPos, CVector3D &stdVel, SimuValue timeStep);
};

#endif // !defined(AFX_BDRYBOUNCINGSLOPE_H__E1225C1A_6A72_4086_8C7D_EFC6EB704604__INCLUDED_)
