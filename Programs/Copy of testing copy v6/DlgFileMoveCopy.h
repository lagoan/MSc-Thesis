#if !defined(AFX_DLGFILEMOVECOPY_H__C1A67A9A_B02F_46EF_80EE_790896C18162__INCLUDED_)
#define AFX_DLGFILEMOVECOPY_H__C1A67A9A_B02F_46EF_80EE_790896C18162__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileMoveCopy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFileMoveCopy dialog

class CDlgFileMoveCopy : public CDialog
{
// Construction
public:
	CDlgFileMoveCopy(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFileMoveCopy)
	enum { IDD = IDD_FILE_MOVE_COPY_DLG };
	CString	m_strDestDir;
	CString	m_strDestFileExt;
	CString	m_strDestFileName;
	int		m_nDestIdIntvl;
	int		m_nDestStartId;
	int		m_nNumSrcFiles;
	CString	m_strSrcDir;
	CString	m_strSrcFileExt;
	CString	m_strSrcFileName;
	int		m_nSrcIdIntvl;
	int		m_nSrcStartId;
	BOOL	m_bMoveFile;
	BOOL	m_bOverWriteFile;
	BOOL	m_bSubDirForDest;
	int		m_nNumFinishedFiles;
	BOOL	m_bKeepNameNId;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFileMoveCopy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFileMoveCopy)
	afx_msg void OnBtnBrowseSrcDir();
	afx_msg void OnBtnBrowseDestDir();
	afx_msg void OnBtnPreCheck();
	afx_msg void OnBtnDoIt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CSimuManager* m_pSimuManager;
	CString m_strParaFileName;

	CString MakeSrcFileName(int curId);
	CString MakeDestFileName(int curId);
	void ExportParameters();
	void ImportParameters();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILEMOVECOPY_H__C1A67A9A_B02F_46EF_80EE_790896C18162__INCLUDED_)
