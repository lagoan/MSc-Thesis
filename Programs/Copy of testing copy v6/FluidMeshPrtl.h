// FluidMeshPrtl.h: interface for the CFluidMeshPrtl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLUIDMESHPRTL_H__E9E3384D_27DA_4682_9F83_E991FBCBCF99__INCLUDED_)
#define AFX_FLUIDMESHPRTL_H__E9E3384D_27DA_4682_9F83_E991FBCBCF99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MeshPrtl.h"
#include "FluidPrtlSpring.h"

class CFluidMeshPrtl : 	public CFluidPrtlSpring, 	public CMeshPrtl  
{
public:
	CFluidMeshPrtl();
	virtual ~CFluidMeshPrtl();

	void CopyFluidValuesFromPrtl(CFluidMeshPrtl* pPrtl);
	void InterpolatePrtlValuesByDistance(TPointerArray<CFluidPrtl> &aryPossibleNgbrs,
										 SimuValue radius, SimuValue colocationEpsilon);
};

#endif // !defined(AFX_FLUIDMESHPRTL_H__E9E3384D_27DA_4682_9F83_E991FBCBCF99__INCLUDED_)
