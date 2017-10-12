// GridDlnyTetra.h: interface for the CGridDlnyTetra class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDDLNYTETRA_H__0EA0B440_D306_40D7_B3E7_03E6C5218FB5__INCLUDED_)
#define AFX_GRIDDLNYTETRA_H__0EA0B440_D306_40D7_B3E7_03E6C5218FB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Grid.h"
#include "TGeomElemArray.h"
#include "TetraDelaunay.h"

class CGridDlnyTetra : public CGrid  
{
public:
	TGeomElemArray<CTetraDelaunay>** m_gdtTetraArysInCells;
	bool* m_gdtCellFlags;

	CGridDlnyTetra();
	virtual ~CGridDlnyTetra();

	void CreateTetraArraysInCells();
	void DeleteTetraArraysInCells();
	void RegisterTetraInCells(CTetraDelaunay* pTetra);
	void LabelCellsHoldingTetraCtr(CTetraDelaunay* pTetra);

	void RegisterTetraInCellsByBoundingBox(CTetraDelaunay* pTetra);
	bool DeRegisterTetraInCellsByBoundingBox(CTetraDelaunay* pTetra);
	void DeRegisterDeletedTetraInLabeledCells();
	void ClearLabelsInCells();
	void LabelCellsOverlappingTetraBoundingBox(CTetraDelaunay* pTetra);
/*	TGeomElemArray<CTetraDelaunay>* GetTetraArrayFromCell(CVector3D &pPos);
	void GetTetrasFromArrays(CVector3D &minPos, CVector3D &maxPos,
							TPointerArray<CTetraDelaunay> &aryTetras);
	void GetTetrasFromArrays(int minCellId[Z+1], int maxCellId[Z+1],
							TPointerArray<CTetraDelaunay> &aryTetras);
*/
};

#endif // !defined(AFX_GRIDDLNYTETRA_H__0EA0B440_D306_40D7_B3E7_03E6C5218FB5__INCLUDED_)
