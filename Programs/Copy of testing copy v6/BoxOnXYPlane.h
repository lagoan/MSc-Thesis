// BoxOnXYPlane.h: interface for the CBoxOnXYPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOXONXYPLANE_H__CEE2E0A1_5B4A_4E77_95CD_72F96F1EAEC3__INCLUDED_)
#define AFX_BOXONXYPLANE_H__CEE2E0A1_5B4A_4E77_95CD_72F96F1EAEC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RotationToZAxis.h"

class CBoxOnXYPlane	:	public CRotationToZAxis
{
public:
	SimuValue m_boxHeightOnZ;
	SimuValue m_boxWidthOnX;
	SimuValue m_boxWidthOnY;
	CVector3D m_boxTopCtrPos;
	CVector3D m_boxBtmCtrPos;

	CBoxOnXYPlane();
	CBoxOnXYPlane(SimuValue heightOnZ, SimuValue widthOnX, SimuValue widthOnY,
				  SimuValue angleAboutZ, CVector3D* topCtrPos, CVector3D* direction);
	virtual ~CBoxOnXYPlane();

	void DrawBoxOnXYPlane();
	void ExportBoxData(CString objName, FILE* fp);
	void ImportBoxData(FILE* fp);
	void ComputeBoxVariables();

	void GetStandardPosition(CVector3D &transformedPos, CVector3D &standardPos);
	void GetTransformedPosition(CVector3D &standardPos, CVector3D &transformedPos);
	SimuValue GetDistanceToBox(CVector3D &standardPos);

	static void CreatePrtlsInBox(CPrtlFluid* pPrtlFluid,
								 CVector3D* btmCtrPos, CVector3D* direction,
								 SimuValue angleAboutZ, SimuValue angularVel,
								 int layers[Z+1], SimuValue layerDistance);
	static void CreatePrtlsInBoxWithVelocity(CPrtlFluid* pPrtlFluid, CVector3D* btmCtrPos, 
											CVector3D* direction, CVector3D* velocity,
											SimuValue angleAboutZ, SimuValue angularVel,
											int layers[Z+1], SimuValue layerDistance);
	static void CreateStretchingFluidInBox(CPrtlFluid* pPrtlFluid,
										   CVector3D* btmCtrPos, CVector3D* direction,
										   SimuValue angleAboutZ, int layers[Z+1],
										   SimuValue layerDistance, SimuValue stretchSpeed,
										   int stretchLayers);

protected:
	CVector3D m_boxMinPos;
	CVector3D m_boxMaxPos;
};

#endif // !defined(AFX_BOXONXYPLANE_H__CEE2E0A1_5B4A_4E77_95CD_72F96F1EAEC3__INCLUDED_)
