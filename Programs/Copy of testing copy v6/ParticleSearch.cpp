// ParticleSearch.cpp: implementation of the CParticleSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "ParticleSearch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParticleSearch::CParticleSearch()
{
	m_psGrid.m_gCellSize = CSimuManager::m_prtlDistance;
	m_psNumArysOfPrtlsInCell = 0;
	m_psArysOfPrtlsInCell = NULL;
}

CParticleSearch::~CParticleSearch()
{
	if (m_psArysOfPrtlsInCell != NULL)
		DeleteArraysOfPrtlsInCell();
}

void CParticleSearch::RegisterPrtls(int numPrtls, CFluidPrtl** aryPrtls)
{
	T_GetBoundingBoxForParticles<CFluidPrtl>(m_psMinPrtlPos, m_psMaxPrtlPos,
												numPrtls, aryPrtls);
	m_psGrid.UpdateGridBoundaryAndDimension(m_psMinPrtlPos.v, m_psMaxPrtlPos.v);
	m_psGrid.UpdateGridNumbers();
	CreateArraysOfPrtlsInCell();

	for (int i=0; i < numPrtls; i++)
	{
		RegisterOnePrtl(aryPrtls[i]);
	}
}

bool CParticleSearch::RegisterOnePrtl(CFluidPrtl* pPrtl)
{
	int tmpId, cellId[Z+1];
	bool bOutOfBoundary = m_psGrid.GetGridCellIdForInsidePosition(*(pPrtl->m_vpPos), cellId);
	tmpId = m_psGrid.GetCellArrayIdFrom3DGridCellId(cellId[X], cellId[Y], cellId[Z]);
	m_psArysOfPrtlsInCell[tmpId]->AppendOnePointer(pPrtl);
	return bOutOfBoundary;
}

bool CParticleSearch::DeRegisterOnePrtl(CFluidPrtl* pPrtl)
{
	int tmpId, cellId[Z+1];
	m_psGrid.GetGridCellIdForInsidePosition(*(pPrtl->m_vpPos), cellId);
	tmpId = m_psGrid.GetCellArrayIdFrom3DGridCellId(cellId[X], cellId[Y], cellId[Z]);
	return m_psArysOfPrtlsInCell[tmpId]->RemoveOnePointer(pPrtl);
}

void CParticleSearch::GetPrtlsInGridCellAt(int idx, int idy, int idz,
										   TPointerArray<CFluidPrtl>* &aryPrtls)
{
	int aryId = m_psGrid.GetCellArrayIdFrom3DGridCellId(idx, idy, idz);
	aryPrtls = m_psArysOfPrtlsInCell[aryId];
}

void CParticleSearch::GetNgbrPrtls(CVector3D* center, SimuValue radius,
								   TPointerArray<CFluidPrtl> &aryNgbrs)
{
	char location[] = "CParticleSearch::GetNgbrPrtls(...)";

	TPointerArray<CFluidPrtl> aryPossibleNgbrs;
	GetPossibleNgbrPrtls(center, radius, aryPossibleNgbrs);
	SimuValue radiusSqr = radius*radius;
	for (int i=0; i < aryPossibleNgbrs.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = aryPossibleNgbrs[i];
		if (center->GetDistanceSquareToVector(pPrtl->m_vpPos) <= radiusSqr)
			aryNgbrs.AppendOnePointer(pPrtl);
	}
}

// original data pointed by ngbrPrtls is lost, so ngbrPrtls must be NULL
void CParticleSearch::GetPossibleNgbrPrtls(CVector3D* center, SimuValue radius,
										   TPointerArray<CFluidPrtl> &aryNgbrs)
{
	char location[] = "CParticleSearch::GetPossibleNgbrPrtls(...)";

	int tmpId, minId[Z+1], maxId[Z+1];
	m_psGrid.GetOverlapGridCellIdRangeForCube(*center, radius, minId, maxId);
	for (int x=minId[X]; x <= maxId[X]; x++)
	for (int y=minId[Y]; y <= maxId[Y]; y++)
	for (int z=minId[Z]; z <= maxId[Z]; z++)
	{
		tmpId = m_psGrid.GetCellArrayIdFrom3DGridCellId(x, y, z);
		aryNgbrs.AppendPointersFromArray(*m_psArysOfPrtlsInCell[tmpId]);
	}
}

void CParticleSearch::CreateArraysOfPrtlsInCell()
{
	if (m_psNumArysOfPrtlsInCell == m_psGrid.m_gNumCells)
	{
	}
	else if (m_psNumArysOfPrtlsInCell < m_psGrid.m_gNumCells)
	{
		UpdateBufferForArraysOfPrtlsInCell();

		// create more arrays
		while (m_psNumArysOfPrtlsInCell < m_psGrid.m_gNumCells)
			m_psArysOfPrtlsInCell[m_psNumArysOfPrtlsInCell++]
				= new TPointerArray<CFluidPrtl>(SIMU_PARA_PRTL_IN_CELL_ALLOC_SIZE);
	}
	else // (m_psNumArysOfPrtlsInCell > m_psGrid.m_gNumCells)
	{
		// delete extra arrays
		while (m_psNumArysOfPrtlsInCell > m_psGrid.m_gNumCells)
			delete m_psArysOfPrtlsInCell[--m_psNumArysOfPrtlsInCell];

		UpdateBufferForArraysOfPrtlsInCell();
	}
	ResetArraysOfPrtlsInCell();
}

void CParticleSearch::DeleteArraysOfPrtlsInCell()
{
	for (int i=0; i < m_psNumArysOfPrtlsInCell; i++)
		delete m_psArysOfPrtlsInCell[i];
	delete m_psArysOfPrtlsInCell;
}

void CParticleSearch::UpdateBufferForArraysOfPrtlsInCell()
{
	TPointerArray<CFluidPrtl>** temp = m_psArysOfPrtlsInCell;
	m_psArysOfPrtlsInCell = new TPointerArray<CFluidPrtl>*[m_psGrid.m_gNumCells];
	memcpy(m_psArysOfPrtlsInCell, temp,
			m_psNumArysOfPrtlsInCell*sizeof(TPointerArray<CFluidPrtl>*)) ;
	delete temp;
}

void CParticleSearch::ResetArraysOfPrtlsInCell()
{
	for (int i=0; i < m_psNumArysOfPrtlsInCell; i++)
		m_psArysOfPrtlsInCell[i]->ResetArraySize(0);
}
