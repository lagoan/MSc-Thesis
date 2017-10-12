// Point3DIntPos.h: interface for the CPoint3DIntPos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT3DINTPOS_H__A1291EDA_5EA4_4F9E_9309_6B3D81ECFF1B__INCLUDED_)
#define AFX_POINT3DINTPOS_H__A1291EDA_5EA4_4F9E_9309_6B3D81ECFF1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Point3D.h"

class CPoint3DIntPos : public CPoint3D  
{
public:
	CVector3D m_p3dipPos;

	CPoint3DIntPos();
	virtual ~CPoint3DIntPos();

	void TruncateToIntPos(int minExp10);
};

#endif // !defined(AFX_POINT3DINTPOS_H__A1291EDA_5EA4_4F9E_9309_6B3D81ECFF1B__INCLUDED_)
