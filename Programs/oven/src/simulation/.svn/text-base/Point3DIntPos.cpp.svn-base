// Point3DIntPos.cpp: implementation of the CPoint3DIntPos class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Point3DIntPos.h"



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
