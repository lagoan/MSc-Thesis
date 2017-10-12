// Grid.h: interface for the CGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_H__4F8C5F11_3275_43E4_B528_1A24A9CCBDF8__INCLUDED_)
#define AFX_GRID_H__4F8C5F11_3275_43E4_B528_1A24A9CCBDF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGrid  
{
public:
	SimuValue m_gCellSize;
	CVector3D m_gMinPos, m_gMaxPos; // boundary of grid structure
	UINT m_gPointDim[Z+1];
	UINT m_gNumPoints;
	UINT m_gNumPointsOnYZPlane;
	UINT m_gCellDim[Z+1];
	UINT m_gNumCells;
	UINT m_gNumCellsOnYZPlane;
	UINT m_gInsidePointDim[Z+1];
	UINT m_gNumInsidePoints;
	UINT m_gNumInsidePointsOnYZPlane;
	
	CGrid();
	virtual ~CGrid();

	void DrawGridBoundingEdges();
	void DrawGridCell(int idx, int idy, int idz);

	void UpdateGridBoundaryAndDimension(SimuValue minPos[3], SimuValue maxPos[3]);
	void UpdateGridNumbers();
	void AssignGridPositionsToPrtls(int numPrtls, CVirtualPrtl** ptrPrtls);
	int GetPointArrayIdFrom3DGridId(int idx, int idy, int idz);
	void Get3DGridIdFromPointArrayId(int aryId, int gridId[3]);
	int GetCellArrayIdFrom3DGridCellId(int idx, int idy, int idz);
	int GetInsidePointArrayIdFromInsidePoint3DGridId(int idx, int idy, int idz);
	void Get3DGridIdFromInsidePointArrayId(int aryId, int gridId[3]);
	bool GetGridCellIdForInsidePosition(CVector3D &pos, int gridId[Z+1]);
	void GetOverlapGridCellIdRangeForCube(CVector3D &pos, SimuValue radius,
										  int minId[Z+1], int maxId[Z+1]);
	void GetOverlapGridCellIdRangeForBox(CVector3D &minPos, CVector3D &maxPos,
										int minCellId[Z+1], int maxCellId[Z+1]);
	void GetIdRangeOfCellsToCoverBox(CVector3D &minPos, CVector3D &maxPos,
									int minCellId[Z+1], int maxCellId[Z+1]);
	void GetGridIdRangeToCoverBox(CVector3D &minPos, CVector3D &maxPos,
									int minId[Z+1], int maxId[Z+1]);
	void GetGridCellsIntersectedByRay(CVector3D &pos, CVector3D &normal,
									  TClassArray<CIndex3D> &aryCells);
	void GetGridCellsIntersectedBySegmentIn2D(UINT dd0, UINT dd1, UINT d2, int dim2,
											  CVector3D &posA, CVector3D &posB,
											  CVector3D &normal,
											  TClassArray<CIndex3D> &aryCells);
};

#endif // !defined(AFX_GRID_H__4F8C5F11_3275_43E4_B528_1A24A9CCBDF8__INCLUDED_)
