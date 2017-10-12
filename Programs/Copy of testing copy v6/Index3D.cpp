// Index3D.cpp: implementation of the CIndex3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Index3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndex3D::CIndex3D()
{
	m_iIds[X] = -1;
	m_iIds[Y] = -1;
	m_iIds[Z] = -1;
}

CIndex3D::~CIndex3D()
{

}

void CIndex3D::SetIndex3D(int idX, int idY, int idZ)
{
	m_iIds[X] = idX;
	m_iIds[Y] = idY;
	m_iIds[Z] = idZ;
}
