// Spring.h: interface for the CSpring class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRING_H__CA4378E6_6380_4EAF_A490_6FA2CC28D5D4__INCLUDED_)
#define AFX_SPRING_H__CA4378E6_6380_4EAF_A490_6FA2CC28D5D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"

class CSpring : public CGeometryElement  
{
public:
	SimuValue m_csConst;
	SimuValue m_csOrgLen;

	CSpring();
	virtual ~CSpring();

};

#endif // !defined(AFX_SPRING_H__CA4378E6_6380_4EAF_A490_6FA2CC28D5D4__INCLUDED_)
