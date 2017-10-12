// FluidNozzleShuttle.h: interface for the CFluidNozzleShuttle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLUIDNOZZLESHUTTLE_H__9428CA1F_3622_4589_8653_8A6015892D85__INCLUDED_)
#define AFX_FLUIDNOZZLESHUTTLE_H__9428CA1F_3622_4589_8653_8A6015892D85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuFluidNozzle.h"

class CFluidNozzleShuttle : public CSimuFluidNozzle  
{
public:
	SimuValue m_fnsTimeToMove;
	SimuValue m_fnsTimeToBack;

	CFluidNozzleShuttle(SimuValue length, SimuValue radius,
						CVector3D* topCtrPos, CVector3D* direction);
	virtual ~CFluidNozzleShuttle();

	virtual void MoveBoundary(SimuValue curTime, SimuValue timeStep);
};

#endif // !defined(AFX_FLUIDNOZZLESHUTTLE_H__9428CA1F_3622_4589_8653_8A6015892D85__INCLUDED_)
