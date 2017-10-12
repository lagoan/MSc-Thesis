#if !defined(AFX_DLGOPTIONIMEXPORT_H__67B5AA0F_0AA2_492B_8324_BC210C4B293B__INCLUDED_)
#define AFX_DLGOPTIONIMEXPORT_H__67B5AA0F_0AA2_492B_8324_BC210C4B293B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionImExport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionImExport dialog

class CSimuManager;

class CDlgOptionImExport : public CDialog
{
// Construction
public:
	CDlgOptionImExport(CWnd* pParent = NULL);   // standard constructor

	void InitializeImExportOptions();

// Dialog Data
	//{{AFX_DATA(CDlgOptionImExport)
	enum { IDD = IDD_OPTIONS_IM_EXPORT_DLG };
	BOOL	m_bExportFluidData;
	BOOL	m_bImportFluidData;
	CString	m_strDataDirectory;
	CString	m_strFullImportInfoFile;
	BOOL	m_bFluidParticles;
	BOOL	m_bFluidSurface;
	int		m_nImportFrameIntvl;
	int		m_nStartFrameNum;
	BOOL	m_bOverwriteData;
	BOOL	m_bOverwriteTexture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionImExport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionImExport)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnBtnBrowseFile();
	afx_msg void OnBtnResetFrame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateImExportOptions();

	CSimuManager* m_pSimuManager;
	CString m_strImportDir;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONIMEXPORT_H__67B5AA0F_0AA2_492B_8324_BC210C4B293B__INCLUDED_)
