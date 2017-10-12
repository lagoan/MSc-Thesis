// PrtlFluidUpsample.cpp: implementation of the CPrtlFluidUpsample class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "PrtlFluidUpsample.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtlFluidUpsample::CPrtlFluidUpsample()
{
	m_pfuSrfcPrtls.InitializeClassArray(false, SIMU_PARA_PRTL_NUM_ALLOC_SIZE);
	m_pfuSrfcCheckedByConvexHull = false;

	m_pfuDownSampled = false;
	m_pfuLowerPrtlMassLimit = CSimuManager::m_lowerPrtlMassLimitRatio*m_pfPrtlMass;
	m_pfuUpperPrtlMassLimit = CSimuManager::m_upperPrtlMassLimitRatio*m_pfPrtlMass;
	m_pfuMinPrtlDistRatio = SIMU_VALUE_MAX;
	m_pfuDownsampleDistThrshd = CSimuManager::m_downSampleDistRatio*CSimuManager::m_prtlDistance;
	m_pfuNewDownsampledPrtls.SetMasterArrayFlag(false);

	m_pfuSrfcThickness = CSimuManager::m_srfcThicknessRatio*CSimuManager::m_prtlDistance;
	m_pfuUpsampleMinRadiusSqr = CSimuManager::m_upsampleMinRadiusRatio*CSimuManager::m_prtlDistance;
	m_pfuUpsampleMinRadiusSqr = pow(m_pfuUpsampleMinRadiusSqr, 2);
	m_pfuUpsampleMaxRadiusSqr = CSimuManager::m_upsampleMaxRadiusRatio*CSimuManager::m_prtlDistance;
	m_pfuUpsampleMaxRadiusSqr = pow(m_pfuUpsampleMaxRadiusSqr, 2);
	m_pfuUpsampledPrtls.SetMasterArrayFlag(false);
	m_pfuResampled = false;

	if (CSimuManager::SAME_DOWN_UP_SAMPLE_RATE)
	{
		if (CSimuManager::m_maxDownsamplePauseSteps < CSimuManager::m_maxUpsamplePauseSteps)
			CSimuManager::m_maxDownsamplePauseSteps = CSimuManager::m_maxUpsamplePauseSteps;
		if (CSimuManager::m_maxUpsamplePauseSteps < CSimuManager::m_maxDownsamplePauseSteps)
			CSimuManager::m_maxUpsamplePauseSteps = CSimuManager::m_maxDownsamplePauseSteps;
	}
	m_pfuDownsamplePauseSteps = CSimuManager::m_maxDownsamplePauseSteps;
	m_pfuUpsamplePauseSteps = CSimuManager::m_maxUpsamplePauseSteps;
	if (CSimuManager::INTERLEAVE_DOWN_UP_SAMPLE)
	{
		if (m_pfuUpsamplePauseSteps%2 != 0)
			CSimuMsg::Message("Warning:", "It's better m_pfuUpsamplePauseSteps is even number.");
		m_pfuUpsamplePauseSteps = m_pfuUpsamplePauseSteps/2;
	}
	m_pfuMinPrtlMass = 0;
	m_pfuMaxPrtlMass = 0;

	m_pfuPrtlNgbrs = NULL;
	m_pfuHighlitPos = NULL;
	m_pfuCircumSphCtr = NULL;
	m_pfuCircumSphRSqr = NULL;
	m_pfuPause[0] = false;
	m_pfuPause[1] = false;
	m_pfuPause[2] = false;
	m_pfuPause[3] = false;
	m_pfuPause[4] = false;
	m_pfuPause[5] = false;
}

CPrtlFluidUpsample::~CPrtlFluidUpsample()
{
}

void CPrtlFluidUpsample::RenderPrtlFluid()
{
	CPrtlFluid::RenderPrtlFluid();

	if (CSimuManager::DRAW_SRFC_PARTICLES)
		m_pfuSrfcPrtls.RenderGeometryElements();

	if (m_pfuPrtlNgbrs != NULL)
		m_pfuPrtlNgbrs->RenderGeometryElements();

	if (m_pfuHighlitPos != NULL)
		m_pfuHighlitPos->RenderGeometry();

	if (m_pfuCircumSphCtr != NULL)
	{
		m_pfuCircumSphCtr->RenderGeometry();
		SimuColor3v(CSimuManager::COLOR_WHITE->v);
		GLUquadricObj* qObj = gluNewQuadric();
		glPushMatrix();
		SimuTranslate(m_pfuCircumSphCtr->m_p3dPos[X],
					  m_pfuCircumSphCtr->m_p3dPos[Y],
					  m_pfuCircumSphCtr->m_p3dPos[Z]);
		gluSphere(qObj, sqrt(m_pfuCircumSphRSqr), 40, 40);
		glPopMatrix();
		gluDeleteQuadric(qObj);
	}
}

void CPrtlFluidUpsample::PreparePrtlEvolution()
{
	m_pfNeedToRegisterMovedPrtls = true;
	m_pfNeedToRefreshPrtlNeighbors = true;
	RegisterPrtlsForNeighborSearchIfNotYet();
	SearchPrtlNeighbors();

	ComputeVariablePrtlMasses();
	m_pfNeedToUpdatePrtlDensities = true;
	ComputePrtlDensities();
	DownsampleFluidParticles();
	ComputeVariablePrtlMasses();

	m_pfNeedToUpdatePrtlDensities = true;
	m_pfDensity = ComputePrtlDensities();
	AssignOrgPrtlDensities();
	ComputeSrfcIsoDensity();

	ComputeSrfcDetectionThresholdValues();
	GetMinAndMaxPrtlMasses();

	AssignIniTemperatureForPrtls();
	if (CSimuManager::HEAT_TRANSFER)
		m_pfShearModulus = GetShearModulusFromLinearMelting0(m_pfIniTemperature,
															m_pfCoeffTemperature,
															m_pfCoeffShearModulus,
															m_pfConstSummation);
	m_pfPrtlEvolutionIsPrepared = true;
}

void CPrtlFluidUpsample::DetectSrfcPrtls()
{
	if (!CSimuManager::m_bConvexHull4SrfcDetect)
	{
		CPrtlFluid::DetectSrfcPrtls();
		return;
	}

	m_pfuSrfcPrtls.ResetArraySize(0);
	SimuValue pnd, gradLen;
	bool bOnSurface;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (IsPrtlStatusDefinite(pPrtl, m_pfuSrfcPrtls))
			continue;
		pPrtl->ComputePrtlNumDenAndGradLen(m_pfSmoothLen, pnd, gradLen);
		if (pnd <= m_pfuInitialAvgSrfcPND
		 || gradLen >= m_pfuInitialAvgSrfcPNDGradLen)
			bOnSurface = true;  // pre-detect as srfc prtl
		else
			bOnSurface = false;  // pre-detect as insd prtl

		if (bOnSurface != pPrtl->m_fpOnSurface)
		{
			bOnSurface = IsPosOnSrfcByItsConvexHull(pPrtl->m_vpPos, pPrtl->m_fpNgbrs);
		}
		if (bOnSurface)
		{
			m_pfuSrfcPrtls.AppendOnePointer(pPrtl);
			SetSrfcPrtl(pPrtl);
		}
		else
			SetInsdPrtl(pPrtl);
	}
	if (m_pfuSrfcPrtls.m_paNum <= 0)
	{ // if no srfc prtl has been found at all, then use convex hull to detect.
		m_pfuSrfcCheckedByConvexHull = true;
		for (int i=0; i < m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = m_pfPrtls[i];
			if (IsPrtlStatusDefinite(pPrtl, m_pfuSrfcPrtls))
				continue;
			pPrtl->ComputePrtlNumDenAndGradLen(m_pfSmoothLen, pnd, gradLen);
			if (pnd >= m_pfuInitialAvgInsdPND
			 && gradLen <= m_pfuInitialAvgInsdPNDGradLen)
			{
				SetInsdPrtl(pPrtl);
				continue;
			}
			bool bOnSurface = IsPosOnSrfcByItsConvexHull(pPrtl->m_vpPos, pPrtl->m_fpNgbrs);
			if (bOnSurface)
			{
				m_pfuSrfcPrtls.AppendOnePointer(pPrtl);
				SetSrfcPrtl(pPrtl);
			}
			else
				SetInsdPrtl(pPrtl);
		}
		// Update srfc detection threshold values
		ComputeSrfcDetectionThresholdValues();
	}
	// prevent the case that insd prtl does not have insd ngbr.
	int numSrfc, numInsd;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpOnSurface)
			continue;
		GetSrfcAndInsdPrtlNumbers(pPrtl, numSrfc, numInsd);
		if (numInsd == 0)
			SetSrfcPrtl(pPrtl);
	}
}

void CPrtlFluidUpsample::EvolveFluidParticles(SimuValue simuTime)
{
	RegisterPrtlsForNeighborSearchIfNotYet();
	SearchPrtlNeighbors();
	ComputePrtlDensities();

	if (m_pfuDownsamplePauseSteps == 1)
	{
		m_pfuDownsamplePauseSteps = CSimuManager::m_maxDownsamplePauseSteps;
		// densities of new prtls are computed when they are created.
		// densities of old prtls are not changed even when new prtls are added in neighborhood
		long start = clock();
		DownsampleFluidParticles();
		long finish = clock();
		CSimuManager::m_dbTotalDownsampleClock += finish-start;
		GetMinAndMaxPrtlMasses();
		m_pfNeedToRegisterMovedPrtls = true;
	}
	else
		m_pfuDownsamplePauseSteps--;

	if (m_pfuUpsamplePauseSteps == 1)
	{
		m_pfuUpsamplePauseSteps = CSimuManager::m_maxUpsamplePauseSteps;
		// densities of new prtls are computed when they are created.
		// densities of old prtls are not changed even when new prtls are added in neighborhood
		long start = clock();
		UpsampleFluidParticles();
		long finish = clock();
		CSimuManager::m_dbTotalUpsampleClock += finish-start;
		GetMinAndMaxPrtlMasses();
		m_pfNeedToRegisterMovedPrtls = true;
	}
	else
		m_pfuUpsamplePauseSteps --;

	IntegratePrtlMotions(simuTime);
}

void CPrtlFluidUpsample::ComputeSrfcDetectionThresholdValues()
{
	m_pfuInitialAvgInsdPND = 0;
	m_pfuInitialAvgSrfcPND = 0;
	m_pfuInitialAvgInsdPNDGradLen = 0;
	m_pfuInitialAvgSrfcPNDGradLen = 0;
	int numSrfcPrtls = 0;
	SimuValue pnd, gradLen;
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->ComputePrtlNumDenAndGradLen(m_pfSmoothLen, pnd, gradLen);
		if (pPrtl->m_fpOnSurface)
		{
			numSrfcPrtls ++;
			m_pfuInitialAvgSrfcPND += pnd;
			m_pfuInitialAvgSrfcPNDGradLen += gradLen;
		}
		else
		{
			m_pfuInitialAvgInsdPND += pnd;
			m_pfuInitialAvgInsdPNDGradLen += gradLen;
		}
	}
	if (numSrfcPrtls > 0)
	{
		m_pfuInitialAvgSrfcPND /= numSrfcPrtls;
		m_pfuInitialAvgSrfcPNDGradLen /= numSrfcPrtls;
	}
	if (numSrfcPrtls < m_pfPrtls.m_paNum)
	{
		m_pfuInitialAvgInsdPND /= m_pfPrtls.m_paNum - numSrfcPrtls;
		m_pfuInitialAvgInsdPNDGradLen /= m_pfPrtls.m_paNum - numSrfcPrtls;
	}
}

bool CPrtlFluidUpsample::IsPrtlStatusDefinite(CFluidPrtl* pPrtl,
											  TGeomElemArray<CFluidPrtl> &srfcPrtls)
{
	if (pPrtl->m_fpFixedMotion
	 || pPrtl->m_fpNgbrs.m_paNum <= 3)
	{
		srfcPrtls.AppendOnePointer(pPrtl);
		SetSrfcPrtl(pPrtl); // DEFINITE_SURFACE
		return true;
	}
	int numSrfc, numInsd;
	GetSrfcAndInsdPrtlNumbers(pPrtl, numSrfc, numInsd);
	if (numInsd > 0 && numInsd == pPrtl->m_fpNgbrs.m_paNum)
	{
		SetInsdPrtl(pPrtl); // DEFINITE_INSIDE
		return true;
	}
	return false;
}

bool CPrtlFluidUpsample::HasAnySrfcPrtlsInNgbrhood(CVector3D* pos, SimuValue radius)
{
	SimuValue radiusSqr = radius*radius;
	TPointerArray<CFluidPrtl> possibleNgbrPrtls;
	m_pfPrtlSearch.GetPossibleNgbrPrtls(pos, CSimuManager::m_prtlDistance, possibleNgbrPrtls);
	for (int i=0; i < possibleNgbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = possibleNgbrPrtls[i];
		if (!ngbrPrtl->m_fpOnSurface)
			continue;
		if (ngbrPrtl->m_vpPos->GetDistanceSquareToVector(pos) <= radiusSqr)
			return true;
	}
	return false;
}

void CPrtlFluidUpsample::GetSrfcAndInsdPrtlNumbers(CFluidPrtl* pPrtl,
												   int &numSrfc, int &numInsd)
{
	numSrfc = 0; numInsd = 0;
	for (int i=0; i < pPrtl->m_fpNgbrs.m_paNum; i++)
	{
		if (pPrtl->m_fpNgbrs[i]->m_fpOnSurface)
			numSrfc ++;
		else
			numInsd ++;
	}
}

bool CPrtlFluidUpsample::IsPosOnSrfcByItsConvexHull(CVector3D* pos,
													TGeomElemArray<CFluidPrtl> &ngbrPrtls)
{
	TGeomElemArray<CPoint3DOnEdge> points(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CTriangle3DIntNorm> triangles(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CEdge3DOnTriangle> edges(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);

	CConvexHull::GetDistinctPointFromFluidPrtl(ngbrPrtls, points, CConvexHull::m_chMinPointDistSqr);
	CPoint3DOnEdge* pPoint = CConvexHull::AddOnePoint3DIntPos(pos, points);
	pPoint->SetPoint3DColor(CConvexHull::m_chCurPColor);
	CConvexHull::RobustConvexHullGeneration(points, triangles, edges);
	bool bOnSurface;
	if (triangles.m_paNum <= 0)
		bOnSurface = true;
	else
		bOnSurface = CConvexHull::PointIsOnConvexHull(pPoint, triangles);
	if (!bOnSurface)
	{
		SimuValue minDist = CConvexHull::GetMinDistanceToConvexHullFromInsidePos(
											pos, triangles);
		if (minDist < m_pfuSrfcThickness)
		{
			bOnSurface = true;
		}
	}
	return bOnSurface;
}

void CPrtlFluidUpsample::GetMinAndMaxPrtlMasses()
{
	if (m_pfPrtls.m_paNum <= 0)
		return;

	m_pfuMinPrtlMass = SIMU_VALUE_MAX;
	m_pfuMaxPrtlMass = -SIMU_VALUE_MAX;
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (m_pfuMinPrtlMass > pPrtl->m_fpMass)
			m_pfuMinPrtlMass = pPrtl->m_fpMass;
		if (m_pfuMaxPrtlMass < pPrtl->m_fpMass)
			m_pfuMaxPrtlMass = pPrtl->m_fpMass;
	}
	if (m_pfuMinPrtlMass < m_pfuLowerPrtlMassLimit)
		CSimuMsg::ExitWithMessage("CPrtlFluidUpsample::GetMinAndMaxPrtlMasses()",
								"Small prtl mass is found.");
}

void CPrtlFluidUpsample::ComposeSimuInfoAfterOneTimeStep(int numTimeSteps)
{
	char tmpStr[32];
/*
	sprintf(tmpStr, "[%d]", numTimeSteps); m_pfSimuInfo = tmpStr;
	sprintf(tmpStr, " n=%d", m_pfPrtls.m_paNum); m_pfSimuInfo += tmpStr;
	sprintf(tmpStr, " mtm=%.2f", m_pfMotionTime); m_pfSimuInfo += tmpStr;
	if (CSimuManager::CREATE_SURFACE_MESH)
	{
		sprintf(tmpStr, " stm=%.1f", m_pfSurfaceTime); m_pfSimuInfo += tmpStr;
	}
*/
	CPrtlFluid::ComposeSimuInfoAfterOneTimeStep(numTimeSteps);

	sprintf(tmpStr, " minM=%.2f", m_pfuMinPrtlMass); m_pfSimuInfo += tmpStr;
	sprintf(tmpStr, " maxM=%.f", m_pfuMaxPrtlMass); m_pfSimuInfo += tmpStr;
	if (m_pfuUpsamplePauseSteps == CSimuManager::m_maxUpsamplePauseSteps)
	{
		sprintf(tmpStr, "<%d>", CDelaunayTetrahedron::m_dtrNumMissedPoints); m_pfSimuInfo += tmpStr;
		CDelaunayTetrahedron::m_dtrNumMissedPoints = 0;
	}
//	sprintf(tmpStr, " minDR=%.2f", m_pfuMinPrtlDistRatio); m_pfSimuInfo += tmpStr;
	if (m_pfuDownSampled)
	{
		m_pfuDownSampled = false;
		sprintf(tmpStr, " m%d", m_pfuNewDownsampledPrtls.m_paNum); m_pfSimuInfo += tmpStr;
	}
	if (m_pfuResampled)
	{
		m_pfuResampled = false;
		sprintf(tmpStr, "(%d)", m_pfuUpsampledPrtls.m_paNum); m_pfSimuInfo += tmpStr;
	}
	if (m_pfuSrfcCheckedByConvexHull)
	{
		m_pfuSrfcCheckedByConvexHull = false;
		sprintf(tmpStr, "[s]"); m_pfSimuInfo += tmpStr;
	}
}

void CPrtlFluidUpsample::DownsampleFluidParticles()
{
	char location[] = "CPrtlFluidUpsample::DownsampleFluidParticles()";

	m_pfuNewDownsampledPrtls.ResetArraySize(0);
	CFluidPrtl* newPrtl;
	CFluidPrtl* closestPrtl;
	SimuValue closestDist;
	SimuValue deletedPrtlMass = 0;
	m_pfuMinPrtlDistRatio = SIMU_VALUE_MAX;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_geDeleted) continue; // pPrtl have been deleted as close ngbr
		if (pPrtl->m_fpFixedMotion) continue;
		if (pPrtl->m_fpNgbrs.m_paNum <= 0 && !pPrtl->m_fpFixedMotion)
		{ // no ngbr prtls
			deletedPrtlMass += pPrtl->m_fpMass;
			m_pfPrtlSearch.DeRegisterOnePrtl(pPrtl);
			pPrtl->m_geDeleted = true;
			continue;
		}
		closestDist = CFluidPrtl::GetClosestNgbrPrtl(pPrtl, closestPrtl, pPrtl->m_fpNgbrs);
		if (closestPrtl == NULL)
		{
			pPrtl->m_geHighlit = true;
			CSimuMsg::ExitWithMessage(location, "At least one ngbr prtl must be closestDist.");
			pPrtl->m_geHighlit = false;
			continue;
		}
		if (closestPrtl->m_geDeleted)
			CSimuMsg::ExitWithMessage(location, "Close ngbr has been deleted.");
		if (m_pfuMinPrtlDistRatio > closestDist)
			m_pfuMinPrtlDistRatio = closestDist;
		if (closestPrtl->m_fpFixedMotion
		 && closestDist > CSimuManager::m_intolerablePrtlDist)
			continue;
		if (CSimuManager::NOT_MERGE_COLLIDING_PRTLS
		 && (pPrtl->m_fpInCollision || closestPrtl->m_fpInCollision)
		 && closestDist > CSimuManager::m_intolerablePrtlDist)
			continue;
		if (closestDist <= m_pfuDownsampleDistThrshd)
		{
			if (pPrtl->m_fpMass + closestPrtl->m_fpMass <= m_pfuUpperPrtlMassLimit)
			{
				if (closestPrtl->m_fpFixedMotion)
					newPrtl = MergeTwoPrtls(closestPrtl, pPrtl, true);
				else
					newPrtl = MergeTwoPrtls(pPrtl, closestPrtl, false);
				if (newPrtl != NULL)
					m_pfuNewDownsampledPrtls.AppendOnePointer(newPrtl);
			}
		}
	}
	m_pfuMinPrtlDistRatio /= CSimuManager::m_prtlDistance;

	if (m_pfuNewDownsampledPrtls.m_paNum > 0)
		m_pfuDownSampled = true;
	else
		m_pfuDownSampled = false;
	m_pfPrtls.AppendPointersFromArray(m_pfuNewDownsampledPrtls);
	m_pfuNewDownsampledPrtls.CleanDeletedElements();
	m_pfPrtls.CleanDeletedElements();
	if (deletedPrtlMass > 0)
	{
		if (m_pfTotalFreePrtlMass - deletedPrtlMass <= SIMU_VALUE_EPSILON)
			CSimuMsg::ExitWithMessage(location, "Too much prtl mass is deleted.");
		SimuValue massRatio = m_pfTotalFreePrtlMass/(m_pfTotalFreePrtlMass - deletedPrtlMass);
		for (int i=0; i < m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = m_pfPrtls[i];
			if (pPrtl->m_fpFixedMotion) continue;
			pPrtl->m_fpMass *= massRatio;
		}
	}
//	ConfirmSymmetricPrtlNeighbors();
}

CFluidPrtl* CPrtlFluidUpsample::MergeTwoPrtls(CFluidPrtl* pPrtl0, CFluidPrtl* pPrtl1,
											  bool bMergeIntoFirst)
{
	char location[] = "CPrtlFluidUpsample::MergeTwoPrtls(...)";

	if (pPrtl0->m_fpMass <= SIMU_VALUE_EPSILON
	 || pPrtl1->m_fpMass <= SIMU_VALUE_EPSILON)
	{
		CSimuMsg::ExitWithMessage(location, "Prtl mass is too small.");
		return NULL;
	}
	CFluidPrtl* newPrtl = CreateOneFluidPrtl();
	newPrtl->m_fpMass = pPrtl0->m_fpMass + pPrtl1->m_fpMass;
	if (pPrtl0->m_fpNewPrtl && pPrtl1->m_fpNewPrtl)
		newPrtl->m_fpNewPrtl = true;
	else // at least one is not new
		newPrtl->m_fpNewPrtl = false;
	if (bMergeIntoFirst)
	{
		newPrtl->m_vpPos->SetValue(pPrtl0->m_vpPos);
	}
	else
	{
		// compute center of mass as new prtl position
		newPrtl->m_vpPos->SetValue(pPrtl0->m_vpPos, pPrtl0->m_fpMass/newPrtl->m_fpMass);
		newPrtl->m_vpPos->AddedBy(pPrtl1->m_vpPos, pPrtl1->m_fpMass/newPrtl->m_fpMass);
	}
	// interpolate prtl values
	TPointerArray<CFluidPrtl> possibleNgbrPrtls(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	possibleNgbrPrtls.ResetArraySize(0);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(newPrtl->m_vpPos, m_pfSmoothLen, possibleNgbrPrtls);
	newPrtl->InteroplatePrtlValues(possibleNgbrPrtls, m_pfSmoothLen);
	// conserve kinetic energy
	SimuValue oldEnergy = pPrtl0->m_fpMass*pPrtl0->m_vpVel->LengthSquare();
	oldEnergy += pPrtl1->m_fpMass*pPrtl1->m_vpVel->LengthSquare();
	SimuValue newEnergy = newPrtl->m_fpMass*newPrtl->m_vpVel->LengthSquare();
	if (fabs(newEnergy) > SIMU_VALUE_EPSILON)
	{
		SimuValue ratio = sqrt(oldEnergy/newEnergy);
		newPrtl->m_vpVel->ScaledBy(ratio);
		newEnergy = newPrtl->m_fpMass*newPrtl->m_vpVel->LengthSquare();
	}
	// remove two old prtls from neighborhood
	if (!m_pfPrtlSearch.DeRegisterOnePrtl(pPrtl0))
		CSimuMsg::ExitWithMessage(location, "Prtl0 is not found.");
	if (!m_pfPrtlSearch.DeRegisterOnePrtl(pPrtl1))
		CSimuMsg::ExitWithMessage(location, "Prtl1 is not found.");
	pPrtl0->DeregisterFromNgbrPrtls();
	pPrtl1->DeregisterFromNgbrPrtls();
	pPrtl0->m_geDeleted = true;
	pPrtl1->m_geDeleted = true;
	// setup new neighboring relations
	TGeomElemArray<CFluidPrtl> ngbrPrtls(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetNgbrPrtls(newPrtl->m_vpPos, m_pfSmoothLen, ngbrPrtls);
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = ngbrPrtls[i];
		newPrtl->m_fpNgbrs.AppendOnePointer(pPrtl);
		pPrtl->m_fpNgbrs.AppendOnePointer(newPrtl);
	}
	m_pfPrtlSearch.RegisterOnePrtl(newPrtl);
	// determine if on surface
	if ((pPrtl0->m_fpOnSurface && pPrtl1->m_fpOnSurface)
	 || (!pPrtl0->m_fpOnSurface && !pPrtl1->m_fpOnSurface))
	{
		newPrtl->m_fpOnSurface = pPrtl0->m_fpOnSurface;
		if (newPrtl->m_fpOnSurface)
			SetSrfcPrtl(newPrtl);
	}
	else
	{
		if (IsPosOnSrfcByItsConvexHull(newPrtl->m_vpPos, ngbrPrtls))
			SetSrfcPrtl(newPrtl);
		else
			SetInsdPrtl(newPrtl);
	}
	return newPrtl;
}

void CPrtlFluidUpsample::UpsampleFluidParticles()
{
	m_pfuResampled = false;
	m_pfuUpsampledPrtls.ResetArraySize(0);
	TGeomElemArray<CFluidPrtl> ngbrPrtls(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);

	bool bSuccessful;
	m_pfuDTConstructor.m_dtrAbort = false;
	bSuccessful = m_pfuDTConstructor.ConstructDelaunayTetrahedra(m_pfPrtls);
	for (int i = 0; i < m_pfuDTConstructor.m_dtrTetrahedra.m_paNum && bSuccessful; i++)
	{ // m_pfuDTConstructor.m_dtrTetrahedra.m_paNum may increase if new prtl is added.
		CTetraDelaunay* pTetra = m_pfuDTConstructor.m_dtrTetrahedra[i];
		if (pTetra->m_geDeleted
		 || pTetra->m_ttrdCircumRSqr < m_pfuUpsampleMinRadiusSqr
		 || pTetra->IsArtificialTetra())
			continue;
		CPoint3DOnTetra* upsampleP = new CPoint3DOnTetra();
		upsampleP->m_p3dPos.SetValue(&pTetra->m_ttrdCircumCtrPos);
		ngbrPrtls.ResetArraySize(0);
		int status = IsPointValidForUpsample(upsampleP, ngbrPrtls);
		bool bAddedIntoDT = false;
		if (status == ON_FLUID_SURFACE || status == INSIDE_FLUID)
		{
			bAddedIntoDT = m_pfuDTConstructor.AddPointToDelaunayTessellation(upsampleP);
		}
		if (bAddedIntoDT)
		{
			m_pfuResampled = true;
			m_pfuDTConstructor.m_dtrPoints.AppendOnePointer(upsampleP);
			CFluidPrtl* newPrtl = AddingOnePrtlAtUpsamplePos(upsampleP, ngbrPrtls);
			m_pfuUpsampledPrtls.AppendOnePointer(newPrtl);
			if (status == ON_FLUID_SURFACE)
				SetSrfcPrtl(newPrtl);
			else
				SetInsdPrtl(newPrtl);
		}
		else
		{
			delete upsampleP;
			pTetra->m_ttrdNoUpsample = true;
			if (m_pfuDTConstructor.m_dtrAbort)
				break;
		}
	}
	m_pfPrtls.AppendPointersFromArray(m_pfuUpsampledPrtls);

	m_pfuDTConstructor.m_dtrTetraSearchGrid.DeleteTetraArraysInCells();
	m_pfuDTConstructor.m_dtrPoints.ResetArraySize(0);
	m_pfuDTConstructor.m_dtrTetrahedra.ResetArraySize(0);
}

bool CPrtlFluidUpsample::IsPointBreakingFluidBoundaries(CPoint3D* upsampleP)
{
	for (int i=0; i < m_pfBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfBoundaries[i];
		if (pBdry->PosBreakBoundary(&upsampleP->m_p3dPos))
			return true;
	}
	if (m_pfSimuManager == NULL) return false;
	for (int i=0; i < m_pfSimuManager->m_aryBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfSimuManager->m_aryBoundaries[i];
		if (pBdry->PosBreakBoundary(&upsampleP->m_p3dPos))
			return true;
	}
	return false;
}

bool CPrtlFluidUpsample::IsPointBreakingFluidInterfaces(CPoint3D* upsampleP)
{
	if (m_pfSimuManager == NULL) return false;

	CVector3D srfcNormal;
	TGeomElemArray<CFluidPrtl> ngbrPrtls(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	for (int k=0; k < m_pfSimuManager->m_aryPrtlFluid.m_paNum; k++)
	{
		CPrtlFluid* pFluid = m_pfSimuManager->m_aryPrtlFluid[k];
		if (pFluid == this) continue;
		ngbrPrtls.ResetArraySize(0);
		pFluid->m_pfPrtlSearch.GetNgbrPrtls(&upsampleP->m_p3dPos, m_pfSmoothLen, ngbrPrtls);
		if (ngbrPrtls.m_paNum < CSimuManager::m_interSrfcNgbrNumThreshold)
			continue;
		SimuValue distToSrfc = CConvexHull::DistanceFromExtraPointToConvexHull(upsampleP,
																			   ngbrPrtls,
																			   &srfcNormal);
		if (distToSrfc < 0) // inside the other fluid
			return true;
	}
	return false;
}

int CPrtlFluidUpsample::IsPointValidForUpsample(CPoint3D* upsampleP,
												TGeomElemArray<CFluidPrtl> &ngbrPrtls)
{
	if (IsPointBreakingFluidBoundaries(upsampleP))
		return OUTSIDE_FLUID_SURFACE;
	if (IsPointBreakingFluidInterfaces(upsampleP))
		return OUTSIDE_FLUID_SURFACE;

	TGeomElemArray<CFluidPrtl> possibleNgbrPrtls(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(&upsampleP->m_p3dPos, m_pfSmoothLen, possibleNgbrPrtls);
	SimuValue* weights = new SimuValue[possibleNgbrPrtls.m_paNum]; 
	SimuValue totalWeight = 0;
	CVector3D minPos, maxPos; // bounding box
	minPos[X] = SIMU_VALUE_MAX; maxPos[X] = -SIMU_VALUE_MAX;
	minPos[Y] = SIMU_VALUE_MAX; maxPos[Y] = -SIMU_VALUE_MAX;
	minPos[Z] = SIMU_VALUE_MAX; maxPos[Z] = -SIMU_VALUE_MAX;
	for (int i=0; i < possibleNgbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = possibleNgbrPrtls[i];
		SimuValue distSqr = upsampleP->m_p3dPos.GetDistanceSquareToVector(pPrtl->m_vpPos);
		if (distSqr > m_pfSmoothLenSqr)
		{
			weights[i] = SIMU_NEGATIVE_NUMBER;
			continue;
		}
		if (distSqr < m_pfuUpsampleMinRadiusSqr)
		{
			delete weights;
			return TOO_CLOSE_TO_EXISTING_PRTL; // upsample pos is too close to existing prtl 
		}
		ngbrPrtls.AppendOnePointer(pPrtl);
		weights[i] = (pPrtl->m_fpMass/pPrtl->m_fpDensity)
					 *CSimuUtility::SplineWeightFunction(sqrt(distSqr), m_pfSmoothLen);
		totalWeight += weights[i];
		CSimuUtility::AdjustBoundingBoxWithPos(minPos, maxPos, pPrtl->m_vpPos);
	}
	if (ngbrPrtls.m_paNum < CSimuManager::m_minNumNgbrForUpsample)
	{
		delete weights;
		return OUTSIDE_FLUID_SURFACE;
	}
	// enlarge bounding box with surface thickness
	minPos[X] -= m_pfuSrfcThickness; maxPos[X] += m_pfuSrfcThickness;
	minPos[Y] -= m_pfuSrfcThickness; maxPos[Y] += m_pfuSrfcThickness;
	minPos[Z] -= m_pfuSrfcThickness; maxPos[Z] += m_pfuSrfcThickness;
	if (CSimuUtility::IsPosOutsideBoundingBox(minPos, maxPos, &upsampleP->m_p3dPos))
	{
		delete weights;
		return OUTSIDE_FLUID_SURFACE;
	}

	// compute mass as weighted average mass of ngbr prtls,
	SimuValue mass = 0;
	if (totalWeight > SIMU_VALUE_EPSILON)
	{
		for (int i=0; i < possibleNgbrPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = possibleNgbrPrtls[i];
			if (weights[i] < 0) continue;
			SimuValue deltaMass = pPrtl->m_fpMass*weights[i]/totalWeight;
			if (pPrtl->m_fpMass - deltaMass < m_pfuLowerPrtlMassLimit)
			{ // new prtl would make ngbr prtl mass to be lower than limit
				mass = -1; // this value make this function to return TOO_SMALL_MASS
				break; 
			}
			mass += deltaMass;
		}
	}
	delete weights;
	if (mass < m_pfuLowerPrtlMassLimit)
		return TOO_SMALL_MASS;

	CVector3D srfcNormal;
	SimuValue distToSrfc = CConvexHull::DistanceFromExtraPointToConvexHull(upsampleP,
																		   ngbrPrtls,
																		   &srfcNormal);
	if (distToSrfc > m_pfuSrfcThickness)
	{
		return OUTSIDE_FLUID_SURFACE;
	}
	else if (distToSrfc < -m_pfuSrfcThickness)
	{
		return INSIDE_FLUID;
	}
	return ON_FLUID_SURFACE;
}

SimuValue CPrtlFluidUpsample::ComputeMassAtPos(CVector3D* pPos,
											   TGeomElemArray<CFluidPrtl> &ngbrPrtls)
{
	TGeomElemArray<CFluidPrtl> possibleNgbrPrtls(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(pPos, m_pfSmoothLen, possibleNgbrPrtls);
	SimuValue* weights = new SimuValue[possibleNgbrPrtls.m_paNum]; 
	SimuValue totalWeight = 0;
	for (int i=0; i < possibleNgbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = possibleNgbrPrtls[i];
		SimuValue distSqr  = pPos->GetDistanceSquareToVector(pPrtl->m_vpPos);
		if (distSqr > m_pfSmoothLenSqr)
		{
			weights[i] = SIMU_NEGATIVE_NUMBER;
		}
		else
		{
			ngbrPrtls.AppendOnePointer(pPrtl);
			weights[i] = (pPrtl->m_fpMass/pPrtl->m_fpDensity)
						 *CSimuUtility::SplineWeightFunction(sqrt(distSqr), m_pfSmoothLen);
			totalWeight += weights[i];
		}
	}
	// compute mass as weighted average mass of ngbr prtls,
	SimuValue mass = 0;
	if (totalWeight > SIMU_VALUE_EPSILON)
	{
		for (int i=0; i < possibleNgbrPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = possibleNgbrPrtls[i];
			if (weights[i] < 0) continue;
			SimuValue deltaMass = pPrtl->m_fpMass*weights[i]/totalWeight;
			mass += deltaMass;
		}
	}
	delete weights;
	return mass;
}

CFluidPrtl* CPrtlFluidUpsample::AddingOnePrtlAtUpsamplePos(CPoint3D* upsampleP,
														   TGeomElemArray<CFluidPrtl>
														   &ngbrPrtls)
{
	char location[] = "CPrtlFluidUpsample::AddingOnePrtlAtUpsamplePos(...)";

	m_pfuPrtlNgbrs = &ngbrPrtls;
	m_pfuHighlitPos = upsampleP;
	m_pfuCircumSphCtr = upsampleP;
	m_pfuCircumSphRSqr = pow(m_pfSmoothLen, 2);

	CFluidPrtl* newPrtl = CreateOneFluidPrtl();
	newPrtl->m_fpNewPrtl = true;
	newPrtl->m_vpPos->SetValue(&upsampleP->m_p3dPos);

	newPrtl->SetVirtualPrtlColor(CDelaunayTetrahedron::m_dtrCircumCtrColor);
	CSimuMsg::CancelablePauseMessage("Added upsample prtl.", m_pfuPause[2]);
	// add new prtl to prtl search and setup new neighboring relations
	// and compute total weight for new prtl mass assignment
	m_pfPrtlSearch.RegisterOnePrtl(newPrtl);
	SimuValue* weight = new SimuValue[ngbrPrtls.m_paNum]; 
	SimuValue totalWeight = 0;
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = ngbrPrtls[i];
		pPrtl->m_fpNgbrs.AppendOnePointer(newPrtl);
		newPrtl->m_fpNgbrs.AppendOnePointer(pPrtl);

		SimuValue distance = pPrtl->m_vpPos->GetDistanceToVector(newPrtl->m_vpPos);
		SimuValue distWeight = CSimuUtility::SplineWeightFunction(distance, m_pfSmoothLen);
		weight[i] = (pPrtl->m_fpMass/pPrtl->m_fpDensity)*distWeight;
		totalWeight += weight[i];
	}
	
	// compute new prtl mass as weighted average mass of ngbr prtls,
	// compute old and new kinetic energy
	SimuValue oldEnergy = 0;
	SimuValue newEnergyFixedPrtls = 0;
	SimuValue newEnergyNonFixedPrtls = 0;
	newPrtl->m_fpMass = 0;
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = ngbrPrtls[i];
		SimuValue velSquare = pPrtl->m_vpVel->LengthSquare();
		oldEnergy += pPrtl->m_fpMass*velSquare;
		SimuValue deltaMass = pPrtl->m_fpMass*weight[i]/totalWeight;
		pPrtl->m_fpMass -= deltaMass;
		newPrtl->m_fpMass += deltaMass;
		if (pPrtl->m_fpFixedMotion)
			newEnergyFixedPrtls += pPrtl->m_fpMass*velSquare;
		else
			newEnergyNonFixedPrtls += pPrtl->m_fpMass*velSquare;
	}
	delete weight;

	newPrtl->InteroplatePrtlValues(ngbrPrtls, m_pfSmoothLen);
	newEnergyNonFixedPrtls += newPrtl->m_fpMass*newPrtl->m_vpVel->LengthSquare();
	// conserve kinetic energy
	if (fabs(newEnergyNonFixedPrtls) > SIMU_VALUE_EPSILON)
	{
		if (oldEnergy < newEnergyFixedPrtls)
			CSimuMsg::ExitWithMessage(location,
					"Since mass is reduced on ngbr prtls, newEnergyFixedPrtls must be <= oldEnergy");
		SimuValue ratio = sqrt((oldEnergy-newEnergyFixedPrtls)/newEnergyNonFixedPrtls);
		for (int i=0; i < ngbrPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = ngbrPrtls[i];
			if (!pPrtl->m_fpFixedMotion)
				pPrtl->m_vpVel->ScaledBy(ratio);
		}
		newPrtl->m_vpVel->ScaledBy(ratio);
	}

	m_pfuPrtlNgbrs = NULL;
	m_pfuHighlitPos = NULL;
	m_pfuCircumSphCtr = NULL;

	return newPrtl;
}
