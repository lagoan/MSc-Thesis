// DataExImport.cpp: implementation of the CDataExImport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DataExImport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString CDataExImport::m_deiFrameNumberFormat = "_%04d"; // 4 digits could accomodate 10000 frames.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataExImport::CDataExImport()
{
	m_deiFrameNumberFormat = "_%04d"; // 4 digits could accomodate 10000 frames.

	m_deiImported = false;
	m_deiName = "Data_Ex_Import";
}

CDataExImport::~CDataExImport()
{

}

void CDataExImport::ExportData(CString exportDir, UINT nCurFrame)
{
}

void CDataExImport::ImportData(CString importDir, UINT nCurFrame)
{
}

CString CDataExImport::MakePOVRaySceneFileName(CString directory, UINT nCurFrame)
{
	char	strFrameNum[10];
	sprintf(strFrameNum, m_deiFrameNumberFormat, nCurFrame);

	CString fileName = directory;
	fileName += "\\";
	fileName += "pov_ray_frame";
	fileName += strFrameNum;
	fileName += ".pov";
	return fileName;
}

CString CDataExImport::MakePOVRayDataFileName(CString directory, UINT nCurFrame,
											  CString name)
{
	char	strFrameNum[10];
	sprintf(strFrameNum, m_deiFrameNumberFormat, nCurFrame);

	CString fileName = directory;
	fileName += "\\";
	fileName += "pov_ray";
	fileName += "_";
	fileName += name;
	fileName += strFrameNum;
	fileName += ".pov";
	return fileName;
}

CString CDataExImport::MakePOVRayDataFileName(CString directory, UINT nCurFrame)
{
	return MakePOVRayDataFileName(directory, nCurFrame, m_deiName);
}

CString CDataExImport::MakeSurfaceMeshDataFileName(UINT nCurFrame)
{
	char	strFrameNum[10];
	sprintf(strFrameNum, m_deiFrameNumberFormat, nCurFrame);

	CString fileName = "surface_mesh";
	fileName += "_";
	fileName += m_deiName;
	fileName += strFrameNum;
	fileName += ".inc";
	return fileName;
}

CString CDataExImport::MakeSurfaceMeshTextureDataFileName(UINT nCurFrame)
{
	char	strFrameNum[10];
	sprintf(strFrameNum, m_deiFrameNumberFormat, nCurFrame);

	CString fileName = "surface_mesh_texture";
	fileName += "_";
	fileName += m_deiName;
	fileName += strFrameNum;
	fileName += ".inc";
	return fileName;
}

CString CDataExImport::MakePrtlDataFileName(UINT nCurFrame)
{
	char	strFrameNum[10];
	sprintf(strFrameNum, m_deiFrameNumberFormat, nCurFrame);

	CString fileName;
	if (CSimuManager::m_enumImExportFormat == CSimuManager::AFTER_2005_JUNE_29_5PM)
		fileName = "prtl";
	else
		fileName = "prtl_positions";
	fileName += "_";
	fileName += m_deiName;
	fileName += strFrameNum;
	fileName += ".txt";
	return fileName;
}

CString CDataExImport::MakeCompletePrtlDataFileName(CString directory, UINT nCurFrame)
{
	CString fileName = directory;
	fileName += "\\";
	fileName += MakePrtlDataFileName(nCurFrame);
	return fileName;
}

CString CDataExImport::MakeCompleteSimuInfoFileName(CString directory)
{
	if (CSimuManager::m_enumImExportFormat == CSimuManager::AFTER_2005_JUNE_29_5PM)
	{
		CString fileName = directory;
		fileName += "\\";
		fileName += APP_OUTPUT_INFO_FILE_NAME;
		fileName += "_";
		fileName += m_deiName;
		fileName += ".txt";
		return fileName;
	}
	else
		return CSimuManager::m_strSimuInfoFile;
}
