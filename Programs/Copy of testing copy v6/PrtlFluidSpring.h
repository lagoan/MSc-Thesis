// PrtlFluidSpring.h: interface for the CPrtlFluidSpring class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLFLUIDSPRING_H__84C015D3_0918_48B5_B9A8_52C8BF052701__INCLUDED_)
#define AFX_PRTLFLUIDSPRING_H__84C015D3_0918_48B5_B9A8_52C8BF052701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PrtlFluid.h"
#include "FluidPrtlSpring.h"
#include "PrtlInSrfcMesh.h"

class CPrtlFluidSpring : public CPrtlFluid  
{
public:
	SimuValue m_pfsMinPrtlDistSqr;
	TGeomElemArray<CMeshPrtl> m_pfsMeshPrtls;
	TGeomElemArray<CPrtlTriangle> m_pfsSrfcTris;
	CPrtlInSrfcMesh m_pfsPrtlInSrfcMesh;

	CSimuCGSolver3D m_pfsCGSolver3D;
	SimuValue m_pfsCGTolerance;

	CPrtlFluidSpring();
	virtual ~CPrtlFluidSpring();

	virtual void RenderPrtlFluid();

	virtual void ExportSurfaceMeshIntoPOVRayMesh2ObjectFile(CString &txFileName, FILE* fp);
	virtual void ImportSurfaceMeshFromPOVRayMesh2ObjectFile(FILE* fp);

	virtual CFluidPrtl* CreateOneFluidPrtl();
	virtual void PreparePrtlEvolution();
	void CreateFluidPrtlsFromMeshPrtls();
	void DeleteInsidePrtlsCloseToSrfc();
	void MergeTwoPrtls(CFluidPrtl* pStayPrtl, CFluidPrtl* pDelePrtl);
	void InitializePrtlSprings();
	void CollectSrfcPrtls();
	virtual void EvolveFluidParticles(SimuValue simuTime);
	void IntegratePrtlPositionsWithSpringForce();
	void SetupSymmetricSparseMatrixForSpringForce();
	void ComputeSpringForceDerivative(SimuValue springConst, SimuValue springOrgLen,
									  SimuValue springLen, SimuValue matrixFD[9],
									  CVector3D* springVect);
	void UpdateSrfcMesh();
};

#endif // !defined(AFX_PRTLFLUIDSPRING_H__84C015D3_0918_48B5_B9A8_52C8BF052701__INCLUDED_)
