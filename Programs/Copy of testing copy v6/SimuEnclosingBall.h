// SimuEnclosingBall.h: interface for the CSimuEnclosingBall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUENCLOSINGBALL_H__7E3A315F_C974_4485_A7EF_826D4038FC2D__INCLUDED_)
#define AFX_SIMUENCLOSINGBALL_H__7E3A315F_C974_4485_A7EF_826D4038FC2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimuEnclosingBall  
{
public:
	bool m_ebShow;
	bool m_ebMovable;
	SimuValue m_ebRadius;
	CVector3D m_ebPos;
	CVector3D m_ebColor;

	CSimuEnclosingBall();
	virtual ~CSimuEnclosingBall();

	void RenderEnclosingBall();
};

#endif // !defined(AFX_SIMUENCLOSINGBALL_H__7E3A315F_C974_4485_A7EF_826D4038FC2D__INCLUDED_)
