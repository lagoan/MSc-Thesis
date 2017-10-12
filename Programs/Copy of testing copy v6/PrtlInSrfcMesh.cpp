// PrtlInSrfcMesh.cpp: implementation of the CPrtlInSrfcMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "PrtlInSrfcMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtlInSrfcMesh::CPrtlInSrfcMesh()
{
	m_pismGrid.m_gCellSize = CSimuManager::m_prtlDistance;
	m_pismGridPrtls.InitializeClassArray(true, SIMU_PARA_PRTL_NUM_ALLOC_SIZE);

	m_pismPrtl = NULL;
	m_pismTri = NULL;
	m_pismArySrfcMesh = NULL;
}

CPrtlInSrfcMesh::~CPrtlInSrfcMesh()
{

}

void CPrtlInSrfcMesh::RenderPrtlInSrfcMesh()
{
	if (CSimuManager::DRAW_MESHER_PRTL)
		m_pismGridPrtls.RenderGeometryElements();

	if (m_pismPrtl != NULL)
		m_pismPrtl->RenderGeometry();

	if (m_pismTri != NULL)
		m_pismTri->RenderGeometry();

	if (m_pismArySrfcMesh != NULL)
		m_pismArySrfcMesh->RenderGeometryElements();
}

void CPrtlInSrfcMesh::CreatePrtlInSrfcMesh(TGeomElemArray<CPrtlTriangleMesh> &arySrfcMesh)
{
	if (arySrfcMesh.m_paNum <= 0)
		return;

	m_pismArySrfcMesh = &arySrfcMesh;

	// create bounding box over srfc mesh
	CreateBoundingBoxOverSrfcMesh(m_pismGrid, arySrfcMesh);

	// create prtls at grid location in bounding box
	CreatePrtlAtGridLocation(m_pismGrid, m_pismGridPrtls);
//	CSimuMsg::PauseMessage("After CreatePrtlAtGridLocation(...)");

	// for each srfc mesh
	// identify inside prtls near srfc mesh
	// sweep through prtls along grid locations
	for (int i=0; i < arySrfcMesh.m_paNum; i++)
	{
		CPrtlTriangleMesh* pSrfcMesh = arySrfcMesh[i];
		IdentifyInsideGridPrtls(m_pismGrid, m_pismGridPrtls, pSrfcMesh->m_ptmTris);
	}
//	CSimuMsg::PauseMessage("After IdentifyInsideGridPrtls(...)");

	for (int i=0; i < m_pismGridPrtls.m_paNum; i++)
	{
		CMeshPrtl* pPrtl = m_pismGridPrtls[i];
		if (!pPrtl->m_mpInside)
			pPrtl->m_geDeleted = true;
	}
	m_pismGridPrtls.CleanDeletedElements();

	// clean colocation prtls
	// grid prtl near srfc prtl: delete grid prtl
	// srfc prtl near srfc prtl: merge two prtls, and triangles are little twisted.

	m_pismArySrfcMesh = NULL;
}

void CPrtlInSrfcMesh::CreateBoundingBoxOverSrfcMesh(TPrtlGrid<CMeshPrtl> &pismGrid,
													TGeomElemArray<CPrtlTriangleMesh>
													&arySrfcMesh)
{
	CPrtlTriangleMesh* pSrfcMesh = arySrfcMesh[0];
	CVector3D minPos, maxPos;
	T_GetBoundingBoxForParticles<CMeshPrtl>(minPos, maxPos, pSrfcMesh->m_ptmTriPrtls.m_paNum,
											pSrfcMesh->m_ptmTriPrtls.m_paPtrs);
	for (int i=1; i < arySrfcMesh.m_paNum; i++)
	{
		pSrfcMesh = arySrfcMesh[i];
		for (int j=0; j < pSrfcMesh->m_ptmTriPrtls.m_paNum; j++)
		{
			CMeshPrtl* pPrtl = pSrfcMesh->m_ptmTriPrtls[j];
			CSimuUtility::AdjustBoundingBoxWithPos(minPos, maxPos, pPrtl->m_vpPos);
		}
	}
	pismGrid.UpdateGridBoundaryAndDimension(minPos.v, maxPos.v);
	pismGrid.UpdateGridNumbers();
}

void CPrtlInSrfcMesh::CreatePrtlAtGridLocation(TPrtlGrid<CMeshPrtl> &pismGrid,
											   TGeomElemArray<CMeshPrtl> &pismGridPrtls)
{
	bool bMasterArray = pismGridPrtls.GetMasterArrayFlag();
	pismGridPrtls.SetMasterArrayFlag(false);
	pismGridPrtls.ResetArraySize(pismGrid.m_gNumPoints);
	pismGridPrtls.SetMasterArrayFlag(bMasterArray);
	for (int i=0; i < pismGridPrtls.m_paNum; i++)
	{
		CMeshPrtl* pPrtl = new CMeshPrtl();
		pPrtl->m_mpInside = false;
		pPrtl->m_mpOnSurface = false;
		pPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_SRFC);
		pismGridPrtls[i] = pPrtl;
	}
	CVector3D prtlPos, minPos;
	minPos.SetValue(&pismGrid.m_gMinPos);
	SimuValue d = pismGrid.m_gCellSize;
	for (int x=0; x < pismGrid.m_gPointDim[X]; x++) { prtlPos[X] = minPos[X] + d * x;
	for (int y=0; y < pismGrid.m_gPointDim[Y]; y++) { prtlPos[Y] = minPos[Y] + d * y;
	for (int z=0; z < pismGrid.m_gPointDim[Z]; z++) { prtlPos[Z] = minPos[Z] + d * z;

		int aryId = pismGrid.GetPointArrayIdFrom3DGridId(x, y, z);
		pismGridPrtls[aryId]->m_vpPos->SetValue(&prtlPos);
		pismGridPrtls[aryId]->m_mpId = aryId;
	}}}
}

void CPrtlInSrfcMesh::IdentifyInsideGridPrtls(TPrtlGrid<CMeshPrtl> &pismGrid,
											  TGeomElemArray<CMeshPrtl> &pismGridPrtls,
											  TGeomElemArray<CPrtlTriangle> &srfcTris)
{
	int i;
	int* labelPrtls = new int[pismGridPrtls.m_paNum];
	for (i=0; i < pismGridPrtls.m_paNum; i++)
	{
		CMeshPrtl* pPrtl = pismGridPrtls[i];
		pPrtl->m_mpDistanceToSrfc = SIMU_VALUE_MAX;
		pPrtl->m_mpNormal.SetValue((SimuValue)0);
		labelPrtls[i] = UNDECIDED_GRID_PRTL;
	}
	int debugPrtlId = -1;
	// lable grid prtls near surface triangles
	CVector3D triNormal, closestPos, sideNormal[3];
	CVector3D minPos, maxPos;
	int minId[3], maxId[3];
	for (i=0; i < srfcTris.m_paNum; i++)
	{
		CPrtlTriangle* pTri = srfcTris[i];
		triNormal.SetValue(&pTri->m_ptrAreaNormal);
		triNormal.Normalize();
		CMeshPrtl* triPs[3] = {pTri->m_ptrPs[0], pTri->m_ptrPs[1], pTri->m_ptrPs[2]};
		CVector3D* triPos[3] = {triPs[0]->m_vpPos, triPs[1]->m_vpPos, triPs[2]->m_vpPos};
		for (int k=0; k < 3; k++)
		{
			CSimuUtility::GetClosestPointOnLine(triPos[k], triPos[(k+1)%3], triPos[(k+2)%3],
												&closestPos);
			sideNormal[k].SetValueAsSubstraction(triPos[k], &closestPos);
		}
		T_GetBoundingBoxForParticles<CMeshPrtl>(minPos, maxPos, 3, triPs);
		pismGrid.GetGridIdRangeToCoverBox(minPos, maxPos, minId, maxId);
		for (int d=X; d <= Z; d++)
		{
			if (minId[d] == maxId[d])
			{ // make at least two-layer range
				minId[d] --; maxId[d] ++;
				// practically, the following 2 conditions are not met at the same time.
				if (minId[d] < 0)
					minId[d] = 0;
				if (maxId[d] >= pismGrid.m_gPointDim[d])
					maxId[d] = pismGrid.m_gPointDim[d] - 1;
			}
		}
		for (int x=minId[X]; x <= maxId[X]; x++)
		for (int y=minId[Y]; y <= maxId[Y]; y++)
		for (int z=minId[Z]; z <= maxId[Z]; z++)
		{
			int aryId = pismGrid.GetPointArrayIdFrom3DGridId(x, y, z);
			CMeshPrtl* pPrtl = pismGridPrtls[aryId];
			if (pPrtl->m_geId == debugPrtlId)
			{
				m_pismPrtl = pPrtl;
				m_pismTri = pTri;
				CSimuMsg::PauseMessage("Before TestInsidePrtlOnTri(...)");
			}
			TestInsidePrtlOnTri(pPrtl, pTri, sideNormal, &triNormal, labelPrtls[aryId]);
			if (pPrtl->m_geId == debugPrtlId)
			{
				m_pismPrtl = NULL;
				m_pismTri = NULL;
			}
		}
	}
	// lable the rest of grid prtls
	int curAryId, preAryId;
	for (int x=0; x < pismGrid.m_gPointDim[X]; x++)
	for (int y=0; y < pismGrid.m_gPointDim[Y]; y++)
	for (int z=0; z < pismGrid.m_gPointDim[Z]; z++)
	{
		curAryId = pismGrid.GetPointArrayIdFrom3DGridId(x, y, z);
		if (labelPrtls[curAryId] == UNDECIDED_GRID_PRTL)
		{
			if (z == 0 || z == pismGrid.m_gPointDim[Z] - 1)
				labelPrtls[curAryId] = OUTSIDE_GRID_PRTL;
			else // z > 0 && z < pismGrid.m_gPointDim[Z] - 1
			{
				preAryId = pismGrid.GetPointArrayIdFrom3DGridId(x, y, z-1);
				labelPrtls[curAryId] = labelPrtls[preAryId];
			}
		}
		if (labelPrtls[curAryId] == INSIDE_GRID_PRTL)
		{
			pismGridPrtls[curAryId]->m_mpInside = true;
			pismGridPrtls[curAryId]->SetVirtualPrtlColor(CSimuManager::COLOR_INSIDE);
		}
	}

	delete labelPrtls;
}

void CPrtlInSrfcMesh::TestInsidePrtlOnTri(CMeshPrtl* pPrtl, CPrtlTriangle* pTri,
										  CVector3D sideNormal[3], CVector3D *triNormal,
										  int &label)
{
	char location[] = "CPrtlInSrfcMesh::TestInsidePrtlOnTri(...)";

	CVector3D *prtlPos = pPrtl->m_vpPos;
	CMeshPrtl* triPs[3] = {pTri->m_ptrPs[0], pTri->m_ptrPs[1], pTri->m_ptrPs[2]};
	CVector3D* triPos[3] = {triPs[0]->m_vpPos, triPs[1]->m_vpPos, triPs[2]->m_vpPos};
	CVector3D tmpVect;
	int k;
	for (k=0; k < 3; k++)
	{
		tmpVect.SetValueAsSubstraction(prtlPos, triPos[(k+1)%3]);
		if (sideNormal[k].DotProduct(&tmpVect) < 0)
			break;
	}
	int distanceType;
	SimuValue closestDist = SIMU_VALUE_MAX;
	CVector3D closestPos;
	CMeshPrtl *closePrtl0, *closePrtl1;
	if (k >= 3) // prtlPos is inside the prism of infinite length
	{	// and its projection point is inside triangle
		SimuValue planeD = CSimuUtility::DotProduct(triNormal, triPos[0]);
		closestDist = - planeD + CSimuUtility::DotProduct(triNormal, prtlPos);
		closestPos.SetValue(prtlPos);
		closestPos.AddedBy(triNormal, -closestDist);
		closestDist = fabs(closestDist);
		distanceType = 0; // close to a triangle
	}
	else
	{	// find the closest point on the edges or on the vertices
		CVector3D tmpPos;
		SimuValue tmpDist;
		for (int k=0; k < 3; k++)
		{
			int tmpCase = CSimuUtility::GetClosestPointOnSegment(
										prtlPos, triPos[k], triPos[(k+1)%3], &tmpPos);
			tmpDist = prtlPos->GetDistanceToVector(&tmpPos);
			if (closestDist > tmpDist)
			{
				closestDist = tmpDist;
				closestPos.SetValue(&tmpPos);
				if (tmpCase == 0)
				{ // close to a vertex
					distanceType = 1; closePrtl0 = triPs[k]; closePrtl1 = NULL;
				}
				else if (tmpCase == 1)
				{ // close to a vertex
					distanceType = 1; closePrtl0 = triPs[(k+1)%3]; closePrtl1 = NULL;
				}
				else if (tmpCase == 2)
				{ // close to an edge
					distanceType = 2; closePrtl0 = triPs[k]; closePrtl1 = triPs[(k+1)%3];
				}
			}
		}
	}
	if (pPrtl->m_mpDistanceToSrfc <= closestDist)
		return;

	CVector3D srfcNormal;
	if (distanceType == 0)
	{
		srfcNormal.SetValue(triNormal);
	}
	else if (distanceType == 1)
	{
		closePrtl0->ComputeNormalFromNgbrTris();
		srfcNormal.SetValue(&closePrtl0->m_mpNormal);
	}
	else if (distanceType == 2)
	{
		SimuValue triArea = pTri->m_ptrAreaNormal.Length();
		srfcNormal.SetValue(&(pTri->m_ptrAreaNormal), 1/triArea);
		int j;
		for (j=0; j < closePrtl0->m_mpTris.m_paNum; j++)
		{
			CPrtlTriangle* ngbrTri = closePrtl0->m_mpTris[j];
			if (ngbrTri == pTri) continue;
			if (ngbrTri->HasTriPrtl(closePrtl1))
			{
				triArea = ngbrTri->m_ptrAreaNormal.Length();
				srfcNormal.AddedBy(&(ngbrTri->m_ptrAreaNormal), 1/triArea);
				break;
			}
		}
		srfcNormal.Normalize();
		if (j >= closePrtl0->m_mpTris.m_paNum)
		{
			CSimuMsg::ExitWithMessage(location, "Two triangles sharing edge is not found.");
		}
	}
	else
		CSimuMsg::ExitWithMessage(location, "Error in code: only 3 programmed types.");

	CVector3D relativePos;
	relativePos.SetValueAsSubstraction(prtlPos, &closestPos);
	if (srfcNormal.DotProduct(&relativePos) <= 0)
	{
		label = INSIDE_GRID_PRTL;
	}
	else
	{
		label = OUTSIDE_GRID_PRTL;
	}
	pPrtl->m_mpDistanceToSrfc = closestDist;
	pPrtl->m_mpNormal.SetValue(&srfcNormal);
	pPrtl->SetClosestSrfcPos(&closestPos);
}
