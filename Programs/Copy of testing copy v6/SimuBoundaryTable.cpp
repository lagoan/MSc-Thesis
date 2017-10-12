// SimuBoundaryTable.cpp: implementation of the CSimuBoundaryTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuBoundaryTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuBoundaryTable::CSimuBoundaryTable()
{
	m_bdryType = TABLE;
}

CSimuBoundaryTable::CSimuBoundaryTable(CVector3D* tblCenter, SimuValue tblSideLen)
{
	m_bdryType = TABLE;

	if (tblCenter != NULL)
		m_tblCenter.SetValue(tblCenter);
	else
		m_tblCenter.SetValue((SimuValue)0);
	m_tblSideLen = tblSideLen;

	ComputeTableVariables();
}

CSimuBoundaryTable::~CSimuBoundaryTable()
{
}

void CSimuBoundaryTable::DrawBoundary()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	SimuColor3v(m_bdryColor->v);
	glBegin(GL_QUADS);
	SimuNormal3d(0, 1, 0);
	SimuVertex3d(m_tblMinX, m_tblCenter[Y], m_tblMinZ);
	SimuVertex3d(m_tblMinX, m_tblCenter[Y], m_tblMaxZ);
	SimuVertex3d(m_tblMaxX, m_tblCenter[Y], m_tblMaxZ);
	SimuVertex3d(m_tblMaxX, m_tblCenter[Y], m_tblMinZ);
	glEnd();
	glPopAttrib();
}

void CSimuBoundaryTable::ExportBoundaryData(FILE* fp)
{
	fprintf(fp, "#declare m_tblSideLen = %f;\n", m_tblSideLen);
	fprintf(fp, "#declare m_tblCenter_Level = %f;\n", m_tblCenter[Y]);
}

void CSimuBoundaryTable::ImportBoundaryData(FILE* fp)
{
	float tmpFloat;
	fscanf(fp, "#declare m_tblSideLen = %f;\n", &tmpFloat); m_tblSideLen = tmpFloat;
	fscanf(fp, "#declare m_tblCenter_Level = %f;\n", &tmpFloat); m_tblCenter[Y] = tmpFloat;
	ComputeTableVariables();
}

bool CSimuBoundaryTable::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	CVector3D* pos = pPrtl->m_vpPos;
	// prevent from penetrating table
	SimuValue tmpDist = pos->v[Y] - m_tblCenter[Y];
	if (tmpDist < 0)
	{
		SimuValue tmpX = pos->v[X] - m_tblCenter[X];
		if (tmpX < m_tblMinX) return false;
		if (tmpX > m_tblMaxX) return false;
		SimuValue tmpZ = pos->v[Z] - m_tblCenter[Z];
		if (tmpZ < m_tblMinZ) return false;
		if (tmpZ > m_tblMaxZ) return false;
		if (fabs(tmpDist) > m_tblThickness) return false;

		pos->v[Y] += -tmpDist*(1+m_bdryDistDumpingRate);
		if (pPrtl->m_vpVel->v[Y] < 0)
			pPrtl->m_vpVel->v[Y] *= -m_bdryEnergyDumpingRate;
		pPrtl->m_vpVel->v[X] *= m_bdryFrictionRate;
		pPrtl->m_vpVel->v[Z] *= m_bdryFrictionRate;
		return true;
	}
	return false;
}

bool CSimuBoundaryTable::PosBreakBoundary(CVector3D* pPos)
{
	// check if particle is under the table
	SimuValue tmpDist = pPos->v[Y] - m_tblCenter[Y];
	if (tmpDist < 0)
	{
		if (pPos->v[X] < m_tblMinX) return true;
		if (pPos->v[X] > m_tblMaxX) return true;
		if (pPos->v[Z] < m_tblMinZ) return true;
		if (pPos->v[Z] > m_tblMaxZ) return true;
		return false;
	}
	return true;
}

void CSimuBoundaryTable::ComputeTableVariables()
{
	m_tblHalfSideLen = m_tblSideLen/2;
	m_tblThickness = CSimuManager::m_prtlDistance*2;
	m_tblMinX = m_tblCenter[X] - m_tblHalfSideLen;
	m_tblMaxX = m_tblCenter[X] + m_tblHalfSideLen;
	m_tblMinZ = m_tblCenter[Z] - m_tblHalfSideLen;
	m_tblMaxZ = m_tblCenter[Z] + m_tblHalfSideLen;
}
