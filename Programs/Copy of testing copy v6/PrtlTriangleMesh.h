// PrtlTriangleMesh.h: interface for the CPrtlTriangleMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLTRIANGLEMESH_H__92C0D43F_69BA_47D9_B4E1_B0CF1B20F1F4__INCLUDED_)
#define AFX_PRTLTRIANGLEMESH_H__92C0D43F_69BA_47D9_B4E1_B0CF1B20F1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"
#include "MeshPrtl.h"
#include "PrtlTriangle.h"

class CPrtlTriangleMesh : public CGeometryElement  
{
public:
	TGeomElemArray<CMeshPrtl> m_ptmTriPrtls;
	TGeomElemArray<CPrtlTriangle> m_ptmTris;

	CPrtlTriangleMesh();
	virtual ~CPrtlTriangleMesh();

	virtual void RenderGeometry();

	void ExportTriangleMeshIntoPOVRayMesh2ObjectFile(FILE* fp);
	void ImportTriangleMeshIntoPOVRayMesh2ObjectFile(FILE* fp);
};

#endif // !defined(AFX_PRTLTRIANGLEMESH_H__92C0D43F_69BA_47D9_B4E1_B0CF1B20F1F4__INCLUDED_)
