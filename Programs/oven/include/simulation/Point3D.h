// Point3D.h: interface for the CPoint3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT3D_H__592883AA_9DD5_4D14_9B46_3EF0BAFE9A40__INCLUDED_)
#define AFX_POINT3D_H__592883AA_9DD5_4D14_9B46_3EF0BAFE9A40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"
#include "Vector3D.h"
#include "FluidPrtl.h"

class CPoint3D : public CGeometryElement  
{
public:
	static int m_p3dMaxId;

	CVector3D m_p3dPos;

#ifdef	SIMU_PARA_ENABLE_POINT_3D_COLOR
	CVector3D m_p3dColor;
#endif

	CPoint3D();
	virtual ~CPoint3D();

	virtual void RenderGeometry();
	void SetPoint3DColor(CVector3D* color);
};

#endif // !defined(AFX_POINT3D_H__592883AA_9DD5_4D14_9B46_3EF0BAFE9A40__INCLUDED_)
