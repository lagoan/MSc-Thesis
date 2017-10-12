// Point3DOnEdge.cpp: implementation of the CPoint3DOnEdge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Point3DOnEdge.h"
#include "Edge3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoint3DOnEdge::CPoint3DOnEdge()
{
	m_p3doeEdges = new TGeomElemArray<CEdge3D>();
	m_p3doeEdges->InitializeClassArray(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
}

CPoint3DOnEdge::~CPoint3DOnEdge()
{
	delete m_p3doeEdges;
}

CEdge3D* CPoint3DOnEdge::GetEdgeWithPoint(CPoint3DOnEdge* pPoint)
{
	for (int i=0; i < m_p3doeEdges->m_paNum; i++)
	{
		CEdge3D* pEdge = m_p3doeEdges->m_paPtrs[i];
		if (pEdge->GetTheOtherPoint(this) == pPoint)
			return pEdge;
	}
	return NULL;
}
