// Point3DOnEdge.h: interface for the CPoint3DOnEdge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT3DONEDGE_H__48E9F3A7_E3A8_4D1E_9D10_62189F151065__INCLUDED_)
#define AFX_POINT3DONEDGE_H__48E9F3A7_E3A8_4D1E_9D10_62189F151065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Point3DIntPos.h"
#include "TGeomElemArray.h"

class CEdge3D;

class CPoint3DOnEdge : public CPoint3DIntPos  
{
public:
	TGeomElemArray<CEdge3D> *m_p3doeEdges;

	CPoint3DOnEdge();
	virtual ~CPoint3DOnEdge();

	CEdge3D* GetEdgeWithPoint(CPoint3DOnEdge* pPoint);
};

#endif // !defined(AFX_POINT3DONEDGE_H__48E9F3A7_E3A8_4D1E_9D10_62189F151065__INCLUDED_)
