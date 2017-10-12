// DelaunayTetrahedron.cpp: implementation of the CDelaunayTetrahedron class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "DelaunayTetrahedron.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SimuValue CDelaunayTetrahedron::m_dtrMinPointDistSqr = pow(CSimuManager::m_prtlDistance*SIMU_PARA_MIN_PRTL_DISTANCE_RATIO, 2);
SimuValue CDelaunayTetrahedron::m_dtrRadiusSqrTolerance = pow(CSimuManager::m_prtlDistance*SIMU_PARA_MIN_PRTL_DISTANCE_RATIO, 2); // 0; // 
SimuValue CDelaunayTetrahedron::m_dtrMinTetraVolume = SIMU_PARA_MIN_TETRAHEDRAL_VOLUME; // 0.1; // 
bool CDelaunayTetrahedron::m_dtrOnlyOnCtrPoint = true;
int CDelaunayTetrahedron::m_dtrNumMissedPoints = 0;
// debug variables
int CDelaunayTetrahedron::m_dtrPreparationClocks	= 0;
int CDelaunayTetrahedron::m_dtrAddPointsClocks		= 0;
int CDelaunayTetrahedron::m_dtrSearchClocks			= 0;
int CDelaunayTetrahedron::m_dtrAdjacentSearchClocks	= 0;
int CDelaunayTetrahedron::m_dtrDeleteTetraClocks	= 0;
int CDelaunayTetrahedron::m_dtrCreateTetraClocks	= 0;
int CDelaunayTetrahedron::m_dtrSetupNgbrhoodClocks	= 0;
int CDelaunayTetrahedron::m_dtrDeRegisterTetraClocks= 0;
int CDelaunayTetrahedron::m_dtrRegisterTetraClocks	= 0;
int CDelaunayTetrahedron::m_dtrNumDeletedTetra		= 0;
CVector3D* CDelaunayTetrahedron::m_dtrMinPos = NULL;
CVector3D* CDelaunayTetrahedron::m_dtrMaxPos = NULL;
CVector3D* CDelaunayTetrahedron::m_dtrCtrColor = NULL;
CVector3D* CDelaunayTetrahedron::m_dtrTriPColor = NULL;
CVector3D* CDelaunayTetrahedron::m_dtrTetraPColor = NULL;
CVector3D* CDelaunayTetrahedron::m_dtrNgbrPColor = NULL;
CVector3D* CDelaunayTetrahedron::m_dtrTestPColor = NULL;
CVector3D* CDelaunayTetrahedron::m_dtrCircumCtrColor = NULL;
CFluidPrtl* CDelaunayTetrahedron::m_dtrCurPrtl = NULL;
TGeomElemArray<CFluidPrtl>* CDelaunayTetrahedron::m_dtrPrtlNgbrs = NULL;
TGeomElemArray<CPoint3DOnTetra>* CDelaunayTetrahedron::m_dtrDataPoints = NULL;
TGeomElemArray<CTetraDelaunay>* CDelaunayTetrahedron::m_dtrTetras = NULL;
TGeomElemArray<CTriangleOnTetra>* CDelaunayTetrahedron::m_dtrTris = NULL;
CPoint3DOnTetra* CDelaunayTetrahedron::m_dtrCtrPoint = NULL;
CPoint3DOnTetra* CDelaunayTetrahedron::m_dtrNewDataPoint = NULL;
CPoint3D* CDelaunayTetrahedron::m_dtrCircumCenter = NULL;
SimuValue CDelaunayTetrahedron::m_dtrCircumRSqr = 0;
bool CDelaunayTetrahedron::m_dtrRunDebugVersion = false;
bool CDelaunayTetrahedron::m_dtrPause[30] = 
{true, true, false, false, true, false, false, false, false, false,
false, false, false, false, false, false, false, false, false, true,
false, 
false, false, false, false, false, false, false, false, false}; // not used

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDelaunayTetrahedron::CDelaunayTetrahedron()
{
	m_dtrPoints.InitializeClassArray(true, SIMU_PARA_PRTL_NUM_ALLOC_SIZE);
	m_dtrTetrahedra.InitializeClassArray(true, SIMU_PARA_TETRA_NUM_ALLOC_SIZE);
	m_dtrTetraSearchGrid.m_gCellSize = CSimuManager::m_prtlDistance*CSimuManager::m_smoothLengthRatio;
}

CDelaunayTetrahedron::~CDelaunayTetrahedron()
{

}

bool CDelaunayTetrahedron::ConstructDelaunayTetrahedra(TGeomElemArray<CFluidPrtl> &aryPrtls)
{
	char location[] = "CDelaunayTetrahedron::ConstructDelaunayTetrahedra(aryPrtls)";

	long start = clock();
	CVector3D minPos, maxPos;
	GetEnlargedBoundingBox(NULL, aryPrtls, minPos, maxPos, CSimuManager::m_prtlDistance);
	CreateOrthogonalTetraPoints(minPos, maxPos, m_dtrPoints, m_dtrBoundingPs);
	// create first tetrahedron
	CTetraDelaunay* pTetra = m_dtrTetrahedra.CreateOneElement();
	pTetra->SetTetrahedronPoints(m_dtrBoundingPs[0], m_dtrBoundingPs[1],
								 m_dtrBoundingPs[2], m_dtrBoundingPs[3]);
	if (pTetra->SetupNgbrTetra() == false)
		return false;
	CSimuMsg::CancelablePauseMessage("Created first tetrahedra.", m_dtrPause[10]);
	// prepare for tetrahedral search
	m_dtrTetraSearchGrid.UpdateGridBoundaryAndDimension(minPos.v, maxPos.v);
	m_dtrTetraSearchGrid.UpdateGridNumbers();
	m_dtrTetraSearchGrid.CreateTetraArraysInCells();
	m_dtrTetraSearchGrid.RegisterTetraInCells(pTetra);
	// create data points from prtls
	GetDistinctPointFromFluidPrtl(aryPrtls, m_dtrPoints, m_dtrMinPointDistSqr);

	long finish = clock();
	m_dtrPreparationClocks += finish-start;

	start = clock();
	// add data points into delaunay tessellation
	for (int i=4; i < m_dtrPoints.m_paNum; i++)
	{
		CPoint3DOnTetra* pPoint = m_dtrPoints[i];
		if (pPoint->m_p3dotTetras->m_paNum > 0)
			CSimuMsg::ExitWithMessage(location, "This point is already on a tetrahedron.");
		if (AddPointToDelaunayTessellation(pPoint) == false)
			return false;
//		ConfirmDelaunayTessellation(m_dtrTetrahedra, m_dtrPoints);
	}
	finish = clock();
	m_dtrAddPointsClocks += finish-start;

	CSimuMsg::CancelablePauseMessage("Finish delaunay tessellation.", m_dtrPause[17]);
	return true;
}

bool CDelaunayTetrahedron::AddPointToDelaunayTessellation(CPoint3DOnTetra* pPoint)
{
	char location[] = "CDelaunayTetrahedron::AddPointToDelaunayTessellation(pPoint)";

	if (m_dtrRunDebugVersion)
	{
		return AddPointToDelaunayDebugVersion(pPoint);
	}
	// find the first intersect tetrahedron
	long start = clock();
	CTetraDelaunay* enclosingTetra = SearchIntersectingTetraByGrid(pPoint);
	long finish = clock();
	m_dtrSearchClocks += finish-start;
	if (enclosingTetra == NULL)
	{
		CSimuMsg::ExitWithMessage(location, "Enclosing tetra is not found.");
		return false;
	}

	start = clock();
	// search for enclosing tetrahedra through adjacency
	// <intersectTetra> will be deregisted from grid and, 
	// in case of coplanarity, it is used for undeleting tetrahedra
	TGeomElemArray<CTetraDelaunay> intersectTetra;
	intersectTetra.AppendOnePointer(enclosingTetra);
	enclosingTetra->m_geDeleted = true; // label it for later delete
	int curArrayId = 0;
	// collect triangles for new tetra
	TGeomElemArray<CTriangleOnTetra> triangles(true, SIMU_PARA_NGBR_TRI_ALLOC_SIZE);
	while (curArrayId < intersectTetra.m_paNum)
	{
		// get the current element in queue
		CTetraDelaunay* pTetra = intersectTetra[curArrayId++];
		for (int k=0; k < 4; k++)
		{
			CTetraDelaunay* ngbrTetra = (CTetraDelaunay*)pTetra->m_ttrNgbrTetras[k];
			if (ngbrTetra != NULL)
			{
				if (ngbrTetra->m_geDeleted) continue;
				if (IsPointEnclosedByCircumSphere(pPoint, ngbrTetra, m_dtrRadiusSqrTolerance))
				{ // if ngbrTetra intersects newPoint, append it to queue
					intersectTetra.AppendOnePointer(ngbrTetra);
					ngbrTetra->m_geDeleted = true; // label it for later delete
					continue;
				}
			}
			// find one triangle that will form a new tetrahedron with newPoint
			CPoint3DOnTetra* triPs[3];
			pTetra->GetOppositePoints(k, triPs);
			if (CSimuUtility::ArePositionsCoplanar(&triPs[0]->m_p3dPos,
												   &triPs[1]->m_p3dPos,
												   &triPs[2]->m_p3dPos,
												   &pPoint->m_p3dPos,
												   m_dtrMinTetraVolume))
			{	// a degenerate tetra is found and abort addition of pPoint.
				// undelete tetrahedra
				for (int j=0; j < intersectTetra.m_paNum; j++)
				{
					intersectTetra[j]->m_geDeleted = false;
				}
				finish = clock();
				m_dtrAdjacentSearchClocks += finish-start;
				m_dtrNumMissedPoints ++;
				return false;
			}
			// prepare triangles for new tetrahedra
			CTriangleOnTetra* newTri = triangles.CreateOneElement();
			newTri->SetTrianglePoints(triPs[0], triPs[1], triPs[2]);
		}
	} // end of while on tetrahedral stack
	finish = clock();
	m_dtrAdjacentSearchClocks += finish-start;

	// delete intersecting tetra from neighborhood
	start = clock();
	DeleteTetrahedraBeingLabeled(intersectTetra);
	m_dtrNumDeletedTetra += intersectTetra.m_paNum;
	finish = clock();
	m_dtrDeleteTetraClocks += finish-start;

	int j;
	// create new tetrahedra
	start = clock();
	TGeomElemArray<CTetraDelaunay> newTetras(true, SIMU_PARA_NGBR_TETRA_ALLOC_SIZE);;
	CreateNewTetrahedraFromTriangles(pPoint, triangles, newTetras);
	finish = clock();
	m_dtrCreateTetraClocks += finish-start;

	start = clock();
	// setup neighborhood
	for (j=0; j < newTetras.m_paNum; j++)
	{
		CTetraDelaunay* pTetra = newTetras[j];
		if (pTetra->SetupNgbrTetra() == false)
		{
			finish = clock();
			m_dtrSetupNgbrhoodClocks += finish-start;
			// if not to abort, the deleted tetra must be recovered and the new ones deleted.
			m_dtrAbort = true;
			return false;
		}
	}
	finish = clock();
	m_dtrSetupNgbrhoodClocks += finish-start;

	// deregister intersecting tetra from grid
	start = clock();
	m_dtrTetraSearchGrid.ClearLabelsInCells();
	for (j=0; j < intersectTetra.m_paNum; j++)
	{
		m_dtrTetraSearchGrid.LabelCellsHoldingTetraCtr(intersectTetra[j]);
	}
	m_dtrTetraSearchGrid.DeRegisterDeletedTetraInLabeledCells();
	finish = clock();
	m_dtrDeRegisterTetraClocks += finish-start;

	// register new tetrahedra
	start = clock();
	for (j=0; j < newTetras.m_paNum; j++)
	{
		CTetraDelaunay* pTetra = newTetras[j];
		m_dtrTetraSearchGrid.RegisterTetraInCells(pTetra);
	}
	m_dtrTetrahedra.TakeOverPointersFromArray(newTetras);
	finish = clock();
	m_dtrRegisterTetraClocks += finish-start;
	return true;
}

bool CDelaunayTetrahedron::AddPointToDelaunayDebugVersion(CPoint3DOnTetra* pPoint)
{
	char location[] = "CDelaunayTetrahedron::AddPointToDelaunayDebugVersion(pPoint)";

	pPoint->SetPoint3DColor(CSimuManager::COLOR_RED);
	m_dtrNewDataPoint = pPoint;
	m_dtrDataPoints = &m_dtrPoints;
	m_dtrTetras = &m_dtrTetrahedra;
	// find the first intersect tetrahedron
	long start = clock();
	CTetraDelaunay* enclosingTetra = SearchIntersectingTetraByGrid(pPoint);
	long finish = clock();
	m_dtrSearchClocks += finish-start;
	if (enclosingTetra == NULL)
	{
		CSimuMsg::ExitWithMessage(location, "Enclosing tetra is not found.");
		pPoint->SetPoint3DColor(CSimuManager::COLOR_WHITE);
		m_dtrNewDataPoint = NULL;
		m_dtrDataPoints = NULL;
		m_dtrTetras = NULL;
		return false;
	}

	start = clock();
	// search for enclosing tetrahedra through adjacency
	// <intersectTetra> will be deregisted from grid and, 
	// in case of coplanarity, it is used for undeleting tetrahedra
	TGeomElemArray<CTetraDelaunay> intersectTetra;
	intersectTetra.AppendOnePointer(enclosingTetra);
	enclosingTetra->m_geDeleted = true; // label it for later delete
	int curArrayId = 0;
	// collect triangles for new tetra
	TGeomElemArray<CTriangleOnTetra> triangles(true, SIMU_PARA_NGBR_TRI_ALLOC_SIZE);
	m_dtrTris = &triangles;
	while (curArrayId < intersectTetra.m_paNum)
	{
		// get the current element in queue
		CTetraDelaunay* pTetra = intersectTetra[curArrayId++];
		for (int k=0; k < 4; k++)
		{
			CTetraDelaunay* ngbrTetra = (CTetraDelaunay*)pTetra->m_ttrNgbrTetras[k];
			if (ngbrTetra != NULL)
			{
				if (ngbrTetra->m_geDeleted) continue;
				if (IsPointEnclosedByCircumSphere(pPoint, ngbrTetra, m_dtrRadiusSqrTolerance))
				{ // if ngbrTetra intersects newPoint, append it to queue
					intersectTetra.AppendOnePointer(ngbrTetra);
					ngbrTetra->m_geDeleted = true; // label it for later delete
					continue;
				}
			}
			// find one triangle that will form a new tetrahedron with newPoint
			CPoint3DOnTetra* triPs[3];
			pTetra->GetOppositePoints(k, triPs);
			if (CSimuUtility::ArePositionsCoplanar(&triPs[0]->m_p3dPos,
												   &triPs[1]->m_p3dPos,
												   &triPs[2]->m_p3dPos,
												   &pPoint->m_p3dPos,
												   m_dtrMinTetraVolume))
			{	// a degenerate tetra is found and abort addition of pPoint.
				// undelete tetrahedra
				for (int j=0; j < intersectTetra.m_paNum; j++)
				{
					intersectTetra[j]->m_geDeleted = false;
				}
				finish = clock();
				m_dtrAdjacentSearchClocks += finish-start;
				m_dtrNumMissedPoints ++;
				pPoint->SetPoint3DColor(CSimuManager::COLOR_WHITE);
				m_dtrNewDataPoint = NULL;
				m_dtrDataPoints = NULL;
				m_dtrTetras = NULL;
				return false;
			}
			// prepare triangles for new tetrahedra
			CTriangleOnTetra* newTri = triangles.CreateOneElement();
			newTri->SetTrianglePoints(triPs[0], triPs[1], triPs[2]);
			CSimuMsg::PauseMessage("Find a new triangle.");
		}
	} // end of while on tetrahedral stack
	finish = clock();
	m_dtrAdjacentSearchClocks += finish-start;

	// delete intersecting tetra from neighborhood
	start = clock();
	DeleteTetrahedraBeingLabeled(intersectTetra);
	m_dtrNumDeletedTetra += intersectTetra.m_paNum;
	finish = clock();
	m_dtrDeleteTetraClocks += finish-start;

	int j;
	// create new tetrahedra
	start = clock();
	TGeomElemArray<CTetraDelaunay> newTetras(true, SIMU_PARA_NGBR_TETRA_ALLOC_SIZE);;
	CreateNewTetrahedraFromTriangles(pPoint, triangles, newTetras);
	finish = clock();
	m_dtrCreateTetraClocks += finish-start;

	start = clock();
	// setup neighborhood
	for (j=0; j < newTetras.m_paNum; j++)
	{
		CTetraDelaunay* pTetra = newTetras[j];
		triangles[j]->m_geHighlit = true;
		if (pTetra->SetupNgbrTetra() == false)
		{
			finish = clock();
			m_dtrSetupNgbrhoodClocks += finish-start;
			return false;
		}
		triangles[j]->m_geHighlit = false;
	}
	finish = clock();
	m_dtrSetupNgbrhoodClocks += finish-start;

	// deregister intersecting tetra from grid
	start = clock();
	m_dtrTetraSearchGrid.ClearLabelsInCells();
	for (j=0; j < intersectTetra.m_paNum; j++)
	{
		m_dtrTetraSearchGrid.LabelCellsHoldingTetraCtr(intersectTetra[j]);
	}
	m_dtrTetraSearchGrid.DeRegisterDeletedTetraInLabeledCells();
	finish = clock();
	m_dtrDeRegisterTetraClocks += finish-start;

	// register new tetrahedra and setup neighborhood
	start = clock();
	for (j=0; j < newTetras.m_paNum; j++)
	{
		CTetraDelaunay* pTetra = newTetras[j];
		m_dtrTetraSearchGrid.RegisterTetraInCells(pTetra);
	}
	m_dtrTetrahedra.TakeOverPointersFromArray(newTetras);
	finish = clock();
	m_dtrRegisterTetraClocks += finish-start;

	pPoint->SetPoint3DColor(CSimuManager::COLOR_WHITE);
	m_dtrNewDataPoint = NULL;
	m_dtrDataPoints = NULL;
	m_dtrTetras = NULL;
	return true;
}

CTetraDelaunay* CDelaunayTetrahedron::SearchIntersectingTetraByGrid(CPoint3DOnTetra* pPoint)
{
	if (false && m_dtrRunDebugVersion)
	{
		return SearchIntersectingTetra(pPoint, m_dtrTetrahedra);
	}
	TGeomElemArray<CTetraDelaunay>* aryTetra;
	int ctrCellId[3];
	m_dtrTetraSearchGrid.GetGridCellIdForInsidePosition(pPoint->m_p3dPos, ctrCellId);
	int minCellId[3], maxCellId[3];
	minCellId[X] = ctrCellId[X]; maxCellId[X] = ctrCellId[X];
	minCellId[Y] = ctrCellId[Y]; maxCellId[Y] = ctrCellId[Y];
	minCellId[Z] = ctrCellId[Z]; maxCellId[Z] = ctrCellId[Z];
	while ((minCellId[X] >= 0 && minCellId[X] < m_dtrTetraSearchGrid.m_gCellDim[X])
		|| (maxCellId[X] >= 0 && maxCellId[X] < m_dtrTetraSearchGrid.m_gCellDim[X])
		|| (minCellId[Y] >= 0 && minCellId[Y] < m_dtrTetraSearchGrid.m_gCellDim[Y])
		|| (maxCellId[Y] >= 0 && maxCellId[Y] < m_dtrTetraSearchGrid.m_gCellDim[Y])
		|| (minCellId[Z] >= 0 && minCellId[Z] < m_dtrTetraSearchGrid.m_gCellDim[Z])
		|| (maxCellId[Z] >= 0 && maxCellId[Z] < m_dtrTetraSearchGrid.m_gCellDim[Z]))
	{
		for (int x=minCellId[X]; x <= maxCellId[X]; x++)
		{
			if (x < 0 || x >= m_dtrTetraSearchGrid.m_gCellDim[X])
				continue; // out of grid boundary
		for (int y=minCellId[Y]; y <= maxCellId[Y]; y++)
		{
			if (y < 0 || y >= m_dtrTetraSearchGrid.m_gCellDim[Y])
				continue; // out of grid boundary
		for (int z=minCellId[Z]; z <= maxCellId[Z]; z++)
		{
			if (z < 0 || z >= m_dtrTetraSearchGrid.m_gCellDim[Z])
				continue; // out of grid boundary
			if (x > minCellId[X] && x < maxCellId[X]
			 && y > minCellId[Y] && y < maxCellId[Y]
			 && z > minCellId[Z] && z < maxCellId[Z])
				continue; // not on the boundary layer
			int aryId = m_dtrTetraSearchGrid.GetCellArrayIdFrom3DGridCellId(x, y, z);
			aryTetra = m_dtrTetraSearchGrid.m_gdtTetraArysInCells[aryId];
			for (int i=0; i < aryTetra->m_paNum; i++)
			{
				CTetraDelaunay* pTetra = aryTetra->m_paPtrs[i];
				if (IsPointEnclosedByCircumSphere(pPoint, pTetra, m_dtrRadiusSqrTolerance))
				{ // if pTetra encloses pPoint, return it.
					return pTetra;
				}
			}
		}}}
		minCellId[X] --; maxCellId[X] ++;
		minCellId[Y] --; maxCellId[Y] ++;
		minCellId[Z] --; maxCellId[Z] ++;
	}
	if (false && m_dtrRunDebugVersion)
	{
		return SearchIntersectingTetra(pPoint, m_dtrTetrahedra);
	}
	return NULL;
}

CTetraDelaunay* CDelaunayTetrahedron::SearchIntersectingTetra(CPoint3DOnTetra* pPoint,
															  TGeomElemArray<CTetraDelaunay>
															  &tetrahedra)
{
	for (int i=0; i < tetrahedra.m_paNum; i++)
	{
		CTetraDelaunay* pTetra = tetrahedra.m_paPtrs[i];
		if (IsPointEnclosedByCircumSphere(pPoint, pTetra, m_dtrRadiusSqrTolerance))
		{ // if pTetra encloses pPoint, return it.
			return pTetra;
		}
	}
}

void CDelaunayTetrahedron::RenderDelaunayTetrahedralization()
{
	glPushAttrib(GL_CURRENT_BIT);

	if (m_dtrPause[18] && m_dtrMinPos != NULL && m_dtrMaxPos != NULL)
	{
		CSimuUtility::DrawBoundingBox(*m_dtrMinPos, *m_dtrMaxPos);
	}

	if (m_dtrNewDataPoint != NULL)
		m_dtrNewDataPoint->RenderGeometry();

	if (m_dtrDataPoints != NULL)
	{
		if (m_dtrPause[19]) // show added data points only
		{
			for (int i=0; i < m_dtrDataPoints->m_paNum; i++)
			{
				CPoint3DOnTetra* pPoint = m_dtrDataPoints->m_paPtrs[i];
				if (pPoint->m_p3dotTetras->m_paNum > 0)
					pPoint->RenderGeometry();
			}
		}
		else
			m_dtrDataPoints->RenderGeometryElements();
	}

	if (m_dtrTetras != NULL
	 && CSimuManager::DRAW_TETRAHEDRA)
	{
		if (m_dtrPause[21])
		{
			for (int j=0; j < m_dtrTetras->m_paNum; j++)
			{
				CTetraDelaunay* pTetra = m_dtrTetras->m_paPtrs[j];
				if (pTetra->IsArtificialTetra()) continue;
				if (m_dtrCtrPoint == NULL
				 || pTetra->HasTetraPoint(m_dtrCtrPoint))
					pTetra->RenderGeometry();
			}
		}
		else
			m_dtrTetras->RenderGeometryElements();
	}

	if (m_dtrTris != NULL
	 && CSimuManager::DRAW_SURFACE_MESH)
	{
		m_dtrTris->RenderGeometryElements();
	}

	if (m_dtrCircumCenter != NULL)
	{
		m_dtrCircumCenter->RenderGeometry();
		SimuColor3v(m_dtrNgbrPColor->v);
		GLUquadricObj* qObj = gluNewQuadric();
		glPushMatrix();
		SimuTranslate(m_dtrCircumCenter->m_p3dPos[X],
					  m_dtrCircumCenter->m_p3dPos[Y],
					  m_dtrCircumCenter->m_p3dPos[Z]);
		gluSphere(qObj, sqrt(m_dtrCircumRSqr), 40, 40);
		glPopMatrix();
		gluDeleteQuadric(qObj);
	}

	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CDelaunayTetrahedron::GetEnlargedBoundingBox(CFluidPrtl* pPrtl,
												  TGeomElemArray<CFluidPrtl> &aryPrtls,
												  CVector3D &minPos, CVector3D &maxPos,
												  SimuValue enlargeSize)
{
	// get bounding box
	T_GetBoundingBoxForParticles<CFluidPrtl>(minPos, maxPos,
											 aryPrtls.m_paNum, aryPrtls.m_paPtrs);
	if (pPrtl != NULL)
		CSimuUtility::AdjustBoundingBoxWithPos(minPos, maxPos, pPrtl->m_vpPos);
	// get enlarged bounding box to ensure enclosing
	for (int d=X; d <= Z; d++)
	{
		minPos[d] -= enlargeSize;
		maxPos[d] += enlargeSize;
	}
}

void CDelaunayTetrahedron::CreateOrthogonalTetraPoints(CVector3D &minPos, CVector3D &maxPos,
													   TGeomElemArray<CPoint3DOnTetra>
													   &dataPoints,
													   CPoint3DOnTetra* boundingPs[4])
{
	// create first 4 artificial points
	CVector3D coveringPlaneNormal;
	coveringPlaneNormal.SetValueAsSubstraction(&minPos, &maxPos);
	coveringPlaneNormal.Normalize();
	CVector3D axisNormals[3];
	axisNormals[0].SetElements(1, 0, 0);
	axisNormals[1].SetElements(0, 1, 0);
	axisNormals[2].SetElements(0, 0, 1);
	for (int i=0; i < 3; i++)
	{
		boundingPs[i] = dataPoints.CreateOneElement();
		boundingPs[i]->m_p3dotArtificial = true;
		CSimuUtility::LineIntersectPlane(&minPos, &axisNormals[i],
										 &maxPos, &coveringPlaneNormal,
										 &boundingPs[i]->m_p3dPos);
	}
	boundingPs[3] = dataPoints.CreateOneElement();
	boundingPs[3]->m_p3dotArtificial = true;
	boundingPs[3]->m_p3dPos.SetValue(&minPos);
}

void CDelaunayTetrahedron::InitializeTessellation(CFluidPrtl* pPrtl,
												  TGeomElemArray<CFluidPrtl> &aryPrtls,
												  SimuValue tetraEnlargeSize,
												  TGeomElemArray<CPoint3DOnTetra> &dataPoints,
												  TGeomElemArray<CTetraDelaunay> &tetrahedra,
												  CPoint3DOnTetra* &ctrPoint,
												  CPoint3DOnTetra* boundingPs[4])
{
	CVector3D minPos, maxPos;
	GetEnlargedBoundingBox(pPrtl, aryPrtls, minPos, maxPos, tetraEnlargeSize);
	CreateOrthogonalTetraPoints(minPos, maxPos, dataPoints, boundingPs);
	// create data points from prtls
	GetDistinctPointFromFluidPrtl(aryPrtls, dataPoints, m_dtrMinPointDistSqr);
	ctrPoint = dataPoints.CreateOneElement();
	ctrPoint->m_p3dPos.SetValue(pPrtl->m_vpPos);
	ctrPoint->SetPoint3DColor(m_dtrCtrColor);
	// create first 4 tetrahedra
	for (int k=0; k < 4; k++)
	{
		CTetraDelaunay* pTetra = tetrahedra.CreateOneElement();
		pTetra->SetTetrahedronPoints(boundingPs[k], boundingPs[(k+1)%4], boundingPs[(k+2)%4],
									 ctrPoint);
		if (pTetra->SetupNgbrTetra() == false)
			return;
	}
	CSimuMsg::CancelablePauseMessage("Created first 4 tetrahedra.", m_dtrPause[10]);
}

bool CDelaunayTetrahedron::SearchForUpsamplePos(CFluidPrtl* pPrtl,
												TPointerArray<CFluidPrtl> &ngbrPrtls,
												TGeomElemArray<CPoint3DOnTetra> &dataPoints,
												TGeomElemArray<CTetraDelaunay> &tetrahedra,
												CPoint3DOnTetra* &ctrPoint,
												SimuValue upsampleMinRadius,
												SimuValue upsampleMaxRadius,
												CVector3D* upsamplePos)
{
	dataPoints.ResetArraySize(0);
	tetrahedra.ResetArraySize(0);

	// create ctr point
	ctrPoint = dataPoints.CreateOneElement();
	ctrPoint->m_p3dPos.SetValue(pPrtl->m_vpPos);
	ctrPoint->SetPoint3DColor(m_dtrCtrColor);
	// create ngbr points
	m_dtrMinPointDistSqr = pow(upsampleMinRadius*0.001, 2);
	GetDistinctPointFromFluidPrtl(ngbrPrtls, dataPoints, m_dtrMinPointDistSqr);
	// find the closest point to ctr point
	SimuValue minDistSqr = SIMU_VALUE_MAX;
	CPoint3DOnTetra* closestP = NULL;
	for (int k=0; k < dataPoints.m_paNum; k++)
	{
		CPoint3DOnTetra* pPoint = dataPoints[k];
		if (pPoint == ctrPoint) continue;
		pPoint->SetPoint3DColor(m_dtrNgbrPColor);
		SimuValue distSqr = pPoint->m_p3dPos.GetDistanceSquareToVector(&ctrPoint->m_p3dPos);
		if (minDistSqr > distSqr)
		{
			minDistSqr = distSqr;
			closestP = pPoint;
		}
	}
	if (closestP == NULL)
	{
		CSimuMsg::CancelablePauseMessage("Closest point is not found", m_dtrPause[0]);
		return false;
	}
	closestP->SetPoint3DColor(m_dtrTriPColor);

	CPoint3DOnTetra* triPoint = GetTrianglePoint(ctrPoint, closestP, dataPoints);
	if (triPoint == NULL)
	{
		CSimuMsg::CancelablePauseMessage("First triangle point is not found", m_dtrPause[1]);
		return false;
	}
	triPoint->SetPoint3DColor(m_dtrTriPColor);

	// set debug variables
	m_dtrTetras = &tetrahedra;
	m_dtrDataPoints = &dataPoints;

	SimuValue circumRSqr;
	CPoint3D circumSphCtr; circumSphCtr.SetPoint3DColor(m_dtrCircumCtrColor);

	int curTetraNgbrId;
	CTetraDelaunay* curTetra = NULL;
	CPoint3DOnTetra* triPs[3] = {ctrPoint, closestP, triPoint};
	CPoint3DOnTetra* avoidP = NULL;
	CPoint3DOnTetra* tetraP;
	bool bHasOpenTri = true;
	while (bHasOpenTri)
	{
		triPs[0]->SetPoint3DColor(m_dtrTriPColor);
		triPs[1]->SetPoint3DColor(m_dtrTriPColor);
		triPs[2]->SetPoint3DColor(m_dtrTriPColor);
		ctrPoint->SetPoint3DColor(m_dtrCtrColor);
		tetraP = GetTetrahedronPoint(triPs[0], triPs[1], triPs[2], avoidP,
									 circumSphCtr, circumRSqr, dataPoints);
		if (tetraP != NULL)
		{
			CTetraDelaunay* pTetra = tetrahedra.CreateOneElement();
			pTetra->SetTetrahedronPoints(triPs[0], triPs[1], triPs[2], tetraP);
			if (pTetra->SetupNgbrTetra() == false)
			{
				CSimuMsg::CancelablePauseMessage("First tetrahedron is not found.", m_dtrPause[4]);
				m_dtrTetras = NULL;
				m_dtrDataPoints = NULL;
				triPs[0]->SetPoint3DColor(m_dtrNgbrPColor);
				triPs[1]->SetPoint3DColor(m_dtrNgbrPColor);
				triPs[2]->SetPoint3DColor(m_dtrNgbrPColor);
				ctrPoint->SetPoint3DColor(m_dtrCtrColor);
				return false;
			}
			m_dtrCircumCenter = &circumSphCtr;
			m_dtrCircumRSqr = circumRSqr;
			if (circumRSqr >= upsampleMinRadius
			 && circumRSqr <= upsampleMaxRadius)
			{
				ConfirmDelaunayTetrahedron(dataPoints, pTetra, m_dtrRadiusSqrTolerance);
				upsamplePos->SetValue(&circumSphCtr.m_p3dPos);
				CSimuMsg::CancelablePauseMessage("Upsample position is found.", m_dtrPause[2]);
				m_dtrCircumCenter = NULL;
				m_dtrTetras = NULL;
				m_dtrDataPoints = NULL;
				triPs[0]->SetPoint3DColor(m_dtrNgbrPColor);
				triPs[1]->SetPoint3DColor(m_dtrNgbrPColor);
				triPs[2]->SetPoint3DColor(m_dtrNgbrPColor);
				ctrPoint->SetPoint3DColor(m_dtrCtrColor);
				tetraP->SetPoint3DColor(m_dtrNgbrPColor);
				return true;
			}
			m_dtrCircumCenter = NULL;
			CSimuMsg::CancelablePauseMessage("After adding tetrahedron.", m_dtrPause[3]);
			tetraP->SetPoint3DColor(m_dtrNgbrPColor);
		}
		else if (curTetra == NULL) // && tetraP == NULL 
		{
			CSimuMsg::CancelablePauseMessage("First tetrahedron is not found.", m_dtrPause[4]);
			m_dtrTetras = NULL;
			m_dtrDataPoints = NULL;
			triPs[0]->SetPoint3DColor(m_dtrNgbrPColor);
			triPs[1]->SetPoint3DColor(m_dtrNgbrPColor);
			triPs[2]->SetPoint3DColor(m_dtrNgbrPColor);
			ctrPoint->SetPoint3DColor(m_dtrCtrColor);
			return false;
		}
		else // curTetra != NULL && tetraP == NULL 
		{
			curTetra->m_ttrNgbrTetras[curTetraNgbrId] = curTetra;
			CSimuMsg::CancelablePauseMessage("No tetrahedron is found on triangle.", m_dtrPause[5]);
		}
		triPs[0]->SetPoint3DColor(m_dtrNgbrPColor);
		triPs[1]->SetPoint3DColor(m_dtrNgbrPColor);
		triPs[2]->SetPoint3DColor(m_dtrNgbrPColor);
		ctrPoint->SetPoint3DColor(m_dtrCtrColor);
		// find an open triangle on tetrahedra
		triPs[0] = NULL; triPs[1] = NULL; triPs[2] = NULL; avoidP = NULL;
		for (int i=0; i < tetrahedra.m_paNum; i++)
		{
			curTetra = tetrahedra[i];
			for (int j=0; j < 4; j++)
			{
				CPoint3DOnTetra* pPoint = curTetra->m_ttrPs[j];
				if (pPoint == ctrPoint) continue;
				if (curTetra->m_ttrNgbrTetras[j] == NULL)
				{
					curTetraNgbrId = j;
					curTetra->GetOppositePoints(j, triPs);
					avoidP = pPoint;
					break;
				}
			}
			if (triPs[0] == NULL)
				curTetra->m_geDeleted = true; // no open triangle on <pTetra>
			else
				break;
		}
		if (triPs[0] == NULL) // triangle point is not found
			bHasOpenTri = false; // no open triangle is found for new delaunay tetrahedron
	}

	ConfirmDelaunayTessellation(tetrahedra, dataPoints);

	sprintf(CSimuMsg::GetEmptyBuf(), "%d delaunay tetra from %d(%d) points around ctr point.",
				tetrahedra.m_paNum, GetNumOfPointsOnTetra(dataPoints), dataPoints.m_paNum);
	CSimuMsg::CancelablePauseMessage(CSimuMsg::GetCheckedBuf(), m_dtrPause[6]);

	m_dtrTetras = NULL;
	m_dtrDataPoints = NULL;
	return false;
}

bool CDelaunayTetrahedron::ResumeSearchUpsamplePos(TGeomElemArray<CPoint3DOnTetra>
												   &dataPoints,
												   TGeomElemArray<CTetraDelaunay> &tetrahedra,
												   CPoint3DOnTetra* ctrPoint,
												   SimuValue upsampleMinRadius,
												   SimuValue upsampleMaxRadius,
												   CVector3D* upsamplePos)
{
	// set debug variables
	m_dtrTetras = &tetrahedra;
	m_dtrDataPoints = &dataPoints;

	SimuValue circumRSqr;
	CPoint3D circumSphCtr; circumSphCtr.SetPoint3DColor(m_dtrCircumCtrColor);

	while (true)
	{
		// find an open triangle on tetrahedra
		int curTetraNgbrId;
		CTetraDelaunay* curTetra = NULL;
		CPoint3DOnTetra* triPs[3] = {NULL, NULL, NULL};
		CPoint3DOnTetra* avoidP = NULL;
		for (int i=0; i < tetrahedra.m_paNum; i++)
		{
			curTetra = tetrahedra[i];
			for (int j=0; j < 4; j++)
			{
				CPoint3DOnTetra* pPoint = curTetra->m_ttrPs[j];
				if (pPoint == ctrPoint) continue;
				if (curTetra->m_ttrNgbrTetras[j] == NULL)
				{
					curTetraNgbrId = j;
					curTetra->GetOppositePoints(j, triPs);
					avoidP = pPoint;
					break;
				}
			}
			if (triPs[0] == NULL)
				curTetra->m_geDeleted = true; // no open triangle on <pTetra>
			else
				break;
		}
		if (triPs[0] == NULL) // triangle point is not found
			break; // no open triangle is found for new delaunay tetrahedron

		triPs[0]->SetPoint3DColor(m_dtrTriPColor);
		triPs[1]->SetPoint3DColor(m_dtrTriPColor);
		triPs[2]->SetPoint3DColor(m_dtrTriPColor);
		ctrPoint->SetPoint3DColor(m_dtrCtrColor);
		CPoint3DOnTetra* tetraP = GetTetrahedronPoint(triPs[0], triPs[1], triPs[2], avoidP,
													 circumSphCtr, circumRSqr, dataPoints);
		if (tetraP != NULL)
		{
			CTetraDelaunay* pTetra = tetrahedra.CreateOneElement();
			pTetra->SetTetrahedronPoints(triPs[0], triPs[1], triPs[2], tetraP);
			if (pTetra->SetupNgbrTetra() == false)
			{
				CSimuMsg::CancelablePauseMessage("Upsample tetrahedron is not found.", m_dtrPause[4]);
				m_dtrTetras = NULL;
				m_dtrDataPoints = NULL;
				triPs[0]->SetPoint3DColor(m_dtrNgbrPColor);
				triPs[1]->SetPoint3DColor(m_dtrNgbrPColor);
				triPs[2]->SetPoint3DColor(m_dtrNgbrPColor);
				ctrPoint->SetPoint3DColor(m_dtrCtrColor);
				tetraP->SetPoint3DColor(m_dtrNgbrPColor);
				return false;
			}
			m_dtrCircumCenter = &circumSphCtr;
			m_dtrCircumRSqr = circumRSqr;
			if (circumRSqr >= upsampleMinRadius
			 && circumRSqr <= upsampleMaxRadius)
			{
				ConfirmDelaunayTetrahedron(dataPoints, pTetra, m_dtrRadiusSqrTolerance);
				upsamplePos->SetValue(&circumSphCtr.m_p3dPos);
				CSimuMsg::CancelablePauseMessage("Upsample position is found.", m_dtrPause[2]);
				m_dtrCircumCenter = NULL;
				m_dtrTetras = NULL;
				m_dtrDataPoints = NULL;
				triPs[0]->SetPoint3DColor(m_dtrNgbrPColor);
				triPs[1]->SetPoint3DColor(m_dtrNgbrPColor);
				triPs[2]->SetPoint3DColor(m_dtrNgbrPColor);
				ctrPoint->SetPoint3DColor(m_dtrCtrColor);
				tetraP->SetPoint3DColor(m_dtrNgbrPColor);
				return true;
			}
			m_dtrCircumCenter = NULL;
			CSimuMsg::CancelablePauseMessage("After adding tetrahedron.", m_dtrPause[3]);
			tetraP->SetPoint3DColor(m_dtrNgbrPColor);
		}
		else // tetraP == NULL 
		{
			curTetra->m_ttrNgbrTetras[curTetraNgbrId] = curTetra;
			CSimuMsg::CancelablePauseMessage("No tetrahedron is found on triangle.", m_dtrPause[5]);
		}
		triPs[0]->SetPoint3DColor(m_dtrNgbrPColor);
		triPs[1]->SetPoint3DColor(m_dtrNgbrPColor);
		triPs[2]->SetPoint3DColor(m_dtrNgbrPColor);
		ctrPoint->SetPoint3DColor(m_dtrCtrColor);
	}

	ConfirmDelaunayTessellation(tetrahedra, dataPoints);

	sprintf(CSimuMsg::GetEmptyBuf(), "%d delaunay tetra from %d(%d) points around ctr point.",
				tetrahedra.m_paNum, GetNumOfPointsOnTetra(dataPoints), dataPoints.m_paNum);
	CSimuMsg::CancelablePauseMessage(CSimuMsg::GetCheckedBuf(), m_dtrPause[6]);

	m_dtrTetras = NULL;
	m_dtrDataPoints = NULL;
	return false;
}

bool CDelaunayTetrahedron::SearchUpsamplePos(CFluidPrtl* pPrtl,
											 TGeomElemArray<CFluidPrtl> &ngbrPrtls,
											 SimuValue upsampleMinRadius,
											 SimuValue upsampleMaxRadius,
											 int &idi, int &idj, int &idk,
											 CPoint3D* upsampleP)
{
	char location[] = "CDelaunayTetrahedron::SearchUpsamplePos(...)";

	CPoint3D circumCrlCtr;
	CPoint3D circumSphCtr;
	circumSphCtr.SetPoint3DColor(CDelaunayTetrahedron::m_dtrCircumCtrColor);
	CVector3D circumCrlNormal;
	CVector3D bisecPlaneNormal;
	CVector3D bisecPlanePos;
	SimuValue circumRSqr;
	SimuValue distSqr;
	for (int i=idi; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl0 = ngbrPrtls[i];
		for (int j=idj; j < ngbrPrtls.m_paNum; j++)
		{
			CFluidPrtl* pPrtl1 = ngbrPrtls[j];
			if (pPrtl1 == pPrtl0) continue;
			if (!CSimuUtility::GetCircumCircleCenter(pPrtl1->m_vpPos, pPrtl0->m_vpPos,
													 pPrtl->m_vpPos, &circumCrlCtr.m_p3dPos))
				continue;
			circumRSqr = circumCrlCtr.m_p3dPos.GetDistanceSquareToVector(pPrtl->m_vpPos);
			if (circumRSqr > upsampleMaxRadius*upsampleMaxRadius)
				continue;
			CSimuUtility::ComputeTriangleNormal(pPrtl1->m_vpPos, pPrtl0->m_vpPos,
												pPrtl->m_vpPos, &circumCrlNormal);
			for (int k=idk; k < ngbrPrtls.m_paNum; k++)
			{
				CFluidPrtl* pPrtl2 = ngbrPrtls[k];
				if (pPrtl2 == pPrtl1 || pPrtl2 == pPrtl0) continue;
				bisecPlaneNormal.SetValueAsSubstraction(pPrtl2->m_vpPos, pPrtl->m_vpPos);
				bisecPlaneNormal.Normalize();
				if (fabs(circumCrlNormal.DotProduct(&bisecPlaneNormal)) <= SIMU_VALUE_EPSILON)
				{ // zero volume tetrahedron, ie, 4 coplanar points
					if (circumCrlCtr.m_p3dPos.GetDistanceSquareToVector(pPrtl2->m_vpPos)
					  > circumCrlCtr.m_p3dPos.GetDistanceSquareToVector(pPrtl->m_vpPos))
						continue; // tetraP outside the circum circle
					// move 4th point a little to break coplanary
					pPrtl2->m_vpPos->AddedBy(&circumCrlNormal, SIMU_PARA_MIN_PRTL_DISTANCE_RATIO);
					bisecPlaneNormal.SetValueAsSubstraction(pPrtl2->m_vpPos, pPrtl->m_vpPos);
					bisecPlaneNormal.Normalize();
					if (fabs(circumCrlNormal.DotProduct(&bisecPlaneNormal)) <= SIMU_VALUE_EPSILON)
					{
						CSimuMsg::ExitWithMessage(location, "4 points are still coplanar.");
						return false;
					}
				}
				bisecPlanePos.SetValueAsAddition(pPrtl2->m_vpPos, pPrtl->m_vpPos);
				bisecPlanePos.ScaledBy(0.5);
				if (!CSimuUtility::LineIntersectPlane(&circumCrlCtr.m_p3dPos, &circumCrlNormal,
													  &bisecPlanePos, &bisecPlaneNormal,
													  &circumSphCtr.m_p3dPos))
				{
					CSimuMsg::ExitWithMessage(location, "Circum sphere center is not found.");
					return false;
				}
				circumRSqr = circumSphCtr.m_p3dPos.GetDistanceSquareToVector(pPrtl->m_vpPos);
				if (circumRSqr < upsampleMinRadius*upsampleMinRadius
				 || circumRSqr > upsampleMaxRadius*upsampleMaxRadius)
					continue;
				m_dtrCircumCenter = &circumSphCtr;
				m_dtrCircumRSqr = circumRSqr;
				CSimuMsg::CancelablePauseMessage("Test a sphere", m_dtrPause[0]);
				int m;
				for (m=0; m < ngbrPrtls.m_paNum; m++)
				{
					CFluidPrtl* testPrtl = ngbrPrtls[m];
					if (testPrtl == pPrtl2 || testPrtl == pPrtl1
					 || testPrtl == pPrtl0) continue;
					distSqr = circumSphCtr.m_p3dPos.GetDistanceSquareToVector(testPrtl->m_vpPos);
					if (distSqr < circumRSqr)
						break; // the circumsphere is not empty
				}
				if (m >= ngbrPrtls.m_paNum)
				{
					m_dtrCurPrtl = pPrtl;
					m_dtrPrtlNgbrs = &ngbrPrtls;
					m_dtrCurPrtl->SetVirtualPrtlColor(CDelaunayTetrahedron::m_dtrCtrColor);
					pPrtl0->SetVirtualPrtlColor(CDelaunayTetrahedron::m_dtrTriPColor);
					pPrtl1->SetVirtualPrtlColor(CDelaunayTetrahedron::m_dtrTriPColor);
					pPrtl2->SetVirtualPrtlColor(CDelaunayTetrahedron::m_dtrTetraPColor);
					CSimuMsg::CancelablePauseMessage("Found an upsample pos.", m_dtrPause[1]);
					m_dtrCurPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_INSIDE);
					pPrtl0->SetVirtualPrtlColor(CSimuManager::COLOR_INSIDE);
					pPrtl1->SetVirtualPrtlColor(CSimuManager::COLOR_INSIDE);
					pPrtl2->SetVirtualPrtlColor(CSimuManager::COLOR_INSIDE);
					m_dtrCurPrtl = NULL;
					m_dtrPrtlNgbrs = NULL;
					m_dtrCircumCenter = NULL;
					upsampleP->m_p3dPos.SetValue(&circumSphCtr.m_p3dPos);
					idi = i; idj = j; idk = k+1; // prepare for resume the search
					return true; // the circumsphere is empty
				} // end of loop-m
				m_dtrCircumCenter = NULL;
			} // end of loop-k
			idk = 0;
		} // end of loop-j
		idj = 0;
	}
	return false;
}

void CDelaunayTetrahedron::DelaunayTessellation(CFluidPrtl* pPrtl,
												TGeomElemArray<CFluidPrtl> &ngbrPrtls,
												SimuValue tetraEnlargeSize,
												TGeomElemArray<CPoint3DOnTetra> &dataPoints,
												TGeomElemArray<CTetraDelaunay> &tetrahedra,
												CPoint3DOnTetra* &ctrPoint,
												CPoint3DOnTetra* boundingPs[4])
{
	InitializeTessellation(pPrtl, ngbrPrtls, tetraEnlargeSize,
							dataPoints, tetrahedra, ctrPoint, boundingPs);
	// debug variables
	m_dtrCtrPoint = ctrPoint;
	m_dtrDataPoints = &dataPoints;
	m_dtrTetras = &tetrahedra;
	// add data points into delaunay tessellation
	for (int i=0; i < dataPoints.m_paNum; i++)
	{
		CPoint3DOnTetra* pPoint = dataPoints[i];
		if (pPoint->m_p3dotTetras->m_paNum > 0)
			continue; // this prevents first 4 points and ctrPoint from being added.

		AddPointToDelaunayTessellation(pPoint, dataPoints, tetrahedra, ctrPoint);
	}
//	ConfirmDelaunayTessellation(tetrahedra, dataPoints);

	CSimuMsg::CancelablePauseMessage("Finish delaunay tessellation.", m_dtrPause[17]);
	m_dtrCtrPoint = NULL;
	m_dtrDataPoints = NULL;
	m_dtrTetras = NULL;
}

bool CDelaunayTetrahedron::AddPointToDelaunayTessellation(CPoint3DOnTetra* newPoint,
														  TGeomElemArray<CPoint3DOnTetra>
														  &dataPoints,
														  TGeomElemArray<CTetraDelaunay>
														  &tetrahedra,
														  CPoint3DOnTetra* ctrPoint)
{
	if (m_dtrRunDebugVersion)
	{
		m_dtrCtrPoint = ctrPoint;
		m_dtrDataPoints = &dataPoints;
		m_dtrTetras = &tetrahedra;
		m_dtrNewDataPoint = newPoint;
		m_dtrNewDataPoint->SetPoint3DColor(m_dtrTestPColor);
		bool bResult = AddPointToDelaunayDebugVersion(newPoint, dataPoints, tetrahedra, ctrPoint);
		m_dtrCtrPoint = NULL;
		m_dtrDataPoints = NULL;
		m_dtrTetras = NULL;
		m_dtrNewDataPoint = NULL;
		return bResult;
	}

	char location[] = "CDelaunayTetrahedron::AddPointToDelaunayTessellation(...)";

	// find the first intersect tetrahedron
	CTetraDelaunay* enclosingTetra = SearchIntersectTetra(newPoint, ctrPoint, tetrahedra);
	if (enclosingTetra == NULL)
	{
		return false;
	}
	// search for enclosing tetrahedra through adjacency
	// <intersectTetra> will be deregisted from grid and, 
	// in case of coplanarity, it is used for undeleting tetrahedra
	TGeomElemArray<CTetraDelaunay> intersectTetra;
	intersectTetra.AppendOnePointer(enclosingTetra);
	enclosingTetra->m_geDeleted = true; // label it for later delete
	int curArrayId = 0;
	// collect triangles for new tetra
	TGeomElemArray<CTriangleOnTetra> triangles(true, SIMU_PARA_NGBR_TRI_ALLOC_SIZE);
	while (curArrayId < intersectTetra.m_paNum)
	{
		// get the current element in queue
		CTetraDelaunay* pTetra = intersectTetra[curArrayId++];
		for (int k=0; k < 4; k++)
		{
			CTetraDelaunay* ngbrTetra = (CTetraDelaunay*)pTetra->m_ttrNgbrTetras[k];
			if (ngbrTetra != NULL)
			{
				if (ngbrTetra->m_geDeleted) continue;
				if (IsPointEnclosedByCircumSphere(newPoint, ngbrTetra, m_dtrRadiusSqrTolerance))
				{ // if ngbrTetra intersects newPoint, append it to queue
					intersectTetra.AppendOnePointer(ngbrTetra);
					ngbrTetra->m_geDeleted = true; // label it for later delete
					continue;
				}
			}
			// find one triangle that will form a new tetrahedron with newPoint
			CPoint3DOnTetra* triPs[3];
			pTetra->GetOppositePoints(k, triPs);
			if (CSimuUtility::ArePositionsCoplanar(&triPs[0]->m_p3dPos,
												   &triPs[1]->m_p3dPos,
												   &triPs[2]->m_p3dPos,
												   &newPoint->m_p3dPos,
												   m_dtrMinTetraVolume))
			{	// a degenerate tetra is found and abort addition of pPoint.
				// undelete tetrahedra
				for (int j=0; j < intersectTetra.m_paNum; j++)
				{
					intersectTetra[j]->m_geDeleted = false;
				}
				return false;
			}
			// prepare triangles for new tetrahedra
			CTriangleOnTetra* newTri = triangles.CreateOneElement();
			newTri->SetTrianglePoints(triPs[0], triPs[1], triPs[2]);
		}
	} // end of while on tetrahedral stack
	DeleteTetrahedraBeingLabeled(intersectTetra);
	TGeomElemArray<CTetraDelaunay> newTetras(true, SIMU_PARA_NGBR_TETRA_ALLOC_SIZE);;
	CreateNewTetrahedraFromTriangles(newPoint, triangles, newTetras);
	for (int j=0; j < newTetras.m_paNum; j++)
	{
		CTetraDelaunay* pTetra = newTetras[j];
		if (pTetra->SetupNgbrTetra() == false)
			return false;
	}
	tetrahedra.TakeOverPointersFromArray(newTetras);
	return true;
}

bool CDelaunayTetrahedron::AddPointToDelaunayDebugVersion(CPoint3DOnTetra* newPoint,
														  TGeomElemArray<CPoint3DOnTetra>
														  &dataPoints,
														  TGeomElemArray<CTetraDelaunay>
														  &tetrahedra,
														  CPoint3DOnTetra* ctrPoint)
{
	char location[] = "CDelaunayTetrahedron::AddPointToDelaunayDebugVersion(...)";

	// debug variables
	CSimuMsg::CancelablePauseMessage("Try to add new data point.", m_dtrPause[11]);
	ConfirmDelaunayTessellation(tetrahedra, dataPoints);
	// find the first intersect tetrahedron
	CTetraDelaunay* enclosingTetra = SearchIntersectTetra(newPoint, ctrPoint, tetrahedra);
	if (enclosingTetra == NULL)
	{
		return false;
	}
	// debug variables
	enclosingTetra->SetTetraPointColor(m_dtrTetraPColor);
	ctrPoint->SetPoint3DColor(m_dtrCtrColor);
	CSimuMsg::CancelablePauseMessage("Enclosing tetrahedron is found.", m_dtrPause[12]);
	enclosingTetra->SetTetraPointColor(m_dtrNgbrPColor);
	ctrPoint->SetPoint3DColor(m_dtrCtrColor);
	
	// search for enclosing tetrahedra through adjacency
	// <intersectTetra> will be deregisted from grid and, 
	// in case of coplanarity, it is used for undeleting tetrahedra
	TGeomElemArray<CTetraDelaunay> intersectTetra;
	intersectTetra.AppendOnePointer(enclosingTetra);
	enclosingTetra->m_geDeleted = true; // label it for later delete
	int curArrayId = 0;
	// collect triangles for new tetra
	TGeomElemArray<CTriangleOnTetra> triangles(true, SIMU_PARA_NGBR_TRI_ALLOC_SIZE);
	while (curArrayId < intersectTetra.m_paNum)
	{
		// get the current element in queue
		CTetraDelaunay* pTetra = intersectTetra[curArrayId++];
		for (int k=0; k < 4; k++)
		{
			CTetraDelaunay* ngbrTetra = (CTetraDelaunay*)pTetra->m_ttrNgbrTetras[k];
			if (ngbrTetra != NULL)
			{
				if (ngbrTetra->m_geDeleted) continue;
				// debug variables
				ngbrTetra->SetTetraPointColor(m_dtrTetraPColor);
				ctrPoint->SetPoint3DColor(m_dtrCtrColor);
				CSimuMsg::CancelablePauseMessage("Check neighbor tetrahedron.", m_dtrPause[13]);
				ngbrTetra->SetTetraPointColor(m_dtrNgbrPColor);
				ctrPoint->SetPoint3DColor(m_dtrCtrColor);
				if (IsPointEnclosedByCircumSphere(newPoint, ngbrTetra, m_dtrRadiusSqrTolerance))
				{ // if ngbrTetra intersects newPoint, append it to queue
					intersectTetra.AppendOnePointer(ngbrTetra);
					ngbrTetra->m_geDeleted = true; // label it for later delete
					// debug variables
					ngbrTetra->SetTetraPointColor(m_dtrTetraPColor);
					ctrPoint->SetPoint3DColor(m_dtrCtrColor);
					CSimuMsg::CancelablePauseMessage("Intersecting tetrahedron is found.", m_dtrPause[14]);
					ngbrTetra->SetTetraPointColor(m_dtrNgbrPColor);
					ctrPoint->SetPoint3DColor(m_dtrCtrColor);
					continue;
				}
			}
			// find one triangle that will form a new tetrahedron with newPoint
			CPoint3DOnTetra* triPs[3];
			pTetra->GetOppositePoints(k, triPs);
			if (CSimuUtility::ArePositionsCoplanar(&triPs[0]->m_p3dPos,
												   &triPs[1]->m_p3dPos,
												   &triPs[2]->m_p3dPos,
												   &newPoint->m_p3dPos,
												   m_dtrMinTetraVolume))
			{	// a degenerate tetra is found and abort addition of pPoint.
				// undelete tetrahedra
				for (int j=0; j < intersectTetra.m_paNum; j++)
				{
					intersectTetra[j]->m_geDeleted = false;
				}
				return false;
			}
			// prepare triangles for new tetrahedra
			CTriangleOnTetra* newTri = triangles.CreateOneElement();
			newTri->SetTrianglePoints(triPs[0], triPs[1], triPs[2]);

			// debug variables
			newTri->SetTrianglePointColor(m_dtrTriPColor);
			CSimuMsg::CancelablePauseMessage("New triangle is created.", m_dtrPause[9]);
			newTri->SetTrianglePointColor(m_dtrNgbrPColor);
		}
	} // end of while on tetrahedral stack
	// debug variables
	CSimuMsg::CancelablePauseMessage("Intersecting tetrahedra have been labeled being deleted.", m_dtrPause[15]);

	DeleteTetrahedraBeingLabeled(intersectTetra);

	TGeomElemArray<CTetraDelaunay> newTetras(true, SIMU_PARA_NGBR_TETRA_ALLOC_SIZE);;
	CreateNewTetrahedraFromTriangles(newPoint, triangles, newTetras);
	for (int j=0; j < newTetras.m_paNum; j++)
	{
		CTetraDelaunay* pTetra = newTetras[j];
		if (pTetra->SetupNgbrTetra() == false)
			return false;
	}
	tetrahedra.TakeOverPointersFromArray(newTetras);

	dataPoints.AssertGeometricElementConnectivity();
	tetrahedra.AssertGeometricElementConnectivity();
	ConfirmDelaunayTessellation(tetrahedra, dataPoints);
	CSimuMsg::CancelablePauseMessage("Added new data point", m_dtrPause[16]);

	return true;
}

// find the first intersecting tetrahedron; if not found, return NULL
CTetraDelaunay* CDelaunayTetrahedron::SearchIntersectTetra(CPoint3DOnTetra* pPoint,
														   CPoint3DOnTetra* ctrPoint,
														   TGeomElemArray<CTetraDelaunay>
														   &tetrahedra)
{
	if (m_dtrOnlyOnCtrPoint)
	{
		for (int j = 0; j < ctrPoint->m_p3dotTetras->m_paNum; j++)
		{
			CTetraDelaunay* pTetra = (CTetraDelaunay*)ctrPoint->m_p3dotTetras->m_paPtrs[j];
			if (IsPointEnclosedByCircumSphere(pPoint, pTetra, m_dtrRadiusSqrTolerance))
			{ // if pTetra encloses pPoint, return it.
				return pTetra;
			}
		}
	}
	else
	{
		for (int j = 0; j < tetrahedra.m_paNum; j++)
		{
			CTetraDelaunay* pTetra = tetrahedra[j];
			if (IsPointEnclosedByCircumSphere(pPoint, pTetra, m_dtrRadiusSqrTolerance))
			{ // if pTetra encloses pPoint, return it.
				return pTetra;
			}
		}
	}
	return NULL;
}

void CDelaunayTetrahedron::DeleteTetrahedraBeingLabeled(TGeomElemArray<CTetraDelaunay>
														&tetrahedra)
{
	for (int j=0; j < tetrahedra.m_paNum; j++)
	{
		CTetraDelaunay* pTetra = tetrahedra[j];
		if (!pTetra->m_geDeleted) continue;
		for (int k=0; k < 4; k++)
		{
			// take pTetra off its tetra points
			if (!pTetra->m_ttrPs[k]->m_p3dotTetras->RemoveOnePointer(pTetra))
				CSimuMsg::ExitWithMessage("CDelaunayTetrahedron::DeleteTetrahedraBeingLabeled(...)",
											"Tetrahedron is not found on point.");

			CTetraDelaunay* ngbrTetra = (CTetraDelaunay*)pTetra->m_ttrNgbrTetras[k];
			if (ngbrTetra == NULL) continue;
			if (ngbrTetra->m_geDeleted) continue;
			// take pTetra off its ngbr tetras
			ngbrTetra->ReplaceNgbrTetra(pTetra, NULL);
		}
	}
}

void CDelaunayTetrahedron::CreateNewTetrahedraFromTriangles(CPoint3DOnTetra* pPoint,
															TGeomElemArray<CTriangleOnTetra>
															&triangles,
															TGeomElemArray<CTetraDelaunay>
															&newTetras)
{
	for (int j=0; j < triangles.m_paNum; j++)
	{
		CTriangleOnTetra* pTri = triangles[j];
		if (m_dtrRunDebugVersion)
			ConfirmEmptyNgbrTetra(pTri->m_totTriPs);
//		if (m_dtrRunDebugVersion)
//			CSimuMsg::PauseMessage("Before create new tetra.");
		CTetraDelaunay* newTetra = newTetras.CreateOneElement();
		newTetra->SetTetrahedronPoints(pTri->m_totTriPs[0],
									   pTri->m_totTriPs[1],
									   pTri->m_totTriPs[2],
									   pPoint);
	}
}

void CDelaunayTetrahedron::ConfirmEmptyNgbrTetra(CPoint3DOnTetra* triPs[3])
{
	char location[] = "CDelaunayTetrahedron::ConfirmEmptyNgbrTetra()";

	for (int j=0; j < triPs[0]->m_p3dotTetras->m_paNum; j++)
	{
		CTetrahedron* pTet = triPs[0]->m_p3dotTetras->m_paPtrs[j];
		if (pTet->HasTetraPoint(triPs[1])
		 && pTet->HasTetraPoint(triPs[2]))
		{
			CTetrahedron* ngbrTet = pTet->GetNgbrTetra(triPs);
			if (ngbrTet != NULL)
				CSimuMsg::ExitWithMessage(location, "Tetra ngbr relation is not correct.");
		}
	}
}

CTetraDelaunay* CDelaunayTetrahedron::SearchTetraToUpsample(CPoint3DOnTetra* ctrPoint,
															SimuValue upsampleMinRadiusSqr,
															SimuValue upsampleMaxRadiusSqr)
{
	for (int j=0; j < ctrPoint->m_p3dotTetras->m_paNum; j++)
	{
		CTetraDelaunay* pTetra = (CTetraDelaunay*)ctrPoint->m_p3dotTetras->m_paPtrs[j];
		if (pTetra->m_ttrdNoUpsample) continue;
		if (pTetra->m_ttrdCircumRSqr >= upsampleMinRadiusSqr
		 && pTetra->m_ttrdCircumRSqr <= upsampleMaxRadiusSqr)
		{
			return pTetra;
		}
	}
	return NULL;
}

void CDelaunayTetrahedron::GetDistinctPointFromFluidPrtl(TPointerArray<CFluidPrtl>
														 &dataPrtls,
														 TGeomElemArray<CPoint3DOnTetra>
														 &dataPoints,
														 SimuValue minPointDistSqr)
{
	for (int j=0; j < dataPrtls.m_paNum; j++)
	{
		CFluidPrtl* pPrtl = dataPrtls[j];
		if (pPrtl->m_fpNoDelaunay) continue;
/*		for (int k=0; k < dataPoints.m_paNum; k++)
		{
			CPoint3DOnTetra* existingP = dataPoints[k];
			SimuValue distSqr = existingP->m_p3dPos.GetDistanceSquareToVector(pPrtl->m_vpPos);
			if (distSqr <= minPointDistSqr)
				break;
		}
		if (k < dataPoints.m_paNum)
			continue; // skip colocation points
*/		CPoint3DOnTetra* pPoint = dataPoints.CreateOneElement();
		pPoint->m_p3dPos.SetValue(pPrtl->m_vpPos);
//		pPoint->SetPoint3DColor(m_dtrNgbrPColor);
	}
}

CPoint3DOnTetra* CDelaunayTetrahedron::GetTrianglePoint(CPoint3DOnTetra* edgeP0,
														CPoint3DOnTetra* edgeP1,
														TGeomElemArray<CPoint3DOnTetra>
														&dataPoints)
{
	CPoint3DOnTetra* edgePs[2] = {edgeP0, edgeP1};
	CVector3D circumCtrPos;
	SimuValue circumRSqr;
	for (int i=0; i < dataPoints.m_paNum; i++)
	{
		CPoint3DOnTetra* triPoint = dataPoints[i];
		if (triPoint == edgePs[0] || triPoint == edgePs[1]) continue;
		if (!CSimuUtility::GetCircumCircleCenter(&edgePs[0]->m_p3dPos, &edgePs[1]->m_p3dPos,
												 &triPoint->m_p3dPos, &circumCtrPos))
			continue;
		circumRSqr = circumCtrPos.GetDistanceSquareToVector(&triPoint->m_p3dPos);
		// check if other points are closer to the circum center
		int j;
		for (j=0; j < dataPoints.m_paNum; j++)
		{
			CPoint3DOnTetra* testPoint = dataPoints[j];
			if (testPoint == triPoint || testPoint == edgePs[0] || testPoint == edgePs[1])
				continue;
			if (testPoint->m_p3dPos.GetDistanceSquareToVector(&circumCtrPos) < circumRSqr)
				break;
		}
		if (j >= dataPoints.m_paNum)
			return triPoint; // no other points are closer to the mid points
	}
	return NULL;
}

CPoint3DOnTetra* CDelaunayTetrahedron::GetTetrahedronPoint(CPoint3DOnTetra* triP0,
														   CPoint3DOnTetra* triP1,
														   CPoint3DOnTetra* triP2,
														   CPoint3DOnTetra* avoidP,
														   CPoint3D &circumSphCtr,
														   SimuValue &circumRSqr,
														   TGeomElemArray<CPoint3DOnTetra>
														   &dataPoints)
{
	char location[] = "CDelaunayTetrahedron::GetTetrahedronPoint(...)";

	CPoint3D circumCrlCtr;
	CVector3D circumCrlNormal;
	CVector3D bisecPlaneNormal;
	CVector3D bisecPlanePos;
	SimuValue distSqr;
	for (int i=0; i < dataPoints.m_paNum; i++)
	{
		CPoint3DOnTetra* tetraP = dataPoints[i];
		if (tetraP == triP0 || tetraP == triP1 
		 || tetraP == triP2 || tetraP == avoidP) continue;
		if (!CSimuUtility::GetCircumCircleCenter(&triP0->m_p3dPos, &triP1->m_p3dPos,
												 &triP2->m_p3dPos, &circumCrlCtr.m_p3dPos))
		{
			CSimuMsg::ExitWithMessage(location, "Circum circle center is not found.");
			return NULL;
		}
		CSimuUtility::ComputeTriangleNormal(&triP0->m_p3dPos, &triP1->m_p3dPos,
											&triP2->m_p3dPos, &circumCrlNormal);
		bisecPlaneNormal.SetValueAsSubstraction(&triP0->m_p3dPos, &tetraP->m_p3dPos);
		bisecPlaneNormal.Normalize();
		if (fabs(circumCrlNormal.DotProduct(&bisecPlaneNormal)) <= SIMU_VALUE_EPSILON)
		{ // zero volume tetrahedron, ie, 4 coplanar points
			if (circumCrlCtr.m_p3dPos.GetDistanceSquareToVector(&tetraP->m_p3dPos)
			  > circumCrlCtr.m_p3dPos.GetDistanceSquareToVector(&triP0->m_p3dPos))
				continue; // tetraP outside the circum circle
			// move 4th point a little to break coplanary
			tetraP->m_p3dPos.AddedBy(&circumCrlNormal, SIMU_PARA_MIN_PRTL_DISTANCE_RATIO);
			bisecPlaneNormal.SetValueAsSubstraction(&triP0->m_p3dPos, &tetraP->m_p3dPos);
			bisecPlaneNormal.Normalize();
			if (fabs(circumCrlNormal.DotProduct(&bisecPlaneNormal)) <= SIMU_VALUE_EPSILON)
			{
				CSimuMsg::ExitWithMessage(location, "4 points are still coplanar.");
				return NULL;
			}
		}
		bisecPlanePos.SetValueAsAddition(&triP0->m_p3dPos, &tetraP->m_p3dPos);
		bisecPlanePos.ScaledBy(0.5);
		if (!CSimuUtility::LineIntersectPlane(&circumCrlCtr.m_p3dPos, &circumCrlNormal,
											  &bisecPlanePos, &bisecPlaneNormal,
											  &circumSphCtr.m_p3dPos))
		{
			CSimuMsg::ExitWithMessage(location, "Circum sphere center is not found.");
			return NULL;
		}
		tetraP->SetPoint3DColor(m_dtrTetraPColor);
		m_dtrCircumCenter = NULL;
		circumRSqr = circumSphCtr.m_p3dPos.GetDistanceSquareToVector(&triP0->m_p3dPos);
		m_dtrCircumCenter = &circumSphCtr;
		m_dtrCircumRSqr = circumRSqr;
		CSimuMsg::CancelablePauseMessage("Test a sphere", m_dtrPause[7]);
		int j;
		for (j=0; j < dataPoints.m_paNum; j++)
		{
			CPoint3DOnTetra* testPoint = dataPoints[j];
			if (testPoint == triP0 || testPoint == triP1
			 || testPoint == triP2 || testPoint == tetraP) continue;
			distSqr = circumSphCtr.m_p3dPos.GetDistanceSquareToVector(&testPoint->m_p3dPos);
			if (distSqr < circumRSqr)
				break; // the circumsphere is not empty
		}
		if (j >= dataPoints.m_paNum)
		{
			m_dtrCircumCenter = NULL;
			CSimuMsg::CancelablePauseMessage("Found a tetra point.", m_dtrPause[8]);
			return tetraP; // the circumsphere is empty, and a valid tetraP is found.
		}
		tetraP->SetPoint3DColor(m_dtrNgbrPColor);
	}
	m_dtrCircumCenter = NULL;
	return NULL;
}

// if point is at ambiguous distance to circum sphere, its pos is jittered.
bool CDelaunayTetrahedron::IsPointEnclosedByCircumSphere(CPoint3D* pPoint,
														 CTetraDelaunay* pTetra,
														 SimuValue distTolerance)
{
	SimuValue distSqr = pTetra->m_ttrdCircumCtrPos.GetDistanceSquareToVector(
													&pPoint->m_p3dPos);
	distSqr += distTolerance;
	if (distSqr < pTetra->m_ttrdCircumRSqr)
	{
		return true; // point is inside circum sphere
	}
	else // (distSqr >= pTetra->m_ttrdCircumRSqr)
	{
		return false; // point is on or outside circum sphere
	}
}

void CDelaunayTetrahedron::ConfirmDelaunayTessellation(TGeomElemArray<CTetraDelaunay>
													   &tetrahedra,
													   TGeomElemArray<CPoint3DOnTetra>
													   &points)
{
	m_dtrDataPoints = &points;

	tetrahedra.CleanDeletedElements();
	SimuValue sqrTolerance = pow(CSimuManager::m_prtlDistance*0.001, 2);
	for (int i=0; i < tetrahedra.m_paNum; i++)
	{
		CTetraDelaunay* pTetra = tetrahedra[i];
		ConfirmDelaunayTetrahedron(points, pTetra, sqrTolerance);
	}

	m_dtrDataPoints = NULL;
}

void CDelaunayTetrahedron::ConfirmDelaunayTetrahedron(TGeomElemArray<CPoint3DOnTetra> &points,
													  CTetraDelaunay* pTetra,
													  SimuValue distTolerance)
{
	for (int j=0; j < points.m_paNum; j++)
	{
		CPoint3DOnTetra* pPoint = points[j];
		if (pPoint->m_p3dotTetras->m_paNum <= 0)
			continue;
		if (pTetra->HasTetraPoint(pPoint))
			continue;
		if (pPoint->IsAnyTetraDeleted())
			continue;
		if (IsPointEnclosedByCircumSphere(pPoint, pTetra, distTolerance))
		{
			CPoint3D circumSphCtr;
			circumSphCtr.m_p3dPos.SetValue(&pTetra->m_ttrdCircumCtrPos);
			circumSphCtr.SetPoint3DColor(m_dtrCircumCtrColor);
			m_dtrCircumCenter = &circumSphCtr;
			m_dtrCircumRSqr = pTetra->m_ttrdCircumRSqr;
			SimuValue distSqr = pTetra->m_ttrdCircumCtrPos.GetDistanceSquareToVector(
															&pPoint->m_p3dPos);

			pTetra->SetTetraPointColor(m_dtrTetraPColor);
			pPoint->SetPoint3DColor(m_dtrTestPColor);
			SimuValue circumRadius = sqrt(pTetra->m_ttrdCircumRSqr);
			SimuValue distToCtr = sqrt(distSqr);
			sprintf(CSimuMsg::GetEmptyBuf(), "This is not a delaunay tetrahedron: circumR = %f, distance = %f",
											circumRadius, distToCtr);
			CSimuMsg::ExitWithMessage("CDelaunayTetrahedron::ConfirmDelaunayTetrahedron(...)",
										CSimuMsg::GetCheckedBuf());

			IsPointEnclosedByCircumSphere(pPoint, pTetra, distTolerance);

			m_dtrCircumCenter = NULL;
		}
	}
}

int CDelaunayTetrahedron::GetNumOfPointsOnTetra(TGeomElemArray<CPoint3DOnTetra> &points)
{
	int numPointsOnTetra = 0;
	for (int j=0; j < points.m_paNum; j++)
	{
		CPoint3DOnTetra* pPoint = points[j];
		if (pPoint->m_p3dotTetras->m_paNum > 0)
			numPointsOnTetra++;
	}
	return numPointsOnTetra;
}
