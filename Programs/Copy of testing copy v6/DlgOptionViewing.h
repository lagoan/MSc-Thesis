#if !defined(AFX_DLGOPTIONVIEWING_H__A3CDBBB5_E139_4E9A_A8B2_BE8278AB9753__INCLUDED_)
#define AFX_DLGOPTIONVIEWING_H__A3CDBBB5_E139_4E9A_A8B2_BE8278AB9753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionViewing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionViewing dialog

class CDlgOptionViewing : public CDialog
{
// Construction
public:
	CDlgOptionViewing(CWnd* pParent = NULL);   // standard constructor

	void InitializeViewingOptions();

// Dialog Data
	//{{AFX_DATA(CDlgOptionViewing)
	enum { IDD = IDD_OPTIONS_VIEWING_DLG };
	float	m_fBdryBottom;
	float	m_fBdryFar;
	float	m_fBdryLeft;
	float	m_fBdryNear;
	float	m_fBdryRight;
	float	m_fBdryTop;
	BOOL	m_bDrawLight0;
	BOOL	m_bDrawLight1;
	BOOL	m_bDrawXZCoordinates;
	BOOL	m_bEnableLight0;
	BOOL	m_bEnableLight1;
	int		m_iDrawPrtlAs;
	float	m_fSphereRadius;
	int		m_iSphereSlices;
	int		m_iSphereStacks;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionViewing)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionViewing)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateViewingOptions();

	CSimuManager* m_pSimuManager;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONVIEWING_H__A3CDBBB5_E139_4E9A_A8B2_BE8278AB9753__INCLUDED_)
