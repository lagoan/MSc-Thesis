#if !defined(AFX_DLGOPTIONMODELING_H__F9C4BCEA_FB11_4E58_AE98_DE97C6BC6652__INCLUDED_)
#define AFX_DLGOPTIONMODELING_H__F9C4BCEA_FB11_4E58_AE98_DE97C6BC6652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionModeling.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionModeling dialog

class CDlgOptionModeling : public CDialog
{
// Construction
public:
	CDlgOptionModeling(CWnd* pParent = NULL);   // standard constructor

	void InitializeModelingOptions();

// Dialog Data
	//{{AFX_DATA(CDlgOptionModeling)
	enum { IDD = IDD_OPTIONS_MODELING_DLG };
	int		m_iFluidType;
	float	m_fCourantFactor;
	float	m_fPrtlPosJitter;
	float	m_fSrfcPrtlDenThreshold;
	float	m_fShearModulus;
	int		m_iTensorIntegration;
	BOOL	m_bApplyGravity;
	float	m_fPrtlDistance;
	float	m_fSmoothLenRatio;
	float	m_fSoundSpeed;
	float	m_fVelocityTuner;
	int		m_iSpecifyMass;
	float	m_fPrtlDensity;
	float	m_fPrtlMass;
	int		m_nPrtlFluidModel;
	float	m_fCGTolerance;
	int		m_iFluidCompressibility;
	BOOL	m_bApplySrfcTension;
	float	m_fSurfaceTension;
	float	m_fDistDumpRate;
	float	m_fEnergyDumpRate;
	float	m_fFrictionRate;
	float	m_fRelaxationTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionModeling)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionModeling)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateModelingOptions();

	CSimuManager* m_pSimuManager;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONMODELING_H__F9C4BCEA_FB11_4E58_AE98_DE97C6BC6652__INCLUDED_)
