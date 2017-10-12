// PrtlFluidSpring.cpp: implementation of the CPrtlFluidSpring class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "PrtlFluidSpring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtlFluidSpring::CPrtlFluidSpring()
{
	m_pfsMinPrtlDistSqr = pow(0.1*CSimuManager::m_prtlDistance, 2);
	m_pfsMeshPrtls.InitializeClassArray(false, SIMU_PARA_PRTL_NUM_ALLOC_SIZE);
	m_pfsSrfcTris.InitializeClassArray(true, SIMU_PARA_TRI_NUM_ALLOC_SIZE);;

	m_pfsCGTolerance = CSimuManager::m_CGSolverTolerance;
}

CPrtlFluidSpring::~CPrtlFluidSpring()
{
}

void CPrtlFluidSpring::RenderPrtlFluid()
{
	CPrtlFluid::RenderPrtlFluid();

	if (CSimuManager::DRAW_PARTICLES)
		m_pfsMeshPrtls.RenderGeometryElements();

	if (CSimuManager::DRAW_SURFACE_MESH)
		m_pfsSrfcTris.RenderGeometryElements();

	m_pfsPrtlInSrfcMesh.RenderPrtlInSrfcMesh();
}

void CPrtlFluidSpring::ExportSurfaceMeshIntoPOVRayMesh2ObjectFile(CString &txFileName, FILE* fp)
{
	// Export mesh2 header
	fprintf(fp, "\n#declare %s = mesh2 {\n", m_deiName);

	// surface point is called vertex in mesh2 object file
	// so, in this function, surface prtls are called vertex in the comment

	// Output vertex position vectors
	fprintf(fp, "vertex_vectors {\n");
	fprintf(fp, "%d", m_pfsMeshPrtls.m_paNum);
	int i;
	for (i=0; i < m_pfsMeshPrtls.m_paNum; i++)
	{
		m_pfsMeshPrtls[i]->m_mpId = i;
		CVector3D* vector = m_pfsMeshPrtls[i]->m_vpPos;
		fprintf(fp, ",\n<%f, %f, %f>", vector->v[X], vector->v[Y], vector->v[Z]);
	}
	fprintf(fp, "\n}\n");

	// Output vertex normals
	fprintf(fp, "normal_vectors {\n");
	fprintf(fp, "%d", m_pfsMeshPrtls.m_paNum);
	for (i=0; i < m_pfsMeshPrtls.m_paNum; i++)
	{
		CVector3D* vector = &(m_pfsMeshPrtls[i]->m_mpNormal);
		fprintf(fp, ",\n<%f, %f, %f>", vector->v[X], vector->v[Y], vector->v[Z]);
	}
	fprintf(fp, "\n}\n");

	// Output surface triangle indices
	fprintf(fp, "face_indices {\n");
	fprintf(fp, "%d", m_pfsSrfcTris.m_paNum);
	for (i=0; i < m_pfsSrfcTris.m_paNum; i++)
	{
		fprintf(fp, ",\n");
		fprintf(fp, "<%d,%d,%d>",
				m_pfsSrfcTris[i]->m_ptrPs[0]->m_mpId,
				m_pfsSrfcTris[i]->m_ptrPs[1]->m_mpId,
				m_pfsSrfcTris[i]->m_ptrPs[2]->m_mpId);
	}
	fprintf(fp, "\n}\n");

	// Output normal indices for smoothing triangles
	fprintf(fp, "normal_indices {\n");
	fprintf(fp, "%d", m_pfsSrfcTris.m_paNum);
	for (i=0; i < m_pfsSrfcTris.m_paNum; i++)
	{
		fprintf(fp, ",\n");
		// since each vertex has a normal, both vertex and its normal have same index
		fprintf(fp, "<%d,%d,%d>",
				m_pfsSrfcTris[i]->m_ptrPs[0]->m_mpId,
				m_pfsSrfcTris[i]->m_ptrPs[1]->m_mpId,
				m_pfsSrfcTris[i]->m_ptrPs[2]->m_mpId);
	}
	fprintf(fp, "\n}\n");

	// Export mesh2 tailer
	fprintf(fp, "}\n");
}

void CPrtlFluidSpring::ImportSurfaceMeshFromPOVRayMesh2ObjectFile(FILE* fp)
{
	char tmpStr[LEN_DATA_LINE];
	strcpy(tmpStr, "");
	// Import mesh2 header
	fscanf(fp, "\n#declare %s = mesh2 {\n", tmpStr);
	if (strlen(tmpStr) <= 0)
		return;
	m_deiName = tmpStr;

	int number;
	float vectorX, vectorY, vectorZ;
	
	// Import vertex position vectors
	fscanf(fp, "vertex_vectors {\n");
	fscanf(fp, "%d", &number);
	int i;
	m_pfsMeshPrtls.ResetArraySize(0);
	for (i=0; i < number; i++)
	{
		CFluidMeshPrtl* pPrtl = (CFluidMeshPrtl*)CreateOneFluidPrtl();
		m_pfsMeshPrtls.AppendOnePointer(pPrtl);
	}
	for (i=0; i < m_pfsMeshPrtls.m_paNum; i++)
	{
		fscanf(fp, ",\n");
		fscanf(fp, "<%f, %f, %f>", &vectorX, &vectorY, &vectorZ);
		m_pfsMeshPrtls[i]->m_vpPos->SetElements(vectorX, vectorY, vectorZ);
	}
	fscanf(fp, "\n}\n");

	// Import vertex normals
	fscanf(fp, "normal_vectors {\n");
	fscanf(fp, "%d", &number);
	for (i=0; i < m_pfsMeshPrtls.m_paNum; i++)
	{
		fscanf(fp, ",\n");
		fscanf(fp, "<%f, %f, %f>", &vectorX, &vectorY, &vectorZ);
		m_pfsMeshPrtls[i]->m_mpNormal.SetElements(vectorX, vectorY, vectorZ);
	}
	fscanf(fp, "\n}\n");

	int id0, id1, id2;
	// Import surface triangle indices
	fscanf(fp, "face_indices {\n");
	fscanf(fp, "%d", &number);
	m_pfsSrfcTris.ResetArraySize(number);
	for (i=0; i < m_pfsSrfcTris.m_paNum; i++)
	{
		fscanf(fp, ",\n");
		fscanf(fp, "<%d,%d,%d>", &id0, &id1, &id2);
		CPrtlTriangle* pTri = m_pfsSrfcTris[i];
		pTri->m_ptrPs[0] = m_pfsMeshPrtls[id0];
		pTri->m_ptrPs[1] = m_pfsMeshPrtls[id1];
		pTri->m_ptrPs[2] = m_pfsMeshPrtls[id2];
	}
	fscanf(fp, "\n}\n");

	m_pfPrtls.ResetArraySize(0);
	for (i=0; i < m_pfsMeshPrtls.m_paNum; i++)
	{
		CFluidMeshPrtl* pPrtl = (CFluidMeshPrtl*)m_pfsMeshPrtls[i];
		m_pfPrtls.AppendOnePointer(pPrtl);
	}

	// Import mesh2 tailer
	fscanf(fp, "}\n");
}

CFluidPrtl* CPrtlFluidSpring::CreateOneFluidPrtl()
{
	return new CFluidMeshPrtl();
}

void CPrtlFluidSpring::PreparePrtlEvolution()
{
	CreateFluidPrtlsFromMeshPrtls();

	m_pfNeedToRegisterMovedPrtls = true;
	m_pfNeedToRefreshPrtlNeighbors = true;
	RegisterPrtlsForNeighborSearchIfNotYet();
	SearchPrtlNeighbors();

	DeleteInsidePrtlsCloseToSrfc();
	CollectSrfcPrtls();

	AssignInitialPrtlMass();

	InitializePrtlSprings();

	m_pfPrtlEvolutionIsPrepared = true;
}

void CPrtlFluidSpring::CreateFluidPrtlsFromMeshPrtls()
{
	for (int i=0; i < m_pfsMeshPrtls.m_paNum; i++)
	{
		CMeshPrtl* meshPrtl = m_pfsMeshPrtls[i];
		CFluidMeshPrtl* pPrtl = (CFluidMeshPrtl*)CreateOneFluidPrtl();
		m_pfPrtls.AppendOnePointer(pPrtl);
		pPrtl->m_vpPos->SetValue(meshPrtl->m_vpPos);
		if (meshPrtl->m_mpOnSurface)
		{
			pPrtl->m_fpOnSurface = true;
			pPrtl->m_mpOnSurface = true;
			pPrtl->m_mpNormal.SetValue(&meshPrtl->m_mpNormal);
			SetSrfcPrtl(pPrtl);
		}
		else
		{
			pPrtl->m_fpOnSurface = false;
			pPrtl->m_mpOnSurface = false;
		}
		// replace mesh prtl with fluid prtl in triangles
		for (int j=0; j < meshPrtl->m_mpTris.m_paNum; j++)
		{
			CPrtlTriangle* pTri = meshPrtl->m_mpTris[j];
			pTri->ReplaceTriPrtl(meshPrtl, pPrtl);
			pPrtl->m_mpTris.AppendOnePointer(pTri);
		}
		delete meshPrtl;
	}
	m_pfsMeshPrtls.ResetArraySize(0);
}

void CPrtlFluidSpring::DeleteInsidePrtlsCloseToSrfc()
{
	SimuValue distSqr;
	TGeomElemArray<CFluidPrtl> deletedPrtls;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_geDeleted)
			continue;
		if (!pPrtl->m_fpOnSurface)
			continue;
		deletedPrtls.ResetArraySize(0);
		for (int j=0; j < pPrtl->m_fpNgbrs.m_paNum; j++)
		{
			CFluidPrtl* ngbrPrtl = pPrtl->m_fpNgbrs[j];
			distSqr = pPrtl->m_vpPos->GetDistanceSquareToVector(ngbrPrtl->m_vpPos);
			if (distSqr < m_pfsMinPrtlDistSqr)
			{
				if (ngbrPrtl->m_fpOnSurface)
				{
					MergeTwoPrtls(pPrtl, ngbrPrtl);
				}
				deletedPrtls.AppendOnePointer(ngbrPrtl);
			}
		}
		for (int j=0; j < deletedPrtls.m_paNum; j++)
		{
			CFluidPrtl* delePrtl = deletedPrtls[j];
			m_pfPrtlSearch.DeRegisterOnePrtl(delePrtl);
			delePrtl->DeregisterFromNgbrPrtls();
			delePrtl->m_geDeleted = true;
		}
	}
	m_pfPrtls.CleanDeletedElements();
}

void CPrtlFluidSpring::MergeTwoPrtls(CFluidPrtl* pStayPrtl, CFluidPrtl* pDelePrtl)
{
	// replace <delePrtl> with <stayPrtl> in its triangles
	CFluidMeshPrtl* stayPrtl = (CFluidMeshPrtl*)pStayPrtl;
	CFluidMeshPrtl* delePrtl = (CFluidMeshPrtl*)pDelePrtl;
	stayPrtl->m_vpPos->AddedBy(delePrtl->m_vpPos);
	stayPrtl->m_vpPos->ScaledBy(0.5);
	for (int k=0; k < delePrtl->m_mpTris.m_paNum; k++)
	{
		CPrtlTriangle* pTri = delePrtl->m_mpTris[k];
		pTri->ReplaceTriPrtl(delePrtl, stayPrtl);
		stayPrtl->m_mpTris.AppendOnePointer(pTri);
	}
}

void CPrtlFluidSpring::InitializePrtlSprings()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlSpring* pPrtl = (CFluidPrtlSpring*)m_pfPrtls[i];
		pPrtl->InitializeSpringParameters(m_pfsMinPrtlDistSqr);
	}
}

void CPrtlFluidSpring::CollectSrfcPrtls()
{
	m_pfsMeshPrtls.ResetArraySize(0);
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidMeshPrtl* pPrtl = (CFluidMeshPrtl*)m_pfPrtls[i];
		if (pPrtl->m_fpOnSurface)
		{
			m_pfsMeshPrtls.AppendOnePointer(pPrtl);
			pPrtl->RenderGeometry();
		}
	}
}

void CPrtlFluidSpring::EvolveFluidParticles(SimuValue simuTime)
{
	IntegratePrtlPositionsWithSpringForce();
	
	UpdateSrfcMesh();
}

void CPrtlFluidSpring::IntegratePrtlPositionsWithSpringForce()
{
	SetupSymmetricSparseMatrixForSpringForce();

	m_pfInterations = m_pfsCGSolver3D.SolveSymmetricMatrix(m_pfsCGSolver3D.m_matrixSize, m_pfsCGTolerance);

	// integrate on particles
	int notFixedPrtls = 0;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlSpring* pPrtl = (CFluidPrtlSpring*)m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion)
		{
			pPrtl->m_vpPos->AddedBy(pPrtl->m_vpVel, m_pfTimeStep);
		}
		else
		{
			// pPrtl->m_vpVel = dx
			m_pfsCGSolver3D.GetVariableVectorElement(notFixedPrtls++, pPrtl->m_vpVel);
			pPrtl->m_vpPos->AddedBy(pPrtl->m_vpVel);
			pPrtl->m_vpVel->ScaledBy(1/m_pfTimeStep);
		}
	}
}

void CPrtlFluidSpring::SetupSymmetricSparseMatrixForSpringForce()
{
	char location[] = "CPrtlFluidSpring::SetupSymmetricSparseMatrixForSpringForce()";

	int i;

	m_pfsCGSolver3D.FreeSparseMatrixStorage();

	// m_arraySize = m_matrixSize + 1 + (total neighbors on all not-fixed particles)
	m_pfsCGSolver3D.m_arraySize = 0;
	int notFixedPrtls = 0;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlSpring* pPrtl = (CFluidPrtlSpring*)m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->m_fpId = notFixedPrtls++;
		for (int j=0; j < pPrtl->m_fpNgbrs.m_paNum; j++)
		{
			CFluidPrtlSpring* ngbrPrtl = (CFluidPrtlSpring*)pPrtl->m_fpNgbrs[j];
			if (ngbrPrtl->m_fpFixedMotion) continue;
			m_pfsCGSolver3D.m_arraySize ++;
		}
	}
	m_pfsCGSolver3D.m_matrixSize = notFixedPrtls;
	m_pfsCGSolver3D.m_arraySize += m_pfsCGSolver3D.m_matrixSize + 1;

	m_pfsCGSolver3D.AllocateSparseMatrixStorage();

	CVector3D tmpVect;
	SimuValue matrixForceDerivative[9];

	// fill in 2 1D-arrays with the matrix values from all particles
	SimuValue constCoeff = m_pfTimeStep*m_pfTimeStep/m_pfPrtlMass;
	SimuValue tsSquare = m_pfTimeStep*m_pfTimeStep;
	int nArrayId = m_pfsCGSolver3D.m_matrixSize + 1;
	int nMatrixId = 0;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlSpring* pPrtl = (CFluidPrtlSpring*)m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		// initial guesses = 0
		m_pfsCGSolver3D.SetVariableVectorElement(nMatrixId, (SimuValue)0);
		m_pfsCGSolver3D.SetConstantVectorElement(nMatrixId, pPrtl->m_vpVel, m_pfTimeStep);
		m_pfsCGSolver3D.AddConstantVectorElement(nMatrixId, CSimuManager::GRAVITY, tsSquare);
		// In the range 0 to matrixSize-1 on the 2 1D-arrays,
		// m_valueArray is filled with diagonal values for rows, 0 to matrixSize-1;
		// m_indexArray is filled with starting indices for the rows in the 2 1D-arrays
		m_pfsCGSolver3D.m_indexArray[nMatrixId] = nArrayId;
		m_pfsCGSolver3D.SetValueArrayElementAsUnitMatrix(nMatrixId);
		for (int j=0; j < pPrtl->m_fpNgbrs.m_paNum; j++)
		{
			CSpring* pSpring = pPrtl->m_fpsNgbrSprings[j];
			CFluidPrtlSpring* pNgbrPrtl = (CFluidPrtlSpring*)pPrtl->m_fpNgbrs[j];
			// compute spring vector
			tmpVect.SetValueAsSubstraction(pPrtl->m_vpPos, pNgbrPrtl->m_vpPos);
			SimuValue springLen = tmpVect.Length();
			if (springLen <= SIMU_VALUE_EPSILON)
				CSimuMsg::ExitWithMessage(location, "spring length is too small.");
			// accumulate diagonal element from neighbor spring
			ComputeSpringForceDerivative(pSpring->m_csConst, springLen, pSpring->m_csOrgLen,
										 matrixForceDerivative, &tmpVect);
			m_pfsCGSolver3D.AddValueArrayElement(nMatrixId, matrixForceDerivative, -constCoeff);
			// accumulate spring force from neighbor spring
			SimuValue springForceMag = pSpring->m_csConst*(pSpring->m_csOrgLen - springLen);
			tmpVect.ScaledBy(springForceMag/springLen);
			m_pfsCGSolver3D.AddConstantVectorElement(nMatrixId, &tmpVect, constCoeff);

			if (pNgbrPrtl->m_fpFixedMotion)
				continue; // ngbr of fixed motion is not present in matrix

			// fill in off-diagonal elements from neighbor springs
			// m_pfsCGSolver3D.m_indexArray[nArrayId] is the column number in sparse matrix
			m_pfsCGSolver3D.m_indexArray[nArrayId] = pNgbrPrtl->m_fpId;
			// m_pfsCGSolver3D.m_valueArray[nArrayId] is the element in sparse matrix
			m_pfsCGSolver3D.SetValueArrayElement(nArrayId, matrixForceDerivative, constCoeff);
			nArrayId ++;
		}
		nMatrixId ++;
	}

	if (nArrayId == m_pfsCGSolver3D.m_arraySize)
		m_pfsCGSolver3D.m_indexArray[m_pfsCGSolver3D.m_matrixSize] = nArrayId;
		// and m_valueArray[m_matrixSize] will be undefined and not used.
	else
		CSimuMsg::ExitWithMessage(location, "Sparse matrix arrays are not correct in size.");
}

void CPrtlFluidSpring::ComputeSpringForceDerivative(SimuValue springConst,
													SimuValue springOrgLen,
													SimuValue springLen,
													SimuValue matrixFD[9],
													CVector3D* springVect)
{
	SimuValue tmp1 = -springConst*springOrgLen/pow(springLen, 3);
	SimuValue tmp2 = -springConst*(springLen - springOrgLen)/springLen;
	for (int j=0; j < 3; j++)
	for (int k=0; k < 3; k++)
	{
		matrixFD[j*3+k] = springVect->v[j]*springVect->v[k];
		matrixFD[j*3+k] *= tmp1;
		if (j == k)
			matrixFD[j*3+k] += tmp2;
	}
}

void CPrtlFluidSpring::UpdateSrfcMesh()
{
	// update prtl and triangle normals
	for (int i=0; i < m_pfsSrfcTris.m_paNum; i++)
	{
		CPrtlTriangle* pTri = m_pfsSrfcTris[i];
		pTri->ComputeTriAreaNormal();
	}
	for (int i=0; i < m_pfsMeshPrtls.m_paNum; i++)
	{
		CMeshPrtl* pPrtl = m_pfsMeshPrtls[i];
		pPrtl->ComputeNormalFromNgbrTris();
	}
}