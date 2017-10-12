// TetraDelaunay.h: interface for the CTetraDelaunay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TETRADELAUNAY_H__2041B50B_B97D_4DA8_91D3_20FE3DD2A3CE__INCLUDED_)
#define AFX_TETRADELAUNAY_H__2041B50B_B97D_4DA8_91D3_20FE3DD2A3CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tetrahedron.h"

class CTetraDelaunay : public CTetrahedron  
{
public:
	bool m_ttrdNoUpsample;
	CVector3D m_ttrdCircumCtrPos;
	SimuValue m_ttrdCircumRSqr;

	CTetraDelaunay();
	virtual ~CTetraDelaunay();

	virtual void SetTetrahedronPoints(CPoint3DOnTetra* p0, CPoint3DOnTetra* p1,
									  CPoint3DOnTetra* p2, CPoint3DOnTetra* p3);
	bool IsArtificialTetra();
};

#endif // !defined(AFX_TETRADELAUNAY_H__2041B50B_B97D_4DA8_91D3_20FE3DD2A3CE__INCLUDED_)
