#if !defined(AFX_SIMUMONITOR_H__94283121_D313_11D5_B423_00D0B76AB775__INCLUDED_)
#define AFX_SIMUMONITOR_H__94283121_D313_11D5_B423_00D0B76AB775__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimuMonitor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimuMonitor dialog

class CSimuManager;

class CSimuMonitor : public CDialog
{
// Construction
public:
	CSimuMonitor(CWnd* pParent);   // standard constructor
	~CSimuMonitor();

	void UpdateSimuBodyList();
	void ShowSimulationInformation();
	void ShowSeleElemProps();

	bool	m_bGo;
	UINT	m_uFwdIntvls;

// Dialog Data
	//{{AFX_DATA(CSimuMonitor)
	enum { IDD = IDD_MONITOR_DLG };
	CEdit	m_editSimuProgInfo;
	UINT	m_uPrevFwdIntvls;
	CString	m_strSimuPropInfo;
	double	m_dbFrameIntvl;
	BOOL	m_bEnableMessage;
	double	m_dbAnimationClock;
	float	m_fFluidDensity;
	float	m_fPrtlMass;
	CString	m_strAverageTime;
	float	m_fFluidSmoothLen;
	float	m_fPrtlDist;
	double	m_dbTimeStep;
	//}}AFX_DATA

private:
	CSimuManager*	m_pSimuManager;
	CFont*			m_pNewFont;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	enum BodyElementType
	{
		TRIANGLE,
		PARTICLE
	};

	// Generated message map functions
	//{{AFX_MSG(CSimuMonitor)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGo();
	afx_msg void OnBtnForward();
	afx_msg void OnBtnPause();
	afx_msg void OnBtnReset();
	afx_msg void OnChkEnableMessage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUMONITOR_H__94283121_D313_11D5_B423_00D0B76AB775__INCLUDED_)
