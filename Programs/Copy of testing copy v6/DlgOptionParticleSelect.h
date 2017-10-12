#if !defined(AFX_DLGOPTIONPARTICLESELECT_H__A0342A42_5ACB_492D_B359_9AF6F26A07AE__INCLUDED_)
#define AFX_DLGOPTIONPARTICLESELECT_H__A0342A42_5ACB_492D_B359_9AF6F26A07AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionParticleSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionParticleSelect dialog

class CDlgOptionParticleSelect : public CDialog
{
// Construction
public:
	CDlgOptionParticleSelect(CWnd* pParent = NULL);   // standard constructor

	void RefreshData();
	void InitializeParticleSelectOptions();

// Dialog Data
	//{{AFX_DATA(CDlgOptionParticleSelect)
	enum { IDD = IDD_OPTIONS_PRTL_SELECT_DLG };
	CListBox	m_listSelectedPrtls;
	int		m_nPrtlId;
	int		m_nPrtlNgbrs;
	CString	m_strPrtlPos;
	float	m_fEnclosingBallRadius;
	BOOL	m_bShowSelectedPrtls;
	BOOL	m_bShowEnclosingBall;
	int		m_nSelectedNum;
	float	m_fBallCtrX;
	float	m_fBallCtrY;
	float	m_fBallCtrZ;
	CString	m_strPrtlVel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionParticleSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionParticleSelect)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnBtnSelectPrtls();
	afx_msg void OnBtnDeselectPrtls();
	afx_msg void OnBtnDeselectAll();
	afx_msg void OnSelchangeListSelectedPrtls();
	afx_msg void OnBtnCtrFromPrtl();
	afx_msg void OnBtnHighlightCurPrtl();
	afx_msg void OnBtnHighlightNgbrs();
	afx_msg void OnBtnDehighlightAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateParticleSelectOptions();
	void FillInSelectedParticleList();

	CSimuManager* m_pSimuManager;
	CFluidPrtl* m_curSelPrtl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONPARTICLESELECT_H__A0342A42_5ACB_492D_B359_9AF6F26A07AE__INCLUDED_)
