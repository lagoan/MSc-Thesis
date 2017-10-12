// SimuFluidNozzle.h: interface for the CSimuFluidNozzle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUFLUIDNOZZLE_H__C157F71F_CCB0_4492_B9E0_2C9A32F45F0D__INCLUDED_)
#define AFX_SIMUFLUIDNOZZLE_H__C157F71F_CCB0_4492_B9E0_2C9A32F45F0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuBoundaryCylinder.h"

class CSimuFluidNozzle : public CSimuBoundaryCylinder
{
public:
	bool m_fnTurnOff;
	SimuValue m_fnExitSpeed;
	SimuValue m_fnLastEmptyLength;
	SimuValue m_fnLastInjectionTime;
	SimuValue m_fnFrictionEffectiveTime;
	SimuValue m_fnStopFlowingTime;

	CSimuFluidNozzle();
	CSimuFluidNozzle(SimuValue length, SimuValue radius,
					 CVector3D* topCtrPos, CVector3D* direction);
	virtual ~CSimuFluidNozzle();

	void DrawFluidNozzle();
	virtual void ExportBoundaryParameters(FILE* fp);
	virtual void ImportBoundaryParameters(FILE* fp);

	virtual void EnforceBoundaryConstraint(CPrtlFluid* pPrtlFluid); 
	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);

	void CheckFrictionStopTime(SimuValue simuTime);
	void StopFlowing(SimuValue simuTime, CPrtlFluid* pPrtlFluid);
	void InjectFluidParticles(SimuValue curTime, CPrtlFluid* pPrtlFluid);

private:
	int m_fnTotalLayers;
	bool m_fnPrtlInsideNozzle;
};

#endif // !defined(AFX_SIMUFLUIDNOZZLE_H__C157F71F_CCB0_4492_B9E0_2C9A32F45F0D__INCLUDED_)
