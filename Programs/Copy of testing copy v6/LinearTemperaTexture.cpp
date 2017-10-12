// LinearTemperaTexture.cpp: implementation of the CLinearTemperaTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "LinearTemperaTexture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CLinearTemperaTexture::m_lttId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinearTemperaTexture::CLinearTemperaTexture()
{
	m_lttId++;
	char tmpName[LEN_BOUNDARY_NAME];
	sprintf(tmpName, "ltt_%d", m_lttId);
	m_lttName = tmpName;

	m_lttRatioColorR = CSimuManager::m_ratioColorR;
	m_lttRatioColorG = CSimuManager::m_ratioColorG;
	m_lttRatioColorB = CSimuManager::m_ratioColorB;
	m_lttConstColorR = CSimuManager::m_constColorR;
	m_lttConstColorG = CSimuManager::m_constColorG;
	m_lttConstColorB = CSimuManager::m_constColorB;
	m_lttRatioFilter = CSimuManager::m_ratioFilter;
	m_lttConstFilter = CSimuManager::m_constFilter;
	m_lttMinTemperature = CSimuManager::m_minTemperature;
	m_lttMaxTemperature = CSimuManager::m_maxTemperature;
}

CLinearTemperaTexture::~CLinearTemperaTexture()
{

}

void CLinearTemperaTexture::CopyValuesFrom(CLinearTemperaTexture* anotherLTT)
{
	m_lttRatioColorR = anotherLTT->m_lttRatioColorR;
	m_lttRatioColorG = anotherLTT->m_lttRatioColorG;
	m_lttRatioColorB = anotherLTT->m_lttRatioColorB;
	m_lttConstColorR = anotherLTT->m_lttConstColorR;
	m_lttConstColorG = anotherLTT->m_lttConstColorG;
	m_lttConstColorB = anotherLTT->m_lttConstColorB;
	m_lttRatioFilter = anotherLTT->m_lttRatioFilter;
	m_lttConstFilter = anotherLTT->m_lttConstFilter;
	m_lttMinTemperature = anotherLTT->m_lttMinTemperature;
	m_lttMaxTemperature = anotherLTT->m_lttMaxTemperature;
}
