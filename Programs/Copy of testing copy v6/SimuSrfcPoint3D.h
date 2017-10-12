// SimuSrfcPoint3D.h: interface for the CSimuSrfcPoint3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUSRFCPOINT3D_H__DB800732_80F9_46CD_AEF3_CEA5AC0F2811__INCLUDED_)
#define AFX_SIMUSRFCPOINT3D_H__DB800732_80F9_46CD_AEF3_CEA5AC0F2811__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuPoint3D.h"

class CSimuSrfcPoint3D : public CSimuPoint3D
{
public:
	SimuValue m_nx;
	SimuValue m_ny;
	SimuValue m_nz;

	CSimuSrfcPoint3D();
	virtual ~CSimuSrfcPoint3D();

};

#endif // !defined(AFX_SIMUSRFCPOINT3D_H__DB800732_80F9_46CD_AEF3_CEA5AC0F2811__INCLUDED_)
