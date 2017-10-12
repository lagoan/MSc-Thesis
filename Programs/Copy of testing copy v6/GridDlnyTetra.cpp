// GridDlnyTetra.cpp: implementation of the CGridDlnyTetra class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "GridDlnyTetra.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridDlnyTetra::CGridDlnyTetra()
{
}

CGridDlnyTetra::~CGridDlnyTetra()
{

}

void CGridDlnyTetra::CreateTetraArraysInCells()
{
	m_gdtCellFlags = new bool[m_gNumCells];
	m_gdtTetraArysInCells = new TGeomElemArray<CTetraDelaunay>*[m_gNumCells];
	for (int i=0; i < m_gNumCells; i++)
	{
		m_gdtTetraArysInCells[i] = new TGeomElemArray<CTetraDelaunay>();
		m_gdtTetraArysInCells[i]->InitializeClassArray(false, SIMU_PARA_TETRA_IN_CELL_ALLOC_SIZE);
	}
}

void CGridDlnyTetra::DeleteTetraArraysInCells()
{
	delete m_gdtCellFlags;
	for (int i=0; i < m_gNumCells; i++)
		delete m_gdtTetraArysInCells[i];
	delete m_gdtTetraArysInCells;
	m_gdtTetraArysInCells = NULL;
}

void CGridDlnyTetra::RegisterTetraInCells(CTetraDelaunay* pTetra)
{
	int cellId[3];
	GetGridCellIdForInsidePosition(pTetra->m_ttrdCircumCtrPos, cellId);
	int aryId = GetCellArrayIdFrom3DGridCellId(cellId[X], cellId[Y], cellId[Z]);
	m_gdtTetraArysInCells[aryId]->AppendOnePointer(pTetra);
}

void CGridDlnyTetra::LabelCellsHoldingTetraCtr(CTetraDelaunay* pTetra)
{
	int cellId[3];
	GetGridCellIdForInsidePosition(pTetra->m_ttrdCircumCtrPos, cellId);
	int aryId = GetCellArrayIdFrom3DGridCellId(cellId[X], cellId[Y], cellId[Z]);
	m_gdtCellFlags[aryId] = true;
}

void CGridDlnyTetra::RegisterTetraInCellsByBoundingBox(CTetraDelaunay* pTetra)
{
	CVector3D minPos, maxPos;
	int minCellId[3], maxCellId[3];
	pTetra->GetBoundingBox(minPos, maxPos);
	GetOverlapGridCellIdRangeForBox(minPos, maxPos, minCellId, maxCellId);
	for (int x=minCellId[X]; x <= maxCellId[X]; x++)
	for (int y=minCellId[Y]; y <= maxCellId[Y]; y++)
	for (int z=minCellId[Z]; z <= maxCellId[Z]; z++)
	{
		int aryId = GetCellArrayIdFrom3DGridCellId(x, y, z);
		m_gdtTetraArysInCells[aryId]->AppendOnePointer(pTetra);
	}
}

bool CGridDlnyTetra::DeRegisterTetraInCellsByBoundingBox(CTetraDelaunay* pTetra)
{
	bool bTetraFound = true;
	CVector3D minPos, maxPos;
	int minCellId[3], maxCellId[3];
	pTetra->GetBoundingBox(minPos, maxPos);
	GetOverlapGridCellIdRangeForBox(minPos, maxPos, minCellId, maxCellId);
	for (int x=minCellId[X]; x <= maxCellId[X]; x++)
	for (int y=minCellId[Y]; y <= maxCellId[Y]; y++)
	for (int z=minCellId[Z]; z <= maxCellId[Z]; z++)
	{
		int aryId = GetCellArrayIdFrom3DGridCellId(x, y, z);
		if (!m_gdtTetraArysInCells[aryId]->RemoveOnePointer(pTetra))
			bTetraFound = false;
	}
	return bTetraFound;
}

void CGridDlnyTetra::DeRegisterDeletedTetraInLabeledCells()
{
	for (int i=0; i < m_gNumCells; i++)
	{
		if (m_gdtCellFlags[i])
			m_gdtTetraArysInCells[i]->CleanDeletedElements();
	}
}

void CGridDlnyTetra::ClearLabelsInCells()
{
	for (int i=0; i < m_gNumCells; i++)
		m_gdtCellFlags[i] = false;
}

void CGridDlnyTetra::LabelCellsOverlappingTetraBoundingBox(CTetraDelaunay* pTetra)
{
	CVector3D minPos, maxPos;
	int minCellId[3], maxCellId[3];
	pTetra->GetBoundingBox(minPos, maxPos);
	GetOverlapGridCellIdRangeForBox(minPos, maxPos, minCellId, maxCellId);
	for (int x=minCellId[X]; x <= maxCellId[X]; x++)
	for (int y=minCellId[Y]; y <= maxCellId[Y]; y++)
	for (int z=minCellId[Z]; z <= maxCellId[Z]; z++)
	{
		int aryId = GetCellArrayIdFrom3DGridCellId(x, y, z);
		m_gdtCellFlags[aryId] = true;
	}
}
/*
TGeomElemArray<CTetraDelaunay>* CGridDlnyTetra::GetTetraArrayFromCell(CVector3D &pPos)
{
	int cellId[3];
	GetGridCellIdForInsidePosition(pPos, cellId);
	int aryId = GetCellArrayIdFrom3DGridCellId(cellId[X], cellId[Y], cellId[Z]);
	return m_gdtTetraArysInCells[aryId];
}

void CGridDlnyTetra::GetTetrasFromArrays(CVector3D &minPos, CVector3D &maxPos,
										 TPointerArray<CTetraDelaunay> &aryTetras)
{
	int minCellId[3], maxCellId[3];
	GetIdRangeOfCellsToCoverBox(minPos, maxPos, minCellId, maxCellId);
	GetTetrasFromArrays(minCellId, maxCellId, aryTetras);
}

void CGridDlnyTetra::GetTetrasFromArrays(int minCellId[Z+1], int maxCellId[Z+1],
										 TPointerArray<CTetraDelaunay> &aryTetras)
{
	for (int x=minCellId[X]; x <= maxCellId[X]; x++)
	for (int y=minCellId[Y]; y <= maxCellId[Y]; y++)
	for (int z=minCellId[Z]; z <= maxCellId[Z]; z++)
	{
		int aryId = GetCellArrayIdFrom3DGridCellId(x, y, z);
		aryTetras.AppendPointersFromArray(*(m_gdtTetraArysInCells[aryId]));
	}
}
*/