// Tetrahedron.cpp: implementation of the CTetrahedron class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Tetrahedron.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CTetrahedron::m_ttrMaxId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTetrahedron::CTetrahedron()
{
	m_ttrMaxId ++;

	m_ttrPs[0] = NULL;
	m_ttrPs[1] = NULL;
	m_ttrPs[2] = NULL;
	m_ttrPs[3] = NULL;

	m_ttrNgbrTetras[0] = NULL;
	m_ttrNgbrTetras[1] = NULL;
	m_ttrNgbrTetras[2] = NULL;
	m_ttrNgbrTetras[3] = NULL;
}

CTetrahedron::~CTetrahedron()
{

}

void CTetrahedron::RenderGeometry()
{
	if (CSimuManager::DRAW_HIGHLIT_ONLY
	 && !m_geHighlit)
		return;

	for (int i=0; i < 4; i++)
		m_ttrPs[i]->RenderGeometry();

	if (CSimuManager::DRAW_EDGE_ONLY)
	{
		glBegin(GL_LINES);
		for (int i=0; i < 4; i++)
		for (int j=i+1; j < 4; j++)
		{
			SimuVertex3v(m_ttrPs[i]->m_p3dPos.v);
			SimuVertex3v(m_ttrPs[j]->m_p3dPos.v);
		}
		glEnd();
	}
	else
	{
		CVector3D normal;
		CPoint3DOnTetra* oppositePs[3];
		for (int i=0; i < 4; i++)
		{
			if (m_ttrNgbrTetras[i] != NULL)
				continue;
			GetOppositePoints(i, oppositePs);
			CSimuUtility::ComputeTriangleNormalByRefPos(&oppositePs[0]->m_p3dPos,
														&oppositePs[1]->m_p3dPos,
														&oppositePs[2]->m_p3dPos,
														&m_ttrPs[i]->m_p3dPos, false,
														&normal);
			glBegin(GL_TRIANGLES);
			SimuNormal3v(normal.v);
			SimuVertex3v(oppositePs[0]->m_p3dPos.v);
			SimuVertex3v(oppositePs[1]->m_p3dPos.v);
			SimuVertex3v(oppositePs[2]->m_p3dPos.v);
			glEnd();
		}
	}
}

void CTetrahedron::AssertGeometricConnectivity()
{
	char location[] = "CTetrahedron::AssertGeometricConnectivity()";

	if (m_geDeleted)
		CSimuMsg::ExitWithMessage(location, "This tetrahedron has been labeled deleted.");
	for (int k=0; k < 4; k++)
	{
		if (m_ttrPs[k]->m_p3dotTetras->SearchForOnePointer(this) == -1)
			CSimuMsg::ExitWithMessage(location, "Point does not have this tetrahedron.");
		if (m_ttrNgbrTetras[k] != NULL
		 && !m_ttrNgbrTetras[k]->HasNgbrTetra(this))
			CSimuMsg::ExitWithMessage(location, "Ngbr tetra does not have this tetrahedron.");
	}
}

void CTetrahedron::SetTetrahedronPoints(CPoint3DOnTetra* p0, CPoint3DOnTetra* p1,
										CPoint3DOnTetra* p2, CPoint3DOnTetra* p3)
{
	m_ttrPs[0] = p0;
	m_ttrPs[1] = p1;
	m_ttrPs[2] = p2;
	m_ttrPs[3] = p3;

	p0->m_p3dotTetras->AppendOnePointer(this);
	p1->m_p3dotTetras->AppendOnePointer(this);
	p2->m_p3dotTetras->AppendOnePointer(this);
	p3->m_p3dotTetras->AppendOnePointer(this);
}

void CTetrahedron::SetTetraPointColor(CVector3D* color)
{
	m_ttrPs[0]->SetPoint3DColor(color);
	m_ttrPs[1]->SetPoint3DColor(color);
	m_ttrPs[2]->SetPoint3DColor(color);
	m_ttrPs[3]->SetPoint3DColor(color);
}

bool CTetrahedron::HasTetraPoint(CPoint3DOnTetra* pPoint)
{
	if (m_ttrPs[0] == pPoint) return true;
	if (m_ttrPs[1] == pPoint) return true;
	if (m_ttrPs[2] == pPoint) return true;
	if (m_ttrPs[3] == pPoint) return true;
	return false;
}

void CTetrahedron::GetOppositePoints(int id, CPoint3DOnTetra* oppositePs[3])
{
	for (int i=1; i <= 3 ; i++)
		oppositePs[i-1] = m_ttrPs[(id + i)%4];
}

bool CTetrahedron::SetupNgbrTetra()
{
	// setup neighboring tetrahedrons
	for(int i=0; i < 4; i++)
	{
		if (m_ttrNgbrTetras[i] != NULL)
			continue;
		CPoint3DOnTetra* oppositePs[3];
		GetOppositePoints(i, oppositePs);
		for (int j=0; j < oppositePs[0]->m_p3dotTetras->m_paNum; j++)
		{
			CTetrahedron* pTet = oppositePs[0]->m_p3dotTetras->m_paPtrs[j];
			if (pTet != this
			 && pTet->HasTetraPoint(oppositePs[1])
			 && pTet->HasTetraPoint(oppositePs[2]))
			{
				m_ttrNgbrTetras[i] = pTet;
				if (pTet->SetNgbrTetra(oppositePs, this) == false)
					return false;
				break;
			}
		}
	}
	return true;
}

bool CTetrahedron::SetNgbrTetra(CPoint3DOnTetra* sharedPs[3], CTetrahedron* ngbrTetra)
{
	char location[] = "CTetrahedron::SetNgbrTetra(...)";

	for(int i=0; i < 4; i++)
	{
		CPoint3DOnTetra* pPoint = m_ttrPs[i];
		int j;
		for(j=0; j < 3; j++)
			if (pPoint == sharedPs[j])
				break;
		if (j >= 3)
		{
			if (m_ttrNgbrTetras[i] != NULL)
			{
//				CSimuMsg::ExitWithMessage(location, "Too many neighboring tetra.");
				return false;
			}
			m_ttrNgbrTetras[i] = ngbrTetra;
			return true;
		}
	}
	CSimuMsg::ExitWithMessage(location, "Opposite point is not found for ngbrTetra.");
	return false;
}

CTetrahedron* CTetrahedron::GetNgbrTetra(CPoint3DOnTetra* sharedPs[3])
{
	char location[] = "CTetrahedron::GetNgbrTetra(sharedPs)";

	for(int i=0; i < 4; i++)
	{
		CPoint3DOnTetra* pPoint = m_ttrPs[i];
		int j;
		for(j=0; j < 3; j++)
			if (pPoint == sharedPs[j])
				break;
		if (j >= 3)
		{
			return m_ttrNgbrTetras[i];
		}
	}
	CSimuMsg::ExitWithMessage(location, "Opposite point is not found for ngbrTetra.");
}

bool CTetrahedron::HasNgbrTetra(CTetrahedron* ngbrTetra)
{
	for (int k=0; k < 4; k++)
	{
		if (m_ttrNgbrTetras[k] == ngbrTetra)
			return true;
	}
	return false;
}

void CTetrahedron::ReplaceNgbrTetra(CTetrahedron* oldTetra, CTetrahedron* newTetra)
{
	for(int i=0; i < 4; i++)
	{
		if (m_ttrNgbrTetras[i] == oldTetra)
		{
			m_ttrNgbrTetras[i] = newTetra;
			return;
		}
	}
	CSimuMsg::ExitWithMessage("CTetrahedron::ReplaceNgbrTetra(oldTetra, newTetra)",
							"Old tetra is not found.");
}

void CTetrahedron::DisconnectFromNgbrObjects()
{
	for(int i=0; i < 4; i++)
	{
		m_ttrPs[i]->m_p3dotTetras->RemoveOnePointer(this);
		if (m_ttrNgbrTetras[i] != NULL)
		{
			m_ttrNgbrTetras[i]->ReplaceNgbrTetra(this, NULL);
			m_ttrNgbrTetras[i] = NULL;
		}
	}
}

void CTetrahedron::GetBoundingBox(CVector3D &minPos, CVector3D &maxPos)
{
	minPos[X] = m_ttrPs[0]->m_p3dPos[X]; maxPos[X] = m_ttrPs[0]->m_p3dPos[X];
	minPos[Y] = m_ttrPs[0]->m_p3dPos[Y]; maxPos[Y] = m_ttrPs[0]->m_p3dPos[Y];
	minPos[Z] = m_ttrPs[0]->m_p3dPos[Z]; maxPos[Z] = m_ttrPs[0]->m_p3dPos[Z];
	for (int i=1; i < 4; i++)
	{
		CPoint3DOnTetra* pPoint = m_ttrPs[i];
		for (int d=X; d <= Z; d++)
		{
			if (minPos[d] > pPoint->m_p3dPos[d])
				minPos[d] = pPoint->m_p3dPos[d];
			if (maxPos[d] < pPoint->m_p3dPos[d])
				maxPos[d] = pPoint->m_p3dPos[d];
		}
	}
}
