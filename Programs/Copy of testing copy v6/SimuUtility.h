// SimuUtility.h: interface for the CSimuUtility class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUUTILITY_H__3DC1551B_121D_4842_B8D5_C7D8238E7F46__INCLUDED_)
#define AFX_SIMUUTILITY_H__3DC1551B_121D_4842_B8D5_C7D8238E7F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector3D.h"
#include "VirtualPrtl.h"
#include "Triangle3DIntNorm.h"
#include "MeshPrtl.h"

class CSimuUtility  
{
public:
	CSimuUtility();
	virtual ~CSimuUtility();

	static bool IsInvalidSimuValue(SimuValue value);
	static bool IsInvalidCVector3D(CVector3D* vector);
	static void AssertSimuValue(SimuValue value);
	static void AssertSimuVector3D(CVector3D* vector);

	static bool FileDoesExist(CString fileName);

	static SimuValue ComputeSphereVolume(SimuValue radius);

	static void DrawSphereOr3DCrossAtPosition(CVector3D* pPos, SimuValue radius);
	static void DrawCircleOnZ0ByOpenGL(SimuValue radius, SimuValue height, SimuValue segments);
	static void DrawCylinderOnZ0ByOpenGL(SimuValue radius, SimuValue height, SimuValue segments);
	static void DrawBoundingBox(CVector3D &minPos, CVector3D &maxPos);
	static void RotateFromYAxis(CVector3D* dirV, CVector3D* oldV, CVector3D* newV);
	static void RotateFromYAxis(SimuValue cosXY, SimuValue sinXY,
								SimuValue cosXZ, SimuValue sinXZ,
								CVector3D* oldV, CVector3D* newV);
	static void RotateAboutYAxis(SimuValue cosA, SimuValue sinA,
								SimuValue oldX, SimuValue oldZ,
								SimuValue &newX, SimuValue &newZ);
	static void ComputeRightHandRotationMatrix(SimuValue vAX, SimuValue vAY, SimuValue vAZ,
												SimuValue rAngle, SimuValue rMatrix[3][3]);
	static void RotateVector(SimuValue rMatrix[3][3], CVector3D &vOld, CVector3D &vNew);
	static bool Inverse3x3Matrix(SimuValue matrix[9], SimuValue inverse[9]);
	static bool SolveNxNLinearEquations(SimuValue** M, SimuValue* X, int n);
	static bool VerifyNxNLinearEquations(SimuValue** M, SimuValue* X, int n);
	static void PrintNxNLinearEquations(SimuValue** M, SimuValue* X, int n);
	static void Print3x3Matrix(SimuValue A[3][3]);

	static SimuValue Compute3x3MatrixTrace(SimuValue M[3][3]);
	static SimuValue Compute3x3MatrixFrobNorm(SimuValue M[3][3]);

	static int SolveCubicEquation(SimuValue coeff[4], SimuValue x[3]);
	static int SolveQuadraticEquation(SimuValue coeff[3], SimuValue x[2]);

	static SimuValue DotProduct3D(SimuValue* v1, SimuValue* v2);
	static SimuValue DotProduct(CVector3D* v1, CVector3D* v2);
	static SimuValue DotProduct(CVector3D* v1, CVector3D* v2, CVector3D* vShared);
	static SimuValue CrossAndDotProduct(CVector3D* v1, CVector3D* v2, CVector3D* v3);
	static void CrossProduct(CVector3D* p0, CVector3D* p1, CVector3D* pShared,
							 CVector3D* result);
	static SimuValue ComputeTriangleArea3D(SimuValue* pa, SimuValue* pb, SimuValue* pc);
	static SimuValue ComputeTriangleArea(CVector3D* pa, CVector3D* pb,
										 CVector3D* pc);
	static SimuValue ComputeTriangleNormal(CVector3D* pa, CVector3D* pb,
										   CVector3D* pc, CVector3D* n);
	static SimuValue ComputeTriangleNormal(CVector3D* pa, CVector3D* pb,
										   CVector3D* pc, SimuValue* n);
	static void ComputeTriangleNormalByRefPos(CVector3D* pa, CVector3D* pb, CVector3D* pc,
											  CVector3D* pos, bool sameSide, CVector3D* n);
	static void ComputeTriangleNormalByRefDir(CVector3D* pa, CVector3D* pb, CVector3D* pc,
											  CVector3D* dir, bool sameSide, CVector3D* n);
	static void CreateRandomPos(SimuValue boundingCubeLen, CVector3D* ranPos);
	static void CreateRandomNormal(CVector3D* normal);
	static bool IsPointInTriangle3D(SimuValue* p, SimuValue* pa, SimuValue* pb, SimuValue* pc);
	static bool IsPointInTriangle(CVector3D* p,
								  CVector3D* pa, CVector3D* pb, CVector3D* pc);
	static bool SegmentIntersectTriangle(CVector3D* p1, CVector3D* p2,
										 CVector3D* pa, CVector3D* pb,
										 CVector3D* pc, CVector3D* p);
	static bool SegmentIntersectTriangle(CVector3D* p1, CVector3D* p2,
										 CVector3D* pa, CVector3D* pb,
										 CVector3D* pc, CVector3D* n,
										 SimuValue d, CVector3D* p);
	static bool LineIntersectPlane(CVector3D* lineP, CVector3D* lineV,
								   CVector3D* planeP, CVector3D* planeN,
								   CVector3D* intersectP);
	static bool HalfLineIntersectPlane(CVector3D* lineP, CVector3D* lineV,
									   CVector3D* planeP, CVector3D* planeN,
									   CVector3D* intersectP);
	static bool HalfLineIntersectTriangle(CVector3D* p0, CVector3D* dir,
										  CVector3D* pa, CVector3D* pb,
										  CVector3D* pc, CVector3D* p);
	static int GetClosestPointOnSegment(CVector3D* pos, CVector3D* p0,
										CVector3D* p1, CVector3D* clstP);
	static void GetClosestPointOnLine(CVector3D* pos, CVector3D* p0,
									  CVector3D* p1, CVector3D* clstP);
	static int GetClosestPointOnTriangle(CVector3D* pos,
										 CVector3D* p0, CVector3D* p1, CVector3D* p2,
										 CVector3D* clstP);
	static SimuValue GetClosestDistanceToConvexHull(CVector3D* pos,
													TGeomElemArray<CTriangle3DIntNorm>
													&triangles, CVector3D* normal);

	static void GetBoundingBoxForParticles(CVector3D &minPos, CVector3D &maxPos,
										   int numPrtls, CVirtualPrtl** aryPrtls);
	static bool IsPosInsideTetrahedron(CVector3D* pos, CVector3D* p0, CVector3D* p1,
									   CVector3D* p2, CVector3D* p3);
	static bool IsPosInsideTetrahedron(CVector3D* pos, bool bNormalPointingInside,
									   CVector3D* planeNormals[4], SimuValue planeDs[4]);
	static SimuValue ParallelepipedVolume(CVector3D* p0, CVector3D* p1, 
										  CVector3D* p2, CVector3D* p3);
	static bool ArePositionsCoplanar(CVector3D* p0, CVector3D* p1, CVector3D* p2, 
									 CVector3D* p3, SimuValue tolerance);
	static bool GetCircumCircleCenter(CVector3D* p0, CVector3D* p1, CVector3D* p2,
									  CVector3D* ctr);
	static int m_suEdgeMatrixRow, m_suEdgeMatrixCol;
	static SimuValue **m_suEdgeMatrix, *m_suCtrPos;
	static CVector3D* m_suEdge0;
	static CVector3D* m_suEdge1;
	static CVector3D* m_suEdge2;
	static CVector3D* m_suMidP0;
	static CVector3D* m_suMidP1;
	static CVector3D* m_suMidP2;
	static CVector3D* m_suTriNormal;
	static CVector3D* m_suIntersectLineVector0;
	static CVector3D* m_suCircumcircleCenter;
	static bool GetCircumSphereCenter(CVector3D* p0, CVector3D* p1, CVector3D* p2,
									  CVector3D* p3, CVector3D* ctr);

	static SimuValue LinearDerivativeWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue LinearSecondDerivativeWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue SineWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue SineFirstDerivativeWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue SineSecondDerivativeWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue SplineWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue SplineGradientWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue SplineSecondDerivativeFunction(SimuValue distance, SimuValue radius);
	static SimuValue SpikyGradientWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue SpikySecondDerivativeFunction(SimuValue distance, SimuValue radius);
	static SimuValue LaplacianWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue PolyFirstDerivativeWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue PolySecondDerivativeWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue ViscFirstDerivativeWeightFunction(SimuValue distance, SimuValue radius);
	static SimuValue ViscSecondDerivativeWeightFunction(SimuValue distance, SimuValue radius);

	static bool NumberIsWithinRange(int num, int endNum1, int endNum2);
	static bool NumberIsWithinRange(SimuValue num, SimuValue endNum1, SimuValue endNum2);
	static SimuValue AngleBetween2Vectors(CVector3D* pV0, CVector3D* pV1);
	static SimuValue GetAngleOf3Points(CVector3D* pTip0, CVector3D* pMid, CVector3D* pTip1);
	static SimuValue ComputeRadiusInOval(CVector3D* pPos, SimuValue ovalRs[3]);
	static void RestoreOpenGLPreviousColor();

	static void AdjustBoundingBoxWithPos(CVector3D &minPos, CVector3D &maxPos, CVector3D* pos);
	static bool IsPosOutsideBoundingBox(CVector3D &minPos, CVector3D &maxPos, CVector3D* pos);
	static void JitterVectorValue(CVector3D &vect, SimuValue maxDelta);

	static CMeshPrtl* AddDistinctMeshPrtl(TGeomElemArray<CMeshPrtl> &aryPrtls,
										  CVector3D &pos, SimuValue colocationEpsilon);
};

template <class CVirtualPrtl>
void T_GetBoundingBoxForParticles(CVector3D &minPos, CVector3D &maxPos,
								  int numPrtls, CVirtualPrtl** aryPrtls)
{
	minPos[X] = SIMU_VALUE_MAX; maxPos[X] = -SIMU_VALUE_MAX;
	minPos[Y] = SIMU_VALUE_MAX; maxPos[Y] = -SIMU_VALUE_MAX;
	minPos[Z] = SIMU_VALUE_MAX; maxPos[Z] = -SIMU_VALUE_MAX;
	T_UpdateBoundingBoxForParticles(minPos, maxPos, numPrtls, aryPrtls);
}

template <class CVirtualPrtl>
void T_UpdateBoundingBoxForParticles(CVector3D &minPos, CVector3D &maxPos,
									 int numPrtls, CVirtualPrtl** aryPrtls)
{
	for (int i=0; i < numPrtls; i++)
	{
		CVirtualPrtl* pPrtl = aryPrtls[i];
		for (int d=X; d <= Z; d++)
		{
			if (minPos[d] > pPrtl->m_vpPos->v[d])
				minPos[d] = pPrtl->m_vpPos->v[d];
			if (maxPos[d] < pPrtl->m_vpPos->v[d])
				maxPos[d] = pPrtl->m_vpPos->v[d];
		}
	}
}

#endif // !defined(AFX_SIMUUTILITY_H__3DC1551B_121D_4842_B8D5_C7D8238E7F46__INCLUDED_)
