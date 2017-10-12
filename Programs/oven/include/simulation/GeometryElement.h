// GeometryElement.h: interface for the CGeometryElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMETRYELEMENT_H__585CA02E_AEBD_4C89_9161_3427C230C452__INCLUDED_)
#define AFX_GEOMETRYELEMENT_H__585CA02E_AEBD_4C89_9161_3427C230C452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGeometryElement  
{
public:
	static int m_geMaxId;
	int m_geId; // for debug use only
	bool m_geSelected; // for element selection in interactive debug
	bool m_geHighlit; // for debug use only

	bool m_geDeleted;

	CGeometryElement();
	virtual ~CGeometryElement();

	int GetGeometryElementId();
	virtual void RenderGeometry();
	virtual void AssertGeometricConnectivity();
	virtual void GetElementPosition(CVector3D* pPos);
};

#endif // !defined(AFX_GEOMETRYELEMENT_H__585CA02E_AEBD_4C89_9161_3427C230C452__INCLUDED_)
