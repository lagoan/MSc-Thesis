// SimuMsg.h: interface for the CSimuMsg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SIMUMSG_H
#define SIMUMSG_H



#include <QString>
#include "SimuConstant.h"

class CSimuMsg
{
public:

	static char* GetEmptyBuf();
	static char* GetCheckedBuf(bool bCheck=true);

	static void ResetMessageData();
	//static QString GetSimuInfo();

	//static QString PopLastMessage();
	//static void PushMessage(const char* message);
	static void DisplayMessage(const char* msg, bool bDisabled=true);
	static void EraseLastMessage(bool bDisabled=true);
	//static void CleanOldMessages(int maxMsgLen, int numOfMsgsToKeep);

	static bool PushErrorInfo(char* message);
	static bool PushErrorInfo(char* format, char* str1);
	static bool PushErrorInfo(char* format, char* str1, char* str2);
	static bool PushErrorInfo(char* format, char* str1, char* str2, char* str3);
	static bool PushErrorInfo(char* format, char* str1, char* str2, char* str3, char* str4);

	static void Message(char* location, char* message);
	static void PauseMessage(char* message);
	static void CancelablePauseMessage(char* message, bool &bPause);

	static void ExitWithMessage(char* location, char* message, bool bSkip=false);
	//static void ExitWithMessage(char* location, char* format, int id1);
	//static void ExitWithMessage(char* location, char* format, int id1, int id2);
	//static void ExitWithMessage(char* location, char* format, char* str1);
	//static void ExitWithMessage(char* location, char* format, char* str1, char* str2);

private:
	static int	m_smNumSimuInfo;
	static QString	m_smStrSimuInfo;
	static QString	m_smStrSeparator;
	static QString	m_smOverflowMsgStr;
	static char	m_smMsgBuf[LEN_MESSAGE];	// indirectly used by other classes

};

#endif // !defined(AFX_SIMUMSG_H__552E6CE2_CB91_11D5_9B85_0050BADE013B__INCLUDED_)
