// SimuBoundaryTable.h: interface for the CSimuBoundaryTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUBOUNDARYTABLE_H__418FFA35_0EA5_4DAF_A09C_9386590C5EA7__INCLUDED_)
#define AFX_SIMUBOUNDARYTABLE_H__418FFA35_0EA5_4DAF_A09C_9386590C5EA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Boundary.h"

class CSimuBoundaryTable : public CBoundary  
{
public:
	CVector3D m_tblCenter;
	SimuValue m_tblSideLen;
	SimuValue m_tblHalfSideLen;
	SimuValue m_tblThickness;
	SimuValue m_tblMinX, m_tblMaxX;
	SimuValue m_tblMinZ, m_tblMaxZ;

	CSimuBoundaryTable();
	CSimuBoundaryTable(CVector3D* tblCenter, SimuValue tblSideLen);
	virtual ~CSimuBoundaryTable();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);

	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
	virtual bool PosBreakBoundary(CVector3D* pPos);

protected:
	void ComputeTableVariables();
};

#endif // !defined(AFX_SIMUBOUNDARYTABLE_H__418FFA35_0EA5_4DAF_A09C_9386590C5EA7__INCLUDED_)
