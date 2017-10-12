// SimuMsg.cpp: implementation of the CSimuMsg class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include <string>
#include "SimuFlexApp.h"
#include "SimuMsg.h"
#include "SimuManager.h"
#include "system/LoggingSystem.h"

#include <iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// This separates information pushed in and popped out each time
#define	INFO_SEPARATOR			RETURN_STRING
#define OVERFLOW_MESSAGE		"Exit or not?\nOverflow in message buffer "

// private static variables
int	 CSimuMsg::m_smNumSimuInfo		= 0;
QString CSimuMsg::m_smStrSimuInfo	= "";
QString CSimuMsg::m_smStrSeparator	= INFO_SEPARATOR;
QString CSimuMsg::m_smOverflowMsgStr = "";
char	CSimuMsg::m_smMsgBuf[LEN_MESSAGE] = "";
//CSimuMessageDlg* CSimuMsg::m_dlgMessage = NULL;
//CSimuQuestionDlg* CSimuMsg::m_dlgQuestion = NULL;


void CSimuMsg::ExitWithMessage(char* location, char* message, bool bSkip)
{
	string  _res;
	QString _location;
	QString _message;
	//QString _res;

	_location = location;
	_message = message;

	//_res = _location + " " + _message;
	_res = message;
	_res += " at ";
	_res += location;
	//LOGS->logEvent(_res, Critical);
	throw _res;

}




char* CSimuMsg::GetEmptyBuf()
{
	m_smMsgBuf[0] = '\0';
	return m_smMsgBuf;
}

char* CSimuMsg::GetCheckedBuf(bool bCheck/*default=true*/)
{

	// XXX should check
	/* 
	if (bCheck && strlen(m_smMsgBuf) > LEN_MESSAGE)
	{
		AfxGetMainWnd()->KillTimer(SIMU_TIMER_ID);
		m_smMsgBuf[LEN_MESSAGE-1] = '\0';
		m_smOverflowMsgStr = OVERFLOW_MESSAGE; 
		m_smOverflowMsgStr += m_smMsgBuf;
		if (IDYES == MessageBox(AfxGetMainWnd()->m_hWnd,
								(LPCTSTR)m_smOverflowMsgStr, "Error",
								MB_APPLMODAL | MB_ICONQUESTION | MB_YESNO))
		{
			delete ((CSimuManager*)AfxGetMainWnd());
			exit(1);
		}
	}
	*/
	return m_smMsgBuf;
}


bool CSimuMsg::PushErrorInfo(char* message)
{
	//AfxGetMainWnd()->KillTimer(SIMU_TIMER_ID);

	char	tmpBuf[LEN_MESSAGE];
	strcpy(tmpBuf, message);
	//LOGS->logEvent(tmpBuf, Critical);

	//strcat(tmpBuf, "\nExit or not?");
	//if (IDYES == MessageBox(AfxGetMainWnd()->m_hWnd,
	//						tmpBuf, "Error Found", MB_YESNO|MB_ICONERROR))
	//{
	//	delete ((CSimuManager*)AfxGetMainWnd());
	//	exit(1);
	//}
	return false;
}

bool CSimuMsg::PushErrorInfo(char* format, char* str1)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1);
	return PushErrorInfo(tmpBuf);
}

bool CSimuMsg::PushErrorInfo(char* format, char* str1, char* str2)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1, str2);
	return PushErrorInfo(tmpBuf);
}

bool CSimuMsg::PushErrorInfo(char* format, char* str1, char* str2, char* str3)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1, str2, str3);
	return PushErrorInfo(tmpBuf);
}

bool CSimuMsg::PushErrorInfo(char* format, char* str1, char* str2, char* str3, char* str4)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1, str2, str3, str4);
	return PushErrorInfo(tmpBuf);
}


void CSimuMsg::ResetMessageData()
{
	m_smStrSimuInfo		= "";
	m_smNumSimuInfo		= 0;
}

void CSimuMsg::PauseMessage(char* message)
{
	//LOGS->logEvent(message);
}

void CSimuMsg::CancelablePauseMessage(char* message, bool &bPause)
{
	cout << message << endl;
	//LOGS->logEvent(message);
}

void CSimuMsg::DisplayMessage(const char* msg, bool bDisabled/*default=true*/)
{
	//LOGS->logEvent(msg);
}

void CSimuMsg::EraseLastMessage(bool bDisabled/*default=true*/)
{
	
}

void CSimuMsg::Message(char* location, char* message)
{
	QString _location;
	QString _message;
	QString _res;

	_location = location;
	_message = message;

	_res = _location + " " + _message;

	//LOGS->logEvent(_res);

}


#if 0
QString CSimuMsg::GetSimuInfo()
{
	return m_smStrSimuInfo;
}

QString CSimuMsg::PopLastMessage()
{
	QString returnStr;
	int pos = m_smStrSimuInfo.Find(INFO_SEPARATOR);
	if (pos == -1)
	{
		returnStr = m_smStrSimuInfo;
		m_smStrSimuInfo = "";
		m_smNumSimuInfo = 0;
	}
	else
	{
		returnStr = m_smStrSimuInfo.Left(pos);
		m_smStrSimuInfo.Delete(0, pos + m_smStrSeparator.size());
		m_smNumSimuInfo--;
	}
	return returnStr;
}

void CSimuMsg::PushMessage(const char* message)
{
	char	tmpBuf[LEN_MESSAGE];
	m_smStrSimuInfo.Insert(0, m_smStrSimuInfo.size() > 0 ? INFO_SEPARATOR : "");
//	sprintf(tmpBuf, "<%d> %s", ++m_smNumSimuInfo, message);
	sprintf(tmpBuf, "%s", message);
	m_smStrSimuInfo.Insert(0, tmpBuf);
}

void CSimuMsg::DisplayMessage(const char* msg, bool bDisabled/*default=true*/)
{
	#ifdef	SIMU_PARA_DISABLE_MONITOR_MESSAGE
	if (bDisabled) return;
	#endif

	CSimuMsg::PushMessage(msg);
	if (((CSimuManager*)AfxGetMainWnd()) != NULL)
	{
		CSimuMonitor* tmpMonitor = ((CSimuManager*)AfxGetMainWnd())->m_pSimuMonitor;
		if (tmpMonitor->GetSafeHwnd() != NULL)
			tmpMonitor->ShowSimulationInformation();
	}
}

void CSimuMsg::EraseLastMessage(bool bDisabled/*default=true*/)
{
	#ifdef	SIMU_PARA_DISABLE_MONITOR_MESSAGE
	if (bDisabled) return;
	#endif

	PopLastMessage();
}

void CSimuMsg::CleanOldMessages(int maxMsgLen, int numOfMsgsToKeep)
{
	if (m_smStrSimuInfo.size() <= maxMsgLen)
		return;
	QString newSimuInfo = "";
	int i;
	for (i=0; i < numOfMsgsToKeep && newSimuInfo.size() <= maxMsgLen; i++)
	{
		newSimuInfo += PopLastMessage();
		newSimuInfo += RETURN_STRING;
	}
	m_smNumSimuInfo += i;
	m_smStrSimuInfo = newSimuInfo;
}

bool CSimuMsg::PushErrorInfo(char* message)
{
	AfxGetMainWnd()->KillTimer(SIMU_TIMER_ID);

	char	tmpBuf[LEN_MESSAGE];
	strcpy(tmpBuf, message);
	strcat(tmpBuf, "\nExit or not?");
	if (IDYES == MessageBox(AfxGetMainWnd()->m_hWnd,
							tmpBuf, "Error Found", MB_YESNO|MB_ICONERROR))
	{
		delete ((CSimuManager*)AfxGetMainWnd());
		exit(1);
	}
	return false;
}

bool CSimuMsg::PushErrorInfo(char* format, char* str1)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1);
	return PushErrorInfo(tmpBuf);
}

bool CSimuMsg::PushErrorInfo(char* format, char* str1, char* str2)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1, str2);
	return PushErrorInfo(tmpBuf);
}

bool CSimuMsg::PushErrorInfo(char* format, char* str1, char* str2, char* str3)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1, str2, str3);
	return PushErrorInfo(tmpBuf);
}

bool CSimuMsg::PushErrorInfo(char* format, char* str1, char* str2, char* str3, char* str4)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1, str2, str3, str4);
	return PushErrorInfo(tmpBuf);
}

void CSimuMsg::Message(char* location, char* message)
{
	if (AfxGetMainWnd() != NULL)
		((CSimuManager*)AfxGetMainWnd())->m_pSimuView->Invalidate(false);
	m_dlgMessage->ShowMessage(location, message);
}

void CSimuMsg::PauseMessage(char* message)
{
	Message("Pause", message);
}

void CSimuMsg::CancelablePauseMessage(char* message, bool &bPause)
{
	if (!bPause) return;
	sprintf(GetEmptyBuf(), "%s%sCancel this pause from now on?", message, DOUBLE_RETURN_STRING);
	if (AfxGetMainWnd() != NULL)
		((CSimuManager*)AfxGetMainWnd())->m_pSimuView->Invalidate(false);
	int questionAnswer = m_dlgQuestion->ShowQuestion("Cancelable Pause",
													CSimuMsg::GetCheckedBuf());
	if (CSimuQuestionDlg::ANSWER_YES == questionAnswer)
		bPause = false;
}

void CSimuMsg::ExitWithMessage(char* location, char* message, bool bSkip)
{
	if (bSkip) return;

	if (AfxGetMainWnd() != NULL)
	{
		AfxGetMainWnd()->KillTimer(SIMU_TIMER_ID);
		((CSimuManager*)AfxGetMainWnd())->m_pSimuView->Invalidate(false);
	}

	char	tmpBuf[LEN_MESSAGE];
	strcpy(tmpBuf, message);
	strcat(tmpBuf, RETURN_STRING);
	strcat(tmpBuf, "Exit or not?");
//	if (IDYES == MessageBox(AfxGetMainWnd() != NULL ? AfxGetMainWnd()->m_hWnd : NULL,
//							tmpBuf, location, MB_YESNO|MB_ICONERROR))
	if (CSimuQuestionDlg::ANSWER_YES == m_dlgQuestion->ShowQuestion(location, tmpBuf))
	{
		if (AfxGetMainWnd() != NULL)
			delete ((CSimuManager*)AfxGetMainWnd());
		AfxGetApp()->ExitInstance();
		exit(1);
	}
	SimuValue dumbStop = 0;
//	CSimuMsg::PauseMessage(message);
}

void CSimuMsg::ExitWithMessage(char* location, char* format, int id1)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, id1);
	ExitWithMessage(location, tmpBuf);
}
 
void CSimuMsg::ExitWithMessage(char* location, char* format, int id1, int id2)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, id1, id2);
	ExitWithMessage(location, tmpBuf);
}

void CSimuMsg::ExitWithMessage(char* location, char* format, char* str1)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1);
	ExitWithMessage(location, tmpBuf);
}

void CSimuMsg::ExitWithMessage(char* location, char* format, char* str1, char* str2)
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, format, str1, str2);
	ExitWithMessage(location, tmpBuf);
}

/*
bool CSimuMsg::PushErrorInfo(const char* format,
							 const char* str1,
							 const char* str2,
							 const char* str3,
							 const char* str4,
							 const char* str5,
							 const char* str6)
{
	sprintf(infoBuf, format, str1, str2, str3, str4, str5, str6);
	strErrorInfo.Insert(0, strErrorInfo.size() > 0 ? INFO_SEPARATOR : "");
	sprintf(tmpBuf, "<%d> %s", ++numErrorInfo, infoBuf);
	strErrorInfo.Insert(0, tmpBuf);

	bSimuInError = true;

	sprintf(infoBuf, "%s\n", tmpBuf);
	ExportSimulationInfo(infoBuf);
	return false;
}

void CSimuMsg::Message(const char* title,
					   const char* format,
					   const char* str1,
					   const char* str2,
					   const char* str3,
					   const char* str4,
					   const char* str5)
{
	sprintf(infoBuf, format, str1, str2, str3, str4, str5);
	MessageBox(AfxGetMainWnd() != NULL ? AfxGetMainWnd()->m_hWnd : NULL,
				infoBuf, title, MB_OK|MB_ICONINFORMATION);
}

void CSimuMsg::ExitWithMessage(const char* title,
							   const char* format,
							   const char* str1,
							   const char* str2,
							   const char* str3,
							   const char* str4,
							   const char* str5,
							   const char* str6)
{
	PushErrorInfo(format, str1, str2, str3, str4, str5, str6);
	if (AfxGetMainWnd() != NULL)// && AfxGetMainWnd()->IsWindowVisible())
	{
		AfxGetMainWnd()->KillTimer(SIMU_TIMER_ID);
		if (IDYES == MessageBox(AfxGetMainWnd() != NULL ? AfxGetMainWnd()->m_hWnd : NULL,
								strErrorInfo, title, MB_YESNO|MB_ICONERROR))
		{
			delete ((CSimuManager*)AfxGetMainWnd());
			exit(1);
//			PostQuitMessage(0);
		}
	}
	else
	{
		if (IDYES == MessageBox(NULL, strErrorInfo, title, MB_YESNO|MB_ICONERROR))
			exit(1);
	}
}
*/


#endif