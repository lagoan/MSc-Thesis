#if !defined(AFX_DLGOPTIONSURFACE_H__FA4791B7_2124_41BE_AB4E_AF02C654BC6B__INCLUDED_)
#define AFX_DLGOPTIONSURFACE_H__FA4791B7_2124_41BE_AB4E_AF02C654BC6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionSurface.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionSurface dialog

class CDlgOptionSurface : public CDialog
{
// Construction
public:
	CDlgOptionSurface(CWnd* pParent = NULL);   // standard constructor

	void InitializeSurfaceOptions();

// Dialog Data
	//{{AFX_DATA(CDlgOptionSurface)
	enum { IDD = IDD_OPTIONS_SURFACE_DLG };
	BOOL	m_bCreateSrfcMesh;
	float	m_fIsoDenRadiusRatio;
	float	m_fMeshSizeOverPrtlDist;
	float	m_fIsoDensityRatio;
	int		m_rdoIsoDensityChoice;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionSurface)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionSurface)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateSurfaceOptions();

	CSimuManager* m_pSimuManager;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONSURFACE_H__FA4791B7_2124_41BE_AB4E_AF02C654BC6B__INCLUDED_)
