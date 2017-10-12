// PrtlEdge.cpp: implementation of the CPrtlEdge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "PrtlEdge.h"
#include "MeshPrtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtlEdge::CPrtlEdge()
{
	m_pePs[0] = NULL;
	m_pePs[1] = NULL;
}

CPrtlEdge::~CPrtlEdge()
{

}

void CPrtlEdge::RenderGeometry()
{
	glBegin(GL_LINES);
	SimuVertex3v(m_pePs[0]->m_vpPos->v);
	SimuVertex3v(m_pePs[1]->m_vpPos->v);
	glEnd();
}

bool CPrtlEdge::HasEdgePrtl(CMeshPrtl* pPrtl)
{
	if (m_pePs[0] == pPrtl) return true;
	if (m_pePs[1] == pPrtl) return true;
	return false;
}

void CPrtlEdge::ReplaceEdgePrtl(CMeshPrtl* oldPrtl, CMeshPrtl* newPrtl)
{
	char location[] = "CPrtlEdge::ReplaceEdgePrtl(oldPrtl, newPrtl)";

	for (int i=0; i < 2; i++)
	{
		if (m_pePs[i] == oldPrtl)
		{
			m_pePs[i] = newPrtl;
			if (m_pePs[0] == m_pePs[1])
				CSimuMsg::ExitWithMessage(location, "Two edge prtls are same.");
			return;
		}
	}
	CSimuMsg::ExitWithMessage(location, "oldPrtl is not found.");
}

CMeshPrtl* CPrtlEdge::GetTheOtherMeshPrtl(CMeshPrtl* pPrtl)
{
	if (m_pePs[0] == pPrtl)
		return m_pePs[1];
	if (m_pePs[1] == pPrtl)
		return m_pePs[0];
	CSimuMsg::ExitWithMessage("CPrtlEdge::GetTheOtherMeshPrtl(pPrtl)",
							"Mesh prtl is not found.");
	return NULL;
}

bool CPrtlEdge::IsSameEdgeAs(CPrtlEdge* pEdge)
{
	if (m_pePs[0] == pEdge->m_pePs[0]
	 && m_pePs[1] == pEdge->m_pePs[1])
		return true;
	if (m_pePs[0] == pEdge->m_pePs[1]
	 && m_pePs[1] == pEdge->m_pePs[0])
		return true;
	return false;
}

SimuValue CPrtlEdge::ComputeEdgeLength()
{
	return m_pePs[0]->m_vpPos->GetDistanceToVector(m_pePs[1]->m_vpPos);
}
