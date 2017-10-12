// Edge3DOnTriangle.h: interface for the CEdge3DOnTriangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGE3DONTRIANGLE_H__BFBFBC8F_D3A2_4CA4_B438_A3D109F9D422__INCLUDED_)
#define AFX_EDGE3DONTRIANGLE_H__BFBFBC8F_D3A2_4CA4_B438_A3D109F9D422__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Edge3D.h"
#include "Triangle3D.h"

class CEdge3DOnTriangle : public CEdge3D  
{
public:
	CTriangle3D* m_e3dotTris[2];

	CEdge3DOnTriangle();
	virtual ~CEdge3DOnTriangle();

	bool AddOneTriangle(CTriangle3D* pTriangle);
	void RemoveOneTriangle(CTriangle3D* pTriangle);
	void ReplaceOneTriangle(CTriangle3D* oldTriangle, CTriangle3D* newTriangle);
	CTriangle3D* GetTheOtherTriangle(CTriangle3D* pTriangle);
};

#endif // !defined(AFX_EDGE3DONTRIANGLE_H__BFBFBC8F_D3A2_4CA4_B438_A3D109F9D422__INCLUDED_)
