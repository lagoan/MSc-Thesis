// TriangleOnTetra.cpp: implementation of the CTriangleOnTetra class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "TriangleOnTetra.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CTriangleOnTetra::m_totMaxId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriangleOnTetra::CTriangleOnTetra()
{
	m_totMaxId ++;

	m_totTriPs[0] = NULL;
	m_totTriPs[1] = NULL;
	m_totTriPs[2] = NULL;
}

CTriangleOnTetra::~CTriangleOnTetra()
{

}

void CTriangleOnTetra::RenderGeometry()
{
	if (m_geHighlit)
	{
		glPushAttrib(GL_CURRENT_BIT);
		SimuColor3v(CSimuManager::COLOR_RED->v);
	}
	if (CSimuManager::DRAW_EDGE_ONLY)
	{
		glBegin(GL_LINES);
		SimuVertex3v(m_totTriPs[0]->m_p3dPos.v);
		SimuVertex3v(m_totTriPs[1]->m_p3dPos.v);
		SimuVertex3v(m_totTriPs[1]->m_p3dPos.v);
		SimuVertex3v(m_totTriPs[2]->m_p3dPos.v);
		SimuVertex3v(m_totTriPs[2]->m_p3dPos.v);
		SimuVertex3v(m_totTriPs[0]->m_p3dPos.v);
		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLES);
		SimuVertex3v(m_totTriPs[0]->m_p3dPos.v);
		SimuVertex3v(m_totTriPs[1]->m_p3dPos.v);
		SimuVertex3v(m_totTriPs[2]->m_p3dPos.v);
		glEnd();
	}
	if (m_geHighlit)
	{
		glPopAttrib();
		CSimuUtility::RestoreOpenGLPreviousColor();
	}
}

void CTriangleOnTetra::SetTrianglePoints(CPoint3DOnTetra* pP0,
										 CPoint3DOnTetra* pP1,
										 CPoint3DOnTetra* pP2)
{
	m_totTriPs[0] = pP0;
	m_totTriPs[1] = pP1;
	m_totTriPs[2] = pP2;
}

void CTriangleOnTetra::SetTrianglePointColor(CVector3D* color)
{
	m_totTriPs[0]->SetPoint3DColor(color);
	m_totTriPs[1]->SetPoint3DColor(color);
	m_totTriPs[2]->SetPoint3DColor(color);
}
