// SimuDriver.cpp: implementation of the CSimuDriver class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "system/LoggingSystem.h"
#include "SimuFlexApp.h"
#include "SimuMsg.h"
#include "SimuUtility.h"
#include "SimuDriver.h"
#include <math.h>

#include "SimuManager.h"

#include <iostream>

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
	
	++CSimuManager::m_nCurStep;
	//LOGS->logEvent(QString("%1").arg(CSimuManager::m_nCurStep));
	DrivePrtlFluidOneTimeStep();
}

void CSimuDriver::DrivePrtlFluidOneTimeStep()
{
	SimuValue animationClock = m_pSimuManager->m_dbAnimationClock;
	int i;
	//static int currentTimeStep = 0;
	
	// fluid motion
	for (i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
	{
		CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
		long start = clock();
		
		
		
		//if (pPrtlFluid->m_pbFixedParticles == false ||
		if ( pPrtlFluid->objectType == Fluid) {
			pPrtlFluid->EvolveFluidParticles(animationClock, false);
		}

		
		long finish = clock();
		pPrtlFluid->m_pfMotionTime = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
	}

	// vvvvvv surface heating mods vvvvvv
#if 1
	if (CSimuManager::AMBIENT_HEAT_TRANSFER == true) {

		SimuValue thermalTimeStep = m_pSimuManager->m_dbTimeStep
									/CSimuManager::m_subThermalSteps;
		for (int d=0; d < CSimuManager::m_subThermalSteps; d++)
		{

			// reset everything
			for (i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
			{
				CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
				pPrtlFluid->ResetHeatTransferRateOnPrtls();
			}

			// heat from ambient (on surface)
			if (CSimuManager::m_startCookTime  < m_pSimuManager->m_dbAnimationClock) { // its time to start cooking
				
				for (i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
				{
					
					CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
				
					if ( pPrtlFluid->objectType == Fluid ) {
						pPrtlFluid->HeatTransferFromAmbient();
						
						// finally get heat from heatTransferRate
						pPrtlFluid->ComputeTemperatureFromHeatTransferRateOnPrtls(thermalTimeStep);
					}
				
				}
				
			}
			
		}
	}
	// ^^^^^^ surface heating mods ^^^^^^




	// inter-fluid collision
#if 0
	bool evenStep;
	if (CSimuManager::m_nCurStep % 2 == 0) { 
		evenStep = true; 
		i = 0; 
	} else { 
		evenStep = false; 
		i = m_pSimuManager->m_aryPrtlFluid.m_paNum - 1; 
	}

	evenStep = false;
	
	while (i >=0 && i < m_pSimuManager->m_aryPrtlFluid.m_paNum ) {
		CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
		if (evenStep) i ++; else i --;
		//if (pPrtlFluid->m_pbFixedParticles == true) {
		//	continue;
		//}
		//long start = clock();
		
		//if (animationClock >= pPrtlFluid->m_pfOnlyBdryMovingTime
		// && animationClock >= pPrtlFluid->m_pfOnlyGravityTime)
		//{
			pPrtlFluid->InteractWithSolids();

			//pPrtlFluid->InteractWithOtherFluids();
			
			//pPrtlFluid->RegisterPrtlsForNeighborSearchIfNotYet();
			//pPrtlFluid->SearchPrtlNeighbors(true);
			//pPrtlFluid->ComputePrtlDensities();
		//}
		
		//long finish = clock();
		//pPrtlFluid->m_pfInteractTime = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		//pPrtlFluid->m_pfMotionTime += pPrtlFluid->m_pfInteractTime;

		
	}
#endif

	for (i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
	{
		CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
		if ( pPrtlFluid->objectType == Fluid) {
			pPrtlFluid->InteractWithSolids();
		}
		
	}

#endif


	
}
