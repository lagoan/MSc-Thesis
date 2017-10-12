// DlgFileMoveCopy.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DlgFileMoveCopy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFileMoveCopy dialog


CDlgFileMoveCopy::CDlgFileMoveCopy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileMoveCopy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFileMoveCopy)
	m_strDestDir = _T("");
	m_strDestFileExt = _T("");
	m_strDestFileName = _T("");
	m_nDestIdIntvl = 0;
	m_nDestStartId = 0;
	m_nNumSrcFiles = 0;
	m_strSrcDir = _T("");
	m_strSrcFileExt = _T("");
	m_strSrcFileName = _T("");
	m_nSrcIdIntvl = 0;
	m_nSrcStartId = 0;
	m_bMoveFile = FALSE;
	m_bOverWriteFile = FALSE;
	m_bSubDirForDest = FALSE;
	m_nNumFinishedFiles = 0;
	m_bKeepNameNId = FALSE;
	//}}AFX_DATA_INIT

	m_pSimuManager = (CSimuManager*)pParent;
	m_strParaFileName = "file_move_copy.txt";

	m_strDestDir = CSimuManager::m_strAppDataDir;
	m_strDestFileExt = _T("");
	m_strDestFileName = _T("");
	m_nDestIdIntvl = 1;
	m_nDestStartId = 1;
	m_nNumSrcFiles = 0;
	m_strSrcDir = CSimuManager::m_strAppDataDir;
	m_strSrcFileExt = _T("");
	m_strSrcFileName = _T("");
	m_nSrcIdIntvl = 1;
	m_nSrcStartId = 1;
	m_bMoveFile = FALSE;
	m_bOverWriteFile = FALSE;
	m_bSubDirForDest = true;
	m_bKeepNameNId = false;
}


void CDlgFileMoveCopy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFileMoveCopy)
	DDX_Text(pDX, IDC_DEST_DIR, m_strDestDir);
	DDX_Text(pDX, IDC_DEST_FILE_EXT, m_strDestFileExt);
	DDX_Text(pDX, IDC_DEST_FILE_NAME, m_strDestFileName);
	DDX_Text(pDX, IDC_DEST_ID_INTERVAL, m_nDestIdIntvl);
	DDX_Text(pDX, IDC_DEST_START_ID, m_nDestStartId);
	DDX_Text(pDX, IDC_NUM_SRC_FILES, m_nNumSrcFiles);
	DDX_Text(pDX, IDC_SRC_DIR, m_strSrcDir);
	DDX_Text(pDX, IDC_SRC_FILE_EXT, m_strSrcFileExt);
	DDX_Text(pDX, IDC_SRC_FILE_NAME, m_strSrcFileName);
	DDX_Text(pDX, IDC_SRC_ID_INTERVAL, m_nSrcIdIntvl);
	DDX_Text(pDX, IDC_SRC_START_ID, m_nSrcStartId);
	DDX_Check(pDX, IDC_CHK_MOVE_FILE, m_bMoveFile);
	DDX_Check(pDX, IDC_CHK_OVERWRITE_FILE, m_bOverWriteFile);
	DDX_Check(pDX, IDC_CHK_SUB_DIR_FOR_DEST, m_bSubDirForDest);
	DDX_Text(pDX, IDC_NUM_FINISHED_FILES, m_nNumFinishedFiles);
	DDX_Check(pDX, IDC_CHK_KEEP_NAME_N_ID, m_bKeepNameNId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFileMoveCopy, CDialog)
	//{{AFX_MSG_MAP(CDlgFileMoveCopy)
	ON_BN_CLICKED(IDC_BTN_BROWSE_SRC_DIR, OnBtnBrowseSrcDir)
	ON_BN_CLICKED(IDC_BTN_BROWSE_DEST_DIR, OnBtnBrowseDestDir)
	ON_BN_CLICKED(ID_BTN_PRE_CHECK, OnBtnPreCheck)
	ON_BN_CLICKED(ID_BTN_DO_IT, OnBtnDoIt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CString CDlgFileMoveCopy::MakeSrcFileName(int curId)
{
	char strId[10];
	sprintf(strId, CDataExImport::m_deiFrameNumberFormat, curId);
	CString completeFileName = m_strSrcDir;
	completeFileName += "\\";
	completeFileName += m_strSrcFileName;
	completeFileName += strId;
	completeFileName += ".";
	completeFileName += m_strSrcFileExt;
	return completeFileName;
}

CString CDlgFileMoveCopy::MakeDestFileName(int curId)
{
	char strId[10];
	sprintf(strId, CDataExImport::m_deiFrameNumberFormat, curId);
	CString completeFileName = m_strDestDir;
	completeFileName += "\\";
	completeFileName += m_strDestFileName;
	completeFileName += strId;
	completeFileName += ".";
	completeFileName += m_strDestFileExt;
	return completeFileName;
}

void CDlgFileMoveCopy::ExportParameters()
{
	CTime curTime	= CTime::GetCurrentTime();
	CString timeStr	= curTime.Format("%b_%d_%H_%M_%S");
	CString fileName = m_strDestDir;
	fileName += "\\";
	fileName += timeStr;
	fileName += "_";
	fileName += m_strParaFileName;

	FILE* fp;
	if ((fp=fopen(fileName, "w")) == NULL)
	{
		sprintf(CSimuMsg::GetEmptyBuf(), "Can't open file %s to write.", fileName);
		CSimuMsg::ExitWithMessage("CDlgFileMoveCopy::ExportParameters()",
									CSimuMsg::GetCheckedBuf());
		return;
	}
	fprintf(fp, "m_strSrcDir=%s\n", m_strSrcDir);
	fprintf(fp, "m_strSrcFileName=%s\n", m_strSrcFileName);
	fprintf(fp, "m_strSrcFileExt=%s\n", m_strSrcFileExt);
	fprintf(fp, "m_nSrcStartId=%d\n", m_nSrcStartId);
	fprintf(fp, "m_nSrcIdIntvl=%d\n", m_nSrcIdIntvl);

	fprintf(fp, "m_bSubDirForDest=%d\n", m_bSubDirForDest);
	fprintf(fp, "m_bKeepNameNId=%d\n", m_bKeepNameNId);

	fprintf(fp, "m_strDestDir=%s\n", m_strDestDir);
	fprintf(fp, "m_strDestFileName=%s\n", m_strDestFileName);
	fprintf(fp, "m_strDestFileExt=%s\n", m_strDestFileExt);
	fprintf(fp, "m_nDestStartId=%d\n", m_nDestStartId);
	fprintf(fp, "m_nDestIdIntvl=%d\n", m_nDestIdIntvl);

	fprintf(fp, "m_bMoveFile=%d\n", m_bMoveFile);
	fprintf(fp, "m_bOverWriteFile=%d\n", m_bOverWriteFile);

	fclose(fp);
}

void CDlgFileMoveCopy::ImportParameters()
{
}

/////////////////////////////////////////////////////////////////////////////
// CDlgFileMoveCopy message handlers

void CDlgFileMoveCopy::OnBtnBrowseSrcDir() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();

	CFileDialog* fileDialog = new CFileDialog(true);
	fileDialog->m_ofn.lpstrInitialDir = m_strSrcDir;
	if (IDOK == fileDialog->DoModal())
	{
		m_strSrcDir = fileDialog->GetPathName();
		m_strSrcDir.Replace(fileDialog->GetFileName(), "");
		m_strSrcDir.TrimRight("\\");
		m_strSrcFileName = fileDialog->GetFileTitle();
		m_strSrcFileExt = fileDialog->GetFileExt();
		CString index = m_strSrcFileName.Right(5);
		m_strSrcFileName.Replace(index, "");

		m_strDestDir = m_strSrcDir;
		m_strDestFileName = m_strSrcFileName;
		m_strDestFileExt = m_strSrcFileExt;
		UpdateData(false);
	}
	delete fileDialog;
}

void CDlgFileMoveCopy::OnBtnBrowseDestDir() 
{
	// TODO: Add your control notification handler code here
	
	char dirName[LEN_DIR_NAME];
	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = GetSafeHwnd();
	browseInfo.pidlRoot = NULL;
	browseInfo.pszDisplayName = dirName;
	browseInfo.lpszTitle = "Get destination directory";
	browseInfo.ulFlags = BIF_BROWSEINCLUDEFILES;
	browseInfo.lpfn = NULL;
	SHBrowseForFolder(&browseInfo);
	m_strDestDir = dirName;

	UpdateData(false);
}

void CDlgFileMoveCopy::OnBtnPreCheck() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();

	bool bFileExist = false;
	int curId = m_nSrcStartId;
	int numFiles = 0;
	do
	{
		CString completeFileName = MakeSrcFileName(curId);
		if (CSimuUtility::FileDoesExist(completeFileName))
		{
			numFiles ++;
			curId += m_nSrcIdIntvl;
			bFileExist = true;
		}
		else
			bFileExist = false;
	} while (bFileExist);
	m_nNumSrcFiles = numFiles;

	UpdateData(false);
}

void CDlgFileMoveCopy::OnBtnDoIt() 
{
	// TODO: Add your control notification handler code here

	char location[] = "CDlgFileMoveCopy::OnBtnDoIt()";

	UpdateData();
	m_nNumSrcFiles = 0;

	if (m_bSubDirForDest)
	{
		m_strDestDir = m_strSrcDir;
		CTime curTime	= CTime::GetCurrentTime();
		CString timeStr	= curTime.Format("%b_%d_%H_%M_%S");
		m_strDestDir += "\\";
		m_strDestDir += timeStr; 
	}
	if (_chdir(m_strDestDir) == -1)
	{
		if (_mkdir(m_strDestDir) == -1)
			CSimuMsg::ExitWithMessage(location, "Failed command: %s.", CSimuMsg::GetCheckedBuf());
	}
	if (m_bKeepNameNId)
	{
		m_strDestFileName = m_strSrcFileName;
		m_strDestFileExt = m_strSrcFileExt;
		m_nDestIdIntvl = m_nSrcIdIntvl;
		m_nDestStartId = m_nSrcStartId;
	}
	ExportParameters();

	m_nNumFinishedFiles = 0;
	int curSrcId = m_nSrcStartId;
	int curDestId = m_nDestStartId;
	CString commandStr;
	CString srcFile, destFile;
	do
	{
		srcFile = MakeSrcFileName(curSrcId);
		if (!CSimuUtility::FileDoesExist(srcFile))
			break;
		else
		{
			destFile = MakeDestFileName(curDestId);
			if (m_bMoveFile)
			{
				if (m_bOverWriteFile)
					MoveFileEx(srcFile, destFile, MOVEFILE_REPLACE_EXISTING);
				else
					MoveFile(srcFile, destFile);
			}
			else
			{
//				CopyFile(srcFile, destFile, !m_bOverWriteFile);

				// this code is for replacing one string in files
				char oldStr[] = "prtl_cube";
				char newStr[] = "prtl_My_Cube";
				FILE* oldfp;
				FILE* newfp;
				if ((oldfp=fopen(srcFile, "r")) == NULL)
				{
					CSimuMsg::ExitWithMessage(location, "Can not open file to read.");
				}
				if ((newfp=fopen(destFile, "w")) == NULL)
				{
					CSimuMsg::ExitWithMessage(location, "Can not open file to write.");
				}
				int maxReplaces = 1;
				int numReplaces = 0;
				char buf[LEN_DATA_LINE];
				while(!feof(oldfp)) // Read the file until the end.
				{
					strcpy(buf, ""); // clear buffer, otherwise, last line is doubled.
					fgets(buf, sizeof(buf), oldfp);
					if (ferror(oldfp))
					{
						fclose(oldfp);
						fclose(newfp);
						CSimuMsg::ExitWithMessage(location, "Error in reading global parameters.");
						return;
					}
					if (numReplaces < maxReplaces)
					{
						CString tmpStr = buf;
						if (tmpStr.Replace(oldStr, newStr) > 0)
							numReplaces++;
						strcpy(buf, tmpStr);
					}
					// else: not to replace
					fputs(buf, newfp);
				}
				fclose(oldfp);
				fclose(newfp);

			}

			curSrcId += m_nSrcIdIntvl;
			curDestId += m_nDestIdIntvl;

			m_nNumFinishedFiles ++;
			UpdateData(false);
		}
	} while (1);
	sprintf(CSimuMsg::GetEmptyBuf(), "Files have been move/copy to %s.", m_strDestDir);
	CSimuMsg::PauseMessage(CSimuMsg::GetCheckedBuf());
	m_nNumFinishedFiles = 0;
	UpdateData(false);
}
