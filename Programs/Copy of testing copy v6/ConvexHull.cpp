// ConvexHull.cpp: implementation of the CConvexHull class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "ConvexHull.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SimuValue CConvexHull::m_chMinPointDistSqr = pow(CSimuManager::m_prtlDistance*SIMU_PARA_MIN_PRTL_DISTANCE_RATIO, 2);
const int CConvexHull::m_chPrecisionDigitNum = 3;
int CConvexHull::m_chMinExp10 = 0;
SimuValue CConvexHull::m_chBoundingBoxCtrX = 0;
SimuValue CConvexHull::m_chBoundingBoxCtrY = 0;
SimuValue CConvexHull::m_chBoundingBoxCtrZ = 0;
SimuValue CConvexHull::m_chBoundingBoxScale = 0;
CPoint3DOnEdge* CConvexHull::m_chCurPoint = NULL;

CVector3D* CConvexHull::m_chCtrColor = NULL;
CVector3D* CConvexHull::m_chCurPColor = NULL;
CVector3D* CConvexHull::m_chTriPColor = NULL;
CVector3D* CConvexHull::m_chCoveredPColor = NULL;
CVector3D* CConvexHull::m_chNonCoveredPColor = NULL;
CPoint3D* CConvexHull::m_chCtrPoint = NULL;
TGeomElemArray<CPoint3DOnEdge>* CConvexHull::m_chCoveredPoints = NULL;
TGeomElemArray<CPoint3DOnEdge>* CConvexHull::m_chNonCoveredPoints = NULL;
TGeomElemArray<CEdge3DOnTriangle>* CConvexHull::m_chEdges = NULL;
TGeomElemArray<CTriangle3DIntNorm>* CConvexHull::m_chCandidateTriangles = NULL;
TGeomElemArray<CTriangle3DIntNorm>* CConvexHull::m_chUltimateTriangles = NULL;
bool CConvexHull::m_chRunDebugVersion = false;
bool CConvexHull::m_chPause[16] = {
false, false, false, false, false, false, false, false,
false, false, false, false, false, false, false, false};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConvexHull::CConvexHull()
{
}

CConvexHull::~CConvexHull()
{

}

void CConvexHull::RenderConvexHullElements()
{
	glPushAttrib(GL_CURRENT_BIT);

	if (m_chCtrPoint != NULL)
		m_chCtrPoint->RenderGeometry();

	if (m_chCurPoint != NULL)
		m_chCurPoint->RenderGeometry();

	if (m_chCoveredPoints != NULL)
		m_chCoveredPoints->RenderGeometryElements();

	if (m_chNonCoveredPoints != NULL)
		m_chNonCoveredPoints->RenderGeometryElements();

	if (m_chCandidateTriangles != NULL)
	{
		m_chCandidateTriangles->RenderGeometryElements();
		if (CSimuManager::DRAW_TRIANGLE_NORMALS)
		{
			SimuColor3v(CSimuManager::COLOR_RED->v);
			DrawTriangleNormals(m_chCandidateTriangles);
		}
	}
	if (m_chUltimateTriangles != NULL)
	{
		SimuColor3v(CSimuManager::COLOR_WHITE->v);
		m_chUltimateTriangles->RenderGeometryElements();
		if (CSimuManager::DRAW_TRIANGLE_NORMALS)
		{
			SimuColor3v(CSimuManager::COLOR_RED->v);
			DrawTriangleNormals(m_chUltimateTriangles);
		}
	}

	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CConvexHull::DrawTriangleNormals(TGeomElemArray<CTriangle3DIntNorm> *pTris)
{
	CVector3D ctrPos;
	CVector3D normal;
	for (int i=0; i < pTris->m_paNum; i++)
	{
		CTriangle3DIntNorm* pTri = pTris->m_paPtrs[i];
		ctrPos.SetValue((SimuValue)0);
		for (int j=0; j < 3; j++)
		{
			CEdge3D* pEdge = pTri->m_t3dEdges[j];
			ctrPos.AddedBy(&pEdge->m_e3dPoints[0]->m_p3dPos);
			ctrPos.AddedBy(&pEdge->m_e3dPoints[1]->m_p3dPos);
		}
		ctrPos.ScaledBy(0.5/3);
		normal.SetValue(&pTri->m_t3dinNormal);
		normal.Normalize();
		glBegin(GL_LINES);
		SimuVertex3v(ctrPos.v);
		ctrPos.AddedBy(&normal, CSimuManager::m_displayTuner);
		SimuVertex3v(ctrPos.v);
		glEnd();
	}
}

void CConvexHull::ClearDebugVariables()
{
	m_chCtrPoint = NULL;
	m_chCoveredPoints = NULL;
	m_chNonCoveredPoints = NULL;
	m_chEdges = NULL;
	m_chCandidateTriangles = NULL;
	m_chUltimateTriangles = NULL;
}

// if <pos> is inside convex hull the distance is negative;
// otherwise, positive.
// <normal> points the closest direction to the hull surface 
// if <point> is inside the hull; otherwise, zero.
SimuValue CConvexHull::DistanceFromExtraPointToConvexHull(CPoint3D* point,
														  TPointerArray<CFluidPrtl>
														  &dataPrtls, CVector3D* normal)
{
	TGeomElemArray<CPoint3DOnEdge> points(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CTriangle3DIntNorm> triangles(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CEdge3DOnTriangle> edges(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	CConvexHull::GetDistinctPointFromFluidPrtl(dataPrtls, points, m_chMinPointDistSqr);
	CConvexHull::RobustConvexHullGeneration(points, triangles, edges);
	if (triangles.m_paNum <= 0)
		return SIMU_VALUE_MAX; // point is outside hull
	SimuValue minDistance = CSimuUtility::GetClosestDistanceToConvexHull(&point->m_p3dPos,
																		 triangles, normal);
	if (m_chPause[7])
	{
		CPoint3DOnEdge curPoint; curPoint.m_p3dPos.SetValue(&point->m_p3dPos);
		m_chCurPoint = &curPoint;
		m_chCurPoint->SetPoint3DColor(CConvexHull::m_chCurPColor);
		m_chCandidateTriangles = &triangles;
		CPoint3DOnEdge closestP;
		CPoint3DOnEdge* pPoints[3];
		m_chCtrPoint = &closestP;
		sprintf(CSimuMsg::GetEmptyBuf(),
				"%.0f from (%.0f,%.0f,%.0f)(|v|=%.0f) to (%.0f,%.0f,%.0f) on convex hull",
				minDistance, point->m_p3dPos[X], point->m_p3dPos[Y], point->m_p3dPos[Z],
				point->m_p3dPos.Length(), 
				closestP.m_p3dPos[X], closestP.m_p3dPos[Y], closestP.m_p3dPos[Z]);
		CSimuMsg::PauseMessage(CSimuMsg::GetCheckedBuf());
		m_chCandidateTriangles = NULL;
		m_chCurPoint = NULL;
		m_chCtrPoint = NULL;
	}
	return minDistance;
}

void CConvexHull::RobustConvexHullGeneration(TGeomElemArray<CPoint3DOnEdge> &points,
											 TGeomElemArray<CTriangle3DIntNorm> &triangles,
											 TGeomElemArray<CEdge3DOnTriangle> &edges)
{
	if (m_chRunDebugVersion)
	{
		RobustConvexHullDebugVersion(points, triangles, edges);
		return;
	}

	char location[] = "CConvexHull::RobustConvexHullGeneration(...)";

	if (points.m_paNum <= 3)
	{
//		CSimuMsg::ExitWithMessage(location, "There are not more than 3 points.");
		return;
	}

	if (!GetIntPointPosition(points))
		return;

	TGeomElemArray<CTriangle3DIntNorm> candidateTriangles(true, SIMU_PARA_NGBR_TRI_ALLOC_SIZE);
	TGeomElemArray<CPoint3DOnEdge> nonCoveredPoints(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	nonCoveredPoints.AppendPointersFromArray(points);

	int i;
	CPoint3DOnEdge* tetraPs[4] = {NULL, NULL, NULL, NULL};
	tetraPs[0] = nonCoveredPoints.RemoveOnePointerFromEnd();
	// find the furtherest point as the second point
	tetraPs[1] = FindFurtherestPoint(tetraPs[0], nonCoveredPoints);
	if (tetraPs[1] == NULL)
	{
//		CSimuMsg::ExitWithMessage(location, "The second point is not found.");
		ClearDebugVariables();
		return;
	}
	// find the point to form the largest triangle as the third point
	tetraPs[1]->m_geDeleted = true;
	tetraPs[2] = FindPointToFormLargestTriangle(tetraPs[0], tetraPs[1], nonCoveredPoints);
	if (tetraPs[2] == NULL)
	{
//		CSimuMsg::ExitWithMessage(location, "The third point is not found.");
		ClearDebugVariables();
		return;
	}
	// find the point to form the largest tetrahedron as the forth point
	tetraPs[2]->m_geDeleted = true;
	tetraPs[3] = FindPointToFormLargestTetrahedron(tetraPs[0], tetraPs[1], tetraPs[2],
													nonCoveredPoints);
	if (tetraPs[3] == NULL)
	{
//		CSimuMsg::ExitWithMessage(location, "The forth point is not found.");
		ClearDebugVariables();
		return;
	}
	if (CSimuUtility::ParallelepipedVolume(&tetraPs[0]->m_p3dipPos, &tetraPs[1]->m_p3dipPos,
										   &tetraPs[2]->m_p3dipPos, &tetraPs[3]->m_p3dipPos)
		<= SIMU_VALUE_EPSILON)
	{
//		CSimuMsg::ExitWithMessage(location, "The points are degenerate.");
		ClearDebugVariables();
		return;
	}
	tetraPs[3]->m_geDeleted = true;
	nonCoveredPoints.CleanDeletedElements();

	// create the first 4 triangles on the tetrahedron
	Create4TrianglesOnTetrahedron(tetraPs, candidateTriangles, edges);
	CPoint3DOnEdge hullCtrPoint;
	hullCtrPoint.m_p3dipPos.SetValue(&tetraPs[0]->m_p3dipPos);
	hullCtrPoint.m_p3dipPos.AddedBy(&tetraPs[1]->m_p3dipPos);
	hullCtrPoint.m_p3dipPos.AddedBy(&tetraPs[2]->m_p3dipPos);
	hullCtrPoint.m_p3dipPos.AddedBy(&tetraPs[3]->m_p3dipPos);
	hullCtrPoint.m_p3dipPos.ScaledBy(0.25);

	// incremental construct convex hull
	SimuValue tmpDist, maxDist; 
	int iterations = 0;
	while (true)
	{
		iterations++;
		// search for first visible triangle
		CTriangle3DIntNorm* firstTriangle;
		maxDist = 0; m_chCurPoint = NULL;
		while (candidateTriangles.m_paNum > 0)
		{
			// remove the last candidate triangle in list
			firstTriangle = candidateTriangles.RemoveOnePointerFromEnd();
			for (int i=0; i < nonCoveredPoints.m_paNum; i++)
			{
				CPoint3DOnEdge* pPoint = nonCoveredPoints[i];
				if (PosIsAboveIntNormTriangle(&pPoint->m_p3dipPos, firstTriangle, tmpDist))
				{
					if (maxDist < tmpDist)
					{
						maxDist = tmpDist;
						m_chCurPoint = pPoint;
					}
				}
			}
			if (m_chCurPoint == NULL)
				// the removed triangle is invisible to all points
				triangles.AppendOnePointer(firstTriangle);
			else  // firstTriangle is the first visible triangle
				break;
		}
		if (m_chCurPoint == NULL)
			break; // triangles are all invisible to all points
		nonCoveredPoints.RemoveOnePointer(m_chCurPoint);

		// get the ridges
		TGeomElemArray<CEdge3DOnTriangle> ridges;
		TPointerStack<CTriangle3DIntNorm> visibleTriangles;
		visibleTriangles.StackPush(firstTriangle);
		while (!visibleTriangles.StackIsEmpty())
		{
			CTriangle3DIntNorm* visibleTri = visibleTriangles.StackPop();
			CTriangle3DIntNorm* ngbrTri;
			for (int i=0; i < 3; i++)
			{
				CEdge3DOnTriangle* pEdge = (CEdge3DOnTriangle*)visibleTri->m_t3dEdges[i];
				ngbrTri = (CTriangle3DIntNorm*)pEdge->GetTheOtherTriangle(visibleTri);
				if (ngbrTri == NULL)
					continue;
				if (ngbrTri->m_geDeleted)
					CSimuMsg::ExitWithMessage(location, "Error in edge-triangle structure.");
				if (PosIsAboveIntNormTriangle(&m_chCurPoint->m_p3dipPos, ngbrTri, tmpDist))
					visibleTriangles.StackPush(ngbrTri);
				else // found a ridge
				{
					ridges.AppendOnePointer(pEdge);
				}
			}
			DetachTriangleAndLabelDanglingEdgesDeleted(visibleTri);
			visibleTri->m_geDeleted = true;
		}
		delete firstTriangle; // firstTriangle has been removed from candidateTriangles
		candidateTriangles.CleanDeletedElements();
		edges.CleanDeletedElements();

		// create new triangles between ridges and m_chCurPoint
		for (int i=0; i < ridges.m_paNum; i++)
		{
			CEdge3DOnTriangle* pEdge = ridges[i];
			CTriangle3DIntNorm* newTri = candidateTriangles.CreateOneElement();
			newTri->AddOneEdge(pEdge);
			pEdge->AddOneTriangle(newTri);
			for (int j=0; j < 2; j++)
			{
				CPoint3DOnEdge* pPoint = pEdge->m_e3dPoints[j];
				CEdge3DOnTriangle* sideEdge = (CEdge3DOnTriangle*)
												pPoint->GetEdgeWithPoint(m_chCurPoint);
				if (sideEdge == NULL)
				{
					sideEdge = edges.CreateOneElement();
					sideEdge->SetEdgePoints(pPoint, m_chCurPoint);
				}
				newTri->AddOneEdge(sideEdge);
				sideEdge->AddOneTriangle(newTri);
			}
			InitializeTriangleIntegerNormal(newTri, &hullCtrPoint.m_p3dipPos, false);
		}
	}
}

void CConvexHull::RobustConvexHullDebugVersion(TGeomElemArray<CPoint3DOnEdge> &points,
											   TGeomElemArray<CTriangle3DIntNorm> &triangles,
											   TGeomElemArray<CEdge3DOnTriangle> &edges)
{
	char location[] = "CConvexHull::RobustConvexHullDebugVersion(...)";

	if (points.m_paNum <= 3)
	{
//		CSimuMsg::ExitWithMessage(location, "There are not more than 3 points.");
		return;
	}

	if (!GetIntPointPosition(points))
		return;

	TGeomElemArray<CTriangle3DIntNorm> candidateTriangles(true, SIMU_PARA_NGBR_TRI_ALLOC_SIZE);
	TGeomElemArray<CPoint3DOnEdge> nonCoveredPoints(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	nonCoveredPoints.AppendPointersFromArray(points);

	// debug variables
	m_chEdges = &edges;
	m_chUltimateTriangles = &triangles;
	m_chCandidateTriangles = &candidateTriangles;
	m_chNonCoveredPoints = &nonCoveredPoints;
	TGeomElemArray<CPoint3DOnEdge> coveredPoints(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_chCoveredPoints = &coveredPoints;
	CSimuMsg::CancelablePauseMessage("before translation", m_chPause[0]);

	int i;
	CPoint3DOnEdge* tetraPs[4] = {NULL, NULL, NULL, NULL};
	tetraPs[0] = nonCoveredPoints.RemoveOnePointerFromEnd();
	// find the furtherest point as the second point
	tetraPs[1] = FindFurtherestPoint(tetraPs[0], nonCoveredPoints);
	if (tetraPs[1] == NULL)
	{
//		CSimuMsg::ExitWithMessage(location, "The second point is not found.");
		ClearDebugVariables();
		return;
	}
	// find the point to form the largest triangle as the third point
	tetraPs[1]->m_geDeleted = true;
	tetraPs[2] = FindPointToFormLargestTriangle(tetraPs[0], tetraPs[1], nonCoveredPoints);
	if (tetraPs[2] == NULL)
	{
//		CSimuMsg::ExitWithMessage(location, "The third point is not found.");
		ClearDebugVariables();
		return;
	}
	// find the point to form the largest tetrahedron as the forth point
	tetraPs[2]->m_geDeleted = true;
	tetraPs[3] = FindPointToFormLargestTetrahedron(tetraPs[0], tetraPs[1], tetraPs[2],
													nonCoveredPoints);
	if (tetraPs[3] == NULL)
	{
//		CSimuMsg::ExitWithMessage(location, "The forth point is not found.");
		ClearDebugVariables();
		return;
	}
	if (CSimuUtility::ParallelepipedVolume(&tetraPs[0]->m_p3dipPos, &tetraPs[1]->m_p3dipPos,
										   &tetraPs[2]->m_p3dipPos, &tetraPs[3]->m_p3dipPos)
		<= SIMU_VALUE_EPSILON)
	{
//		CSimuMsg::ExitWithMessage(location, "The points are degenerate.");
		ClearDebugVariables();
		return;
	}
	tetraPs[3]->m_geDeleted = true;
	nonCoveredPoints.CleanDeletedElements();
	// debug variables
	coveredPoints.AppendOnePointer(tetraPs[0]);
	coveredPoints.AppendOnePointer(tetraPs[1]);
	coveredPoints.AppendOnePointer(tetraPs[2]);
	coveredPoints.AppendOnePointer(tetraPs[3]);
	tetraPs[0]->m_geDeleted = false;
	tetraPs[1]->m_geDeleted = false;
	tetraPs[2]->m_geDeleted = false;
	tetraPs[3]->m_geDeleted = false;

	// create the first 4 triangles on the tetrahedron
	Create4TrianglesOnTetrahedron(tetraPs, candidateTriangles, edges);
	CPoint3DOnEdge hullCtrPoint;
	hullCtrPoint.m_p3dipPos.SetValue(&tetraPs[0]->m_p3dipPos);
	hullCtrPoint.m_p3dipPos.AddedBy(&tetraPs[1]->m_p3dipPos);
	hullCtrPoint.m_p3dipPos.AddedBy(&tetraPs[2]->m_p3dipPos);
	hullCtrPoint.m_p3dipPos.AddedBy(&tetraPs[3]->m_p3dipPos);
	hullCtrPoint.m_p3dipPos.ScaledBy(0.25);
	// debug variables
	m_chCtrPoint = &hullCtrPoint;
	m_chCtrPoint->SetPoint3DColor(CConvexHull::m_chCtrColor);
	tetraPs[0]->SetPoint3DColor(CConvexHull::m_chCoveredPColor);
	tetraPs[1]->SetPoint3DColor(CConvexHull::m_chCoveredPColor);
	tetraPs[2]->SetPoint3DColor(CConvexHull::m_chCoveredPColor);
	tetraPs[3]->SetPoint3DColor(CConvexHull::m_chCoveredPColor);
	CSimuMsg::CancelablePauseMessage("after create 4 triangles", m_chPause[1]);

	// incremental construct convex hull
	SimuValue tmpDist, maxDist; 
	int iterations = 0;
	while (true)
	{
		iterations++;
		// search for first visible triangle
		CTriangle3DIntNorm* firstTriangle;
		maxDist = 0; m_chCurPoint = NULL;
		while (candidateTriangles.m_paNum > 0)
		{
			// remove the last candidate triangle in list
			firstTriangle = candidateTriangles.RemoveOnePointerFromEnd();
			for (int i=0; i < nonCoveredPoints.m_paNum; i++)
			{
				CPoint3DOnEdge* pPoint = nonCoveredPoints[i];
				if (PosIsAboveIntNormTriangle(&pPoint->m_p3dipPos, firstTriangle, tmpDist))
				{
					if (maxDist < tmpDist)
					{
						maxDist = tmpDist;
						m_chCurPoint = pPoint;
					}
				}
			}
			if (m_chCurPoint == NULL)
				// the removed triangle is invisible to all points
				triangles.AppendOnePointer(firstTriangle);
			else  // firstTriangle is the first visible triangle
				break;
		}
		if (m_chCurPoint == NULL)
			break; // triangles are all invisible to all points
		nonCoveredPoints.RemoveOnePointer(m_chCurPoint);

		// debug variables
		m_chCurPoint->SetPoint3DColor(m_chCurPColor);
		firstTriangle->SetTrianglePointColor(m_chTriPColor);
		CSimuMsg::CancelablePauseMessage("before add one covered point", m_chPause[2]);

		// get the ridges
		TGeomElemArray<CEdge3DOnTriangle> ridges;
		TPointerStack<CTriangle3DIntNorm> visibleTriangles;
		visibleTriangles.StackPush(firstTriangle);
		while (!visibleTriangles.StackIsEmpty())
		{
			CTriangle3DIntNorm* visibleTri = visibleTriangles.StackPop();
			CTriangle3DIntNorm* ngbrTri;
			for (int i=0; i < 3; i++)
			{
				CEdge3DOnTriangle* pEdge = (CEdge3DOnTriangle*)visibleTri->m_t3dEdges[i];
				ngbrTri = (CTriangle3DIntNorm*)pEdge->GetTheOtherTriangle(visibleTri);
				if (ngbrTri == NULL)
					continue;
				if (ngbrTri->m_geDeleted)
					CSimuMsg::ExitWithMessage(location, "Error in edge-triangle structure.");
				if (PosIsAboveIntNormTriangle(&m_chCurPoint->m_p3dipPos, ngbrTri, tmpDist))
					visibleTriangles.StackPush(ngbrTri);
				else // found a ridge
				{
					ridges.AppendOnePointer(pEdge);
				}
			}
			DetachTriangleAndLabelDanglingEdgesDeleted(visibleTri);
			visibleTri->m_geDeleted = true;
		}
		delete firstTriangle; // firstTriangle has been removed from candidateTriangles
		candidateTriangles.CleanDeletedElements();
		edges.CleanDeletedElements();

		// create new triangles between ridges and m_chCurPoint
		for (int i=0; i < ridges.m_paNum; i++)
		{
			CEdge3DOnTriangle* pEdge = ridges[i];
			// debug variables
			pEdge->m_e3dPoints[0]->SetPoint3DColor(m_chTriPColor);
			pEdge->m_e3dPoints[1]->SetPoint3DColor(m_chTriPColor);
			CSimuMsg::CancelablePauseMessage("before add one triangle", m_chPause[3]);
			pEdge->m_e3dPoints[0]->SetPoint3DColor(m_chCoveredPColor);
			pEdge->m_e3dPoints[1]->SetPoint3DColor(m_chCoveredPColor);

			CTriangle3DIntNorm* newTri = candidateTriangles.CreateOneElement();
			newTri->AddOneEdge(pEdge);
			pEdge->AddOneTriangle(newTri);
			for (int j=0; j < 2; j++)
			{
				CPoint3DOnEdge* pPoint = pEdge->m_e3dPoints[j];
				CEdge3DOnTriangle* sideEdge = (CEdge3DOnTriangle*)
												pPoint->GetEdgeWithPoint(m_chCurPoint);
				if (sideEdge == NULL)
				{
					sideEdge = edges.CreateOneElement();
					sideEdge->SetEdgePoints(pPoint, m_chCurPoint);
				}
				newTri->AddOneEdge(sideEdge);
				sideEdge->AddOneTriangle(newTri);
			}
			InitializeTriangleIntegerNormal(newTri, &hullCtrPoint.m_p3dipPos, false);
		}
		// debug variables
		ConfirmHullStructure(points, triangles, candidateTriangles, edges);
		coveredPoints.AppendOnePointer(m_chCurPoint);
		CSimuMsg::CancelablePauseMessage("after add one covered point", m_chPause[4]);
		m_chCurPoint->SetPoint3DColor(m_chCoveredPColor);
		m_chCurPoint = NULL;
	}

	CSimuMsg::CancelablePauseMessage("after create convex hull", m_chPause[5]);

	ClearDebugVariables();
}

void CConvexHull::GetDistinctPointFromFluidPrtl(TPointerArray<CFluidPrtl> &dataPrtls,
												TGeomElemArray<CPoint3DOnEdge> &dataPoints,
												SimuValue minPointDistSqr)
{
	for (int j=0; j < dataPrtls.m_paNum; j++)
	{
		CFluidPrtl* pPrtl = dataPrtls[j];
		if (pPrtl->m_fpNoDelaunay) continue;
		int k;
		for (k=0; k < dataPoints.m_paNum; k++)
		{
			CPoint3DOnEdge* existingP = dataPoints[k];
			SimuValue distSqr = existingP->m_p3dPos.GetDistanceSquareToVector(pPrtl->m_vpPos);
			if (distSqr <= minPointDistSqr)
				break;
		}
		if (k < dataPoints.m_paNum)
			continue;
		CPoint3DOnEdge* pPoint = dataPoints.CreateOneElement();
		pPoint->m_p3dPos.SetValue(pPrtl->m_vpPos);
		pPoint->m_p3doeEdges->ResetArraySize(0);
	}
}

CPoint3DOnEdge* CConvexHull::AddOnePoint3DIntPos(CVector3D* pos,
												 TGeomElemArray<CPoint3DOnEdge> &dataPoints)
{
	CPoint3DOnEdge* pPoint = dataPoints.CreateOneElement();
	pPoint->m_p3dPos.SetValue(pos);
	pPoint->m_p3doeEdges->ResetArraySize(0);
	return pPoint;
}

bool CConvexHull::GetIntPointPosition(TGeomElemArray<CPoint3DOnEdge> &points)
{
	// get bounding box
	CVector3D minPos, maxPos;
	for (int d=X; d <= Z; d++)
	{
		minPos[d] = SIMU_VALUE_MAX;
		maxPos[d] = -SIMU_VALUE_MAX;
	}
	for (int i=0; i < points.m_paNum; i++)
	{
		CVector3D* pPos = &points[i]->m_p3dPos;
		for (int d=X; d <= Z; d++)
		{
			if (minPos[d] > pPos->v[d])
				minPos[d] = pPos->v[d];
			if (maxPos[d] < pPos->v[d])
				maxPos[d] = pPos->v[d];
		}
	}
	// get bounding box center
	CVector3D boundingBoxCtr;
	boundingBoxCtr[X] = 0.5*(minPos[X] + maxPos[X]);
	boundingBoxCtr[Y] = 0.5*(minPos[Y] + maxPos[Y]);
	boundingBoxCtr[Z] = 0.5*(minPos[Z] + maxPos[Z]);
	// get relative positions
	for (int i=0; i < points.m_paNum; i++)
	{
		CPoint3DOnEdge* pPoint = points[i];
		for (int d=X; d <= Z; d++)
		{
			pPoint->m_p3dipPos[d] = pPoint->m_p3dPos[d] - boundingBoxCtr[d];
		}
	}
	// get longest bounding box side
	SimuValue longestBoxSide = -SIMU_VALUE_MAX;
	for (int d=X; d <= Z; d++)
	{
		SimuValue boxSideLen = maxPos[d] - minPos[d];
		if (longestBoxSide < boxSideLen)
			longestBoxSide = boxSideLen;
	}
	if (longestBoxSide <= SIMU_VALUE_EPSILON)
	{
		CSimuMsg::PauseMessage("Bounding box has no thickness.");
		return false;
	}
	// get truncated integer positions
	SimuValue boundingBoxScale = (pow(10.0f, m_chPrecisionDigitNum) - 1)/(0.5*longestBoxSide);
	for (int i=0; i < points.m_paNum; i++)
	{
		CPoint3DOnEdge* pPoint = points[i];
		for (int d=X; d <= Z; d++)
		{
			pPoint->m_p3dipPos[d] *= boundingBoxScale;
			pPoint->m_p3dipPos[d] = floor(pPoint->m_p3dipPos[d]+0.5);
		}
	}
	return true;
}

CPoint3DOnEdge* CConvexHull::FindFurtherestPoint(CPoint3DOnEdge* point,
												 TGeomElemArray<CPoint3DOnEdge> &points)
{
	CPoint3DOnEdge* furtherestPoint = NULL;
	SimuValue tmpDist, maxDist = -1;
	for (int i=0; i < points.m_paNum; i++)
	{
		CPoint3DOnEdge* pPoint = points[i];
		if (pPoint->m_geDeleted) continue;
		tmpDist = point->m_p3dipPos.GetDistanceSquareToVector(&pPoint->m_p3dipPos);
		if (maxDist < tmpDist)
		{
			maxDist = tmpDist;
			furtherestPoint = pPoint;
		}
	}
	return furtherestPoint;
}

CPoint3DOnEdge* CConvexHull::FindPointToFormLargestTriangle(CPoint3DOnEdge* point0,
															CPoint3DOnEdge* point1,
															TGeomElemArray<CPoint3DOnEdge>
															&points)
{
	CPoint3DOnEdge* triPoint = NULL;
	SimuValue tmpArea, maxArea = -1;
	for (int i=0; i < points.m_paNum; i++)
	{
		CPoint3DOnEdge* pPoint = points[i];
		if (pPoint->m_geDeleted) continue;
		tmpArea = CSimuUtility::ComputeTriangleArea(&point0->m_p3dipPos,
													&point1->m_p3dipPos,
													&pPoint->m_p3dipPos);
		if (maxArea < tmpArea)
		{
			maxArea = tmpArea;
			triPoint = pPoint;
		}
	}
	return triPoint;
}

CPoint3DOnEdge* CConvexHull::FindPointToFormLargestTetrahedron(CPoint3DOnEdge* point0,
															   CPoint3DOnEdge* point1,
															   CPoint3DOnEdge* point2,
															   TGeomElemArray<CPoint3DOnEdge>
															   &points)
{
	CPoint3DOnEdge* tetraPoint = NULL;
	SimuValue tmpVol, maxVol = -1;
	for (int i=0; i < points.m_paNum; i++)
	{
		CPoint3DOnEdge* pPoint = points[i];
		if (pPoint->m_geDeleted) continue;
		tmpVol = CSimuUtility::ParallelepipedVolume(&point0->m_p3dipPos,
													&point1->m_p3dipPos,
													&point2->m_p3dipPos,
													&pPoint->m_p3dipPos);
		if (maxVol < tmpVol)
		{
			maxVol = tmpVol;
			tetraPoint = pPoint;
		}
	}
	return tetraPoint;
}

void CConvexHull::Create4TrianglesOnTetrahedron(CPoint3DOnEdge* tetraPs[4],
												TGeomElemArray<CTriangle3DIntNorm> &triangles,
												TGeomElemArray<CEdge3DOnTriangle> &edges)
{
	for (int i=0; i < 4; i++)
	{
		CTriangle3DIntNorm* pTri = triangles.CreateOneElement();
		CPoint3DOnEdge* triPs[3] = {tetraPs[i], tetraPs[(i+1)%4], tetraPs[(i+2)%4]};
		CEdge3DOnTriangle* pEdge[3];
		for (int j=0; j < 3; j++)
		{
			pEdge[j] = (CEdge3DOnTriangle*)triPs[j]->GetEdgeWithPoint(triPs[(j+1)%3]);
			if (pEdge[j] == NULL)
			{
				pEdge[j] = edges.CreateOneElement();
				pEdge[j]->SetEdgePoints(triPs[j], triPs[(j+1)%3]);
			}
			pEdge[j]->AddOneTriangle(pTri);
		}
		pTri->SetTriangleEdges(pEdge[0], pEdge[1], pEdge[2]);
		InitializeTriangleIntegerNormal(pTri, &tetraPs[(i+3)%4]->m_p3dipPos, false);
	}
}

void CConvexHull::InitializeTriangleIntegerNormal(CTriangle3DIntNorm* pTri,
												  CVector3D* refPos, bool sameSide)
{
	CPoint3DOnEdge* triPs[3];
	pTri->GetTrianglePoints(triPs);
	ComputeTriangleIntNormByRefPos(&triPs[0]->m_p3dipPos,
								   &triPs[1]->m_p3dipPos,
								   &triPs[2]->m_p3dipPos,
								   refPos, sameSide, &pTri->m_t3dinNormal);
	pTri->m_t3dinPlaneD = pTri->m_t3dinNormal.DotProduct(&triPs[0]->m_p3dipPos);
	SimuValue intPlaneD = floor(pTri->m_t3dinPlaneD);
	if (pTri->m_t3dinPlaneD != intPlaneD)
		CSimuMsg::ExitWithMessage("CConvexHull::InitializeTriangleIntegerNormal(...)",
									"Non-integer triangle plane distance is found.");
}

void CConvexHull::ComputeTriangleIntNormByRefPos(CVector3D* pa, CVector3D* pb, CVector3D* pc,
												 CVector3D* pos, bool sameSide, CVector3D* n)
{
	char location[] = "CSimuUtility::ComputeTriangleIntNormByRefPos(...)";

	CSimuUtility::CrossProduct(pa, pb, pc, n);
	if (n->Length() <= SIMU_VALUE_EPSILON)
		CSimuMsg::ExitWithMessage(location, "Triangle is degenerate.");
	CVector3D dir;
	dir.SetValueAsSubstraction(pos, pa);
	SimuValue tmpVolume = dir.DotProduct(n);
	if (fabs(tmpVolume) <= SIMU_VALUE_EPSILON)
		CSimuMsg::ExitWithMessage(location, "Testing volume is too small.");
	if ((sameSide && tmpVolume < 0)
	 || (!sameSide && tmpVolume > 0))
		n->ScaledBy(-1);
	ConfirmIntegerNormal(n);
}

bool CConvexHull::PosIsAboveIntNormTriangle(CVector3D* pos, CTriangle3DIntNorm* pTri,
											SimuValue &distance)
{
	char location[] = "CConvexHull::PosIsAboveIntNormTriangle(...)";

	SimuValue posD = pTri->m_t3dinNormal.DotProduct(pos);
	SimuValue intPosD = floor(posD);
	if (posD != intPosD)
		CSimuMsg::ExitWithMessage(location, "Non-integer distance is found.");
	distance = posD - pTri->m_t3dinPlaneD;
	SimuValue intDist = floor(distance);
	if (distance != intDist)
		CSimuMsg::ExitWithMessage(location, "Non-integer distance to triangle is found.");
	if (distance > 0)
		return true;
	return false;
}

void CConvexHull::DetachTriangleAndLabelDanglingEdgesDeleted(CTriangle3DIntNorm* pTri)
{
	// detach triangle from its edges
	for(int i=0; i < 3; i++)
	{
		CEdge3DOnTriangle* pEdge = (CEdge3DOnTriangle*)pTri->m_t3dEdges[i];
		pEdge->RemoveOneTriangle(pTri);
		// detach and label dangling edge deleted
		if (pEdge->m_e3dotTris[0] == NULL && pEdge->m_e3dotTris[1] == NULL)
		{
			pEdge->m_e3dPoints[0]->m_p3doeEdges->RemoveOnePointer(pEdge);
			pEdge->m_e3dPoints[1]->m_p3doeEdges->RemoveOnePointer(pEdge);
			pEdge->m_geDeleted = true;
		}
	}
}

void CConvexHull::ConfirmIntegerNormal(CVector3D* normal)
{
	for(int d=0; d < 3; d++)
	{
		SimuValue intNormal = floor(normal->v[0]);
		if (normal->v[0] != intNormal)
			CSimuMsg::ExitWithMessage("CConvexHull::ConfirmIntegerNormal(normal)",
										"Non-integer normal is found.");
	}
}

void CConvexHull::ConfirmHullStructure(TGeomElemArray<CPoint3DOnEdge> &points,
									   TGeomElemArray<CTriangle3DIntNorm> &ultimateTris,
									   TGeomElemArray<CTriangle3DIntNorm> &candidateTris,
									   TGeomElemArray<CEdge3DOnTriangle> &edges)
{
	char location[] = "CConvexHull::ConfirmHullStructure(...)";

	// confirm points
	for (int i=0; i < points.m_paNum; i++)
	{
		CPoint3DOnEdge* pPoint = points[i];
		if (pPoint->m_geDeleted)
			CSimuMsg::ExitWithMessage(location, "Point has been deleted.");
		for (int j=0; j < pPoint->m_p3doeEdges->m_paNum; j++)
		{
			CEdge3DOnTriangle* pEdge = (CEdge3DOnTriangle*)pPoint->m_p3doeEdges->m_paPtrs[j];
			if (pEdge->m_geDeleted)
				CSimuMsg::ExitWithMessage(location, "Edge on point has been deleted.");
			if (pEdge->m_e3dotTris[0] == NULL || pEdge->m_e3dotTris[1] == NULL)
				CSimuMsg::ExitWithMessage(location, "An open edge is found.");
			if (!pEdge->HasEdgePoint(pPoint))
				CSimuMsg::ExitWithMessage(location, "Point is not found on edge.");
		}
	}
	for (int i=0; i < edges.m_paNum; i++)
	{
		CEdge3DOnTriangle* pEdge = edges[i];
		if (pEdge->m_geDeleted)
			CSimuMsg::ExitWithMessage(location, "Edge has been deleted.");
		if (pEdge->m_e3dotTris[0] == NULL || pEdge->m_e3dotTris[1] == NULL)
			CSimuMsg::ExitWithMessage(location, "Convex hull is not closed.");
	}
	for (int i=0; i < ultimateTris.m_paNum; i++)
	{
		CTriangle3DIntNorm* pTri = ultimateTris[i];
		if (pTri->m_geDeleted)
			CSimuMsg::ExitWithMessage(location, "Ultimate triangle has been deleted.");
		for (int j=0; j < 3; j++)
		{
			CEdge3DOnTriangle* pEdge = (CEdge3DOnTriangle*)pTri->m_t3dEdges[j];
			if (pEdge->m_geDeleted)
				CSimuMsg::ExitWithMessage(location, "Edge on point has been deleted.");
			if (pEdge->m_e3dotTris[0] != pTri && pEdge->m_e3dotTris[1] != pTri)
				CSimuMsg::ExitWithMessage(location, "Triangle is not found on edge.");
		}
	}
	for (int i=0; i < candidateTris.m_paNum; i++)
	{
		CTriangle3DIntNorm* pTri = candidateTris[i];
		if (pTri->m_geDeleted)
			CSimuMsg::ExitWithMessage(location, "Candidate triangle has been deleted.");
		for (int j=0; j < 3; j++)
		{
			CEdge3DOnTriangle* pEdge = (CEdge3DOnTriangle*)pTri->m_t3dEdges[j];
			if (pEdge->m_geDeleted)
				CSimuMsg::ExitWithMessage(location, "Edge on point has been deleted.");
			if (pEdge->m_e3dotTris[0] != pTri && pEdge->m_e3dotTris[1] != pTri)
				CSimuMsg::ExitWithMessage(location, "Triangle is not found on edge.");
		}
	}
}

bool CConvexHull::PointIsOnConvexHull(CPoint3DOnEdge* pPoint,
									  TGeomElemArray<CTriangle3DIntNorm> &triangles)
{
	if (m_chRunDebugVersion)
	{
		m_chCtrPoint = pPoint;
		m_chUltimateTriangles = &triangles;
	}
	bool bFound = false;
	CPoint3DOnEdge* pTriPs[3];
	for(int i=0; i < triangles.m_paNum && !bFound; i++)
	{
		triangles[i]->GetTrianglePoints(pTriPs);
		for (int k=0; k < 3 && !bFound; k++)
			if (pPoint == pTriPs[k])
			{
				bFound = true;
			}
	}
	if (m_chRunDebugVersion)
	{
		m_chCtrPoint = NULL;
		m_chUltimateTriangles = NULL;
	}
	return bFound;
}

// if pos is outside, return -1; otherwise, return positive value which
// is the minimum distance to convex hull.
SimuValue CConvexHull::GetMinDistanceToConvexHullFromInsidePos(CVector3D* pos,
											TGeomElemArray<CTriangle3DIntNorm> &triangles)
{
	SimuValue minDist = SIMU_VALUE_MAX;
	CVector3D relaPos;
	for(int i=0; i < triangles.m_paNum; i++)
	{
		CTriangle3DIntNorm* pTri = triangles[i];
		relaPos.SetValueAsSubstraction(pos, &pTri->m_t3dEdges[0]->m_e3dPoints[0]->m_p3dPos);
		pTri->m_t3dinNormal.Normalize();
		SimuValue tmpDist = relaPos.DotProduct(&pTri->m_t3dinNormal);
		if (tmpDist > 0)
			return -1;
		tmpDist *= -1;
		if (minDist > tmpDist)
		{
			minDist = tmpDist;
		}
	}
	return minDist;
}

// return true if pos is outside or on convex hull; otherwise, false.
bool CConvexHull::MoveInsidePosAwayConvexHull(CVector3D &pos,
											  TGeomElemArray<CTriangle3DIntNorm> &triangles,
											  SimuValue distThresholdOnHull,
											  SimuValue minDistToConvexHull)
{
	SimuValue minDist = SIMU_VALUE_MAX;
	CVector3D relaPos, outwardNormal;
	for(int i=0; i < triangles.m_paNum; i++)
	{
		CTriangle3DIntNorm* pTri = triangles[i];
		relaPos.SetValueAsSubstraction(&pos, &pTri->m_t3dEdges[0]->m_e3dPoints[0]->m_p3dPos);
		pTri->m_t3dinNormal.Normalize();
		SimuValue tmpDist = relaPos.DotProduct(&pTri->m_t3dinNormal);
		if (tmpDist > 0)
			return true;
		tmpDist *= -1;
		if (minDist > tmpDist)
		{
			minDist = tmpDist;
			outwardNormal.SetValue(&pTri->m_t3dinNormal);
		}
	}
	if (minDist <= distThresholdOnHull)
		return true;
	if (minDist < minDistToConvexHull)
		pos.AddedBy(&outwardNormal, minDist - minDistToConvexHull);
	return false;
}
