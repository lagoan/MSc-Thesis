// TGeomElemArray.h: interface for the TGeomElemArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGEOMELEMARRAY_H__67E831EB_8C10_4487_AFBF_6CD46DF5E355__INCLUDED_)
#define AFX_TGEOMELEMARRAY_H__67E831EB_8C10_4487_AFBF_6CD46DF5E355__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TClassArray.h"

template <class CGeometryElement> class TGeomElemArray : public TClassArray<CGeometryElement>
{
public:
	CVector3D m_geaColor;

	TGeomElemArray();
	TGeomElemArray(bool bMasterArray, int extendSize);
	virtual ~TGeomElemArray();

	void CleanDeletedElements();
	void RenderGeometryElements();
	void AssertGeometricElementConnectivity();
};

//////////////////////////////////////////////////////////////////////
// Template Implementation
//////////////////////////////////////////////////////////////////////

template <class CGeometryElement> 
TGeomElemArray<CGeometryElement>::TGeomElemArray()
{
	m_geaColor.SetValue(CSimuManager::COLOR_WHITE);
}

template <class CGeometryElement> 
TGeomElemArray<CGeometryElement>::TGeomElemArray(bool bMasterArray, int extendSize)
{
	InitializeClassArray(bMasterArray, extendSize);
	m_geaColor.SetValue(CSimuManager::COLOR_WHITE);
}

template <class CGeometryElement> 
TGeomElemArray<CGeometryElement>::~TGeomElemArray()
{
}

template <class CGeometryElement> 
void TGeomElemArray<CGeometryElement>::CleanDeletedElements()
{
	int numNotDeleted = 0;
	for (int i=0; i < m_paNum; i++)
	{
		CGeometryElement* pElmnt = m_paPtrs[i];
		if (pElmnt->m_geDeleted)
		{
			if (m_caIsMasterArray)
				delete pElmnt;
		}
		else
		{
			if (numNotDeleted < i)
			{
				m_paPtrs[numNotDeleted] = pElmnt;
			}
			numNotDeleted++;
		}
	}
	m_paNum = numNotDeleted;
}

template <class CGeometryElement> 
void TGeomElemArray<CGeometryElement>::RenderGeometryElements()
{
	glPushAttrib(GL_CURRENT_BIT);
	SimuColor3v(m_geaColor.v);

	for (int i=0; i < m_paNum; i++)
	{
		CGeometryElement* pElmnt = m_paPtrs[i];
		pElmnt->RenderGeometry();
	}

	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

template <class CGeometryElement> 
void TGeomElemArray<CGeometryElement>::AssertGeometricElementConnectivity()
{
	char location[] = "TGeomElemArray<CGeometryElement>::AssertGeometricElementConnectivity()";

	glPushAttrib(GL_CURRENT_BIT);
	for (int i=0; i < m_paNum; i++)
	{
		CGeometryElement* pElmnt = m_paPtrs[i];
		pElmnt->AssertGeometricConnectivity();
	}
	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

#endif // !defined(AFX_TGEOMELEMARRAY_H__67E831EB_8C10_4487_AFBF_6CD46DF5E355__INCLUDED_)
