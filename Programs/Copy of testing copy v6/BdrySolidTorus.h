// BdrySolidTorus.h: interface for the CBdrySolidTorus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDRYSOLIDTORUS_H__6CE5629C_A831_4298_9D6E_8670671541FE__INCLUDED_)
#define AFX_BDRYSOLIDTORUS_H__6CE5629C_A831_4298_9D6E_8670671541FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TorusOnXYPlane.h"
#include "Boundary.h"

class CBdrySolidTorus : 	public CBoundary, 	public CTorusOnXYPlane  
{
public:
	CBdrySolidTorus();
	CBdrySolidTorus(SimuValue innerRadius, SimuValue outerRadius,
					CVector3D* ctrPos, CVector3D* direction);
	virtual ~CBdrySolidTorus();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);

	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
	virtual bool PosBreakBoundary(CVector3D* pPos);
	virtual SimuValue GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl);

protected:
	bool EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel, SimuValue timeStep);
	void GetStandardPosition(CVector3D &transformedPos, CVector3D &standardPos);
	void GetTransformedPosition(CVector3D &standardPos, CVector3D &transformedPos);
};

#endif // !defined(AFX_BDRYSOLIDTORUS_H__6CE5629C_A831_4298_9D6E_8670671541FE__INCLUDED_)
