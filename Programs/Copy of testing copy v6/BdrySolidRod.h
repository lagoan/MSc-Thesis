// BdrySolidRod.h: interface for the CBdrySolidRod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDRYSOLIDROD_H__109B180F_8563_4FF8_9A35_E1442F2CB62B__INCLUDED_)
#define AFX_BDRYSOLIDROD_H__109B180F_8563_4FF8_9A35_E1442F2CB62B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Boundary.h"
#include "CylinderAlongZAxis.h"

class CBdrySolidRod : public CBoundary, public CCylinderAlongZAxis
{
public:
	static int m_bsrMaxId;

	CBdrySolidRod();
	CBdrySolidRod(SimuValue length, SimuValue radius,
				  SimuValue capHeight, bool bCylTopCap, bool bCylBtmCap,
				  CVector3D* topCtrPos, CVector3D* direction);
	virtual ~CBdrySolidRod();

	void InitializeSolidRod();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);

	virtual void MoveBoundary(SimuValue curTime, SimuValue timeStep);
	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
	virtual bool PosBreakBoundary(CVector3D* pPos);
	virtual void AddEnclosedPrtl(CFluidPrtl* pPrtl);
	virtual SimuValue GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl);

	bool IsInsideRod(CVector3D &pos);
	bool ReflectAtClosestPosOnBdry(CVector3D &pPos, CVector3D &pVel, SimuValue timeStep);
	bool EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel, SimuValue timeStep);
	bool EnforceCylinderWallBoundary(CVector3D &pPos, CVector3D &pVel);
};

#endif // !defined(AFX_BDRYSOLIDROD_H__109B180F_8563_4FF8_9A35_E1442F2CB62B__INCLUDED_)
