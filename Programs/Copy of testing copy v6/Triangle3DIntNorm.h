// Triangle3DIntNorm.h: interface for the CTriangle3DIntNorm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIANGLE3DINTNORM_H__B1646DC3_A871_4980_B474_77C6B1C4C8D2__INCLUDED_)
#define AFX_TRIANGLE3DINTNORM_H__B1646DC3_A871_4980_B474_77C6B1C4C8D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Triangle3D.h"

class CTriangle3DIntNorm : public CTriangle3D  
{
public:
	CVector3D m_t3dinNormal;
	SimuValue m_t3dinPlaneD;

	CTriangle3DIntNorm();
	virtual ~CTriangle3DIntNorm();

	virtual void RenderGeometry();
};

#endif // !defined(AFX_TRIANGLE3DINTNORM_H__B1646DC3_A871_4980_B474_77C6B1C4C8D2__INCLUDED_)
