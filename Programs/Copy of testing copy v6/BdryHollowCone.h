// BdryHollowCone.h: interface for the CBdryHollowCone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDRYHOLLOWCONE_H__64269B47_D9D7_43C0_B078_9CCBCCC45CDF__INCLUDED_)
#define AFX_BDRYHOLLOWCONE_H__64269B47_D9D7_43C0_B078_9CCBCCC45CDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Boundary.h"
#include "ConeAlongZAxis.h"

class CBdryHollowCone : public CBoundary, public CConeAlongZAxis  
{
public:
	static int m_hcMaxId;

	CBdryHollowCone();
	CBdryHollowCone(SimuValue coneLen, SimuValue coneTopRadius, SimuValue coneBtmRadius,
					CVector3D* btmCtrPos, CVector3D* direction);
	virtual ~CBdryHollowCone();

	void InitializeHollowCone();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ExportBoundaryParameters(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);
	virtual void ImportBoundaryParameters(FILE* fp);

	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);

	bool EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel, SimuValue timeStep);
};

#endif // !defined(AFX_BDRYHOLLOWCONE_H__64269B47_D9D7_43C0_B078_9CCBCCC45CDF__INCLUDED_)
