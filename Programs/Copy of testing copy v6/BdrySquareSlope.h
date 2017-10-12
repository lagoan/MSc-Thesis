// BdrySquareSlope.h: interface for the CBdrySquareSlope class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDRYSQUARESLOPE_H__FC8CA355_47FD_4A39_9101_8396A60C0A7A__INCLUDED_)
#define AFX_BDRYSQUARESLOPE_H__FC8CA355_47FD_4A39_9101_8396A60C0A7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Boundary.h"
#include "RotationToZAxis.h"

class CBdrySquareSlope : public CBoundary, public CRotationToZAxis  
{
public:
	static int m_bssMaxId;

	SimuValue m_bssSlopeThickness;
	SimuValue m_bssSideLen;
	CVector3D m_bssCtrPos;
	CVector3D m_bssSlopeStadDirection;

	// slope is on the z=0 plane after rotation
	CBdrySquareSlope();
	CBdrySquareSlope(CVector3D* direction, CVector3D* ctrPos,
					 SimuValue sideLength, SimuValue rotationAngleAboutZAxis);
	virtual ~CBdrySquareSlope();

	void InitializeSquareSlope();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);
	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
	virtual bool PosBreakBoundary(CVector3D* pPos);

	virtual bool EnforceSquareSlopeConstraint(CVector3D &stdPos, CVector3D &stdVel, SimuValue timeStep);
	virtual SimuValue GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl);
};

#endif // !defined(AFX_BDRYSQUARESLOPE_H__FC8CA355_47FD_4A39_9101_8396A60C0A7A__INCLUDED_)
