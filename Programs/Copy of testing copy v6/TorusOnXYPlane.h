// TorusOnXYPlane.h: interface for the CTorusOnXYPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TORUSONXYPLANE_H__84A1A862_6B84_44D1_AE54_5CD9A3E43384__INCLUDED_)
#define AFX_TORUSONXYPLANE_H__84A1A862_6B84_44D1_AE54_5CD9A3E43384__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RotationToZAxis.h"

class CTorusOnXYPlane : public CRotationToZAxis  
{
public:
	SimuValue m_trsInnerRadius;
	SimuValue m_trsOuterRadius;
	CVector3D m_trsCtrPos;

	CTorusOnXYPlane();
	CTorusOnXYPlane(SimuValue innerRadius, SimuValue outerRadius,
					CVector3D* ctrPos, CVector3D* direction);
	virtual ~CTorusOnXYPlane();

	void DrawTorusOnXYPlane();
	void ComputeTorusVariables();
	SimuValue GetDistanceToTorus(CVector3D &Pos);

protected:
	SimuValue m_trsMiddleRadius;
	SimuValue m_trsNeckRadius;
};

#endif // !defined(AFX_TORUSONXYPLANE_H__84A1A862_6B84_44D1_AE54_5CD9A3E43384__INCLUDED_)
