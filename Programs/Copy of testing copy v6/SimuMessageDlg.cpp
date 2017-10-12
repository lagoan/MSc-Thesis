// SimuMessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuMessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuMessageDlg dialog


CSimuMessageDlg::CSimuMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimuMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimuMessageDlg)
	m_strMessage = _T("");
	//}}AFX_DATA_INIT
}


void CSimuMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuMessageDlg)
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMessage);
	//}}AFX_DATA_MAP
}

void CSimuMessageDlg::ShowMessage(char* title, char* message)
{
	SetWindowText(title);
	m_strMessage = message;
	UpdateData(false);
	ShowWindow(SW_SHOW);
	RunModalLoop();
}

BEGIN_MESSAGE_MAP(CSimuMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CSimuMessageDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuMessageDlg message handlers

void CSimuMessageDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	EndModalLoop(0);

	CDialog::OnCancel();
}
