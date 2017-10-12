// FluidNozzleShuttle.cpp: implementation of the CFluidNozzleShuttle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "FluidNozzleShuttle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFluidNozzleShuttle::CFluidNozzleShuttle(SimuValue length, SimuValue radius,
										 CVector3D* topCtrPos, CVector3D* direction)
: CSimuFluidNozzle(length, radius, topCtrPos, direction)
{
	// not moving as default
	m_fnsTimeToMove = 0;
	m_fnsTimeToBack = -1;
}

CFluidNozzleShuttle::~CFluidNozzleShuttle()
{

}

void CFluidNozzleShuttle::MoveBoundary(SimuValue curTime, SimuValue timeStep)
{
	if (m_fnsTimeToMove <= curTime && curTime < m_fnsTimeToBack)
	{
		m_cylTopCtrPos.AddedBy(&m_bdryVel, timeStep);
		m_cylBtmCtrPos.AddedBy(&m_bdryVel, timeStep);
		m_bdryColor->SetValue(CSimuManager::COLOR_ORANGE);
	}
	else if (m_fnsTimeToBack <= curTime && curTime < m_fnsTimeToBack+m_fnsTimeToMove)
	{
		m_cylTopCtrPos.AddedBy(&m_bdryVel, -timeStep);
		m_cylBtmCtrPos.AddedBy(&m_bdryVel, -timeStep);
		m_bdryColor->SetValue(CSimuManager::COLOR_CYAN);
	}
	else
		m_bdryColor->SetValue(CSimuManager::COLOR_WHITE);
}
