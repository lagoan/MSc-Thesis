// VirtualPrtl.h: interface for the CVirtualPrtl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALPRTL_H__4AC157BB_D9A9_40BA_882E_4420843898C2__INCLUDED_)
#define AFX_VIRTUALPRTL_H__4AC157BB_D9A9_40BA_882E_4420843898C2__INCLUDED_

#define _CRT_SECURE_NO_DEPRECATE 1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"
#include "Vector3D.h"

class CVirtualPrtl  : virtual public CGeometryElement
{
public:
	static int m_vpMaxId;

	CVector3D* m_vpPos;
	CVector3D* m_vpVel;

#ifdef	SIMU_PARA_ENABLE_PARTICLE_COLOR
	CVector3D m_vpColor;
#endif

	CVirtualPrtl();
	virtual ~CVirtualPrtl();

	virtual void RenderGeometry();
	virtual void RenderPrtlByOpenGL();
	void SetVirtualPrtlColor(CVector3D* color);
};

#endif // !defined(AFX_VIRTUALPRTL_H__4AC157BB_D9A9_40BA_882E_4420843898C2__INCLUDED_)
