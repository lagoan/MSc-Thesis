// SimuBoundaryBox.cpp: implementation of the CSimuBoundaryBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuBoundaryBox.h"
#include "PrtlFluid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuBoundaryBox::CSimuBoundaryBox()
{
	m_bdryType = BOX;
}

CSimuBoundaryBox::CSimuBoundaryBox(CVector3D* boxBottomCenter, SimuValue boxSideWidth)
{
	m_bdryType = BOX;

	m_boxLBCCornor.SetValue(boxBottomCenter);
	m_boxRUFCornor.SetValue(boxBottomCenter);
	SimuValue boxHalfSideWidth = boxSideWidth/2;
	m_boxLBCCornor[X] -= boxHalfSideWidth;
	m_boxRUFCornor[X] += boxHalfSideWidth;
	m_boxLBCCornor[Z] += boxHalfSideWidth;
	m_boxRUFCornor[Z] -= boxHalfSideWidth;
	m_boxRUFCornor[Y] += boxSideWidth; // this makes it a cubic box
}

CSimuBoundaryBox::CSimuBoundaryBox(CVector3D* boxLBCCornor, CVector3D* boxRUFCornor)
{
	m_bdryType = BOX;

	m_boxLBCCornor.SetValue(boxLBCCornor);
	m_boxRUFCornor.SetValue(boxRUFCornor);
}

CSimuBoundaryBox::~CSimuBoundaryBox()
{
}

void CSimuBoundaryBox::DrawBoundary()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	SimuColor3v(m_bdryColor->v);

	// draw bottom edges
	glBegin(GL_LINE_LOOP);
	SimuVertex3d(m_boxLBCCornor[X], m_boxLBCCornor[Y], m_boxLBCCornor[Z]);
	SimuVertex3d(m_boxRUFCornor[X], m_boxLBCCornor[Y], m_boxLBCCornor[Z]);
	SimuVertex3d(m_boxRUFCornor[X], m_boxLBCCornor[Y], m_boxRUFCornor[Z]);
	SimuVertex3d(m_boxLBCCornor[X], m_boxLBCCornor[Y], m_boxRUFCornor[Z]);
	glEnd();
	// draw upper edges
	glBegin(GL_LINE_LOOP);
	SimuVertex3d(m_boxLBCCornor[X], m_boxRUFCornor[Y], m_boxLBCCornor[Z]);
	SimuVertex3d(m_boxRUFCornor[X], m_boxRUFCornor[Y], m_boxLBCCornor[Z]);
	SimuVertex3d(m_boxRUFCornor[X], m_boxRUFCornor[Y], m_boxRUFCornor[Z]);
	SimuVertex3d(m_boxLBCCornor[X], m_boxRUFCornor[Y], m_boxRUFCornor[Z]);
	glEnd();
	// draw side edges
	glBegin(GL_LINES);
	SimuVertex3d(m_boxLBCCornor[X], m_boxLBCCornor[Y], m_boxLBCCornor[Z]);
	SimuVertex3d(m_boxLBCCornor[X], m_boxRUFCornor[Y], m_boxLBCCornor[Z]);
	SimuVertex3d(m_boxRUFCornor[X], m_boxLBCCornor[Y], m_boxLBCCornor[Z]);
	SimuVertex3d(m_boxRUFCornor[X], m_boxRUFCornor[Y], m_boxLBCCornor[Z]);
	SimuVertex3d(m_boxRUFCornor[X], m_boxLBCCornor[Y], m_boxRUFCornor[Z]);
	SimuVertex3d(m_boxRUFCornor[X], m_boxRUFCornor[Y], m_boxRUFCornor[Z]);
	SimuVertex3d(m_boxLBCCornor[X], m_boxLBCCornor[Y], m_boxRUFCornor[Z]);
	SimuVertex3d(m_boxLBCCornor[X], m_boxRUFCornor[Y], m_boxRUFCornor[Z]);
	glEnd();

	glPopAttrib();
}

void CSimuBoundaryBox::ExportBoundaryData(FILE* fp)
{
	fprintf(fp, "#declare %s__m_boxLBCCornor = <%f, %f, %f>;\n", m_deiName,
							  m_boxLBCCornor[X],
							  m_boxLBCCornor[Y],
							  m_boxLBCCornor[Z]);
	fprintf(fp, "#declare %s__m_boxRUFCornor = <%f, %f, %f>;\n", m_deiName,
							  m_boxRUFCornor[X],
							  m_boxRUFCornor[Y],
							  m_boxRUFCornor[Z]);
}

void CSimuBoundaryBox::ExportBoundaryParameters(FILE* fp)
{
	fprintf(fp, "roof open=%d\n", m_boxOpenRoof);
	ExportBoundaryData(fp);
}

void CSimuBoundaryBox::ImportBoundaryData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float x, y, z;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_boxLBCCornor[X] = x;
							m_boxLBCCornor[Y] = y;
							m_boxLBCCornor[Z] = z;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_boxRUFCornor[X] = x;
							m_boxRUFCornor[Y] = y;
							m_boxRUFCornor[Z] = z;
	m_deiName = tmpName;
}

void CSimuBoundaryBox::ImportBoundaryParameters(FILE* fp)
{
	int tmpInt;
	fscanf(fp, "roof open=%d\n", &tmpInt); m_boxOpenRoof = tmpInt;
	ImportBoundaryData(fp);
}

bool CSimuBoundaryBox::EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep)
{
	bool bCollisionOccurs = false;
	SimuValue tmpDist;
	tmpDist = pPrtl->m_vpPos->v[Y] - m_boxRUFCornor[Y];
	if (tmpDist > 0)
	{
		// prevent from penetrating roof
		if (!m_boxOpenRoof && tmpDist < CSimuManager::m_prtlDistance)
		{
			pPrtl->m_vpPos->v[Y] += -tmpDist*(1+m_bdryDistDumpingRate);
			if (pPrtl->m_vpVel->v[Y] > 0)
				pPrtl->m_vpVel->v[Y] *= -m_bdryEnergyDumpingRate;
			bCollisionOccurs = true;
		}
		else
			return false;
	}

	// prevent from penetrating bottom
	tmpDist = pPrtl->m_vpPos->v[Y] - m_boxLBCCornor[Y];
	if (tmpDist < 0 && fabs(tmpDist) < CSimuManager::m_prtlDistance)
	{
		pPrtl->m_vpPos->v[Y] += -tmpDist*(1+m_bdryDistDumpingRate);
		if (pPrtl->m_vpVel->v[Y] < 0)
			pPrtl->m_vpVel->v[Y] *= -m_bdryEnergyDumpingRate;
		bCollisionOccurs = true;
	}
	// prevent from penetrating right side
	tmpDist = pPrtl->m_vpPos->v[X] - m_boxRUFCornor[X];
	if (tmpDist > 0 && fabs(tmpDist) < CSimuManager::m_prtlDistance)
	{
		pPrtl->m_vpPos->v[X] += -tmpDist*(1+m_bdryDistDumpingRate);
		if (pPrtl->m_vpVel->v[X] > 0)
			pPrtl->m_vpVel->v[X] *= -m_bdryEnergyDumpingRate;
		bCollisionOccurs = true;
	}
	// prevent from penetrating left side
	tmpDist = pPrtl->m_vpPos->v[X] - m_boxLBCCornor[X];
	if (tmpDist < 0 && fabs(tmpDist) < CSimuManager::m_prtlDistance)
	{
		pPrtl->m_vpPos->v[X] += -tmpDist*(1+m_bdryDistDumpingRate);
		if (pPrtl->m_vpVel->v[X] < 0)
			pPrtl->m_vpVel->v[X] *= -m_bdryEnergyDumpingRate;
		bCollisionOccurs = true;
	}
	// prevent from penetrating close side
	tmpDist = pPrtl->m_vpPos->v[Z] - m_boxLBCCornor[Z];
	if (tmpDist < 0 && fabs(tmpDist) < CSimuManager::m_prtlDistance)
	{
		pPrtl->m_vpPos->v[Z] += -tmpDist*(1+m_bdryDistDumpingRate);
		if (pPrtl->m_vpVel->v[Z] > 0)
			pPrtl->m_vpVel->v[Z] *= -m_bdryEnergyDumpingRate;
		bCollisionOccurs = true;
	}
	// prevent from penetrating far side
	tmpDist = pPrtl->m_vpPos->v[Z] - m_boxRUFCornor[Z];
	if (tmpDist > 0 && fabs(tmpDist) < CSimuManager::m_prtlDistance)
	{
		pPrtl->m_vpPos->v[Z] += -tmpDist*(1+m_bdryDistDumpingRate);
		if (pPrtl->m_vpVel->v[Z] < 0)
			pPrtl->m_vpVel->v[Z] *= -m_bdryEnergyDumpingRate;
		bCollisionOccurs = true;
	}
	return bCollisionOccurs;
}

bool CSimuBoundaryBox::PosBreakBoundary(CVector3D* pPos)
{
	SimuValue tmpDist;
	tmpDist = pPos->v[Y] - m_boxRUFCornor[Y];
	if (tmpDist > 0)
	{
		// prevent from penetrating roof
		if (!m_boxOpenRoof && tmpDist < CSimuManager::m_prtlDistance)
			return true;
		else
			return false;
	}

	// prevent from penetrating bottom
	tmpDist = pPos->v[Y] - m_boxLBCCornor[Y];
	if (tmpDist < 0 && fabs(tmpDist) < CSimuManager::m_prtlDistance)
		return true;

	// prevent from penetrating right side
	tmpDist = pPos->v[X] - m_boxRUFCornor[X];
	if (tmpDist > 0 && fabs(tmpDist) < CSimuManager::m_prtlDistance)
		return true;

	// prevent from penetrating left side
	tmpDist = pPos->v[X] - m_boxLBCCornor[X];
		return true;

	// prevent from penetrating close side
	tmpDist = pPos->v[Z] - m_boxLBCCornor[Z];
	if (tmpDist < 0 && fabs(tmpDist) < CSimuManager::m_prtlDistance)
		return true;

	// prevent from penetrating far side
	tmpDist = pPos->v[Z] - m_boxRUFCornor[Z];
	if (tmpDist > 0 && fabs(tmpDist) < CSimuManager::m_prtlDistance)
		return true;

	return false;
}
