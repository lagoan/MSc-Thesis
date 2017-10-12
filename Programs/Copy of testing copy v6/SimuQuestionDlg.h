#if !defined(AFX_SIMUQUESTIONDLG_H__B3CBA02D_E314_47CC_A14D_9283320D48C2__INCLUDED_)
#define AFX_SIMUQUESTIONDLG_H__B3CBA02D_E314_47CC_A14D_9283320D48C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimuQuestionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimuQuestionDlg dialog

class CSimuQuestionDlg : public CDialog
{
// Construction
public:
	CSimuQuestionDlg(CWnd* pParent = NULL);   // standard constructor

	int ShowQuestion(char* title, char* question);

	enum {
		ANSWER_YES,
		ANSWER_NO,
		NO_ANSWER
	};

// Dialog Data
	//{{AFX_DATA(CSimuQuestionDlg)
	enum { IDD = IDD_QUESTION_DLG };
	CString	m_strQuestion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuQuestionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSimuQuestionDlg)
	afx_msg void OnNo();
	afx_msg void OnYes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_numAnswer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUQUESTIONDLG_H__B3CBA02D_E314_47CC_A14D_9283320D48C2__INCLUDED_)
