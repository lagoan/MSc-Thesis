// Triangle3D.h: interface for the CTriangle3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIANGLE3D_H__B9A79A93_EEE5_444C_B8C5_17C72748A167__INCLUDED_)
#define AFX_TRIANGLE3D_H__B9A79A93_EEE5_444C_B8C5_17C72748A167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"
#include "Edge3D.h"

class CTriangle3D : public CGeometryElement  
{
public:
	static int m_t3dMaxId;

	CEdge3D* m_t3dEdges[3];
	CVector3D m_t3dNormal;

	CTriangle3D();
	virtual ~CTriangle3D();

	virtual void RenderGeometry();
	void SetTrianglePointColor(CVector3D* color);

	void SetTriangleEdges(CEdge3D* pEdge0, CEdge3D* pEdge1, CEdge3D* pEdge2);
	bool AddOneEdge(CEdge3D* pEdge);
	void GetTrianglePoints(CPoint3DOnEdge* pPoints[3]);
	SimuValue GetDistanceToTriangle(CVector3D* pos);
};

#endif // !defined(AFX_TRIANGLE3D_H__B9A79A93_EEE5_444C_B8C5_17C72748A167__INCLUDED_)
