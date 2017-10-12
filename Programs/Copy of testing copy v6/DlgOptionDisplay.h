#if !defined(AFX_DLGOPTIONDISPLAY_H__31D986A7_4DEC_4882_878F_A3C47185199C__INCLUDED_)
#define AFX_DLGOPTIONDISPLAY_H__31D986A7_4DEC_4882_878F_A3C47185199C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionDisplay dialog

class CSimuManager;

class CDlgOptionDisplay : public CDialog
{
// Construction
public:
	CDlgOptionDisplay(CWnd* pParent = NULL);   // standard constructor

	void InitializeDisplayOptions();

// Dialog Data
	//{{AFX_DATA(CDlgOptionDisplay)
	enum { IDD = IDD_OPTIONS_DISPLAY_DLG };
	BOOL	m_bDrawParticles;
	BOOL	m_bDrawPrtlId;
	BOOL	m_bPrtlDensColor;
	float	m_fDisplayTuner;
	BOOL	m_bColorSrfcPrtls;
	BOOL	m_bRenderGridPrtls;
	BOOL	m_bPrtlSpeedColor;
	BOOL	m_bPrtlMassColor;
	BOOL	m_bDrawHighlitOnly;
	BOOL	m_bDrawBoundary;
	float	m_fParticleIdSize;
	BOOL	m_bDrawCurvNorm;
	BOOL	m_bDrawSrfcMesh;
	BOOL	m_bDrawSrfcPrtlNorm;
	BOOL	m_bDrawTetrahedra;
	BOOL	m_bDrawTriNorm;
	BOOL	m_bDrawSrfcPrtl;
	BOOL	m_bDrawConvexHull;
	BOOL	m_bDrawDelaunay;
	BOOL	m_bUpdateEntireDisplay;
	BOOL	m_bDrawEdgeOnly;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionDisplay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionDisplay)
	afx_msg void OnApply();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateDisplayOptions();

	CSimuManager* m_pSimuManager;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONDISPLAY_H__31D986A7_4DEC_4882_878F_A3C47185199C__INCLUDED_)
