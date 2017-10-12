// Edge3D.cpp: implementation of the CEdge3D class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Edge3D.h"


int CEdge3D::m_e3dMaxId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEdge3D::CEdge3D()
{
	m_e3dMaxId ++;

	m_e3dPoints[0] = NULL;
	m_e3dPoints[1] = NULL;
}

CEdge3D::~CEdge3D()
{

}

void CEdge3D::RenderGeometry()
{
	glBegin(GL_LINES);
	SimuVertex3v(m_e3dPoints[0]->m_p3dPos.v);
	SimuVertex3v(m_e3dPoints[1]->m_p3dPos.v);
	glEnd();
}

void CEdge3D::SetEdgePoints(CPoint3DOnEdge* pPoint0, CPoint3DOnEdge* pPoint1)
{
	m_e3dPoints[0] = pPoint0;
	m_e3dPoints[1] = pPoint1;

	pPoint0->m_p3doeEdges->AppendOnePointer(this);
	pPoint1->m_p3doeEdges->AppendOnePointer(this);
}

bool CEdge3D::HasEdgePoint(CPoint3DOnEdge* pPoint)
{
	if (m_e3dPoints[0] == pPoint)
		return true;
	if (m_e3dPoints[1] == pPoint)
		return true;
	return false;
}

CPoint3DOnEdge* CEdge3D::GetTheOtherPoint(CPoint3DOnEdge* pPoint)
{
	if (m_e3dPoints[0] == pPoint)
		return m_e3dPoints[1];
	if (m_e3dPoints[1] == pPoint)
		return m_e3dPoints[0];
	return NULL;
}
