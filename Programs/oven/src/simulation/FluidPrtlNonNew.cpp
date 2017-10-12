// FluidPrtlNonNew.cpp: implementation of the CFluidPrtlNonNew class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "FluidPrtlNonNew.h"


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
		/*
		if (rotationDerivative == CONVECTED)
		{
			strTenRate[i][j] += ComputeUpperConvectedElement(i, j, velGrad, m_fpnnStrTen);
		}
		else if (rotationDerivative == COROTATIONAL)
		{
			strTenRate[i][j] += ComputeCorotationalElement(i, j, velGrad, m_fpnnStrTen);
		}

		*/

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
#if 1 // backup
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
		/*
		if (rotationDerivative == CONVECTED)
		{
			strTenRate[i][j] += ComputeUpperConvectedElement(i, j, velGrad, m_fpnnStrTen);
		}
		else if (rotationDerivative == COROTATIONAL)
		{
			strTenRate[i][j] += ComputeCorotationalElement(i, j, velGrad, m_fpnnStrTen);
		}
		// else no rotation
		 */
                // XXX CHECK
                strTenRate[i][j] += ComputeCorotationalElement(i, j, velGrad, m_fpnnStrTen);

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
		if (CSimuManager::IMPLICIT_TENSOR_INTEGRATION) {
			m_fpnnStrTen[i][j] += m_fpnnX[i*3+j];
		}
		else {
			m_fpnnStrTen[i][j] += strTenRate[i][j]*timeStep;
			//m_fpnnStrTen[i][j] += strTenRate[i][j]*0.001;
	
		}
	}
}
#endif



#if 0 // option
void CFluidPrtlNonNew::IntegratePrtlStressTensorBySPHModel(SimuValue velocityGradient[3][3],
														   SimuValue timeStep,
														   SimuValue shearModulus,
														   SimuValue decayRate,
														   ERotation rotationDerivative)
{
	char location[] = "CFluidPrtlNonNew::IntegratePrtlStressTensorBySPHModel(...)";
	
	int i, j;
	SimuValue deformationRateTensor[3][3]	= {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	SimuValue strTenRate[3][3]				= {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	
	
	for (int p=0; p<3; p++) {
		for (int q=0; q<3; q++) {
			deformationRateTensor[p][q] = velocityGradient[p][q] + 
			velocityGradient[q][p] ;
		}
	}
	
	SimuValue traceD = deformationRateTensor[0][0] + 
	deformationRateTensor[1][1] + 
	deformationRateTensor[2][2];
	
	
	//SimuValue traceD = 2 * velocityGradient[0][0] + 
	//2 * velocityGradient[1][1] + 
	//2 * velocityGradient[2][2];
	
	SimuValue d = sqrt( 0.5f * traceD * traceD );
	
	//std::cout << " d " << d << " traceD " << traceD << "\n";
	
	//SimuValue J = 100000; // solid-like
	SimuValue J =   100000; // solid-like
	//SimuValue J = 1.0f; // fluid-like
	//SimuValue n = 0.5f;
	SimuValue n = 0.5f;
	//std::cout << " d " << d << "\n";
	SimuValue N = (1.0 - exp(-1.0f * (J + 1.0f) * d)) * (pow(d, n-1.0) + 1.0/d);
	//std::cout << " N = (1.0 - exp(-1.0f * (J + 1.0f) * d))" << (1.0 - exp(-1.0f * (J + 1.0f) * d)) << "    *  (pow(d, n-1.0) + 1.0/d)  " << (pow(d, n-1.0) + 1.0/d) << "\n"; 
	
	
	// S = N(d)D
	
	/*
	 for (i=0; i < 3; i++) {
	 for (j=0; j < 3; j++) {
	 //m_fpnnStrTen[i][j] += N * deformationRateTensor[i][j]*timeStep;
	 m_fpnnStrTen[i][j] = N * deformationRateTensor[i][j];
	 //std::cout << "N " << N << " deformationRateTensor " << deformationRateTensor[i][j] << " m_fpnnStrTen " <<  m_fpnnStrTen[i][j] << " time step " << timeStep <<"\n";
	 }
	 }
	 */
  	
	for (i=0; i < 3; i++) { // run on tensor row, alpha
		for (j=0; j < 3; j++) { // run on tensor column, beta
			strTenRate[i][j] =  N * deformationRateTensor[i][j];
		}
	}
	
	
	for (i=0; i < 3; i++) {
		for (j=0; j < 3; j++) {
			m_fpnnStrTen[i][j] += strTenRate[i][j] * timeStep;
			//m_fpnnStrTen[i][j] = strTenRate[i][j];
		}
	}
	
	
}

#endif 

void CFluidPrtlNonNew::IntegratePrtlVelocityByStressTensor(SimuValue timeStep)
{
	if (m_fpFixedMotion) return;

	ComputeStressOnPrtl(m_fpStress, m_pfSmoothLen);
	AddVelocity(&m_fpStress, timeStep);

}



#if 1
void CFluidPrtlNonNew::ComputeStressOnPrtl(CVector3D &stress, SimuValue smoothLen)
{
	CVector3D tmpNormal;
	SimuValue tmpWeight;
	SimuValue denReciprocal = 1/(m_fpDensity*m_fpDensity);
	
	stress.SetValue((SimuValue)0);
	
	for(int k=0; k < m_fpNgbrs.m_paNum; k++)
	{
		if (this == m_fpNgbrs[k]) continue;
#if 1 // option
		
		CFluidPrtlNonNew* pNgbrPrtl = (CFluidPrtlNonNew*)m_fpNgbrs[k];
		
		
		tmpNormal.SetValueAsSubstraction(m_vpPos, pNgbrPrtl->m_vpPos);
		SimuValue tmpDist = tmpNormal.Length();
		
		tmpNormal.ScaledBy(1.0/tmpDist);
		//tmpWeight = StressFirstDerivativeWeightFunction(tmpDist, smoothLen);
		tmpWeight = SplineGradientWeightFunction(tmpDist, pNgbrPrtl->m_pfSmoothLen);
		tmpNormal.ScaledBy(tmpWeight);
		
		for (int i=0; i < 3; i++) {
			for (int j=0; j < 3; j++) {// loop on gama
				stress[i] += (pNgbrPrtl->m_fpMass / (pNgbrPrtl->m_fpDensity * m_fpDensity))
				* (m_fpnnStrTen[i][j] + pNgbrPrtl->m_fpnnStrTen[i][j]) 
				*  tmpNormal[j];
				
			}
			

		}
		
		
		
#endif
#if 0		 //backup
		
		
		if (this == m_fpNgbrs[k]) continue;
		
		CFluidPrtlNonNew* pNgbrPrtl = (CFluidPrtlNonNew*)m_fpNgbrs[k];
		
		
		
		tmpNormal.SetValueAsSubstraction(m_vpPos, pNgbrPrtl->m_vpPos);
		SimuValue tmpDist = tmpNormal.Length();
		// get normal direction
		if (tmpDist > SIMU_VALUE_EPSILON)
			tmpNormal.ScaledBy(1.0/tmpDist);
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
				   + pNgbrPrtl->m_fpnnStrTen[i][j]/( pNgbrPrtl->m_fpDensity * pNgbrPrtl->m_fpDensity ));   
		}
		
		
#endif
	}
}
#endif

#if 0 // backup
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
			tmpNormal.ScaledBy(1.0/tmpDist);
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
							+ pNgbrPrtl->m_fpnnStrTen[i][j]/pow(pNgbrPrtl->m_fpDensity, 2.0f));
							//+ pNgbrPrtl->m_fpnnStrTen[i][j]/( pNgbrPrtl->m_fpDensity * pNgbrPrtl->m_fpDensity ));   
		}
	}
}
#endif

// interpolate prtl orgDensity, density, temperature, velocity, and stress tensor
void CFluidPrtlNonNew::InterpolatePrtlValues(TPointerArray<CFluidPrtl> &aryPossibleNgbrs,
											 SimuValue radius)
{
	char location[] = "CFluidPrtlNonNew::InterpolatePrtlValues(...)";

	ResetPrtlValues();

	SimuValue dist, weight, totalWeight = 0;
	SimuValue radiusSqr = radius*radius;
	SimuValue tmpDistSqr;
	for (int k=0; k < aryPossibleNgbrs.m_paNum; k++)
	{
		CFluidPrtlNonNew* dataPrtl = (CFluidPrtlNonNew*)aryPossibleNgbrs[k];
		tmpDistSqr = m_vpPos->GetDistanceSquareToVector(dataPrtl->m_vpPos);
		//if (tmpDistSqr > radiusSqr) continue;
		dist = sqrt(tmpDistSqr);
		if (dist <= SIMU_VALUE_EPSILON)
		{
			//if ( ! m_bpIsBubble ) {
				m_fpOrgDensity = dataPrtl->m_fpOrgDensity;
				m_fpDensity = dataPrtl->m_fpDensity;
			//}
			m_fpTemperature = dataPrtl->m_fpTemperature;
			m_vpVel->SetValue(dataPrtl->m_vpVel);
			for (int m=0; m < 3; m++)
			for (int n=0; n < 3; n++)
				m_fpnnStrTen[m][n] = dataPrtl->m_fpnnStrTen[m][n];
			return;
		}
		weight = SplineWeightFunction(dist, dataPrtl->m_pfSmoothLen);
		weight *= dataPrtl->m_fpMass/dataPrtl->m_fpDensity;
		totalWeight += weight;

		//if ( ! m_bpIsBubble ) {
			m_fpOrgDensity += weight*dataPrtl->m_fpOrgDensity;
			m_fpDensity += weight*dataPrtl->m_fpDensity;
		//}
		m_fpTemperature += weight*dataPrtl->m_fpTemperature;
		m_vpVel->AddedBy(dataPrtl->m_vpVel, weight);
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
			m_fpnnStrTen[m][n] += weight*dataPrtl->m_fpnnStrTen[m][n];
	}
	if (totalWeight > SIMU_VALUE_EPSILON)
	{
		//if ( ! m_bpIsBubble ) {
			m_fpOrgDensity /= totalWeight;
			m_fpDensity /= totalWeight;
		//}

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
	
	// XXX decay rate
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

/*
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
*/


void CFluidPrtlNonNew::computeVelocityTunerFromTemperature()
{
	
	
	// modify velocity tuner depending on temperature
	
	// .01
	// .50
	//return 0.5;
	//velocityTuner = 0.05f;
	
	
	double minTemp = 26.0f;
	double maxTemp = 35.0f;
        double minDecay = 0.15f;
	double maxDecay = 0.50f;
	
	velocityTuner = ((maxDecay - minDecay) / (maxTemp - minTemp)) * (m_fpTemperature - minTemp) + minDecay;
	
	if (velocityTuner > maxDecay) {
		velocityTuner = maxDecay;
	}
        //velocityTuner = 0.50f;
        //velocityTuner = 0.3f;
        velocityTuner = 0.3f;
}


