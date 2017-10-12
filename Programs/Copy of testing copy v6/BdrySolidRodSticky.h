// BdrySolidRodSticky.h: interface for the CBdrySolidRodSticky class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDRYSOLIDRODSTICKY_H__82C062C0_7423_4361_9893_3D12794DFA3D__INCLUDED_)
#define AFX_BDRYSOLIDRODSTICKY_H__82C062C0_7423_4361_9893_3D12794DFA3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BdrySolidRod.h"

class CBdrySolidRodSticky : public CBdrySolidRod  
{
public:
	CBdrySolidRodSticky();
	CBdrySolidRodSticky(SimuValue length, SimuValue radius,
						SimuValue capHeight, bool bCylTopCap, bool bCylBtmCap,
						CVector3D* topCtrPos, CVector3D* direction);
	virtual ~CBdrySolidRodSticky();

	SimuValue ComputeDistanceToRod(CVector3D &pPos);
	bool EnforceBoundaryConstraint(CVector3D &pPos, CVector3D &pVel, SimuValue timeStep);
};

#endif // !defined(AFX_BDRYSOLIDRODSTICKY_H__82C062C0_7423_4361_9893_3D12794DFA3D__INCLUDED_)
