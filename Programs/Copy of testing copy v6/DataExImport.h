// DataExImport.h: interface for the CDataExImport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAEXIMPORT_H__7A4E6D58_99F0_4932_A09E_BB95385CD814__INCLUDED_)
#define AFX_DATAEXIMPORT_H__7A4E6D58_99F0_4932_A09E_BB95385CD814__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataExImport  
{
public:
	static CString m_deiFrameNumberFormat;

	CString m_deiName;
	bool m_deiImported;

	CDataExImport();
	virtual ~CDataExImport();

	virtual void ExportData(CString exportDir, UINT nCurFrame);
	virtual void ImportData(CString importDir, UINT nCurFrame);

	static CString MakePOVRaySceneFileName(CString directory, UINT nCurFrame);
	static CString MakePOVRayDataFileName(CString directory, UINT nCurFrame, CString name);
	CString MakePOVRayDataFileName(CString directory, UINT nCurFrame);
	CString MakeSurfaceMeshDataFileName(UINT nCurFrame);
	CString MakeSurfaceMeshTextureDataFileName(UINT nCurFrame);
	CString MakePrtlDataFileName(UINT nCurFrame);
	CString MakeCompletePrtlDataFileName(CString directory, UINT nCurFrame);
	CString MakeCompleteSimuInfoFileName(CString directory);
};

#endif // !defined(AFX_DATAEXIMPORT_H__7A4E6D58_99F0_4932_A09E_BB95385CD814__INCLUDED_)
