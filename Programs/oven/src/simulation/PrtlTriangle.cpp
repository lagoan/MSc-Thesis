// PrtlTriangle.cpp: implementation of the CPrtlTriangle class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "PrtlTriangle.h"
#include "MeshPrtl.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtlTriangle::CPrtlTriangle()
{
	m_ptrPs[0] = NULL;
	m_ptrPs[1] = NULL;
	m_ptrPs[2] = NULL;
}

CPrtlTriangle::~CPrtlTriangle()
{

}

void CPrtlTriangle::RenderGeometry()
{
	if (CSimuManager::SHOW_SELECTED_ELMNT_ONLY 
	 && !m_geSelected)
		return;

	if (CSimuManager::DRAW_EDGE_ONLY)
	{
		glBegin(GL_LINES);
		SimuVertex3v(m_ptrPs[0]->m_vpPos->v);
		SimuVertex3v(m_ptrPs[1]->m_vpPos->v);
		SimuVertex3v(m_ptrPs[1]->m_vpPos->v);
		SimuVertex3v(m_ptrPs[2]->m_vpPos->v);
		SimuVertex3v(m_ptrPs[2]->m_vpPos->v);
		SimuVertex3v(m_ptrPs[0]->m_vpPos->v);
		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLES);
		SimuNormal3v(m_ptrPs[0]->m_mpNormal.v);
		SimuVertex3v(m_ptrPs[0]->m_vpPos->v);
		SimuNormal3v(m_ptrPs[1]->m_mpNormal.v);
		SimuVertex3v(m_ptrPs[1]->m_vpPos->v);
		SimuNormal3v(m_ptrPs[2]->m_mpNormal.v);
		SimuVertex3v(m_ptrPs[2]->m_vpPos->v);
		glEnd();
	}
	if (CSimuManager::DRAW_TRIANGLE_NORMALS)
	{
		glPushAttrib(GL_CURRENT_BIT);
		SimuColor3v(CSimuManager::COLOR_RED->v);
		CVector3D triCenter;
		triCenter.SetValue(m_ptrPs[0]->m_vpPos);
		triCenter.AddedBy(m_ptrPs[1]->m_vpPos);
		triCenter.AddedBy(m_ptrPs[2]->m_vpPos);
		triCenter.ScaledBy(1/3.0);
		CVector3D normalTipPos;
		normalTipPos.SetValue(&m_ptrAreaNormal);
		normalTipPos.Normalize(CSimuManager::m_displayTuner);
		normalTipPos.AddedBy(&triCenter);
		glBegin(GL_LINES);
		SimuVertex3v(triCenter.v);
		SimuVertex3v(normalTipPos.v);
		glEnd();
		SimuValue radius = 0.5*CSimuManager::m_sphereRadius;
		GLUquadricObj* qObj = gluNewQuadric();
		glPushMatrix();
		glTranslated(triCenter[X], triCenter[Y], triCenter[Z]);
		gluSphere(qObj, radius, CSimuManager::m_sphereSlices, CSimuManager::m_sphereStacks);
		glPopMatrix();
		gluDeleteQuadric(qObj);
		glPopAttrib();
		CSimuUtility::RestoreOpenGLPreviousColor();
	}
}

void CPrtlTriangle::AssertGeometricConnectivity()
{
	AssertPrtlTriConnectivity();
}

void CPrtlTriangle::AssertPrtlTriConnectivity(char location[])
{
	if (m_geDeleted)
		CSimuMsg::ExitWithMessage(location, "<this> triangle has been labeled deleted.");

	for (int i=0; i < 3; i++)
	{
		if (m_ptrPs[i]->m_geDeleted)
		{
			CSimuMsg::ExitWithMessage(location, "TetraPrtl has been deleted.");
			return;
		}
		if (!m_ptrPs[i]->HasNeighborTri(this))
		{
			CSimuMsg::ExitWithMessage(location, "TetraPrtl doesn't have <this> triangle.");
			return;
		}
	}
}

void CPrtlTriangle::GetElementPosition(CVector3D* pPos)
{
	pPos->SetValue(m_ptrPs[0]->m_vpPos);
	pPos->AddedBy(m_ptrPs[1]->m_vpPos);
	pPos->AddedBy(m_ptrPs[2]->m_vpPos);
	pPos->ScaledBy(1/3.0);
}

void CPrtlTriangle::SetTriPrtls(CMeshPrtl* triP0, CMeshPrtl* triP1, CMeshPrtl* triP2)
{
	m_ptrPs[0] = triP0;
	m_ptrPs[1] = triP1;
	m_ptrPs[2] = triP2;
	for (int i=0; i < 3; i++)
	{
		m_ptrPs[i]->m_mpTris.AppendOnePointer(this);
	}
}

bool CPrtlTriangle::HasTriPrtl(CMeshPrtl* pPrtl)
{
	for (int i=0; i < 3; i++)
	{
		if (m_ptrPs[i] == pPrtl)
			return true;
	}
	return false;
}

bool CPrtlTriangle::HasSameTriPrtlsAs(CPrtlTriangle* pTri)
{
	char location[] = "CPrtlTriangle::HasSameTriPrtlsAs(pTri)";

	for (int i=0; i < 3; i++)
	{	
		int j;
		for (j=0; j < 3; j++)
		{
			if (m_ptrPs[i] == pTri->m_ptrPs[j])
				break;
		}
		if (j >= 3)
			return false; // m_ptrPs[i] is not equal to any prtl on <pTri>
	}
	return true;
}

void CPrtlTriangle::ReplaceTriPrtl(CMeshPrtl* pOld, CMeshPrtl* pNew)
{
	for (int i=0; i < 3; i++)
	{
		if (m_ptrPs[i] == pOld)
		{
			m_ptrPs[i] = pNew;
			return;
		}
	}

	CSimuMsg::ExitWithMessage("CPrtlTriangle::ReplaceTriPrtl(pOld, pNew)",
								"pOld is not found.");
}

// if <this> is not found from any tri prtl, return false.
bool CPrtlTriangle::SelfDeleteFromTriPrtls(char location[])
{
	for (int i=0; i < 3; i++)
	{
		if (!m_ptrPs[i]->m_mpTris.RemoveOnePointer(this))
		{
			CSimuMsg::ExitWithMessage(location, "Prtl doesn't have <this> triangle.");
			return false;
		}
	}
	m_geDeleted = true;
	return true;
}

int CPrtlTriangle::GetThirdTriPrtlId(CMeshPrtl* triP0, CMeshPrtl* triP1)
{
	char location[] = "CPrtlTriangle::GetThirdTriPrtlId(triP0, triP1)";

	for (int i=0; i < 3; i++)
	{
		if (triP0 == m_ptrPs[i])
		{
			if (triP1 == m_ptrPs[(i+1)%3])
				return (i+2)%3;
			if (triP1 == m_ptrPs[(i+2)%3])
				return (i+1)%3;
			if (triP1 == triP0)
				CSimuMsg::ExitWithMessage(location, "<triP1> is same as <triP0>.");
			else
				CSimuMsg::ExitWithMessage(location, "<triP1> is not on <this> triangle.");
		}
	}
	CSimuMsg::ExitWithMessage(location, "<triP0> is not on <this> triangle.");
}

CMeshPrtl* CPrtlTriangle::GetThirdTriPrtl(CMeshPrtl* triP0, CMeshPrtl* triP1)
{
	char location[] = "CPrtlTriangle::GetThirdTriPrtl(triP0, triP1)";

	for (int i=0; i < 3; i++)
	{
		if (triP0 == m_ptrPs[i])
		{
			if (triP1 == m_ptrPs[(i+1)%3])
				return m_ptrPs[(i+2)%3];
			if (triP1 == m_ptrPs[(i+2)%3])
				return m_ptrPs[(i+1)%3];
			CSimuMsg::ExitWithMessage(location,
						"triP1 is not on an edge with triP0 on <this> triangle.");
		}
	}
	CSimuMsg::ExitWithMessage(location, "triP0 is not on <this> triangle.");
}

CPrtlTriangle* CPrtlTriangle::GetNeighborTriangle(CMeshPrtl* triP0, CMeshPrtl* triP1)
{
	for (int i=0; i < triP0->m_mpTris.m_paNum; i++)
	{
		CPrtlTriangle* pTri = triP0->m_mpTris[i];
		if (pTri != this
		 && pTri->HasTriPrtl(triP1))
			return pTri;
	}
	if (!HasTriPrtl(triP0) || !HasTriPrtl(triP1))
		CSimuMsg::ExitWithMessage("CPrtlTriangle::GetNeighborTriangle(triP0, triP1)",
								"An input prtl is not on <this> triangle.");
	return NULL;
}

SimuValue CPrtlTriangle::ComputeAngleOnTriPrtl(CMeshPrtl* pPrtl)
{
	char location[] = "CPrtlTriangle::ComputeAngleOnTriPrtl(pPrtl)";

	SimuValue angle = 0;
	for (int i=0; i < 3; i++)
	{
		if (pPrtl == m_ptrPs[i])
		{
			angle = CSimuUtility::GetAngleOf3Points(m_ptrPs[(i+1)%3]->m_vpPos,
													m_ptrPs[i]->m_vpPos,
													m_ptrPs[(i+2)%3]->m_vpPos);
			return angle;
		}
	}
	CSimuMsg::ExitWithMessage(location, "pPrtl is not on <this> triangle.");
	return 0;
}

void CPrtlTriangle::ComputeTriAreaNormal()
{
	CSimuUtility::CrossProduct(m_ptrPs[0]->m_vpPos, m_ptrPs[1]->m_vpPos,
							   m_ptrPs[2]->m_vpPos, &m_ptrAreaNormal);
	m_ptrAreaNormal.ScaledBy(0.5);
}

void CPrtlTriangle::ComputeTriAreaNormalByRefNormal(CVector3D* refNormal)
{
	ComputeTriAreaNormal();
	if (refNormal->DotProduct(&m_ptrAreaNormal) < 0)
	{
		CMeshPrtl* tmpPrtl = m_ptrPs[0];
		m_ptrPs[0] = m_ptrPs[1];
		m_ptrPs[1] = tmpPrtl;
		ComputeTriAreaNormal();
		if (refNormal->DotProduct(&m_ptrAreaNormal) < 0)
			CSimuMsg::ExitWithMessage("CPrtlTriangle::ComputeTriAreaNormalByRefNormal(refNormal)",
									"Can not get outward triangle area normal.");
	}
}

void CPrtlTriangle::ComputeTriAreaNormalByRefPos(CVector3D* refPos, bool bInside)
{
	CVector3D edge3;
	ComputeTriAreaNormal();
	edge3.SetValueAsSubstraction(refPos, m_ptrPs[0]->m_vpPos);
	SimuValue tmpVolume = edge3.DotProduct(&m_ptrAreaNormal);
	if ((tmpVolume > 0 && bInside)
	 || (tmpVolume < 0 && !bInside))
	{
		CMeshPrtl* tmpPrtl = m_ptrPs[0];
		m_ptrPs[0] = m_ptrPs[1];
		m_ptrPs[1] = tmpPrtl;
		ComputeTriAreaNormal();
		edge3.SetValueAsSubstraction(refPos, m_ptrPs[0]->m_vpPos);
		SimuValue tmpVolume = edge3.DotProduct(&m_ptrAreaNormal);
		if ((tmpVolume > 0 && bInside)
		 || (tmpVolume < 0 && !bInside))
			CSimuMsg::ExitWithMessage("CPrtlTriangle::ComputeTriAreaNormalByRefPos(...)",
									"Can not get outward triangle area normal.");
	}
}

SimuValue CPrtlTriangle::GetTriangleArea()
{
	return CSimuUtility::ComputeTriangleArea(m_ptrPs[0]->m_vpPos,
											 m_ptrPs[1]->m_vpPos,
											 m_ptrPs[2]->m_vpPos);
}

void CPrtlTriangle::ComputePrtlCurvatureNormalContribution(int idPrtl, SimuValue triArea,
													   CVector3D &contributionVector)
{
	CVector3D* focusP = m_ptrPs[idPrtl]->m_vpPos;
	CVector3D* ngbrPs[2] = {m_ptrPs[(idPrtl+1)%3]->m_vpPos, m_ptrPs[(idPrtl+2)%3]->m_vpPos};
	contributionVector.SetValue((SimuValue)0);
	for (int k=0; k < 2; k++)
	{
		CVector3D* thirdP = ngbrPs[(k+1)%2];
		SimuValue cotan = CSimuUtility::DotProduct(focusP, ngbrPs[k], thirdP);
		cotan /= triArea*2;
		contributionVector.AddedBy(ngbrPs[k], cotan);
		contributionVector.AddedBy(focusP, -cotan);
	}
}

// the triangles must have area normals.
SimuValue CPrtlTriangle::ComputeVolumeEnclosedByTriangles(TPointerArray<CPrtlTriangle> &srfcTris)
{
	SimuValue volume = 0;
	for (int i=0; i < srfcTris.m_paNum; i++)
	{
		CPrtlTriangle* pTri = srfcTris[i];
		// The virtual tet is made of this triangle and (0, 0, 0)
		SimuValue virtualVol = pTri->m_ptrPs[0]->m_vpPos->DotProduct(&pTri->m_ptrAreaNormal);
		volume += virtualVol / 3;
	}
	return volume;
}
