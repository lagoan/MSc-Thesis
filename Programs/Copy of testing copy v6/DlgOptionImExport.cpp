// DlgOptionImExport.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgOptionImExport.h"
#include "SimuManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionImExport dialog


CDlgOptionImExport::CDlgOptionImExport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionImExport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionImExport)
	m_bExportFluidData = FALSE;
	m_bImportFluidData = FALSE;
	m_strDataDirectory = _T("");
	m_strFullImportInfoFile = _T("");
	m_bFluidParticles = FALSE;
	m_bFluidSurface = FALSE;
	m_nImportFrameIntvl = 0;
	m_nStartFrameNum = 0;
	m_bOverwriteData = FALSE;
	m_bOverwriteTexture = FALSE;
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;

	InitializeImExportOptions();
}


void CDlgOptionImExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionImExport)
	DDX_Check(pDX, IDC_CHK_EXPORT_FLUID_DATA, m_bExportFluidData);
	DDX_Check(pDX, IDC_CHK_IMPORT_FLUID_DATA, m_bImportFluidData);
	DDX_Text(pDX, IDC_DATA_DIRECTORY, m_strDataDirectory);
	DDX_Text(pDX, IDC_FULL_IMPORT_INFO_FILE, m_strFullImportInfoFile);
	DDX_Check(pDX, IDC_CHK_PARTICLES, m_bFluidParticles);
	DDX_Check(pDX, IDC_CHK_SURFACE, m_bFluidSurface);
	DDX_Text(pDX, IDC_IMPORT_FRAME_INTVL, m_nImportFrameIntvl);
	DDX_Text(pDX, IDC_START_FRAME_NUM, m_nStartFrameNum);
	DDX_Check(pDX, IDC_CHK_OVERWRITE_DATA, m_bOverwriteData);
	DDX_Check(pDX, IDC_CHK_OVERWRITE_TEXTURE, m_bOverwriteTexture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionImExport, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionImExport)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(IDC_BTN_BROWSE_FILE, OnBtnBrowseFile)
	ON_BN_CLICKED(IDC_BTN_RESET_FRAME, OnBtnResetFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgOptionImExport::InitializeImExportOptions()
{
	m_strDataDirectory = CSimuManager::m_strAppDataDir;

	m_bImportFluidData = CSimuManager::IMPORT_FLUID_DATA;
	m_strImportDir = CSimuManager::m_strImportDir;
	m_strFullImportInfoFile = CSimuManager::m_strImportDir;
	if (CSimuManager::m_strImportParaFile.GetLength() > 0)
	{
		m_strFullImportInfoFile += "\\";
		m_strFullImportInfoFile += CSimuManager::m_strImportParaFile;
	}
	m_nImportFrameIntvl = CSimuManager::m_nImportFrameInterval;
	m_nStartFrameNum = CSimuManager::m_nCurFrame;

	m_bExportFluidData = CSimuManager::EXPORT_FLUID_DATA;
	m_bOverwriteData = CSimuManager::OVERWRITE_EXISTING_DATA;
	m_bOverwriteTexture = CSimuManager::OVERWRITE_TEXTURE;
	m_bFluidParticles = CSimuManager::IM_EXPORT_FLUID_PARTICLES;
	m_bFluidSurface = CSimuManager::IM_EXPORT_FLUID_SURFACE;
}

void CDlgOptionImExport::UpdateImExportOptions()
{
	CSimuManager::m_strAppDataDir = m_strDataDirectory;

	CSimuManager::IMPORT_FLUID_DATA = m_bImportFluidData;
	CSimuManager::m_strImportDir = m_strImportDir;
	CSimuManager::m_strImportParaFile = m_strFullImportInfoFile;
	CSimuManager::m_strImportParaFile.Replace(m_strImportDir, "");
	CSimuManager::m_strImportParaFile.TrimLeft("\\");
	CSimuManager::m_nImportFrameInterval = m_nImportFrameIntvl;
	CSimuManager::m_nCurFrame = m_nStartFrameNum;

	CSimuManager::EXPORT_FLUID_DATA = m_bExportFluidData;
	if (CSimuManager::EXPORT_FLUID_DATA)
		((CWnd*)m_pSimuManager->m_pSimuMonitor->GetDlgItem(IDC_EDIT_FRAME_INTVL))->EnableWindow(true);
	else
		((CWnd*)m_pSimuManager->m_pSimuMonitor->GetDlgItem(IDC_EDIT_FRAME_INTVL))->EnableWindow(false);

	CSimuManager::IM_EXPORT_FLUID_PARTICLES = m_bFluidParticles;
	CSimuManager::IM_EXPORT_FLUID_SURFACE = m_bFluidSurface;
	CSimuManager::OVERWRITE_EXISTING_DATA = m_bOverwriteData;
	CSimuManager::OVERWRITE_TEXTURE = m_bOverwriteTexture;

	CSimuManager::DRAW_PARTICLES = m_bFluidParticles;
	CSimuManager::DRAW_SURFACE_MESH = m_bFluidSurface;

	if (CSimuManager::IMPORT_FLUID_DATA)
		m_pSimuManager->PrepareImportFluidData();
}

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionImExport message handlers

void CDlgOptionImExport::OnCancel() 
{
	// TODO: Add extra cleanup here

	InitializeImExportOptions();
	UpdateData(false);
	
	CDialog::OnCancel();
}

void CDlgOptionImExport::OnOK() 
{
	// TODO: Add extra validation here
	
	OnApply();
	
	CDialog::OnOK();
}

void CDlgOptionImExport::OnApply() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	UpdateImExportOptions();

	m_pSimuManager->RefreshView();
}

void CDlgOptionImExport::OnBtnBrowseFile() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();

	CFileDialog* fileDialog = new CFileDialog(true);
	fileDialog->m_ofn.lpstrInitialDir = m_strImportDir;

	fileDialog->DoModal();
	m_strFullImportInfoFile = fileDialog->GetPathName();
	UpdateData(false);

	m_strImportDir = fileDialog->GetPathName();
	m_strImportDir.Replace(fileDialog->GetFileName(), "");
	m_strImportDir.TrimRight("\\");

	delete fileDialog;
}

void CDlgOptionImExport::OnBtnResetFrame() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	m_nStartFrameNum = 1;
	CSimuManager::m_nCurFrame = m_nStartFrameNum;
	UpdateData(false);
}
