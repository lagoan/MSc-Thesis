// CylinderAlongZAxis.h: interface for the CCylinderAlongZAxis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CYLINDERALONGZAXIS_H__7D463957_CA21_4BCB_94C5_8E79DB73988C__INCLUDED_)
#define AFX_CYLINDERALONGZAXIS_H__7D463957_CA21_4BCB_94C5_8E79DB73988C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RotationToZAxis.h"
#include "TGeomElemArray.h"
#include "FluidPrtl.h"

class CCylinderAlongZAxis : public CRotationToZAxis  
{
public:
	time_t		m_cazNewImportFormatTime;

	// cylinder is originally placed along z axis.
	// bottom center is at z=0, and top center is at z=length
	SimuValue m_cylLength;
	SimuValue m_cylRadius;
	SimuValue m_cylCapHeight;
	CVector3D m_cylTopCtrPos;
	CVector3D m_cylBtmCtrPos;
	bool m_cylTopCap;
	bool m_cylBtmCap;

	int m_cylSlices;
	int m_cylStacks;

	bool m_cylFixedPrtls;
	bool m_cylStickyTop;
	bool m_cylStickyBtm;
	bool m_cylStickyWall;

	CCylinderAlongZAxis();
	CCylinderAlongZAxis(SimuValue length, SimuValue radius, 
						SimuValue capHeight, bool bCylTopCap, bool bCylBtmCap,
						CVector3D* topCtrPos, CVector3D* direction);
	virtual ~CCylinderAlongZAxis();

	void InitializeCylinder();
	void DrawCylinderAlongZAxis();
	void ExportCylinderData(CString objName, FILE* fp);
	void ImportCylinderData(FILE* fp);

	void GetStandardPosition(CVector3D &transformedPos, CVector3D &standardPos);
	void GetTransformedPosition(CVector3D &standardPos, CVector3D &transformedPos);
	SimuValue GetDistanceToCylinder(CVector3D &standardPos);
	void TransferElementsToExpectedPosition(TGeomElemArray<CPrtlTriangle> &aryTris,
											TGeomElemArray<CMeshPrtl> &aryPrtls);

	void CreateFluidPrtlsRandomlyInCylinder(CPrtlFluid* pPrtlFluid, int maxNumPrtls);
	void CreateFluidPrtlsInCylinderWithVelocity(CPrtlFluid* pPrtlFluid, CVector3D *velocity);
	void CreateFluidPrtlsInCylinder(CPrtlFluid* pPrtlFluid);
	void CreateFluidPrtlsInHollowCylinder(CPrtlFluid* pPrtlFluid, SimuValue hollowRadius);
	void CreateFluidPrtlsInCylinderWithFixedTop(CPrtlFluid* pPrtlFluid,
												SimuValue fixedPartWidth,
												SimuValue btmCapHeight);
	void CreateFluidPrtlsInCap(CPrtlFluid* pPrtlFluid, CVector3D* btmCtrPos,
								SimuValue capHeight, bool bCapUp);
	void CreateStretchingFluidInCylinder(CPrtlFluid* pPrtlFluid,
										 CVector3D *velocity,
										 SimuValue hollowRadius,
										 SimuValue stretchSpeed,
										 SimuValue stretchPartWidth,
										 bool bTopStretch, bool bBtmStretch,
										 bool bTopCovered, bool bBtmCovered);

	void CreateTriangularMeshOnCylinder(TGeomElemArray<CPrtlTriangle> &aryTris,
										TGeomElemArray<CMeshPrtl> &aryPrtls,
										SimuValue capHeight);
	void CreateTrianglesOnCapCylinder(TGeomElemArray<CPrtlTriangle> &aryTris,
									  TGeomElemArray<CMeshPrtl> &aryPrtls,
									  int level, bool bOnRim, SimuValue capRs[3],
									  CVector3D &p0, CVector3D &p1, CVector3D &p2);
};

#endif // !defined(AFX_CYLINDERALONGZAXIS_H__7D463957_CA21_4BCB_94C5_8E79DB73988C__INCLUDED_)
