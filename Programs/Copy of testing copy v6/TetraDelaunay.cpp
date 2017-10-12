// TetraDelaunay.cpp: implementation of the CTetraDelaunay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "TetraDelaunay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTetraDelaunay::CTetraDelaunay()
{
	m_ttrdNoUpsample = false;
}

CTetraDelaunay::~CTetraDelaunay()
{

}

void CTetraDelaunay::SetTetrahedronPoints(CPoint3DOnTetra* p0, CPoint3DOnTetra* p1,
										  CPoint3DOnTetra* p2, CPoint3DOnTetra* p3)
{
	char location[] = "CTetraDelaunay::SetTetrahedronPoints(...)";

	CTetrahedron::SetTetrahedronPoints(p0, p1, p2, p3);

	if (!CSimuUtility::GetCircumSphereCenter(&p0->m_p3dPos, &p1->m_p3dPos, &p2->m_p3dPos,
											 &p3->m_p3dPos, &m_ttrdCircumCtrPos))
	{
		SetTetraPointColor(CDelaunayTetrahedron::m_dtrTetraPColor);
		CSimuMsg::ExitWithMessage(location, "Circum sphere center is not found.");
		SetTetraPointColor(CDelaunayTetrahedron::m_dtrNgbrPColor);
	}
	m_ttrdCircumRSqr = m_ttrdCircumCtrPos.GetDistanceSquareToVector(&p0->m_p3dPos);
}

bool CTetraDelaunay::IsArtificialTetra()
{
	if (m_ttrPs[0]->m_p3dotArtificial) return true;
	if (m_ttrPs[1]->m_p3dotArtificial) return true;
	if (m_ttrPs[2]->m_p3dotArtificial) return true;
	if (m_ttrPs[3]->m_p3dotArtificial) return true;
	return false;
}
