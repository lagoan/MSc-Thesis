#if !defined(AFX_DLGOPTIONMESHER_H__2D1B5ECD_5F5C_4044_8565_E31FF628601E__INCLUDED_)
#define AFX_DLGOPTIONMESHER_H__2D1B5ECD_5F5C_4044_8565_E31FF628601E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionMesher.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionMesher dialog

class CDlgOptionMesher : public CDialog
{
// Construction
public:
	CDlgOptionMesher(CWnd* pParent = NULL);   // standard constructor

	void InitializeMesherOptions();

// Dialog Data
	//{{AFX_DATA(CDlgOptionMesher)
	enum { IDD = IDD_OPTIONS_MESHER_DLG };
	BOOL	m_bDrawExtrSrfcPrtl;
	BOOL	m_bDrawIntrSrfcPrtl;
	BOOL	m_bDrawIntrSrfcTri;
	BOOL	m_bDrawPrtl;
	BOOL	m_bDrawTet;
	BOOL	m_bDrawIsomorphicLine;
	BOOL	m_bDebugMesher;
	BOOL	m_bDrawIntrSrfcNormal;
	BOOL	m_bDrawMesherGrid;
	BOOL	m_bDrawExtrSrfcTri;
	BOOL	m_bDrawSrfcTet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionMesher)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionMesher)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateMesherOptions();

	CSimuManager* m_pSimuManager;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONMESHER_H__2D1B5ECD_5F5C_4044_8565_E31FF628601E__INCLUDED_)
