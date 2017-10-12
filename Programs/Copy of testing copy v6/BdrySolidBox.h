// BdrySolidBox.h: interface for the CBdrySolidBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDRYSOLIDBOX_H__C2887E4E_D24C_45FB_8507_099201973025__INCLUDED_)
#define AFX_BDRYSOLIDBOX_H__C2887E4E_D24C_45FB_8507_099201973025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Boundary.h"
#include "BoxOnXYPlane.h"

class CBdrySolidBox : public CBoundary, public CBoxOnXYPlane  
{
public:
	static int m_bsbMaxId;

	SimuValue m_bsbHalfX;
	SimuValue m_bsbHalfY;

	enum BOX_SIDE_ID {
		BOX_SIDE_POS_X,
		BOX_SIDE_NEG_X,
		BOX_SIDE_POS_Y,
		BOX_SIDE_NEG_Y,
		BOX_SIDE_POS_Z,
		BOX_SIDE_NEG_Z,
		BOX_SIX_SIDES
	};

	CBdrySolidBox();
	CBdrySolidBox(SimuValue heightOnZ, SimuValue widthOnX, SimuValue widthOnY,
				  SimuValue angleAboutZ, CVector3D* btmCtrPos, CVector3D* direction);
	virtual ~CBdrySolidBox();

	void InitializeSolidBox();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);

	virtual void MoveBoundary(SimuValue curTime, SimuValue timeStep);
	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
	virtual bool PosBreakBoundary(CVector3D* pPos);
	virtual void AddEnclosedPrtl(CFluidPrtl* pPrtl);
	virtual SimuValue GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl);

	bool IsInsideBox(CVector3D &pos);
	bool EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel, SimuValue timeStep);
};

#endif // !defined(AFX_BDRYSOLIDBOX_H__C2887E4E_D24C_45FB_8507_099201973025__INCLUDED_)
