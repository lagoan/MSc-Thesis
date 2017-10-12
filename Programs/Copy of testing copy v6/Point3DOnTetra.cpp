// Point3DOnTetra.cpp: implementation of the CPoint3DOnTetra class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Point3DOnTetra.h"
#include "Tetrahedron.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoint3DOnTetra::CPoint3DOnTetra()
{
	m_p3dotArtificial = false;
	m_p3dotTetras = new TGeomElemArray<CTetrahedron>();
	m_p3dotTetras->InitializeClassArray(false, SIMU_PARA_DEFAULT_ARRAY_ALLOC_SIZE);
}

CPoint3DOnTetra::~CPoint3DOnTetra()
{
	delete m_p3dotTetras;
}

void CPoint3DOnTetra::AssertGeometricConnectivity()
{
	char location[] = "CPoint3DOnTetra::AssertGeometricConnectivity()";

	if (m_geDeleted)
		CSimuMsg::ExitWithMessage(location, "This point has been labeled deleted.");
	for (int k=0; k < m_p3dotTetras->m_paNum; k++)
	{
		CTetrahedron* ngbrTetra = m_p3dotTetras->m_paPtrs[k];
		if (!ngbrTetra->HasTetraPoint(this))
			CSimuMsg::ExitWithMessage(location, "Error in point-tetra structure.");
	}
}

bool CPoint3DOnTetra::IsAnyTetraDeleted()
{
	for (int k=0; k < m_p3dotTetras->m_paNum; k++)
	{
		CTetrahedron* ngbrTetra = m_p3dotTetras->m_paPtrs[k];
		if (ngbrTetra->m_geDeleted)
			return true;
	}
	return false;
}
