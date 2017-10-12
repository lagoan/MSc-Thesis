// SimuMsg.h: interface for the CSimuMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUMSG_H__552E6CE2_CB91_11D5_9B85_0050BADE013B__INCLUDED_)
#define AFX_SIMUMSG_H__552E6CE2_CB91_11D5_9B85_0050BADE013B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuMessageDlg.h"
#include "SimuQuestionDlg.h"

class CSimuMsg
{
public:
	static void InitializeDialogs(CWnd* pParentWnd);
	static void DeleteDialogs();

	static char* GetEmptyBuf();
	static char* GetCheckedBuf(bool bCheck=true);

	static void ResetMessageData();
	static CString GetSimuInfo();

	static CString PopLastMessage();
	static void PushMessage(const char* message);
	static void DisplayMessage(const char* msg, bool bDisabled=true);
	static void EraseLastMessage(bool bDisabled=true);
	static void CleanOldMessages(int maxMsgLen, int numOfMsgsToKeep);

	static bool PushErrorInfo(char* message);
	static bool PushErrorInfo(char* format, char* str1);
	static bool PushErrorInfo(char* format, char* str1, char* str2);
	static bool PushErrorInfo(char* format, char* str1, char* str2, char* str3);
	static bool PushErrorInfo(char* format, char* str1, char* str2, char* str3, char* str4);

	static void Message(char* location, char* message);
	static void PauseMessage(char* message);
	static void CancelablePauseMessage(char* message, bool &bPause);

	static void ExitWithMessage(char* location, char* message, bool bSkip=false);
	static void ExitWithMessage(char* location, char* format, int id1);
	static void ExitWithMessage(char* location, char* format, int id1, int id2);
	static void ExitWithMessage(char* location, char* format, char* str1);
	static void ExitWithMessage(char* location, char* format, char* str1, char* str2);

private:
	static int	m_smNumSimuInfo;
	static CString	m_smStrSimuInfo;
	static CString	m_smStrSeparator;
	static CString	m_smOverflowMsgStr;
	static char	m_smMsgBuf[LEN_MESSAGE];	// indirectly used by other classes

	static CSimuMessageDlg* m_dlgMessage;
	static CSimuQuestionDlg* m_dlgQuestion;
};

#endif // !defined(AFX_SIMUMSG_H__552E6CE2_CB91_11D5_9B85_0050BADE013B__INCLUDED_)
