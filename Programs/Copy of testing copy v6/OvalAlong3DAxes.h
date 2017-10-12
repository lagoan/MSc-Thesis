// OvalAlong3DAxes.h: interface for the COvalAlong3DAxes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVALALONG3DAXES_H__E3D85B6A_A134_45FE_B37F_4296380E76D5__INCLUDED_)
#define AFX_OVALALONG3DAXES_H__E3D85B6A_A134_45FE_B37F_4296380E76D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RotationToZAxis.h"

class COvalAlong3DAxes : public CRotationToZAxis  
{
public:
	SimuValue m_ovalRadiusOnX;
	SimuValue m_ovalRadiusOnY;
	SimuValue m_ovalRadiusOnZ;
	SimuValue m_ovalRX2;
	SimuValue m_ovalRY2;
	SimuValue m_ovalRZ2;
	CVector3D m_ovalCtrPos;

	COvalAlong3DAxes();
	COvalAlong3DAxes(SimuValue radiusOnX, SimuValue radiusOnY, SimuValue radiusOnZ,
					 SimuValue angleAboutZ, CVector3D* ctrPos, CVector3D* direction);
	virtual ~COvalAlong3DAxes();

	void DrawOvalAlong3DAxes();
	void ExportOvalData(CString objName, FILE* fp);
	void ImportOvalData(FILE* fp);

	void GetStandardPosition(CVector3D &transformedPos, CVector3D &standardPos);
	void GetTransformedPosition(CVector3D &standardPos, CVector3D &transformedPos);

	static void CreateTriangularMeshOnOval(TGeomElemArray<CPrtlTriangle> &aryTris,
										   TGeomElemArray<CMeshPrtl> &aryPrtls,
										   CVector3D &ctrPos, CVector3D &direction,
										   SimuValue ovalRs[3]);
	static void CreateTrianglesOnOval(TGeomElemArray<CPrtlTriangle> &aryTris,
									  TGeomElemArray<CMeshPrtl> &aryPrtls,
									  int level, SimuValue ovalRs[3],
									  CVector3D &p0, CVector3D &p1, CVector3D &p2);
};

#endif // !defined(AFX_OVALALONG3DAXES_H__E3D85B6A_A134_45FE_B37F_4296380E76D5__INCLUDED_)
