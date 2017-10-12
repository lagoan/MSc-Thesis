// DlgOptionMeshTexture.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgOptionMeshTexture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionMeshTexture dialog


CDlgOptionMeshTexture::CDlgOptionMeshTexture(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionMeshTexture::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionMeshTexture)
	m_dbConstColorB = 0.0;
	m_dbConstColorG = 0.0;
	m_dbConstColorR = 0.0;
	m_dbConstFilter = 0.0;
	m_dbMaxTemperature = 0.0;
	m_dbMinTemperature = 0.0;
	m_dbRatioColorB = 0.0;
	m_dbRatioColorG = 0.0;
	m_dbRatioColorR = 0.0;
	m_dbRatioFilter = 0.0;
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;

	InitializeMeshTextureOptions();
}


void CDlgOptionMeshTexture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionMeshTexture)
	DDX_Control(pDX, IDC_COMBO_FLUID_LIST, m_comboFluidList);
	DDX_Text(pDX, IDC_CONST_COLOR_B, m_dbConstColorB);
	DDX_Text(pDX, IDC_CONST_COLOR_G, m_dbConstColorG);
	DDX_Text(pDX, IDC_CONST_COLOR_R, m_dbConstColorR);
	DDX_Text(pDX, IDC_CONST_FILTER, m_dbConstFilter);
	DDX_Text(pDX, IDC_MAX_TEMPERATURE, m_dbMaxTemperature);
	DDX_Text(pDX, IDC_MIN_TEMPERATURE, m_dbMinTemperature);
	DDX_Text(pDX, IDC_RATIO_COLOR_B, m_dbRatioColorB);
	DDX_Text(pDX, IDC_RATIO_COLOR_G, m_dbRatioColorG);
	DDX_Text(pDX, IDC_RATIO_COLOR_R, m_dbRatioColorR);
	DDX_Text(pDX, IDC_RATIO_FILTER, m_dbRatioFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionMeshTexture, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionMeshTexture)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_CBN_SELCHANGE(IDC_COMBO_FLUID_LIST, OnSelchangeComboFluidList)
	ON_CBN_SELENDOK(IDC_COMBO_FLUID_LIST, OnSelendokComboFluidList)
	ON_CBN_KILLFOCUS(IDC_COMBO_FLUID_LIST, OnKillfocusComboFluidList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgOptionMeshTexture::InitializeMeshTextureOptions()
{
	if (m_pSimuManager->m_aryPrtlFluid.m_paNum > 0)
	{
		int curId = m_comboFluidList.GetCurSel();
		m_comboFluidList.ResetContent();
		for (int i=0; i < m_pSimuManager->m_aryPrtlFluid.m_paNum; i++)
		{
			CPrtlFluid* pPrtlFluid = m_pSimuManager->m_aryPrtlFluid[i];
			int id = m_comboFluidList.InsertString(i, pPrtlFluid->m_deiName);
		}
		if (curId >= 0 && curId < m_pSimuManager->m_aryPrtlFluid.m_paNum)
		{
			m_comboFluidList.SetCurSel(curId);
			UpdateMeshTextureOptionsFromPrtlFluid(m_pSimuManager->m_aryPrtlFluid[curId]);
		}
	}
	else
	{
		m_dbConstColorB = CSimuManager::m_constColorB;
		m_dbConstColorG = CSimuManager::m_constColorG;
		m_dbConstColorR = CSimuManager::m_constColorR;
		m_dbConstFilter = CSimuManager::m_constFilter;
		m_dbMaxTemperature = CSimuManager::m_maxTemperature;
		m_dbMinTemperature = CSimuManager::m_minTemperature;
		m_dbRatioColorB = CSimuManager::m_ratioColorB;
		m_dbRatioColorG = CSimuManager::m_ratioColorG;
		m_dbRatioColorR = CSimuManager::m_ratioColorR;
		m_dbRatioFilter = CSimuManager::m_ratioFilter;
	}
}

void CDlgOptionMeshTexture::UpdateMeshTextureOptions()
{
	int curId = m_comboFluidList.GetCurSel();
	if (curId >= 0 && curId < m_pSimuManager->m_aryPrtlFluid.m_paNum)
	{
		CLinearTemperaTexture* pLTT = &(m_pSimuManager->m_aryPrtlFluid[curId]->m_pfLinrTempTxtr);
		pLTT->m_lttConstColorB = m_dbConstColorB;
		pLTT->m_lttConstColorG = m_dbConstColorG;
		pLTT->m_lttConstColorR = m_dbConstColorR;
		pLTT->m_lttConstFilter = m_dbConstFilter;
		pLTT->m_lttRatioColorB = m_dbRatioColorB;
		pLTT->m_lttRatioColorG = m_dbRatioColorG;
		pLTT->m_lttRatioColorR = m_dbRatioColorR;
		pLTT->m_lttRatioFilter = m_dbRatioFilter;
		pLTT->m_lttMaxTemperature = m_dbMaxTemperature;
		pLTT->m_lttMinTemperature = m_dbMinTemperature;
	}
	else
		CSimuMsg::Message("CDlgOptionMeshTexture::UpdateMeshTextureOptions()",
							"Selected fluid is not valid.");
}

void CDlgOptionMeshTexture::UpdateMeshTextureOptionsFromPrtlFluid(CPrtlFluid* pFluid)
{
	CLinearTemperaTexture* pLTT = &(pFluid->m_pfLinrTempTxtr);
	m_dbConstColorB = pLTT->m_lttConstColorB;
	m_dbConstColorG = pLTT->m_lttConstColorG;
	m_dbConstColorR = pLTT->m_lttConstColorR;
	m_dbConstFilter = pLTT->m_lttConstFilter;
	m_dbRatioColorB = pLTT->m_lttRatioColorB;
	m_dbRatioColorG = pLTT->m_lttRatioColorG;
	m_dbRatioColorR = pLTT->m_lttRatioColorR;
	m_dbRatioFilter = pLTT->m_lttRatioFilter;
	m_dbMaxTemperature = pLTT->m_lttMaxTemperature;
	m_dbMinTemperature = pLTT->m_lttMinTemperature;;
	UpdateData(false);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionMeshTexture message handlers

void CDlgOptionMeshTexture::OnCancel() 
{
	// TODO: Add extra cleanup here

	UpdateData(false);
	
	CDialog::OnCancel();
}

void CDlgOptionMeshTexture::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();
	
	CDialog::OnOK();
}

void CDlgOptionMeshTexture::OnApply() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	UpdateMeshTextureOptions();

	m_pSimuManager->RefreshView();
}

void CDlgOptionMeshTexture::OnSelchangeComboFluidList() 
{
	// TODO: Add your control notification handler code here

	int curId = m_comboFluidList.GetCurSel();
	if (curId >= 0 && curId < m_pSimuManager->m_aryPrtlFluid.m_paNum)
	{
		UpdateMeshTextureOptionsFromPrtlFluid(m_pSimuManager->m_aryPrtlFluid[curId]);
	}
	else
		CSimuMsg::Message("CDlgOptionMeshTexture::OnSelchangeComboFluidList() ",
							"Selected fluid is not valid.");
}

void CDlgOptionMeshTexture::OnSelendokComboFluidList() 
{
	// TODO: Add your control notification handler code here
	
	int dumb = 0;
}

void CDlgOptionMeshTexture::OnKillfocusComboFluidList() 
{
	// TODO: Add your control notification handler code here
	
	int dumb = 0;
	int curId = m_comboFluidList.GetCurSel();
}
