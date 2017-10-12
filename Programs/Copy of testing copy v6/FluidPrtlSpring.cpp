// FluidPrtlSpring.cpp: implementation of the CFluidPrtlSpring class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "FluidPrtlSpring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFluidPrtlSpring::CFluidPrtlSpring()
{
	m_fpsNgbrSprings.InitializeClassArray(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
}

CFluidPrtlSpring::~CFluidPrtlSpring()
{

}

void CFluidPrtlSpring::InitializeSpringParameters(SimuValue minSpringOrgLen)
{
	m_fpsNgbrSprings.ResetArraySize(m_fpNgbrs.m_paNum);
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_fpNgbrs[i];
		CSpring* pSpring = m_fpsNgbrSprings[i];
		pSpring->m_csConst = CSimuManager::m_springConstant;
		pSpring->m_csOrgLen = pPrtl->m_vpPos->GetDistanceToVector(m_vpPos);
		if (pSpring->m_csOrgLen < minSpringOrgLen)
		{
			CSimuMsg::ExitWithMessage("CFluidPrtlSpring::InitializeSpringParameters(minSpringOrgLen)",
									"Spring orginal length is too small.");
		}
	}
}