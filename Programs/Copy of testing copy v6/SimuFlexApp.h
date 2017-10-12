// SimuFlexApp.h : main header file for the SIMUFLEXAPP application
//

#if !defined(AFX_SIMUFLEXAPP_H__993A4828_CB30_11D5_9B85_0050BADE013B__INCLUDED_)
#define AFX_SIMUFLEXAPP_H__993A4828_CB30_11D5_9B85_0050BADE013B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <math.h>
#include <time.h>
#include <gl/glu.h>
#include <direct.h>
#include <sys/stat.h>
#include "resource.h"       // main symbols
#include "SimuConstant.h"
#include "SimuMsg.h"
#include "SimuUtility.h"
#include "SimuManager.h"
#include "TemperatureToTexture.h"

/////////////////////////////////////////////////////////////////////////////
// CSimuFlexApp:
// See SimuFlexApp.cpp for the implementation of this class
//

class CSimuFlexApp : public CWinApp
{
public:
	CSimuFlexApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuFlexApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:

	//{{AFX_MSG(CSimuFlexApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CreateTemporaryStaticObjects();
	void DeleteTemporaryStaticObjects();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUFLEXAPP_H__993A4828_CB30_11D5_9B85_0050BADE013B__INCLUDED_)
