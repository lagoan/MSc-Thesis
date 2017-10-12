#if !defined(AFX_DLGOPTIONMESHTEXTURE_H__0F0D2BB2_60C7_40F3_A8BD_C302E460A199__INCLUDED_)
#define AFX_DLGOPTIONMESHTEXTURE_H__0F0D2BB2_60C7_40F3_A8BD_C302E460A199__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionMeshTexture.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionMeshTexture dialog

class CDlgOptionMeshTexture : public CDialog
{
// Construction
public:
	CDlgOptionMeshTexture(CWnd* pParent = NULL);   // standard constructor

	void InitializeMeshTextureOptions();

// Dialog Data
	//{{AFX_DATA(CDlgOptionMeshTexture)
	enum { IDD = IDD_OPTIONS_TEXTURE_DLG };
	CComboBox	m_comboFluidList;
	double	m_dbConstColorB;
	double	m_dbConstColorG;
	double	m_dbConstColorR;
	double	m_dbConstFilter;
	double	m_dbMaxTemperature;
	double	m_dbMinTemperature;
	double	m_dbRatioColorB;
	double	m_dbRatioColorG;
	double	m_dbRatioColorR;
	double	m_dbRatioFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionMeshTexture)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionMeshTexture)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnSelchangeComboFluidList();
	afx_msg void OnSelendokComboFluidList();
	afx_msg void OnKillfocusComboFluidList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateMeshTextureOptions();
	void UpdateMeshTextureOptionsFromPrtlFluid(CPrtlFluid* pFluid);

	CSimuManager* m_pSimuManager;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONMESHTEXTURE_H__0F0D2BB2_60C7_40F3_A8BD_C302E460A199__INCLUDED_)
