// SimuMCTexture.h: interface for the CSimuMCTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUMCTEXTURE_H__BD3467B5_5738_4DD6_8DA8_3A04180B313D__INCLUDED_)
#define AFX_SIMUMCTEXTURE_H__BD3467B5_5738_4DD6_8DA8_3A04180B313D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuMarchingCube.h"

class CSimuMCTexture : public CSimuMarchingCube  
{
public:
	SimuValue* m_colorR;
	SimuValue* m_colorG;
	SimuValue* m_colorB;

	CSimuMCTexture();
	virtual ~CSimuMCTexture();

};

#endif // !defined(AFX_SIMUMCTEXTURE_H__BD3467B5_5738_4DD6_8DA8_3A04180B313D__INCLUDED_)
