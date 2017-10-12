// GeometryElement.cpp: implementation of the CGeometryElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "GeometryElement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CGeometryElement::m_geMaxId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeometryElement::CGeometryElement()
{
	m_geId = CGeometryElement::m_geMaxId++;
	m_geSelected = false;
	m_geHighlit = false;

	m_geDeleted = false;
}

CGeometryElement::~CGeometryElement()
{

}

int CGeometryElement::GetGeometryElementId()
{
	return m_geId;
}

void CGeometryElement::RenderGeometry()
{
}

void CGeometryElement::AssertGeometricConnectivity()
{
}

void CGeometryElement::GetElementPosition(CVector3D* pPos)
{
}
