// TGeomElemArray.h: interface for the TGeomElemArray class.
//
//////////////////////////////////////////////////////////////////////

#ifndef TGEOMELEMARRAY_H
#define TGEOMELEMARRAY_H

#include <QGLWidget>
#include "Vector3D.h"

#include "TClassArray.h"
//#include "SimuManager.h"

template <class CGeometryElement> class TGeomElemArray : public TClassArray<CGeometryElement>
{
public:
	//bool this->m_caIsMasterArray;
	//int this->m_paNum;
	//int this->m_paMax;
	//CGeometryElement** this->m_paPtrs;
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
	//m_geaColor.SetValue(CSimuManager::COLOR_WHITE);
}

template <class CGeometryElement> 
TGeomElemArray<CGeometryElement>::TGeomElemArray(bool bMasterArray, int extendSize)
{
	InitializeClassArray(bMasterArray, extendSize);
	//m_geaColor.SetValue(CSimuManager::COLOR_WHITE);
}

template <class CGeometryElement> 
TGeomElemArray<CGeometryElement>::~TGeomElemArray()
{
}

template <class CGeometryElement> 
void TGeomElemArray<CGeometryElement>::CleanDeletedElements()
{
	int numNotDeleted = 0;
	for (int i=0; i < this->m_paNum; i++)
	{
		CGeometryElement* pElmnt = this->m_paPtrs[i];
		if (pElmnt->m_geDeleted)
		{
			if (this->m_caIsMasterArray)
				delete pElmnt;
		}
		else
		{
			if (numNotDeleted < i)
			{
				this->m_paPtrs[numNotDeleted] = pElmnt;
			}
			numNotDeleted++;
		}
	}
	this->m_paNum = numNotDeleted;
}

template <class CGeometryElement> 
void TGeomElemArray<CGeometryElement>::RenderGeometryElements()
{
	glPushAttrib(GL_CURRENT_BIT);
	SimuColor3v(m_geaColor.v);

	for (int i=0; i < this->m_paNum; i++)
	{
		CGeometryElement* pElmnt = this->m_paPtrs[i];
		pElmnt->RenderGeometry();
	}

	glPopAttrib();
	//CSimuUtility::RestoreOpenGLPreviousColor();
}

template <class CGeometryElement> 
void TGeomElemArray<CGeometryElement>::AssertGeometricElementConnectivity()
{
	char location[] = "TGeomElemArray<CGeometryElement>::AssertGeometricElementConnectivity()";

	glPushAttrib(GL_CURRENT_BIT);
	for (int i=0; i < this->m_paNum; i++)
	{
		CGeometryElement* pElmnt = this->m_paPtrs[i];
		pElmnt->AssertGeometricConnectivity();
	}
	glPopAttrib();
	//CSimuUtility::RestoreOpenGLPreviousColor();
}

#endif // !defined(AFX_TGEOMELEMARRAY_H__67E831EB_8C10_4487_AFBF_6CD46DF5E355__INCLUDED_)
