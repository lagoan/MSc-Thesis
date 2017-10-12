// Grid.cpp: implementation of the CGrid class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Grid.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrid::CGrid()
{
	m_gCellSize = 0;
	m_gPointDim[X] = 0;
	m_gPointDim[Y] = 0;
	m_gPointDim[Z] = 0;
	m_gNumPoints = 0;
	m_gNumPointsOnYZPlane = 0;
	m_gCellDim[X] = 0;
	m_gCellDim[Y] = 0;
	m_gCellDim[Z] = 0;
	m_gNumCells = 0;
	m_gNumCellsOnYZPlane = 0;
}

CGrid::~CGrid()
{

}

void CGrid::DrawGridBoundingEdges()
{
	SimuColor3v(CSimuManager::COLOR_GREEN->v);
	CSimuUtility::DrawBoundingBox(m_gMinPos, m_gMaxPos);
}

void CGrid::DrawGridCell(int idx, int idy, int idz)
{
	CVector3D points[8];
	for (int x=0; x <= 1; x++)
	for (int y=0; y <= 1; y++)
	for (int z=0; z <= 1; z++)
	{
		int pId = 4*x + 2*y + z;
		points[pId][X] = m_gMinPos[X] + (idx + x) * m_gCellSize;
		points[pId][Y] = m_gMinPos[Y] + (idy + y) * m_gCellSize;
		points[pId][Z] = m_gMinPos[Z] + (idz + z) * m_gCellSize;
		CSimuUtility::DrawSphereOr3DCrossAtPosition(&points[pId], CSimuManager::m_sphereRadius);
	}
}

void CGrid::UpdateGridBoundaryAndDimension(SimuValue minPos[3], SimuValue maxPos[3])
{
	char location[] = "CGrid::UpdateGridBoundaryAndDimension(...)";

	if (m_gCellSize <= SIMU_VALUE_EPSILON)
		CSimuMsg::ExitWithMessage(location, "Grid cell size is not positive.");

	for (unsigned int d=X; d <= Z; d++)
	{
		// update grid boundary: enlarge it to grid locations
		m_gMinPos[d] = m_gCellSize*(int)floor(minPos[d]/m_gCellSize);
		m_gMaxPos[d] = m_gCellSize*(int)ceil(maxPos[d]/m_gCellSize);
		// update grid dimension
		m_gPointDim[d] = 1+(int)ceil((m_gMaxPos[d] - m_gMinPos[d])/m_gCellSize);
		if (m_gPointDim[d] < 1)
			CSimuMsg::ExitWithMessage(location, "Grid dimension must be positive.");
		m_gCellDim[d] = m_gPointDim[d] - 1;
		m_gInsidePointDim[d] = m_gPointDim[d] - 2;
	}
}

void CGrid::UpdateGridNumbers()
{
	m_gNumPointsOnYZPlane = m_gPointDim[Y]*m_gPointDim[Z];
	m_gNumPoints = m_gPointDim[X]*m_gNumPointsOnYZPlane;

	m_gNumCellsOnYZPlane = m_gCellDim[Y]*m_gCellDim[Z];
	m_gNumCells = m_gCellDim[X]*m_gNumCellsOnYZPlane;

	m_gNumInsidePointsOnYZPlane = m_gInsidePointDim[Y]*m_gInsidePointDim[Z];
	m_gNumInsidePoints = m_gInsidePointDim[X]*m_gNumInsidePointsOnYZPlane;

	if (m_gNumPoints > pow(10.0f, 8))
		CSimuMsg::ExitWithMessage("CGrid::UpdateGridNumbers()",
									"Grid dimension is too large.");
}

void CGrid::AssignGridPositionsToPrtls(int numPrtls, CVirtualPrtl** ptrPrtls)
{
	if (numPrtls != m_gNumPoints)
		CSimuMsg::ExitWithMessage("CGrid::AssignGridPositionsToPrtls(numPrtls, ptrPrtls)",
								"Grid point number is not equal to prtl number.");
	CVector3D tmpPos;
	for (int x=0; x < m_gPointDim[X]; x++) { tmpPos[X] = m_gMinPos[X] + m_gCellSize * x;
	for (int y=0; y < m_gPointDim[Y]; y++) { tmpPos[Y] = m_gMinPos[Y] + m_gCellSize * y;
	for (int z=0; z < m_gPointDim[Z]; z++) { tmpPos[Z] = m_gMinPos[Z] + m_gCellSize * z;

		int aryId = GetPointArrayIdFrom3DGridId(x, y, z);
		ptrPrtls[aryId]->m_vpPos->SetValue(&tmpPos);
	}}}
}

int CGrid::GetPointArrayIdFrom3DGridId(int idx, int idy, int idz)
{
	return idx*m_gNumPointsOnYZPlane + idy*m_gPointDim[Z] + idz;
}

void CGrid::Get3DGridIdFromPointArrayId(int aryId, int gridId[3])
{
	gridId[X] = (int)floor(1.0*aryId/m_gNumPointsOnYZPlane);
	int remainNum = aryId - gridId[X]*m_gNumPointsOnYZPlane;
	gridId[Y] = (int)floor(1.0*remainNum/m_gPointDim[Z]);
	gridId[Z] = remainNum - gridId[Y]*m_gPointDim[Z];
}

int CGrid::GetCellArrayIdFrom3DGridCellId(int idx, int idy, int idz)
{
	return idx*m_gNumCellsOnYZPlane + idy*m_gCellDim[Z] + idz;
}

int CGrid::GetInsidePointArrayIdFromInsidePoint3DGridId(int idx, int idy, int idz)
{
	return idx*m_gNumInsidePointsOnYZPlane + idy*m_gInsidePointDim[Z] + idz;
}

void CGrid::Get3DGridIdFromInsidePointArrayId(int aryId, int gridId[3])
{
	gridId[X] = (int)floor(1.0*aryId/m_gNumInsidePointsOnYZPlane);
	int remainNum = aryId - gridId[X]*m_gNumInsidePointsOnYZPlane;
	gridId[Y] = (int)floor(1.0*remainNum/m_gInsidePointDim[Z]);
	gridId[Z] = remainNum - gridId[Y]*m_gInsidePointDim[Z];
}

bool CGrid::GetGridCellIdForInsidePosition(CVector3D &pos, int gridId[Z+1])
{
	bool bOutOfBoundary = false;
	for (int d=X; d <= Z; d++)
	{
		gridId[d] = (int)floor((pos[d] - m_gMinPos[d])/m_gCellSize);
		if (gridId[d] < 0)
		{
			gridId[d] = 0;
			bOutOfBoundary = true;
		}
		if (gridId[d] >= m_gCellDim[d])
		{
			gridId[d] = m_gCellDim[d] - 1;
			bOutOfBoundary = true;
		}
	}
	return bOutOfBoundary;
}

void CGrid::GetOverlapGridCellIdRangeForCube(CVector3D &pos, SimuValue radius,
											 int minId[Z+1], int maxId[Z+1])
{
	for (int d=X; d <= Z; d++)
	{
		minId[d] = (int)floor((pos[d] - radius - m_gMinPos[d])/m_gCellSize);
		maxId[d] = (int)floor((pos[d] + radius - m_gMinPos[d])/m_gCellSize);
		if (minId[d] < 0)
			minId[d] = 0;
		if (maxId[d] >= m_gCellDim[d])
			maxId[d] = m_gCellDim[d] - 1;
	}
}

void CGrid::GetOverlapGridCellIdRangeForBox(CVector3D &minPos, CVector3D &maxPos,
											int minCellId[Z+1], int maxCellId[Z+1])
{
	for (int d=X; d <= Z; d++)
	{
		minCellId[d] = (int)floor((minPos[d] - m_gMinPos[d])/m_gCellSize);
		maxCellId[d] = (int)floor((maxPos[d] - m_gMinPos[d])/m_gCellSize);
		if (minCellId[d] < 0)
			minCellId[d] = 0;
		if (maxCellId[d] >= m_gCellDim[d])
			maxCellId[d] = m_gCellDim[d] - 1;
	}
}

void CGrid::GetIdRangeOfCellsToCoverBox(CVector3D &minPos, CVector3D &maxPos,
										int minCellId[Z+1], int maxCellId[Z+1])
{
	char location[] = "CGrid::GetGridRangeForInsideBoundingBox(...)";

	for (int d=X; d <= Z; d++)
	{
		minCellId[d] = (int)floor((minPos[d] - m_gMinPos[d])/m_gCellSize);
		maxCellId[d] = (int)floor((maxPos[d] - m_gMinPos[d])/m_gCellSize);
		if (minCellId[d] < 0)
		{
			SimuValue delta = minPos[d] - m_gMinPos[d];
			if (delta < -m_gCellSize*SIMU_VALUE_EPSILON)
				CSimuMsg::ExitWithMessage(location, "Min id range is less than min id.",
											CSimuManager::m_bSkipWarning);
			minCellId[d] = 0;
		}
		else if (minCellId[d] >= m_gCellDim[d])
		{
			if (minPos[d] - m_gMaxPos[d] > SIMU_VALUE_EPSILON)
				CSimuMsg::ExitWithMessage(location, "Min id range is beyond max id.",
											CSimuManager::m_bSkipWarning);
			minCellId[d] = m_gCellDim[d] - 1;
		}
		if (maxCellId[d] < 0)
		{
			SimuValue delta = maxPos[d] - m_gMinPos[d];
			if (delta < -m_gCellSize*SIMU_VALUE_EPSILON)
				CSimuMsg::ExitWithMessage(location, "Max id range is less than min id.",
											CSimuManager::m_bSkipWarning);
			maxCellId[d] = 0;
		}
		else if (maxCellId[d] >= m_gCellDim[d])
		{
			if (maxPos[d] - m_gMaxPos[d] > SIMU_VALUE_EPSILON)
				CSimuMsg::ExitWithMessage(location, "Max id range is beyond max id.",
											CSimuManager::m_bSkipWarning);
			maxCellId[d] = m_gCellDim[d] - 1;
		}
	}
}

void CGrid::GetGridIdRangeToCoverBox(CVector3D &minPos, CVector3D &maxPos,
									 int minId[Z+1], int maxId[Z+1])
{
	char location[] = "CGrid::GetGridIdRangeToCoverBox(...)";

	for (int d=X; d <= Z; d++)
	{
		minId[d] = (int)floor((minPos[d] - m_gMinPos[d])/m_gCellSize);
		maxId[d] = (int)ceil((maxPos[d] - m_gMinPos[d])/m_gCellSize);
		if (minId[d] < 0)
		{
			CSimuMsg::ExitWithMessage(location, "Min id range is less than min id.",
										CSimuManager::m_bSkipWarning);
			minId[d] = 0;
		}
		if (maxId[d] >= m_gPointDim[d])
		{
			CSimuMsg::ExitWithMessage(location, "Max id range is beyond max id.",
										CSimuManager::m_bSkipWarning);
			maxId[d] = m_gPointDim[d] - 1;
		}
	}
}

void CGrid::GetGridCellsIntersectedByRay(CVector3D &pos, CVector3D &normal,
										 TClassArray<CIndex3D> &aryCells)
{
	char location[] = "CGrid::GetGridCellsIntersectedByRay(...)";

	if (pos[X] < m_gMinPos[X] || pos[X] > m_gMaxPos[X]
	 || pos[Y] < m_gMinPos[Y] || pos[Y] > m_gMaxPos[Y]
	 || pos[Z] < m_gMinPos[Z] || pos[Z] > m_gMaxPos[Z])
	{
		CSimuMsg::ExitWithMessage(location, "Ray starting position is out of grid.");
		return;
	}

	aryCells.ResetArraySize(0);

	unsigned int d, d0, d1, d2;
	for (d = 0; d < 3; d++)
		if (fabs(normal[d]) > SIMU_VALUE_EPSILON)
		{	d0 = d; d1 = (d+1)%3; d2 = (d+2)%3; break; }
	if (d == 3)
		CSimuMsg::ExitWithMessage(location, "Ray normal is zero.");

	CVector3D pA, pB;
	SimuValue segmentLen;
	int dim0;
	int dim0Adder	= normal[d0] > 0 ? 1 : 0;
	int dim0Step	= normal[d0] > 0 ? 1 : -1;
	pA.SetValue(&pos);
	dim0 = (int)floor((pos[d0] - m_gMinPos[d0])/m_gCellSize);
	while (true)
	{
		pB[d0] = m_gMinPos[d0] + (dim0 + dim0Adder) * m_gCellSize;
		segmentLen = (pB[d0] - pos[d0])/normal[d0];
		pB[d1] = pos[d1] + segmentLen * normal[d1];
		pB[d2] = pos[d2] + segmentLen * normal[d2];
		// on cell plane dim0, get cell id ranges of dim(d1, d2) for segment (pA, pB), 
		GetGridCellsIntersectedBySegmentIn2D(d1, d2, d0, dim0, pA, pB, normal, aryCells);
		if (pB[d0] <= m_gMinPos[d0] || pB[d0] >= m_gMaxPos[d0]
		 || pB[d1] <= m_gMinPos[d1] || pB[d1] >= m_gMaxPos[d1]
		 || pB[d2] <= m_gMinPos[d2] || pB[d2] >= m_gMaxPos[d2])
			return; // pB is on the grid boundary or outside
		// else pB is still inside the grid
		pA.SetValue(&pB);
		dim0 += dim0Step;
	}
}

void CGrid::GetGridCellsIntersectedBySegmentIn2D(unsigned int dd0, unsigned int dd1, unsigned int d2, int dim2,
												 CVector3D &posA, CVector3D &posB,
												 CVector3D &normal,
												 TClassArray<CIndex3D> &aryCells)
{
	char location[] = "CGrid::GetGridCellsIntersectedBySegmentIn2D(...)";

	unsigned int d0 = dd0, d1 = dd1;

	if (posA[d0] < m_gMinPos[d0] || posA[d0] > m_gMaxPos[d0]
	 || posA[d1] < m_gMinPos[d1] || posA[d1] > m_gMaxPos[d1]
	 || posA[d2] < m_gMinPos[d2] || posA[d2] > m_gMaxPos[d2])
	{
		CSimuMsg::ExitWithMessage(location, "Segment starting position is out of grid.");
		return;
	}
	if (fabs(normal[d0]) > SIMU_VALUE_EPSILON)		{ d0 = dd0; d1 = dd1; }
	else if (fabs(normal[d1]) > SIMU_VALUE_EPSILON)	{ d0 = dd1; d1 = dd0; }
	else // (normal[d0] == 0) && (normal[d1] == 0)
	{
		CIndex3D* newCellId = aryCells.CreateOneElement();
		newCellId->m_iIds[d0] = (int)floor((posA[d0] - m_gMinPos[d0])/m_gCellSize);
		newCellId->m_iIds[d1] = (int)floor((posA[d1] - m_gMinPos[d1])/m_gCellSize);
		newCellId->m_iIds[d2] = dim2;
		return;
	}
	// now, normal[d0] != 0

	int dim0Bdry = (int)floor((posB[d0] - m_gMinPos[d0])/m_gCellSize);
	int dim1Bdry = (int)floor((posB[d1] - m_gMinPos[d1])/m_gCellSize);

	CVector3D pA, pB;
	SimuValue segmentLen;
	int dim0;
	int dim0Adder	= normal[d0] > 0 ? 1 : 0;
	int dim0Step	= normal[d0] > 0 ? 1 : -1;
	pA.SetValue(&posA);
	dim0 = (int)floor((posA[d0] - m_gMinPos[d0])/m_gCellSize);
	while (true)
	{
		pB[d0] = m_gMinPos[d0] + (dim0 + dim0Adder) * m_gCellSize;
		segmentLen = (pB[d0] - posA[d0])/normal[d0];
		pB[d1] = posA[d1] + segmentLen * normal[d1];

		// on cell line dim0, get cell id range of dim(d1) for segment (pA, pB), 
		int dim1Lead = (int)floor((pA[d1] - m_gMinPos[d1])/m_gCellSize);
		int dim1Last = (int)floor((pB[d1] - m_gMinPos[d1])/m_gCellSize);
		int dim1Step = normal[d1] >= 0 ? 1 : -1;
		int dim1 = dim1Lead;
		while (CSimuUtility::NumberIsWithinRange(dim1, dim1Lead, dim1Last)
			&& CSimuUtility::NumberIsWithinRange(dim1, 0, m_gCellDim[d1]-1)
			&& (dim1*dim1Step <= dim1Bdry*dim1Step))  // this seems unnecessary due to dim0*dim0Step > dim0Bdry*dim0Step
		{
			CIndex3D* newCellId = aryCells.CreateOneElement();
			newCellId->m_iIds[d0] = dim0;
			newCellId->m_iIds[d1] = dim1;
			newCellId->m_iIds[d2] = dim2;
			dim1 += dim1Step;
		}
		if (dim1Last*dim1Step > dim1Bdry*dim1Step // this seems unnecessary due to dim0*dim0Step > dim0Bdry*dim0Step
		 || !CSimuUtility::NumberIsWithinRange(dim1Last, 0, m_gCellDim[d1]-1))
			return;
		pA.SetValue(&pB);
		dim0 += dim0Step;
		if (dim0*dim0Step > dim0Bdry*dim0Step
		 || !CSimuUtility::NumberIsWithinRange(dim0, 0, m_gCellDim[d0]-1))
			return;
	}
}
