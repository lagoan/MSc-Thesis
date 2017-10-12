#if !defined(AFX_SIMUMESSAGEDLG_H__CAF15ACC_76C0_4C65_8442_ECDFCEE69C94__INCLUDED_)
#define AFX_SIMUMESSAGEDLG_H__CAF15ACC_76C0_4C65_8442_ECDFCEE69C94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimuMessageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimuMessageDlg dialog

class CSimuMessageDlg : public CDialog
{
// Construction
public:
	CSimuMessageDlg(CWnd* pParent = NULL);   // standard constructor

	void ShowMessage(char* title, char* message);

// Dialog Data
	//{{AFX_DATA(CSimuMessageDlg)
	enum { IDD = IDD_MESSAGE_DLG };
	CString	m_strMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSimuMessageDlg)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUMESSAGEDLG_H__CAF15ACC_76C0_4C65_8442_ECDFCEE69C94__INCLUDED_)
