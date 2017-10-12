// PrtlInSrfcMesh.h: interface for the CPrtlInSrfcMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLINSRFCMESH_H__94C55EBD_5F7D_46AE_BC8F_680699E66D8A__INCLUDED_)
#define AFX_PRTLINSRFCMESH_H__94C55EBD_5F7D_46AE_BC8F_680699E66D8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TPrtlGrid.h"
#include "MeshPrtl.h"
#include "PrtlTriangleMesh.h"

class CPrtlInSrfcMesh  
{
public:
	enum { INSIDE_GRID_PRTL, OUTSIDE_GRID_PRTL, UNDECIDED_GRID_PRTL };
	TPrtlGrid<CMeshPrtl> m_pismGrid;
	TGeomElemArray<CMeshPrtl> m_pismGridPrtls;

	CPrtlInSrfcMesh();
	virtual ~CPrtlInSrfcMesh();

	CMeshPrtl* m_pismPrtl;
	CPrtlTriangle* m_pismTri;
	TGeomElemArray<CPrtlTriangleMesh>* m_pismArySrfcMesh;
	void RenderPrtlInSrfcMesh();

	void CreatePrtlInSrfcMesh(TGeomElemArray<CPrtlTriangleMesh> &arySrfcMesh);
	void CreateBoundingBoxOverSrfcMesh(TPrtlGrid<CMeshPrtl> &pismGrid,
										TGeomElemArray<CPrtlTriangleMesh> &arySrfcMesh);
	void CreatePrtlAtGridLocation(TPrtlGrid<CMeshPrtl> &pismGrid,
										 TGeomElemArray<CMeshPrtl> &pismGridPrtls);
	void IdentifyInsideGridPrtls(TPrtlGrid<CMeshPrtl> &pismGrid,
										TGeomElemArray<CMeshPrtl> &pismGridPrtls,
										TGeomElemArray<CPrtlTriangle> &srfcMesh);
	void TestInsidePrtlOnTri(CMeshPrtl* pPrtl, CPrtlTriangle* pTri,
									CVector3D sideNormal[3], CVector3D *triNormal, int &label);
};

#endif // !defined(AFX_PRTLINSRFCMESH_H__94C55EBD_5F7D_46AE_BC8F_680699E66D8A__INCLUDED_)
