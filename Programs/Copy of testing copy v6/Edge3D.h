// Edge3D.h: interface for the CEdge3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGE3D_H__C5026127_3BC1_49EB_B56B_D6DDDA7BD5EC__INCLUDED_)
#define AFX_EDGE3D_H__C5026127_3BC1_49EB_B56B_D6DDDA7BD5EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"
#include "Point3DOnEdge.h"

class CEdge3D : public CGeometryElement  
{
public:
	static int m_e3dMaxId;

	CPoint3DOnEdge* m_e3dPoints[2];

	CEdge3D();
	virtual ~CEdge3D();

	void RenderGeometry();

	void SetEdgePoints(CPoint3DOnEdge* pPoint0, CPoint3DOnEdge* pPoint1);
	bool HasEdgePoint(CPoint3DOnEdge* pPoint);
	CPoint3DOnEdge* GetTheOtherPoint(CPoint3DOnEdge* pPoint);
};

#endif // !defined(AFX_EDGE3D_H__C5026127_3BC1_49EB_B56B_D6DDDA7BD5EC__INCLUDED_)
