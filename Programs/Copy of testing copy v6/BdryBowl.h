// BdryBowl.h: interface for the CBdryBowl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDRYBOWL_H__D93AFF7A_F955_4087_83D5_5435BB1FF1A1__INCLUDED_)
#define AFX_BDRYBOWL_H__D93AFF7A_F955_4087_83D5_5435BB1FF1A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Boundary.h"

class CBdryBowl : public CBoundary,	public COvalAlong3DAxes    
{
public:
	SimuValue m_bbBigR2;
	SimuValue m_bbBigRadius;
	SimuValue m_bbRimRadius;
	SimuValue m_bbHeight;
	CVector3D m_bbBtmPos;

	CBdryBowl();
	CBdryBowl(CVector3D* direction, CVector3D* btmPos, SimuValue height, SimuValue bigRadius);
	virtual ~CBdryBowl();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);
	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
};

#endif // !defined(AFX_BDRYBOWL_H__D93AFF7A_F955_4087_83D5_5435BB1FF1A1__INCLUDED_)
