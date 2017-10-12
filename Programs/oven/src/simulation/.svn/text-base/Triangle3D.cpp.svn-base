// Triangle3D.cpp: implementation of the CTriangle3D class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Triangle3D.h"


int CTriangle3D::m_t3dMaxId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriangle3D::CTriangle3D()
{
	m_t3dMaxId ++;

	m_t3dEdges[0] = NULL;
	m_t3dEdges[1] = NULL;
	m_t3dEdges[2] = NULL;
}

CTriangle3D::~CTriangle3D()
{

}

void CTriangle3D::RenderGeometry()
{
	if (CSimuManager::DRAW_EDGE_ONLY)
	{
		m_t3dEdges[0]->RenderGeometry();
		m_t3dEdges[1]->RenderGeometry();
		m_t3dEdges[2]->RenderGeometry();
	}
	else
	{
		CPoint3DOnEdge* pPoints[3];
		GetTrianglePoints(pPoints);
		glBegin(GL_TRIANGLES);
		SimuVertex3v(pPoints[0]->m_p3dPos.v);
		SimuVertex3v(pPoints[1]->m_p3dPos.v);
		SimuVertex3v(pPoints[2]->m_p3dPos.v);
		glEnd();
	}
}

void CTriangle3D::SetTrianglePointColor(CVector3D* color)
{
	CPoint3DOnEdge* pPoints[3];
	GetTrianglePoints(pPoints);
	pPoints[0]->SetPoint3DColor(color);
	pPoints[1]->SetPoint3DColor(color);
	pPoints[2]->SetPoint3DColor(color);
}

void CTriangle3D::SetTriangleEdges(CEdge3D* pEdge0, CEdge3D* pEdge1, CEdge3D* pEdge2)
{
	m_t3dEdges[0] = pEdge0;
	m_t3dEdges[1] = pEdge1;
	m_t3dEdges[2] = pEdge2;
}

bool CTriangle3D::AddOneEdge(CEdge3D* pEdge)
{
	if (m_t3dEdges[0] == NULL)
		m_t3dEdges[0] = pEdge;
	else if (m_t3dEdges[1] == NULL)
		m_t3dEdges[1] = pEdge;
	else if (m_t3dEdges[2] == NULL)
		m_t3dEdges[2] = pEdge;
	else
	{
		CSimuMsg::ExitWithMessage("CTriangle3D::AddOneEdge(pEdge)",
								"Too many edges in this triangle.");
		return false;
	}
	return true;
}

void CTriangle3D::GetTrianglePoints(CPoint3DOnEdge* pPoints[3])
{
	pPoints[0] = m_t3dEdges[0]->m_e3dPoints[0];
	pPoints[1] = m_t3dEdges[0]->m_e3dPoints[1];
	pPoints[2] = m_t3dEdges[1]->m_e3dPoints[0];
	if (pPoints[2] == pPoints[0] || pPoints[2] == pPoints[1])
		pPoints[2] = m_t3dEdges[1]->m_e3dPoints[1];
}

SimuValue CTriangle3D::GetDistanceToTriangle(CVector3D* pos)
{
	CVector3D tmpVector;
	tmpVector.SetValueAsSubstraction(pos, &m_t3dEdges[0]->m_e3dPoints[0]->m_p3dPos);
	return tmpVector.DotProduct(&m_t3dNormal);
}
