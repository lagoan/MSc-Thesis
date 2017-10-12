// Point3DOnTetra.h: interface for the CPoint3DOnTetra class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT3DONTETRA_H__3A5776C7_9058_4C68_8C90_56707ECC7021__INCLUDED_)
#define AFX_POINT3DONTETRA_H__3A5776C7_9058_4C68_8C90_56707ECC7021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Point3D.h"
#include "TGeomElemArray.h"

class CTetrahedron;

class CPoint3DOnTetra : public CPoint3D  
{
public:
	bool m_p3dotArtificial;
	TGeomElemArray<CTetrahedron>* m_p3dotTetras;

	CPoint3DOnTetra();
	virtual ~CPoint3DOnTetra();

	virtual void AssertGeometricConnectivity();
	bool IsAnyTetraDeleted();
};

#endif // !defined(AFX_POINT3DONTETRA_H__3A5776C7_9058_4C68_8C90_56707ECC7021__INCLUDED_)
