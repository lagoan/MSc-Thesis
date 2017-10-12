// Tetrahedron.h: interface for the CTetrahedron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TETRAHEDRON_H__1CF63C3D_7CB8_46CD_A37C_87069DC03EC7__INCLUDED_)
#define AFX_TETRAHEDRON_H__1CF63C3D_7CB8_46CD_A37C_87069DC03EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"
#include "Point3DOnTetra.h"

class CTetrahedron : public CGeometryElement  
{
public:
	static int m_ttrMaxId;

	CPoint3DOnTetra* m_ttrPs[4];
	CTetrahedron* m_ttrNgbrTetras[4];

	CTetrahedron();
	virtual ~CTetrahedron();

	virtual void RenderGeometry();
	virtual void AssertGeometricConnectivity();
	virtual void SetTetrahedronPoints(CPoint3DOnTetra* p0, CPoint3DOnTetra* p1,
									  CPoint3DOnTetra* p2, CPoint3DOnTetra* p3);
	void SetTetraPointColor(CVector3D* color);
	bool HasTetraPoint(CPoint3DOnTetra* pPoint);
	void GetOppositePoints(int id, CPoint3DOnTetra* oppositePs[3]);
	bool SetupNgbrTetra();
	bool SetNgbrTetra(CPoint3DOnTetra* sharedPs[3], CTetrahedron* ngbrTetra);
	CTetrahedron* GetNgbrTetra(CPoint3DOnTetra* sharedPs[3]);
	bool HasNgbrTetra(CTetrahedron* ngbrTetra);
	void ReplaceNgbrTetra(CTetrahedron* oldTetra, CTetrahedron* newTetra);
	void DisconnectFromNgbrObjects();
	void GetBoundingBox(CVector3D &minPos, CVector3D &maxPos);
};

#endif // !defined(AFX_TETRAHEDRON_H__1CF63C3D_7CB8_46CD_A37C_87069DC03EC7__INCLUDED_)
