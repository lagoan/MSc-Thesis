// FluidPrtlNonNew.cpp: implementation of the CFluidPrtlNonNew class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "FluidPrtlNonNew.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CFluidPrtlNonNew::m_fpnnMRow = 9;
int CFluidPrtlNonNew::m_fpnnMCol = 10;
SimuValue** CFluidPrtlNonNew::m_fpnnM = NULL;
SimuValue* CFluidPrtlNonNew::m_fpnnX = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFluidPrtlNonNew::CFluidPrtlNonNew()
{
	m_fpnnStrTen[0][0] = SIMU_PARA_INITIAL_DIAGONAL_STRESS;
	m_fpnnStrTen[1][1] = SIMU_PARA_INITIAL_DIAGONAL_STRESS;
	m_fpnnStrTen[2][2] = SIMU_PARA_INITIAL_DIAGONAL_STRESS;
	m_fpnnStrTen[0][1] = 0;
	m_fpnnStrTen[0][2] = 0;
	m_fpnnStrTen[1][0] = 0;
	m_fpnnStrTen[1][2] = 0;
	m_fpnnStrTen[2][0] = 0;
	m_fpnnStrTen[2][1] = 0;
}

CFluidPrtlNonNew::~CFluidPrtlNonNew()
{

}

void CFluidPrtlNonNew::IntegratePrtlStressTensorByPlasticModel(SimuValue velGrad[3][3],
															   SimuValue timeStep,
															   SimuValue shearModulus,
															   SimuValue decayRate,
															   ERotation rotationDerivative)
{
	char location[] = "CFluidPrtlNonNew::IntegratePrtlStressTensorByPlasticModel(...)";

	int i, j, k, row;
	SimuValue strTenRate[3][3];
	ComputeDeviatedStressTensor(m_fpnnStrTen);
	SimuValue tensorNorm = CSimuUtility::Compute3x3MatrixFrobNorm(m_fpnnStrTen);
	SimuValue plasticRatio;
	if (tensorNorm > CSimuManager::m_plasticYieldValue)
		plasticRatio = (tensorNorm - CSimuManager::m_plasticYieldValue)
					  *decayRate/tensorNorm;
	else
		plasticRatio = 0;
	for (i=0; i < 3; i++) // run on tensor row, alpha
	for (j=0; j < 3; j++) // run on tensor column, beta
	{
		strTenRate[i][j] = 0.5*(velGrad[i][j] + velGrad[j][i]);
		strTenRate[i][j] *= shearModulus;
		strTenRate[i][j] -= plasticRatio*m_fpnnStrTen[i][j];
		if (rotationDerivative == CONVECTED)
		{
			strTenRate[i][j] += ComputeUpperConvectedElement(i, j, velGrad, m_fpnnStrTen);
		}
		else if (rotationDerivative == COROTATIONAL)
		{
			strTenRate[i][j] += ComputeCorotationalElement(i, j, velGrad, m_fpnnStrTen);
		}
		// else no rotation
		if (CSimuManager::IMPLICIT_TENSOR_INTEGRATION)
		{
			row = i*3+j;	// matrix row
			// constant vector in matrix
			m_fpnnM[row][9] = strTenRate[i][j]*timeStep;
			// compute non-zero elements in matrix row
			m_fpnnM[row][row] = 1 + decayRate*timeStep;
			for (k=0; k < 3; k++) // run on gamma
			{
				if (k != j)	// gamma != beta
					m_fpnnM[row][i*3+k] = (velGrad[k][j] - velGrad[j][k])*timeStep;
				if (k != i)	// gamma != alpha
					m_fpnnM[row][k*3+j] = (velGrad[i][k] - velGrad[k][i])*timeStep;
			}
		}
	}

	if (CSimuManager::IMPLICIT_TENSOR_INTEGRATION)
	{
		if (!CSimuUtility::SolveNxNLinearEquations(m_fpnnM, m_fpnnX, 9))
		{
			CSimuUtility::Print3x3Matrix(velGrad);
		}
	}

	for (i=0; i < 3; i++)
	for (j=0; j < 3; j++)
	{
		if (CSimuManager::IMPLICIT_TENSOR_INTEGRATION)
			m_fpnnStrTen[i][j] += m_fpnnX[i*3+j];
		else
			m_fpnnStrTen[i][j] += strTenRate[i][j]*timeStep;
	}
}

void CFluidPrtlNonNew::IntegratePrtlStressTensorBySPHModel(SimuValue velGrad[3][3],
														   SimuValue timeStep,
														   SimuValue shearModulus,
														   SimuValue decayRate,
														   ERotation rotationDerivative)
{
	char location[] = "CFluidPrtlNonNew::IntegratePrtlStressTensorBySPHModel(...)";

	int i, j, k, row;
	SimuValue strTenRate[3][3];
	for (i=0; i < 3; i++) // run on tensor row, alpha
	for (j=0; j < 3; j++) // run on tensor column, beta
	{
		strTenRate[i][j] = ComputeSPHStressTensorRateElement(i, j, decayRate, shearModulus,
															 velGrad, m_fpnnStrTen);
		if (rotationDerivative == CONVECTED)
		{
			strTenRate[i][j] += ComputeUpperConvectedElement(i, j, velGrad, m_fpnnStrTen);
		}
		else if (rotationDerivative == COROTATIONAL)
		{
			strTenRate[i][j] += ComputeCorotationalElement(i, j, velGrad, m_fpnnStrTen);
		}
		// else no rotation

		if (CSimuManager::IMPLICIT_TENSOR_INTEGRATION)
		{
			row = i*3+j;	// matrix row
			// constant vector in matrix
			m_fpnnM[row][9] = strTenRate[i][j]*timeStep;
			// compute non-zero elements in matrix row
			m_fpnnM[row][row] = 1 + decayRate*timeStep;
			for (k=0; k < 3; k++) // run on gamma
			{
				if (k != j)	// gamma != beta
					m_fpnnM[row][i*3+k] = (velGrad[k][j] - velGrad[j][k])*timeStep;
				if (k != i)	// gamma != alpha
					m_fpnnM[row][k*3+j] = (velGrad[i][k] - velGrad[k][i])*timeStep;
			}
		}
	}

	if (CSimuManager::IMPLICIT_TENSOR_INTEGRATION)
	{
		if (!CSimuUtility::SolveNxNLinearEquations(m_fpnnM, m_fpnnX, 9))
		{
			CSimuUtility::Print3x3Matrix(velGrad);
		}
	}

	for (i=0; i < 3; i++)
	for (j=0; j < 3; j++)
	{
		if (CSimuManager::IMPLICIT_TENSOR_INTEGRATION)
			m_fpnnStrTen[i][j] += m_fpnnX[i*3+j];
		else
			m_fpnnStrTen[i][j] += strTenRate[i][j]*timeStep;
	}
}

void CFluidPrtlNonNew::IntegratePrtlVelocityByStressTensor(CVector3D &stress,
														   SimuValue timeStep,
														   SimuValue smoothLen)
{
	if (m_fpFixedMotion) return;

	ComputeStressOnPrtl(m_fpStress, smoothLen);
	AddVelocity(&m_fpStress, timeStep);
}

void CFluidPrtlNonNew::ComputeStressOnPrtl(CVector3D &stress, SimuValue smoothLen)
{
	stress.SetValue((SimuValue)0);
	CVector3D tmpNormal;
	SimuValue tmpWeight;
	SimuValue denReciprocal = 1/(m_fpDensity*m_fpDensity);
	for(int k=0; k < m_fpNgbrs.m_paNum; k++)
	{
		CFluidPrtlNonNew* pNgbrPrtl = (CFluidPrtlNonNew*)m_fpNgbrs[k];
		tmpNormal.SetValueAsSubstraction(m_vpPos, pNgbrPrtl->m_vpPos);
		SimuValue tmpDist = tmpNormal.Length();
		// get normal direction
		if (tmpDist > SIMU_VALUE_EPSILON)
			tmpNormal.ScaledBy(1/tmpDist);
		else
			continue;

		tmpWeight = StressFirstDerivativeWeightFunction(tmpDist, smoothLen);
		tmpNormal.ScaledBy(tmpWeight);
		for (int i=0; i < 3; i++)
		{
			for (int j=0; j < 3; j++) // loop on gama
				stress[i] += (m_fpMass + pNgbrPrtl->m_fpMass)*0.5
							* tmpNormal[j]
							* (m_fpnnStrTen[i][j]*denReciprocal
							  + pNgbrPrtl->m_fpnnStrTen[i][j]/pow(pNgbrPrtl->m_fpDensity, 2));
		}
	}
}

// interpolate prtl orgDensity, density, temperature, velocity, and stress tensor
void CFluidPrtlNonNew::InteroplatePrtlValues(TPointerArray<CFluidPrtl> &aryPossibleNgbrs,
											 SimuValue radius)
{
	char location[] = "CFluidPrtlNonNew::InteroplatePrtlValues(...)";

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
			for (int m=0; m < 3; m++)
			for (int n=0; n < 3; n++)
				m_fpnnStrTen[m][n] = dataPrtl->m_fpnnStrTen[m][n];
			return;
		}
		weight = CSimuUtility::SplineWeightFunction(dist, radius);
		weight *= dataPrtl->m_fpMass/dataPrtl->m_fpDensity;
		totalWeight += weight;

		m_fpOrgDensity += weight*dataPrtl->m_fpOrgDensity;
		m_fpDensity += weight*dataPrtl->m_fpDensity;
		m_fpTemperature += weight*dataPrtl->m_fpTemperature;
		m_vpVel->AddedBy(dataPrtl->m_vpVel, weight);
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
			m_fpnnStrTen[m][n] += weight*dataPrtl->m_fpnnStrTen[m][n];
	}
	if (totalWeight > SIMU_VALUE_EPSILON)
	{
		m_fpOrgDensity /= totalWeight;
		m_fpDensity /= totalWeight;
		m_fpTemperature /= totalWeight;
		m_vpVel->ScaledBy(1/totalWeight);
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
			m_fpnnStrTen[m][n] /= totalWeight;
	}
	else if (totalWeight == 0)
	{
		m_geHighlit = true;
		CSimuMsg::ExitWithMessage(location, "No data prtl is found within radius.", CSimuManager::m_bSkipWarning);
		m_geHighlit = false;
	}
}

void CFluidPrtlNonNew::ResetPrtlValues()
{
	CFluidPrtl::ResetPrtlValues();
	for (int m=0; m < 3; m++)
	for (int n=0; n < 3; n++)
		m_fpnnStrTen[m][n] = 0;
}

void CFluidPrtlNonNew::ComputeDeviatedStressTensor(SimuValue strTen[3][3])
{
	SimuValue tensorTrace = CSimuUtility::Compute3x3MatrixTrace(strTen);
	tensorTrace /= 3.0;
	for (int m=0; m < 3; m++)
	for (int n=0; n < 3; n++)
		if (m == n)
			strTen[m][n] -= tensorTrace;
}

SimuValue CFluidPrtlNonNew::ComputeSPHStressTensorRateElement(int i, int j,
															  SimuValue decayRate,
															  SimuValue shearModulus,
															  SimuValue velGrad[3][3],
															  SimuValue strTen[3][3])
{
	SimuValue strTenRate = 0.5*(velGrad[i][j] + velGrad[j][i]);
	if (i == j)
		strTenRate -= SIMU_CONST_ONE_THIRD*(velGrad[0][0] + velGrad[1][1] + velGrad[2][2]);
	strTenRate *= shearModulus;
	strTenRate -= decayRate*strTen[i][j]; 

	return strTenRate;
}

SimuValue CFluidPrtlNonNew::ComputeUpperConvectedElement(int i, int j,
														 SimuValue velGrad[3][3],
														 SimuValue strTen[3][3])
{
	SimuValue rotTenElmt = 0;
    for (int k=0; k < 3; k++) // run on gamma
	{
		rotTenElmt += velGrad[k][i]*strTen[k][j];
		rotTenElmt += strTen[i][k]*velGrad[k][j];
	}
	rotTenElmt *= -CSimuManager::m_rotationFactor;
	if (fabs(CSimuManager::m_maxAngVelTenElmnt) < fabs(rotTenElmt))
		CSimuManager::m_maxAngVelTenElmnt = rotTenElmt;
	return rotTenElmt;
}

SimuValue CFluidPrtlNonNew::ComputeCorotationalElement(int i, int j,
													   SimuValue velGrad[3][3],
													   SimuValue strTen[3][3])
{
	SimuValue rotTenElmt = 0;
    for (int k=0; k < 3; k++) // run on gamma
	{
		rotTenElmt += (velGrad[k][i] - velGrad[i][k])*strTen[k][j];
		rotTenElmt -= strTen[i][k]*(velGrad[j][k] - velGrad[k][j]);
	}
	rotTenElmt *= -CSimuManager::m_rotationFactor*0.5;
	if (fabs(CSimuManager::m_maxAngVelTenElmnt) < fabs(rotTenElmt))
		CSimuManager::m_maxAngVelTenElmnt = rotTenElmt;
	return rotTenElmt;
}

SimuValue CFluidPrtlNonNew::StressFirstDerivativeWeightFunction(SimuValue distance,
																SimuValue radius)
{
	return CSimuUtility::SplineGradientWeightFunction(distance, radius);
//	return CSimuUtility::SineFirstDerivativeWeightFunction(distance, radius);
//	return CSimuUtility::LinearDerivativeWeightFunction(distance, radius);
//	return CSimuUtility::PolyFirstDerivativeWeightFunction(distance, radius);
//	return CSimuUtility::ViscFirstDerivativeWeightFunction(distance, radius);
}

SimuValue CFluidPrtlNonNew::StressSecondDerivativeWeightFunction(SimuValue distance,
																 SimuValue radius)
{
	return CSimuUtility::SplineSecondDerivativeFunction(distance, radius);
//	return CSimuUtility::SineSecondDerivativeWeightFunction(distance, radius);
//	return CSimuUtility::LinearSecondDerivativeWeightFunction(distance, radius);
//	return CSimuUtility::PolySecondDerivativeWeightFunction(distance, radius);
//	return CSimuUtility::ViscSecondDerivativeWeightFunction(distance, radius);
}
