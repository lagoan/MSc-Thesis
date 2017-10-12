// SimuBoundaryBox.h: interface for the CSimuBoundaryBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUBOUNDARYBOX_H__3C77564D_419E_46E0_BCE9_4EE127062FF5__INCLUDED_)
#define AFX_SIMUBOUNDARYBOX_H__3C77564D_419E_46E0_BCE9_4EE127062FF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Boundary.h"

class CSimuBoundaryBox : public CBoundary  
{
public:
	CVector3D m_boxLBCCornor; // left, bottom, close corner
	CVector3D m_boxRUFCornor; // right, up, far corner
	bool m_boxOpenRoof; // if roof is open, the 4 sides are unlimited high.

	CSimuBoundaryBox();
	CSimuBoundaryBox(CVector3D* boxBottomCenter, SimuValue boxSideWidth);
	CSimuBoundaryBox(CVector3D* boxLBCCornor, CVector3D* boxRUFCornor);
	virtual ~CSimuBoundaryBox();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ExportBoundaryParameters(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);
	virtual void ImportBoundaryParameters(FILE* fp);

	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
	virtual bool PosBreakBoundary(CVector3D* pPos);
};

#endif // !defined(AFX_SIMUBOUNDARYBOX_H__3C77564D_419E_46E0_BCE9_4EE127062FF5__INCLUDED_)
