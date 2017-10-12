// SimuPoint3D.h: interface for the CSimuPoint3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUPOINT3D_H__1E46B38C_F9E7_4A1A_B282_69C17CFFF2F0__INCLUDED_)
#define AFX_SIMUPOINT3D_H__1E46B38C_F9E7_4A1A_B282_69C17CFFF2F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimuPoint3D
{
public:
	SimuValue m_x;
	SimuValue m_y;
	SimuValue m_z;

	CSimuPoint3D();
	virtual ~CSimuPoint3D();

};

#endif // !defined(AFX_SIMUPOINT3D_H__1E46B38C_F9E7_4A1A_B282_69C17CFFF2F0__INCLUDED_)
