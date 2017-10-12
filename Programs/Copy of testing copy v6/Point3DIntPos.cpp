// Point3DIntPos.cpp: implementation of the CPoint3DIntPos class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Point3DIntPos.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoint3DIntPos::CPoint3DIntPos()
{

}

CPoint3DIntPos::~CPoint3DIntPos()
{

}

void CPoint3DIntPos::TruncateToIntPos(int minExp10)
{
	for (int d=X; d <= Z; d++)
	{
		m_p3dipPos[d] = floor(m_p3dPos[d]*pow(10.0f, -minExp10));
	}
}
