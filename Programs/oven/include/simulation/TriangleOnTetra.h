// TriangleOnTetra.h: interface for the CTriangleOnTetra class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIANGLEONTETRA_H__719B2BE3_901F_43B1_8FB9_795453AC823B__INCLUDED_)
#define AFX_TRIANGLEONTETRA_H__719B2BE3_901F_43B1_8FB9_795453AC823B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"
#include "Point3DOnTetra.h"

class CTriangleOnTetra : public CGeometryElement  
{
public:
	static int m_totMaxId;

	CPoint3DOnTetra* m_totTriPs[3];

	CTriangleOnTetra();
	virtual ~CTriangleOnTetra();

	virtual void RenderGeometry();
	void SetTrianglePoints(CPoint3DOnTetra* pP0, CPoint3DOnTetra* pP1, CPoint3DOnTetra* pP2);
	void SetTrianglePointColor(CVector3D* color);
};

#endif // !defined(AFX_TRIANGLEONTETRA_H__719B2BE3_901F_43B1_8FB9_795453AC823B__INCLUDED_)
