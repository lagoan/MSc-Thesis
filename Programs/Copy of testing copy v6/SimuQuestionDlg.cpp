// SimuQuestionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuQuestionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuQuestionDlg dialog


CSimuQuestionDlg::CSimuQuestionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimuQuestionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimuQuestionDlg)
	m_strQuestion = _T("");
	//}}AFX_DATA_INIT

	m_numAnswer = NO_ANSWER;
}

int CSimuQuestionDlg::ShowQuestion(char* title, char* question)
{
	SetWindowText(title);
	m_strQuestion = question;
	UpdateData(false);
	ShowWindow(SW_SHOW);
	int reuslt = RunModalLoop();
	return m_numAnswer;
}

void CSimuQuestionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuQuestionDlg)
	DDX_Text(pDX, IDC_EDIT_QUESTION, m_strQuestion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimuQuestionDlg, CDialog)
	//{{AFX_MSG_MAP(CSimuQuestionDlg)
	ON_BN_CLICKED(IDNO, OnNo)
	ON_BN_CLICKED(IDYES, OnYes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuQuestionDlg message handlers

void CSimuQuestionDlg::OnNo() 
{
	// TODO: Add your control notification handler code here

	m_numAnswer = ANSWER_NO;
	EndModalLoop(ANSWER_NO);

	CDialog::OnCancel();
}

void CSimuQuestionDlg::OnYes() 
{
	// TODO: Add your control notification handler code here

	m_numAnswer = ANSWER_YES;
	EndModalLoop(ANSWER_YES);

	CDialog::OnCancel();
}
