// FluidPrtl.cpp: implementation of the CFluidPrtl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "FluidPrtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFluidPrtl::CFluidPrtl()
{
	m_fpOnSurface = false;
	m_fpFixedMotion = false;
	m_fpNoDelaunay = false;
	m_fpInCollision = false;
	m_fpNewPrtl = false;
	m_fpMass = 0;
	m_fpDensity = 0;
	m_fpPressure = 0;
	m_fpHTRate = 0;
	m_fpTemperature = CSimuManager::m_prtlIniTemperature;

	m_fpNgbrs.InitializeClassArray(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
}

CFluidPrtl::~CFluidPrtl()
{

}

void CFluidPrtl::SetAsFixedMotion(SimuValue airPressure)
{
	m_fpFixedMotion = true;
	if (!m_fpNoDelaunay)
		SetVirtualPrtlColor(CSimuManager::COLOR_FIXED);
}

void CFluidPrtl::AddVelocity(CVector3D* deltaVel)
{
	m_vpVel->AddedBy(deltaVel);
}

void CFluidPrtl::AddVelocity(CVector3D* deltaVel, SimuValue scalar)
{
	m_vpVel->AddedBy(deltaVel, scalar);
}

void CFluidPrtl::DeregisterFromNgbrPrtls()
{
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		if (!m_fpNgbrs[i]->m_fpNgbrs.RemoveOnePointer(this))
			CSimuMsg::ExitWithMessage("CFluidPrtl::DeregisterFromNgbrPrtls()",
										"This prtl is not found in ngbr list.");
	}
}

SimuValue CFluidPrtl::ComputeDistWeightSum(SimuValue radius)
{
	SimuValue dist, weightSum = 0;
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_fpNgbrs[i];
		dist = m_vpPos->GetDistanceToVector(pPrtl->m_vpPos);
		if (dist > radius) continue;
		weightSum += CSimuUtility::SplineWeightFunction(dist, radius);
	}
	weightSum += CSimuUtility::SplineWeightFunction(0, radius);
	return weightSum;
}

SimuValue CFluidPrtl::ComputeDistWeightSumAndGrad(CVector3D &grad, SimuValue radius)
{
	grad.SetValue((SimuValue)0);
	SimuValue dist, weight, weightSum = 0;
	CVector3D relaPosVect;
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_fpNgbrs[i];
		relaPosVect.SetValueAsSubstraction(m_vpPos, pPrtl->m_vpPos);
		dist = relaPosVect.Length();
		if (dist > radius) continue;
		weight = CSimuUtility::SplineWeightFunction(dist, radius);
		weightSum += weight;
		if (dist > SIMU_VALUE_EPSILON)
			grad.AddedBy(&relaPosVect, weight/dist);
	}
	weightSum += CSimuUtility::SplineWeightFunction(0, radius);
	return weightSum;
}

void CFluidPrtl::ComputePrtlNumDenAndGradLen(SimuValue radius,
											 SimuValue &pnd, SimuValue &gradLen)
{
	pnd = 0;
	CVector3D grad, vector;
	SimuValue dist, gradWeight;
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = m_fpNgbrs[i];
		vector.SetValueAsSubstraction(m_vpPos, ngbrPrtl->m_vpPos);
		dist = vector.Length();
		if (dist > radius) continue;
		SimuValue ngbrVol = ngbrPrtl->m_fpMass/ngbrPrtl->m_fpDensity;
		pnd += ngbrVol*CSimuUtility::SplineWeightFunction(dist, radius);

		if (dist <= SIMU_VALUE_EPSILON) continue;
		gradWeight = - ngbrVol*CSimuUtility::SplineGradientWeightFunction(dist, radius);
		grad.AddedBy(&vector, gradWeight);
	}
	gradLen = grad.Length();
}

void CFluidPrtl::ComputePrtlNumDenGradLen(SimuValue radius, SimuValue &gradLen)
{
	CVector3D grad, vector;
	SimuValue dist, gradWeight;
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = m_fpNgbrs[i];
		vector.SetValueAsSubstraction(m_vpPos, ngbrPrtl->m_vpPos);
		dist = vector.Length();
		if (dist > radius) continue;
		SimuValue ngbrVol = ngbrPrtl->m_fpMass/ngbrPrtl->m_fpDensity;
		if (dist <= SIMU_VALUE_EPSILON) continue;
		gradWeight = - ngbrVol*CSimuUtility::SplineGradientWeightFunction(dist, radius);
		grad.AddedBy(&vector, gradWeight);
	}
	gradLen = grad.Length();
}

void CFluidPrtl::ComputePrtlDensity(SimuValue radius)
{
	char location[] = "CFluidPrtl::ComputePrtlDensity(radius)";

	SimuValue dist, weight;
	m_fpDensity = 0;
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		dist = m_vpPos->GetDistanceToVector(m_fpNgbrs[i]->m_vpPos);
		if (dist > radius) continue;
		weight = CSimuUtility::SplineWeightFunction(dist, radius);
		m_fpDensity += weight*m_fpNgbrs[i]->m_fpMass;
	}

	if (m_fpMass < SIMU_VALUE_EPSILON)
		CSimuMsg::ExitWithMessage(location, "Particle has no mass.");
	m_fpDensity += m_fpMass*CSimuUtility::SplineWeightFunction(0, radius);

	if (m_fpDensity < SIMU_VALUE_EPSILON)
		CSimuMsg::ExitWithMessage(location, "Particle has no density.");
}

void CFluidPrtl::ComputePrtlPressureGradient(SimuValue radius)
{
	CVector3D presGrad;
	m_fpPresGrad.SetValue((SimuValue)0);
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		ComputePrtlPressureGradientFromOneNgbr(m_fpNgbrs[i], presGrad, radius);
		m_fpPresGrad.AddedBy(&presGrad);
	}
}

void CFluidPrtl::ComputePrtlPressureGradientFromOneNgbr(CFluidPrtl* ngbrPrtl,
														CVector3D &presGrad,
														SimuValue radius)
{
	char location[] = "CFluidPrtl::ComputePrtlPressureGradientFromOneNgbr(...)";

	if (this == ngbrPrtl)
		CSimuMsg::ExitWithMessage(location, "Neighbor prtl is same as <this> prtl.");

	SimuValue dist, weight;

	presGrad.SetValueAsSubstraction(m_vpPos, ngbrPrtl->m_vpPos);
	dist = presGrad.Length();
	if (dist < SIMU_VALUE_EPSILON)
	{
//		CSimuMsg::PauseMessage("Two prtls are too close.", CSimuManager::m_bSkipWarning);
		return;
	}
	presGrad.ScaledBy(1/dist);
	if (CSimuManager::m_bSpikyGradient)
		weight = CSimuUtility::SpikyGradientWeightFunction(dist, radius);
	else
		weight = CSimuUtility::SplineGradientWeightFunction(dist, radius);
	weight *= m_fpPressure/pow(m_fpDensity, 2)
			+ ngbrPrtl->m_fpPressure/pow(ngbrPrtl->m_fpDensity, 2);
	weight *= - ngbrPrtl->m_fpMass;
	presGrad.ScaledBy(weight);
//	CSimuUtility::AssertSimuVector3D(&presGrad);
}

SimuValue CFluidPrtl::ComputeHeatChange(SimuValue radius)
{
	SimuValue heatChange = 0;
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		heatChange += ComputeHeatChangeFromOneNgbr(m_fpNgbrs[i], radius);
	}
	return heatChange;
}

SimuValue CFluidPrtl::ComputeHeatChangeFromOneNgbr(CFluidPrtl* ngbrPrtl, SimuValue radius)
{
	SimuValue dist = ngbrPrtl->m_vpPos->GetDistanceToVector(m_vpPos);
	SimuValue heatChange = ngbrPrtl->m_fpTemperature - m_fpTemperature;
	if (CSimuManager::NOT_CONSERVE_HEAT)
		heatChange *= ngbrPrtl->m_fpMass/ngbrPrtl->m_fpDensity;
	else
		heatChange *= (ngbrPrtl->m_fpMass + m_fpMass)/(ngbrPrtl->m_fpDensity + m_fpDensity);
	heatChange *= CSimuUtility::SplineWeightFunction(dist, radius);
	return heatChange;
}

void CFluidPrtl::ComputeViscosity(CVector3D &viscosity, SimuValue radius)
{
	CVector3D interVisc;
	viscosity.SetValue((SimuValue)0);
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		ComputeViscosityFromOneNgbr(m_fpNgbrs[i], interVisc, radius);
		viscosity.AddedBy(&interVisc);
	}
	viscosity.ScaledBy(CSimuManager::m_viscosity);
}

void CFluidPrtl::ComputeViscosityFromOneNgbr(CFluidPrtl* ngbrPrtl, CVector3D &interVisc,
											 SimuValue radius)
{
	char location[] = "CFluidPrtl::ComputeViscosityFromOneNgbr(...)";

	if (this == ngbrPrtl)
		CSimuMsg::ExitWithMessage(location, "Neighbor prtl is same as <this> prtl.");

	SimuValue dist, weight;

	interVisc.SetValueAsSubstraction(ngbrPrtl->m_vpVel, m_vpVel);
	dist = m_vpPos->GetDistanceToVector(ngbrPrtl->m_vpPos);
	weight = CSimuUtility::SplineGradientWeightFunction(dist, radius);
	weight *= ngbrPrtl->m_fpMass/(pow(m_fpDensity, 2) + pow(ngbrPrtl->m_fpDensity, 2));
	interVisc.ScaledBy(weight);
//	CSimuUtility::AssertSimuVector3D(&interVisc);
}

void CFluidPrtl::ComputeVelocityGradient(SimuValue velGrad[3][3], SimuValue radius)
{
	int i, j, k;
	for (i=0; i < 3; i++)
	for (j=0; j < 3; j++)
		velGrad[i][j] = 0;

	CVector3D tmpNormal;
	tmpNormal.SetValue((SimuValue)0);
	SimuValue tmpWeight;
	for(k=0; k < m_fpNgbrs.m_paNum; k++)
	{
		CFluidPrtl* pNgbrPrtl = m_fpNgbrs[k];
		tmpNormal.SetValueAsSubstraction(m_vpPos, pNgbrPrtl->m_vpPos);
		SimuValue tmpDist = tmpNormal.Length();
		// get normal direction
		if (tmpDist <= SIMU_VALUE_EPSILON)
			continue;
		tmpWeight = CSimuUtility::SplineGradientWeightFunction(tmpDist, radius);
		tmpWeight *= pNgbrPrtl->m_fpMass/pNgbrPrtl->m_fpDensity;
		tmpNormal.ScaledBy(tmpWeight/tmpDist);
		for (i=0; i < 3; i++)
		{
			SimuValue deltaVel_i_ = pNgbrPrtl->m_vpVel->v[i] - m_vpVel->v[i];
			for (j=0; j < 3; j++)
			{
				velGrad[i][j] += deltaVel_i_*tmpNormal[j];
//				CSimuUtility::AssertSimuValue(velGrad[i][j]);
			}
		}
	}
}

SimuValue CFluidPrtl::ComputeMaxRelativeVelMagnitude()
{
	SimuValue maxMagnitude = -SIMU_VALUE_MAX;
	CVector3D relativeVel;
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		relativeVel.SetValueAsSubstraction(m_fpNgbrs[i]->m_vpVel, m_vpVel);
		SimuValue velMagnitude = relativeVel.Length();
		if (maxMagnitude < velMagnitude)
			maxMagnitude = velMagnitude;
	}
	return maxMagnitude;
}

SimuValue CFluidPrtl::ComputeMaxRelativePresGradMagnitude()
{
	SimuValue maxMagnitude = -SIMU_VALUE_MAX;
	CVector3D relativePresGrad;
	for (int i=0; i < m_fpNgbrs.m_paNum; i++)
	{
		relativePresGrad.SetValueAsSubstraction(&(m_fpNgbrs[i]->m_fpPresGrad), &m_fpPresGrad);
		SimuValue velMagnitude = relativePresGrad.Length();
		if (maxMagnitude < velMagnitude)
			maxMagnitude = velMagnitude;
	}
	return maxMagnitude;
}

// interpolate prtl orgDensity, density, temperature, and velocity
void CFluidPrtl::InteroplatePrtlValues(TPointerArray<CFluidPrtl> &aryPossibleNgbrs,
									   SimuValue radius)
{
	char location[] = "CFluidPrtl::InteroplatePrtlValues(...)";

	ResetPrtlValues();

	SimuValue dist, weight, totalWeight = 0;
	SimuValue radiusSqr = radius*radius;
	SimuValue tmpDistSqr;
	for (int k=0; k < aryPossibleNgbrs.m_paNum; k++)
	{
		CFluidPrtlNonNew* dataPrtl = (CFluidPrtlNonNew*)aryPossibleNgbrs[k];
		tmpDistSqr = m_vpPos->GetDistanceSquareToVector(dataPrtl->m_vpPos);
		if (tmpDistSqr > radiusSqr) continue;
		dist = sqrt(tmpDistSqr);
		if (dist <= SIMU_VALUE_EPSILON)
		{
			m_fpOrgDensity = dataPrtl->m_fpOrgDensity;
			m_fpDensity = dataPrtl->m_fpDensity;
			m_fpTemperature = dataPrtl->m_fpTemperature;
			m_vpVel->SetValue(dataPrtl->m_vpVel);
			return;
		}
		weight = CSimuUtility::SplineWeightFunction(dist, radius);
		weight *= dataPrtl->m_fpMass/dataPrtl->m_fpDensity;
		totalWeight += weight;

		m_fpOrgDensity += weight*dataPrtl->m_fpOrgDensity;
		m_fpDensity += weight*dataPrtl->m_fpDensity;
		m_fpTemperature += weight*dataPrtl->m_fpTemperature;
		m_vpVel->AddedBy(dataPrtl->m_vpVel, weight);
	}
	if (totalWeight > SIMU_VALUE_EPSILON)
	{
		m_fpOrgDensity /= totalWeight;
		m_fpDensity /= totalWeight;
		m_fpTemperature /= totalWeight;
		m_vpVel->ScaledBy(1/totalWeight);
	}
	else if (totalWeight == 0)
	{
		m_geHighlit = true;
		CSimuMsg::ExitWithMessage(location, "No data prtl is found within radius.", CSimuManager::m_bSkipWarning);
		m_geHighlit = false;
	}
}

void CFluidPrtl::ResetPrtlValues()
{
	m_fpOrgDensity = 0;
	m_fpDensity = 0;
	m_fpTemperature = 0;
	m_vpVel->SetValue((SimuValue)0);
}

SimuValue CFluidPrtl::GetClosestNgbrPrtl(CFluidPrtl* pPrtl, CFluidPrtl* &closestPrtl,
										 TGeomElemArray<CFluidPrtl> &ngbrPrtls)
{
	closestPrtl = NULL;
	SimuValue minDist = SIMU_VALUE_MAX;
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		SimuValue distSqr = pPrtl->m_vpPos->GetDistanceSquareToVector(ngbrPrtl->m_vpPos);
		if (minDist > distSqr)
		{
			minDist = distSqr;
			closestPrtl = ngbrPrtl;
		}
	}
	minDist = sqrt(minDist);
	return minDist;
}

SimuValue CFluidPrtl::ComputeWeightedPrtlNumDenAndGrad(CVector3D &pos, CVector3D &grad,
													   TPointerArray<CFluidPrtl> &ngbrPrtls,
													   SimuValue radius)
{
	grad.SetElements(0, 0, 0);
	SimuValue prtlNumDensity = 0;
	SimuValue radiusSquare = radius*radius;
	SimuValue dist, weight, volume;
	CVector3D vector;
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		vector.SetValueAsSubstraction(&pos, ngbrPrtl->m_vpPos);
		dist = vector.LengthSquare();
		if (dist > radiusSquare) continue;
		dist = sqrt(dist);
		volume = ngbrPrtl->m_fpMass/ngbrPrtl->m_fpDensity;
		prtlNumDensity += volume*CSimuUtility::SplineWeightFunction(dist, radius);
		if (dist <= SIMU_VALUE_EPSILON) continue;
		vector.ScaledBy(1/dist);
		weight = - volume*CSimuUtility::SplineGradientWeightFunction(dist, radius);
		grad.AddedBy(&vector, weight);
	}
	return prtlNumDensity;
}

void CFluidPrtl::CreateRandomPrtls(CVector3D* ctrPos, SimuValue radius, int numTotalPrtls,
								   TGeomElemArray<CFluidPrtl> &ranPrtls)
{
	int numPrtls = 0;
	int iterations = 0;
	CVector3D ranPos;
	while (numPrtls < numTotalPrtls
		&& iterations < numTotalPrtls*100
			// <ranPos> may be out of radius, thus allow more iterations
		)
	{
		iterations++;
		if (iterations >= numTotalPrtls*100)
			CSimuMsg::ExitWithMessage("CFluidPrtl::CreateRandomPrtls(...)",
									"No enough prtls are created.");

		CSimuUtility::CreateRandomPos(radius, &ranPos);
		if (ranPos.Length() <= radius)
		{
			CFluidPrtl* pPrtl = ranPrtls.CreateOneElement();
			pPrtl->m_vpPos->SetValue(&ranPos);
			pPrtl->m_vpPos->AddedBy(ctrPos);
			numPrtls++;
		}
	}
}

SimuValue CFluidPrtl::ComputeVERatio(CVector3D* presGrad, CVector3D* stress)
{
	SimuValue veRatio = presGrad->DotProduct(stress);
	veRatio /= presGrad->LengthSquare();
	return veRatio;
}

SimuValue CFluidPrtl::ComputeWeightLaplacian(CVector3D* ctrPos, SimuValue radius,
											 TGeomElemArray<CFluidPrtl> &ngbrPrtls)
{
	SimuValue weightLaplacian = 0;
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		SimuValue dist = ctrPos->GetDistanceToVector(ngbrPrtl->m_vpPos);
		if (dist > radius) continue;
		SimuValue volume = ngbrPrtl->m_fpMass/ngbrPrtl->m_fpDensity;
		weightLaplacian += volume*CSimuUtility::PolySecondDerivativeWeightFunction(dist, radius);
	}
	return weightLaplacian;
}

void CFluidPrtl::ComputeWeightGradient(CVector3D* ctrPos, SimuValue radius,
									   TGeomElemArray<CFluidPrtl> &ngbrPrtls, CVector3D* grad)
{
	grad->SetElements(0, 0, 0);
	SimuValue radiusSquare = radius*radius;
	CVector3D vector;
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		vector.SetValueAsSubstraction(ctrPos, ngbrPrtl->m_vpPos);
		SimuValue dist = vector.LengthSquare();
		if (dist > radiusSquare) continue;
		dist = sqrt(dist);
		SimuValue volume = ngbrPrtl->m_fpMass/ngbrPrtl->m_fpDensity;
		if (dist <= SIMU_VALUE_EPSILON) continue;
		vector.ScaledBy(1/dist);
		SimuValue weight = - volume*CSimuUtility::SplineGradientWeightFunction(dist, radius);
		grad->AddedBy(&vector, weight);
	}
}
