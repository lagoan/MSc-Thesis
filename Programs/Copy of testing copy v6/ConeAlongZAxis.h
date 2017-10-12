// ConeAlongZAxis.h: interface for the CConeAlongZAxis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONEALONGZAXIS_H__82B09204_F9FD_4045_8553_807A143D0326__INCLUDED_)
#define AFX_CONEALONGZAXIS_H__82B09204_F9FD_4045_8553_807A143D0326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RotationToZAxis.h"

class CConeAlongZAxis : public CRotationToZAxis  
{
public:
	// cone is originally placed along z axis.
	// bottom center is at z=0, and top center is at z=length
	SimuValue m_coneLength;
	SimuValue m_coneTopRadius; // top radius is smaller than btm radius
	SimuValue m_coneBtmRadius;
	SimuValue m_coneUpperLength; // length of virtual upper cone 
	SimuValue m_coneSlopeRatio; // (top radious)/(virtual upper cone height)
	SimuValue m_coneHalfAngleOfTip;
	SimuValue m_coneCosHalfAngleOfTip;
	CVector3D m_coneTopCtrPos;
	CVector3D m_coneBtmCtrPos;
	CVector3D m_coneStandardTipPos;

	int m_coneSlices;
	int m_coneStacks;

	CConeAlongZAxis();
	CConeAlongZAxis(SimuValue coneLen, SimuValue coneTopRadius, SimuValue coneBtmRadius,
					CVector3D* btmCtrPos, CVector3D* direction);
	virtual ~CConeAlongZAxis();

	void InitializeCone();
	void DrawConeAlongZAxis();
	void ExportConeData(CString objName, FILE* fp);
	void ImportConeData(FILE* fp);

	void GetStandardPosition(CVector3D &transformedPos, CVector3D &standardPos);
	void GetTransformedPosition(CVector3D &standardPos, CVector3D &transformedPos);
	SimuValue GetRadius(SimuValue heightOnZ);
};

#endif // !defined(AFX_CONEALONGZAXIS_H__82B09204_F9FD_4045_8553_807A143D0326__INCLUDED_)
