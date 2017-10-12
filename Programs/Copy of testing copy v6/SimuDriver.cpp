// SimuDriver.cpp: implementation of the CSimuDriver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuMsg.h"
#include "SimuUtility.h"
#include "SimuDriver.h"
#include <math.h>

#include "SimuManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuDriver::CSimuDriver(CSimuManager* pSimuManager)
{
	if (pSimuManager == NULL)
	{
		CSimuMsg::ExitWithMessage("CSimuDriver::CSimuDriver", "pSimuManager == NULL");
	}

	m_pSimuManager	= pSimuManager;
}

CSimuDriver::~CSimuDriver()
{
}

void CSimuDriver::RunAnimationOneTimeStep()
{
	char location[] = "CSimuDriver::RunAnimationOneTimeStep()";
	TRACE("step %d\n", ++CSimuManager::m_nCurStep);
	
	DrivePrtlFluidOneTimeStep();
}

void CSimuDriver::DrivePrtlFluidOneTimeStep()
{
	SimuValue animationClock = m_pSimuManager->m_pSimuMonitor->m_dbAnimationClock;
	int i;
	// fluid motion
	for (i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
	{
		CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
		long start = clock();
		pPrtlFluid->MoveFluidBoundary(animationClock);
		if (animationClock >= pPrtlFluid->m_pfOnlyBdryMovingTime)
		{
			if (animationClock >= pPrtlFluid->m_pfOnlyGravityTime)
			{
				pPrtlFluid->EvolveFluidParticles(animationClock);
				pPrtlFluid->EnforceFluidBoundary(animationClock);
				pPrtlFluid->RegisterPrtlsForNeighborSearchIfNotYet();
				pPrtlFluid->SearchPrtlNeighbors();
				pPrtlFluid->ComputePrtlDensities();
			}
			else
				pPrtlFluid->MovePrtlsByGravity();
		}
		long finish = clock();
		pPrtlFluid->m_pfMotionTime = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
	}
	// inter-fluid heat transfer
	SimuValue thermalTimeStep = m_pSimuManager->m_pSimuMonitor->m_dbTimeStep
								/CSimuManager::m_subThermalSteps;
	for (int d=0; d < CSimuManager::m_subThermalSteps; d++)
	{
		for (i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
		{
			long start = clock();
			CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
			pPrtlFluid->ResetHeatTransferRateOnPrtls();
			long finish = clock();
			pPrtlFluid->m_pfMotionTime += ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		}
		for (i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
		{
			long start = clock();
			CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
			if (animationClock >= pPrtlFluid->m_pfOnlyBdryMovingTime
			 && animationClock >= pPrtlFluid->m_pfOnlyGravityTime)
			{
				for (int j=i+1; j < m_pSimuManager->m_aryPrtlFluid.m_paNum; j++)
				{
					CPrtlFluid* pOtherFluid = m_pSimuManager->m_aryPrtlFluid[j];
					pPrtlFluid->HeatTransferWithOtherFluids(pOtherFluid);
				}
			}
			long finish = clock();
			pPrtlFluid->m_pfMotionTime += ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		}
		for (i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
		{
			long start = clock();
			CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
			pPrtlFluid->ComputeTemperatureFromHeatTransferRateOnPrtls(thermalTimeStep);
			long finish = clock();
			pPrtlFluid->m_pfMotionTime += ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		}
	}
	// inter-fluid collision
	bool evenStep;
	if (CSimuManager::m_nCurStep % 2 == 0)
	{ evenStep = true; i = 0; }
	else
	{ evenStep = false; i = m_pSimuManager->m_aryPrtlFluid.m_paNum - 1; }
	while (i >=0 && i < m_pSimuManager->m_aryPrtlFluid.m_paNum)
	{
		CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
		long start = clock();
		if (animationClock >= pPrtlFluid->m_pfOnlyBdryMovingTime
		 && animationClock >= pPrtlFluid->m_pfOnlyGravityTime)
		{
			pPrtlFluid->InteractWithOtherFluids();
			pPrtlFluid->RegisterPrtlsForNeighborSearchIfNotYet();
			pPrtlFluid->SearchPrtlNeighbors();
			pPrtlFluid->ComputePrtlDensities();
		}
		long finish = clock();
		pPrtlFluid->m_pfInteractTime = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		pPrtlFluid->m_pfMotionTime += pPrtlFluid->m_pfInteractTime;

		if (evenStep) i ++; else i --;
	}
	// surface construction
	for (i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
	{
		CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
		if (CSimuManager::CREATE_SURFACE_MESH
		 && (!CSimuManager::EXPORT_FLUID_DATA
			|| !CSimuManager::IM_EXPORT_FLUID_SURFACE
			|| m_pSimuManager->IsTimeToExport()))
		{
			long start = clock();
			pPrtlFluid->ConstructFluidSurfaceMeshByMarchingCube();
			long finish = clock();
			pPrtlFluid->m_pfSurfaceTime = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		}
		else
			pPrtlFluid->m_pfSurfaceTime = 0;

		CSimuManager::m_dbTotalInteractTime += pPrtlFluid->m_pfInteractTime;
		CSimuManager::m_dbTotalMotionTime += pPrtlFluid->m_pfMotionTime;
		CSimuManager::m_dbTotalRunningTime += pPrtlFluid->m_pfMotionTime
											+ pPrtlFluid->m_pfSurfaceTime;
		pPrtlFluid->ShowSimuInfoAfterOneTimeStep(CSimuManager::m_nCurStep);
	}
	// drive the current prtl fluid, this code should be obsolete.
	CPrtlFluid* pPrtlFluid = m_pSimuManager->m_curPrtlFluid;
	if (pPrtlFluid != NULL
	 && pPrtlFluid->m_deiImported != true)
	{
		long start = clock();
		pPrtlFluid->MoveFluidBoundary(animationClock);
		if (animationClock >= pPrtlFluid->m_pfOnlyBdryMovingTime)
		{
			if (animationClock >= pPrtlFluid->m_pfOnlyGravityTime)
			{
				pPrtlFluid->EvolveFluidParticles(animationClock);
				pPrtlFluid->EnforceFluidBoundary(animationClock);
			}
			else
				pPrtlFluid->MovePrtlsByGravity();
		}
		long finish = clock();
		pPrtlFluid->m_pfMotionTime = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;

		if (CSimuManager::CREATE_SURFACE_MESH
		 && (!CSimuManager::EXPORT_FLUID_DATA
			|| !CSimuManager::IM_EXPORT_FLUID_SURFACE
			|| m_pSimuManager->IsTimeToExport()))
		{
			long start = clock();
			pPrtlFluid->ConstructFluidSurfaceMeshByMarchingCube();
			long finish = clock();
			pPrtlFluid->m_pfSurfaceTime = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		}
		else
			pPrtlFluid->m_pfSurfaceTime = 0;

		CSimuManager::m_dbTotalInteractTime += pPrtlFluid->m_pfInteractTime;
		CSimuManager::m_dbTotalMotionTime += pPrtlFluid->m_pfMotionTime;
		CSimuManager::m_dbTotalRunningTime += pPrtlFluid->m_pfMotionTime
											+ pPrtlFluid->m_pfSurfaceTime;
		pPrtlFluid->ShowSimuInfoAfterOneTimeStep(CSimuManager::m_nCurStep);
	}
}
