// MeshPrtl.cpp: implementation of the CMeshPrtl class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "MeshPrtl.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeshPrtl::CMeshPrtl()
{
	m_mpDistanceToSrfc = SIMU_VALUE_MAX;

	m_mpTris.InitializeClassArray(false, SIMU_PARA_NGBR_TRI_ALLOC_SIZE);
}

CMeshPrtl::~CMeshPrtl()
{

}

void CMeshPrtl::RenderGeometry()
{
	RenderPrtlByOpenGL();

	if (CSimuManager::DRAW_SRFC_PRTL_NORMALS
	 && m_mpOnSurface)
	{
		glPushAttrib(GL_CURRENT_BIT);
		SimuColor3v(CSimuManager::COLOR_RED->v);
		CVector3D normalTipPos;
		normalTipPos.SetValue(&m_mpNormal);
		normalTipPos.Normalize(CSimuManager::m_displayTuner);
		normalTipPos.AddedBy(m_vpPos);
		glBegin(GL_LINES);
		SimuVertex3v(m_vpPos->v);
		SimuVertex3v(normalTipPos.v);
		glEnd();
		glPopAttrib();
		CSimuUtility::RestoreOpenGLPreviousColor();
	}
}

void CMeshPrtl::AssertGeometricConnectivity()
{
	AssertPrtlConnectivity();
}

void CMeshPrtl::AssertPrtlConnectivity(char location[])
{
	if (m_geDeleted)
		CSimuMsg::ExitWithMessage(location, "<this> prtl has been labeled deleted.");
	for (int i=0; i < m_mpTris.m_paNum; i++)
	{
		CPrtlTriangle* pTri = m_mpTris[i];
		if (pTri->m_geDeleted)
		{
			CSimuMsg::ExitWithMessage(location, "Triangle has been deleted.");
		}
		if (!pTri->HasTriPrtl(this))
		{
			CSimuMsg::ExitWithMessage(location, "Triangle doesn't have <this> prtl.");
		}
	}
}

void CMeshPrtl::SetClosestSrfcPos(CVector3D *srfcPos)
{
	m_vpVel->SetValue(srfcPos);
}

void CMeshPrtl::GetClosestSrfcPos(CVector3D *srfcPos)
{
	srfcPos->SetValue(m_vpVel);
}

bool CMeshPrtl::HasNeighborTriPrtl(CMeshPrtl* pPrtl)
{
	for (int i=0; i < m_mpTris.m_paNum; i++)
	{
		CPrtlTriangle* pTri = m_mpTris[i];
		if (pTri->HasTriPrtl(pPrtl))
			return true;
	}
	return false;
}

bool CMeshPrtl::HasNeighborTri(CPrtlTriangle* pTri)
{
	if (m_mpTris.SearchForOnePointer(pTri) == -1)
		return false; // not found
	return true;
}

void CMeshPrtl::ComputeNormalFromNgbrTris()
{
	m_mpNormal.SetValue((SimuValue)0);
	for (int j=0; j < m_mpTris.m_paNum; j++)
	{
		m_mpNormal.AddedBy(&(m_mpTris[j]->m_ptrAreaNormal));
	}
	m_mpNormal.Normalize();
}
