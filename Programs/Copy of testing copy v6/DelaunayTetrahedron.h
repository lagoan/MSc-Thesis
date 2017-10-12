// DelaunayTetrahedron.h: interface for the CDelaunayTetrahedron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAUNAYTETRAHEDRON_H__00AC8115_00A6_4E06_AA91_BB9F8F99AB32__INCLUDED_)
#define AFX_DELAUNAYTETRAHEDRON_H__00AC8115_00A6_4E06_AA91_BB9F8F99AB32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TGeomElemArray.h"
#include "FluidPrtl.h"
#include "Point3DOnTetra.h"
#include "TetraDelaunay.h"
#include "TriangleOnTetra.h"
#include "GridDlnyTetra.h"

class CDelaunayTetrahedron  
{
public:
	CPoint3DOnTetra* m_dtrBoundingPs[4];
	TGeomElemArray<CPoint3DOnTetra> m_dtrPoints;
	TGeomElemArray<CTetraDelaunay> m_dtrTetrahedra;
	CGridDlnyTetra m_dtrTetraSearchGrid;
	bool m_dtrAbort;

	static SimuValue m_dtrMinPointDistSqr;
	static SimuValue m_dtrRadiusSqrTolerance;
	static SimuValue m_dtrMinTetraVolume;
	static bool m_dtrOnlyOnCtrPoint;
	static int m_dtrNumMissedPoints;

	CDelaunayTetrahedron();
	virtual ~CDelaunayTetrahedron();

	bool ConstructDelaunayTetrahedra(TGeomElemArray<CFluidPrtl> &aryPrtls);
	bool AddPointToDelaunayTessellation(CPoint3DOnTetra* pPoint);
	bool AddPointToDelaunayDebugVersion(CPoint3DOnTetra* pPoint);
	CTetraDelaunay* SearchIntersectingTetraByGrid(CPoint3DOnTetra* pPoint);
	CTetraDelaunay* SearchIntersectingTetra(CPoint3DOnTetra* pPoint,
											TGeomElemArray<CTetraDelaunay> &tetrahedra);

	static void RenderDelaunayTetrahedralization();

	static void GetEnlargedBoundingBox(CFluidPrtl* pPrtl,
									   TGeomElemArray<CFluidPrtl> &aryPrtls,
									   CVector3D &minPos, CVector3D &maxPos,
									   SimuValue enlargeSize);
	static void CreateOrthogonalTetraPoints(CVector3D &minPos, CVector3D &maxPos,
											TGeomElemArray<CPoint3DOnTetra> &dataPoints,
											CPoint3DOnTetra* boundingPs[4]);
	static void InitializeTessellation(CFluidPrtl* pPrtl,
									   TGeomElemArray<CFluidPrtl> &ngbrPrtls,
									   SimuValue tetraEnlargeSize,
									   TGeomElemArray<CPoint3DOnTetra> &dataPoints,
									   TGeomElemArray<CTetraDelaunay> &tetrahedra,
									   CPoint3DOnTetra* &ctrPoint,
									   CPoint3DOnTetra* boundingPs[4]);
	static bool SearchForUpsamplePos(CFluidPrtl* pPrtl,
									 TPointerArray<CFluidPrtl> &ngbrPrtls,
									 TGeomElemArray<CPoint3DOnTetra> &dataPoints,
									 TGeomElemArray<CTetraDelaunay> &tetrahedra,
									 CPoint3DOnTetra* &ctrPoint,
									 SimuValue upsampleMinRadius,
									 SimuValue upsampleMaxRadius,
									 CVector3D* upsamplePos);
	static bool ResumeSearchUpsamplePos(TGeomElemArray<CPoint3DOnTetra> &dataPoints,
										TGeomElemArray<CTetraDelaunay> &tetrahedra,
										CPoint3DOnTetra* ctrPoint,
										SimuValue upsampleMinRadius,
										SimuValue upsampleMaxRadius,
										CVector3D* upsamplePos);
	static bool SearchUpsamplePos(CFluidPrtl* pPrtl, TGeomElemArray<CFluidPrtl> &ngbrPrtls,
								  SimuValue upsampleMinRadius, SimuValue upsampleMaxRadius,
								  int &idi, int &idj, int &idk, CPoint3D* upsamplePos);
	static void DelaunayTessellation(CFluidPrtl* pPrtl,
									 TGeomElemArray<CFluidPrtl> &ngbrPrtls,
									 SimuValue tetraEnlargeSize,
									 TGeomElemArray<CPoint3DOnTetra> &dataPoints,
									 TGeomElemArray<CTetraDelaunay> &tetrahedra,
									 CPoint3DOnTetra* &ctrPoint,
									 CPoint3DOnTetra* boundingPs[4]);
	static bool AddPointToDelaunayTessellation(CPoint3DOnTetra* newPoint,
											   TGeomElemArray<CPoint3DOnTetra> &dataPoints,
											   TGeomElemArray<CTetraDelaunay> &tetrahedra,
											   CPoint3DOnTetra* ctrPoint);
	static bool AddPointToDelaunayDebugVersion(CPoint3DOnTetra* newPoint,
											   TGeomElemArray<CPoint3DOnTetra> &dataPoints,
											   TGeomElemArray<CTetraDelaunay> &tetrahedra,
											   CPoint3DOnTetra* ctrPoint);
	static CTetraDelaunay* SearchIntersectTetra(CPoint3DOnTetra* pPoint,
												CPoint3DOnTetra* ctrPoint,
												TGeomElemArray<CTetraDelaunay> &tetrahedra);
	static void DeleteTetrahedraBeingLabeled(TGeomElemArray<CTetraDelaunay> &tetrahedra);
	static void CreateNewTetrahedraFromTriangles(CPoint3DOnTetra* pPoint,
												 TGeomElemArray<CTriangleOnTetra> &triangles,
												 TGeomElemArray<CTetraDelaunay> &tetrahedra);
	static void ConfirmEmptyNgbrTetra(CPoint3DOnTetra* triPs[3]);

	static CTetraDelaunay* SearchTetraToUpsample(CPoint3DOnTetra* ctrPoint,
												 SimuValue upsampleMinRadius,
												 SimuValue upsampleMaxRadius);

	static void GetDistinctPointFromFluidPrtl(TPointerArray<CFluidPrtl> &dataPrtls,
											  TGeomElemArray<CPoint3DOnTetra> &dataPoints,
											  SimuValue minPointDistSqr);

	static CPoint3DOnTetra* GetTrianglePoint(CPoint3DOnTetra* edgeP0,
											 CPoint3DOnTetra* edgeP1,
											 TGeomElemArray<CPoint3DOnTetra> &dataPoints);
	static CPoint3DOnTetra* GetTetrahedronPoint(CPoint3DOnTetra* triP0,
												CPoint3DOnTetra* triP1,
												CPoint3DOnTetra* triP2,
												CPoint3DOnTetra* avoidP,
												CPoint3D &circumCtr,
												SimuValue &circumRSqr,
												TGeomElemArray<CPoint3DOnTetra> &dataPoints);
	static bool IsPointEnclosedByCircumSphere(CPoint3D* pPoint, CTetraDelaunay* pTetra,
											  SimuValue distTolerance);
	static void ConfirmDelaunayTessellation(TGeomElemArray<CTetraDelaunay> &tetrahedra,
											TGeomElemArray<CPoint3DOnTetra> &points);
	static void ConfirmDelaunayTetrahedron(TGeomElemArray<CPoint3DOnTetra> &points,
										   CTetraDelaunay* pTetra, SimuValue distTolerance);
	static int GetNumOfPointsOnTetra(TGeomElemArray<CPoint3DOnTetra> &points);

	static int m_dtrPreparationClocks;
	static int m_dtrAddPointsClocks;
	static int m_dtrSearchClocks;
	static int m_dtrAdjacentSearchClocks;
	static int m_dtrDeleteTetraClocks;
	static int m_dtrCreateTetraClocks;
	static int m_dtrSetupNgbrhoodClocks;
	static int m_dtrDeRegisterTetraClocks;
	static int m_dtrRegisterTetraClocks;
	static int m_dtrNumDeletedTetra;
	static CVector3D* m_dtrMinPos;
	static CVector3D* m_dtrMaxPos;
	static CVector3D* m_dtrCtrColor;
	static CVector3D* m_dtrTriPColor;
	static CVector3D* m_dtrTetraPColor;
	static CVector3D* m_dtrNgbrPColor;
	static CVector3D* m_dtrTestPColor;
	static CVector3D* m_dtrCircumCtrColor;
	static CFluidPrtl* m_dtrCurPrtl;
	static TGeomElemArray<CFluidPrtl>* m_dtrPrtlNgbrs;
	static TGeomElemArray<CPoint3DOnTetra>* m_dtrDataPoints;
	static TGeomElemArray<CTetraDelaunay>* m_dtrTetras;
	static TGeomElemArray<CTriangleOnTetra>* m_dtrTris;
	static CPoint3DOnTetra* m_dtrCtrPoint;
	static CPoint3DOnTetra* m_dtrNewDataPoint;
	static CPoint3D* m_dtrCircumCenter;
	static SimuValue m_dtrCircumRSqr;
	static bool m_dtrRunDebugVersion;
	static bool m_dtrPause[30];
};

#endif // !defined(AFX_DELAUNAYTETRAHEDRON_H__00AC8115_00A6_4E06_AA91_BB9F8F99AB32__INCLUDED_)
