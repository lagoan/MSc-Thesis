// PrtlTriangleMesh.cpp: implementation of the CPrtlTriangleMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "PrtlTriangleMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtlTriangleMesh::CPrtlTriangleMesh()
{
}

CPrtlTriangleMesh::~CPrtlTriangleMesh()
{

}

void CPrtlTriangleMesh::RenderGeometry()
{
	if (CSimuManager::DRAW_EXTR_SRFC_PARTICLES)
		m_ptmTriPrtls.RenderGeometryElements();

	if (CSimuManager::DRAW_EXTR_SRFC_TRIANGLES)
		m_ptmTris.RenderGeometryElements();
}
