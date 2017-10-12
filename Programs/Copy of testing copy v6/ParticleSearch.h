// ParticleSearch.h: interface for the CParticleSearch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTICLESEARCH_H__96E4374A_8A5F_4D25_B228_2DE5744E44F5__INCLUDED_)
#define AFX_PARTICLESEARCH_H__96E4374A_8A5F_4D25_B228_2DE5744E44F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TPrtlGrid.h"
#include "TPointerArray.h"
#include "FluidPrtl.h"

class CParticleSearch  
{
public:
	CVector3D m_psMinPrtlPos, m_psMaxPrtlPos; // particle bounding box
	TPrtlGrid<CFluidPrtl> m_psGrid;
	int m_psNumArysOfPrtlsInCell;
	TPointerArray<CFluidPrtl>** m_psArysOfPrtlsInCell;

	CParticleSearch();
	virtual ~CParticleSearch();

	void RegisterPrtls(int numPrtls, CFluidPrtl** aryPrtls);
	bool RegisterOnePrtl(CFluidPrtl* pPrtl);
	bool DeRegisterOnePrtl(CFluidPrtl* pPrtl);
	void GetPrtlsInGridCellAt(int idx, int idy, int idz,
							  TPointerArray<CFluidPrtl>* &aryPrtls);
	void GetNgbrPrtls(CVector3D* center, SimuValue radius,
					  TPointerArray<CFluidPrtl> &aryNgbrs);
	void GetPossibleNgbrPrtls(CVector3D* center, SimuValue radius,
							  TPointerArray<CFluidPrtl> &aryNgbrs);

	void CreateArraysOfPrtlsInCell();
	void DeleteArraysOfPrtlsInCell();
	void UpdateBufferForArraysOfPrtlsInCell();
	void ResetArraysOfPrtlsInCell();
};

#endif // !defined(AFX_PARTICLESEARCH_H__96E4374A_8A5F_4D25_B228_2DE5744E44F5__INCLUDED_)
