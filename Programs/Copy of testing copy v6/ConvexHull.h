// ConvexHull.h: interface for the CConvexHull class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVEXHULL_H__59A3F8F2_3101_4FB8_AFB0_19CC483548C1__INCLUDED_)
#define AFX_CONVEXHULL_H__59A3F8F2_3101_4FB8_AFB0_19CC483548C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TGeomElemArray.h"
#include "TPointerStack.h"
#include "Point3DIntPos.h"
#include "Triangle3DIntNorm.h"
#include "Edge3DOnTriangle.h"

class CConvexHull  
{
public:
	static SimuValue m_chMinPointDistSqr;
	static const int m_chPrecisionDigitNum;
	static int m_chMinExp10;
	static SimuValue m_chBoundingBoxCtrX;
	static SimuValue m_chBoundingBoxCtrY;
	static SimuValue m_chBoundingBoxCtrZ;
	static SimuValue m_chBoundingBoxScale;
	static CPoint3DOnEdge* m_chCurPoint;

	// debug variables
	static CVector3D* m_chCtrColor;
	static CVector3D* m_chCurPColor;
	static CVector3D* m_chTriPColor;
	static CVector3D* m_chCoveredPColor;
	static CVector3D* m_chNonCoveredPColor;
	static CPoint3D* m_chCtrPoint;
	static TGeomElemArray<CPoint3DOnEdge> *m_chCoveredPoints;
	static TGeomElemArray<CPoint3DOnEdge> *m_chNonCoveredPoints;
	static TGeomElemArray<CEdge3DOnTriangle> *m_chEdges;
	static TGeomElemArray<CTriangle3DIntNorm> *m_chCandidateTriangles;
	static TGeomElemArray<CTriangle3DIntNorm> *m_chUltimateTriangles;
	static bool m_chRunDebugVersion;
	static bool m_chPause[16];

	CConvexHull();
	virtual ~CConvexHull();

	static void RenderConvexHullElements();
	static void DrawTriangleNormals(TGeomElemArray<CTriangle3DIntNorm> *pTris);
	static void ClearDebugVariables();

	static SimuValue DistanceFromExtraPointToConvexHull(CPoint3D* point,
														TPointerArray<CFluidPrtl> &dataPrtls,
														CVector3D* normal);
	static void RobustConvexHullGeneration(TGeomElemArray<CPoint3DOnEdge> &points,
										   TGeomElemArray<CTriangle3DIntNorm> &triangles,
										   TGeomElemArray<CEdge3DOnTriangle> &edges);

	static void RobustConvexHullDebugVersion(TGeomElemArray<CPoint3DOnEdge> &points,
										   TGeomElemArray<CTriangle3DIntNorm> &triangles,
										   TGeomElemArray<CEdge3DOnTriangle> &edges);

	static void GetDistinctPointFromFluidPrtl(TPointerArray<CFluidPrtl> &dataPrtls,
											  TGeomElemArray<CPoint3DOnEdge> &dataPoints,
											  SimuValue minPointDistSqr);
	static CPoint3DOnEdge* AddOnePoint3DIntPos(CVector3D* pos,
											  TGeomElemArray<CPoint3DOnEdge> &dataPoints);
	static bool GetIntPointPosition(TGeomElemArray<CPoint3DOnEdge> &points);

	static CPoint3DOnEdge* FindFurtherestPoint(CPoint3DOnEdge* point,
											   TGeomElemArray<CPoint3DOnEdge> &points);
	static CPoint3DOnEdge* FindPointToFormLargestTriangle(CPoint3DOnEdge* point0,
														  CPoint3DOnEdge* point1,
												TGeomElemArray<CPoint3DOnEdge> &points);
	static CPoint3DOnEdge* FindPointToFormLargestTetrahedron(CPoint3DOnEdge* point0,
															 CPoint3DOnEdge* point1,
															 CPoint3DOnEdge* point2,
												TGeomElemArray<CPoint3DOnEdge> &points);
	static void Create4TrianglesOnTetrahedron(CPoint3DOnEdge* tetraPoints[4],
											  TGeomElemArray<CTriangle3DIntNorm> &triangles,
											  TGeomElemArray<CEdge3DOnTriangle> &edges);
	static void InitializeTriangleIntegerNormal(CTriangle3DIntNorm* pTri,
												CVector3D* refPos, bool sameSide);
	static void ComputeTriangleIntNormByRefPos(CVector3D* pa, CVector3D* pb, CVector3D* pc,
											   CVector3D* pos, bool sameSide, CVector3D* n);
	static bool PosIsAboveIntNormTriangle(CVector3D* pos, CTriangle3DIntNorm* pTri,
										  SimuValue &distance);
	static void DetachTriangleAndLabelDanglingEdgesDeleted(CTriangle3DIntNorm* pTri);
	static void ConfirmIntegerNormal(CVector3D* normal);
	static void ConfirmHullStructure(TGeomElemArray<CPoint3DOnEdge> &points,
									 TGeomElemArray<CTriangle3DIntNorm> &ultimateTris,
									 TGeomElemArray<CTriangle3DIntNorm> &candidateTris,
									 TGeomElemArray<CEdge3DOnTriangle> &edges);

	static bool PointIsOnConvexHull(CPoint3DOnEdge* pPoint,
									TGeomElemArray<CTriangle3DIntNorm> &triangles);
	static SimuValue GetMinDistanceToConvexHullFromInsidePos(CVector3D* pos,
											TGeomElemArray<CTriangle3DIntNorm> &triangles);
	static bool MoveInsidePosAwayConvexHull(CVector3D &pos,
											TGeomElemArray<CTriangle3DIntNorm> &triangles,
											SimuValue distThresholdOnHull,
											SimuValue minDistToConvexHull);
};

#endif // !defined(AFX_CONVEXHULL_H__59A3F8F2_3101_4FB8_AFB0_19CC483548C1__INCLUDED_)
