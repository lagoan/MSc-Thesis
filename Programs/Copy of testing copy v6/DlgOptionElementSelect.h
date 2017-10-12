#if !defined(AFX_DLGOPTIONELEMENTSELECT_H__91AD1540_B418_45EF_91B5_419A8C96E7F8__INCLUDED_)
#define AFX_DLGOPTIONELEMENTSELECT_H__91AD1540_B418_45EF_91B5_419A8C96E7F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionElementSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionElementSelect dialog

class CDlgOptionElementSelect : public CDialog
{
// Construction
public:
	CDlgOptionElementSelect(CWnd* pParent = NULL);   // standard constructor

	void RefreshData();
	void InitializeElementSelectOptions();

// Dialog Data
	//{{AFX_DATA(CDlgOptionElementSelect)
	enum { IDD = IDD_OPTIONS_ELMNT_SELECT_DLG };
	CListBox	m_listSelectedElmnt;
	float	m_fBallCtrX;
	float	m_fBallCtrY;
	float	m_fBallCtrZ;
	BOOL	m_bShowEnclosingBall;
	float	m_fEnclosingBallRadius;
	int		m_nElmntId;
	CString	m_strElmntPos;
	int		m_nSelectedNum;
	BOOL	m_bShowSelectedElmnt;
	int		m_nElmntType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionElementSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionElementSelect)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnBtnSelectElmnt();
	afx_msg void OnBtnDeselectElmnt();
	afx_msg void OnBtnDeselectAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateElementSelectOptions();
	void FillInSelectedElementList();

	CSimuManager* m_pSimuManager;
	CGeometryElement* m_curSelElmnt;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONELEMENTSELECT_H__91AD1540_B418_45EF_91B5_419A8C96E7F8__INCLUDED_)
