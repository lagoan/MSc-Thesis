// Triangle3DIntNorm.cpp: implementation of the CTriangle3DIntNorm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Triangle3DIntNorm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriangle3DIntNorm::CTriangle3DIntNorm()
{

}

CTriangle3DIntNorm::~CTriangle3DIntNorm()
{

}

void CTriangle3DIntNorm::RenderGeometry()
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
		CVector3D normal;
		normal.SetValue(&m_t3dinNormal);
		normal.Normalize();
		glBegin(GL_TRIANGLES);
		SimuNormal3v(normal.v);
		SimuVertex3v(pPoints[0]->m_p3dPos.v);
		SimuVertex3v(pPoints[1]->m_p3dPos.v);
		SimuVertex3v(pPoints[2]->m_p3dPos.v);
		glEnd();
	}
}
