// FluidPrtlSpring.h: interface for the CFluidPrtlSpring class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLUIDPRTLSPRING_H__515307F5_8319_46C8_94E6_9FB6B09D1D6D__INCLUDED_)
#define AFX_FLUIDPRTLSPRING_H__515307F5_8319_46C8_94E6_9FB6B09D1D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FluidPrtlNonNew.h"
#include "Spring.h"
#include "SimuCGSolver3D.h"

class CFluidPrtlSpring : public CFluidPrtlNonNew  
{
public:
	TGeomElemArray<CSpring> m_fpsNgbrSprings;

	CFluidPrtlSpring();
	virtual ~CFluidPrtlSpring();

	void InitializeSpringParameters(SimuValue minSpringOrgLen);
};

#endif // !defined(AFX_FLUIDPRTLSPRING_H__515307F5_8319_46C8_94E6_9FB6B09D1D6D__INCLUDED_)
