// SimuBoundaryCylinder.h: interface for the CSimuBoundaryCylinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUBOUNDARYCYLINDER_H__C884543B_5026_4D59_AFD7_34E53425EFBF__INCLUDED_)
#define AFX_SIMUBOUNDARYCYLINDER_H__C884543B_5026_4D59_AFD7_34E53425EFBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Boundary.h"
#include "CylinderAlongZAxis.h"
#include "FluidMeshPrtl.h"

class CSimuBoundaryCylinder : public CBoundary  , public CCylinderAlongZAxis
{
public:
	static int m_bcMaxId;

	bool m_bcOpenTop;
	bool m_bcOpenBtm;
	SimuValue m_bcOpenOutletWidth;

	CSimuBoundaryCylinder();
	CSimuBoundaryCylinder(SimuValue length, SimuValue radius,
						  CVector3D* topCtrPos, CVector3D* direction);
	virtual ~CSimuBoundaryCylinder();

	void InitializeCylinder();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ExportBoundaryParameters(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);
	virtual void ImportBoundaryParameters(FILE* fp);

	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
	void EnforceBoundaryConstraint(CFluidMeshPrtl* pPrtl);
	virtual bool PosBreakBoundary(CVector3D* pPos);
	bool IsPosInCylinder(CVector3D *pos);

protected:
	// the bigger <openSlopeRatio>, the more open outlet and thus the more chance inside
	// openSlopeRatio = 0 means vertical open outlet.
	bool IsInCylinder(CVector3D &pos);
	bool IsBreakingCylinderWall(CVector3D &pPos, CVector3D &revNormal);
	bool IsBreakingCylinderOutlet(CVector3D &pPos, CVector3D &revNormal);
	bool EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel, SimuValue timeStep);
	int EnforceCylinderTopBoundary(CVector3D &pPos, CVector3D &pVel);
	int EnforceCylinderBtmBoundary(CVector3D &pPos, CVector3D &pVel);
	bool EnforceCylinderWallBoundary(CVector3D &pPos, CVector3D &pVel);
	void EnforceCylinderOutletBoundary(CVector3D &pPos, CVector3D &pVel);
};

#endif // !defined(AFX_SIMUBOUNDARYCYLINDER_H__C884543B_5026_4D59_AFD7_34E53425EFBF__INCLUDED_)
