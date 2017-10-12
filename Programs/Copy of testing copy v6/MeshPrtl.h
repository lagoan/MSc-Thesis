// MeshPrtl.h: interface for the CMeshPrtl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHPRTL_H__C78D90C2_C84E_4B7D_99A2_B816BA1FC1B1__INCLUDED_)
#define AFX_MESHPRTL_H__C78D90C2_C84E_4B7D_99A2_B816BA1FC1B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VirtualPrtl.h"
#include "PrtlTriangle.h"

class CMeshPrtl : virtual public CVirtualPrtl
{
public:
	// for avoiding repeatitive visits and
	// for output surface mesh into POVRay mesh2 object file
	int m_mpId;

	bool m_mpInside;
	bool m_mpOnSurface;
	bool m_mpFixedPosition;
	CVector3D m_mpNormal;
	SimuValue m_mpDistanceToSrfc;

	TGeomElemArray<CPrtlTriangle> m_mpTris;

	CMeshPrtl();
	virtual ~CMeshPrtl();

	virtual void RenderGeometry();

	virtual void AssertGeometricConnectivity();
	void AssertPrtlConnectivity(char location[]="CMeshPrtl::CheckPrtlConnectivity()");

	void SetClosestSrfcPos(CVector3D *srfcPos);
	void GetClosestSrfcPos(CVector3D *srfcPos);

	bool HasNeighborTriPrtl(CMeshPrtl* pPrtl);
	bool HasNeighborTri(CPrtlTriangle* pTri);
	void ComputeNormalFromNgbrTris();
};

#endif // !defined(AFX_MESHPRTL_H__C78D90C2_C84E_4B7D_99A2_B816BA1FC1B1__INCLUDED_)
