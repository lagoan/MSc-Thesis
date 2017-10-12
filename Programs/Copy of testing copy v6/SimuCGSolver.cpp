// SimuCGSolver.cpp: implementation of the CSimuCGSolver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuCGSolver.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuCGSolver::CSimuCGSolver()
{
	m_matrixSize = 0;
	m_arraySize = 0;
	m_indexArray = NULL;
	m_valueArray = NULL;
	m_B	= NULL;
	m_X	= NULL;
	m_R	= NULL;
	m_Z	= NULL;
	m_P	= NULL;
	m_Q	= NULL;
}

CSimuCGSolver::~CSimuCGSolver()
{
	FreeSparseMatrixStorage();
}

int CSimuCGSolver::SolveSymmetricMatrix(int maxIterations, SimuValue tol)
{
	SimuValue resid, alpha, beta, rho, rho_1;

	SimuValue normb = HugeVectorNormal(m_B);

	// m_X is initialized with initial guess
	
	MatrixMultiply(m_X, m_R);	// then, m_R = A*m_X
	
	HugeVectorAddition(m_R, -1, m_B);	// m_R = m_B - m_R 

	if (normb == 0.0) 
		normb = 1;

	resid = HugeVectorNormal(m_R) / normb;
	if (resid <= tol)
	    return 0;

	int i;
	for (i = 1; i <= maxIterations; i++)
	{
		SolveDiagonalMatrix(m_Z, m_R); // m_Z = M(-1)*m_R
		// no preconditioner
//		for (int j=0; j < m_matrixSize; j++) m_Z[j] = m_R[j];

		rho = HugeVectorDotProduct(m_Z, m_R); // rho = dot(r, z);

		if (i == 1)
		{
			HugeVectorAssignment(m_P, m_Z); // p = z;
		}
		else
		{
			beta = rho / rho_1;
			HugeVectorAddition(m_P, beta, m_Z); // p = z + beta * p;
		}

		MatrixMultiply(m_P, m_Q);	// q = A*p;
		alpha = rho / HugeVectorDotProduct(m_P, m_Q); // dot(p, q);

		HugeVectorAddition(m_X, m_P, alpha);	// x += alpha * p;
		HugeVectorAddition(m_R, m_Q, -alpha);	// r -= alpha * q;

		resid = HugeVectorNormal(m_R) / normb; // resid = norm(r) / normb
		if (resid <= tol)
			return i;

		rho_1 = rho;
	}

	return i;
}

void CSimuCGSolver::FreeSparseMatrixStorage()
{
	if (m_matrixSize > 0)
	{
		m_matrixSize = 0;
		delete m_B; m_B	= NULL;
		delete m_X; m_X	= NULL;
		delete m_R; m_R	= NULL;
		delete m_Z; m_Z	= NULL;
		delete m_P; m_P	= NULL;
		delete m_Q; m_Q	= NULL;
	}
	if (m_arraySize > 0)
	{
		m_arraySize = 0;
		delete []m_indexArray; m_indexArray = NULL;
		delete []m_valueArray; m_valueArray = NULL;
	}
}

void CSimuCGSolver::AllocateSparseMatrixStorage()
{
	m_indexArray = new int[m_arraySize];
	m_valueArray = new SimuValue[m_arraySize];

	m_B	= new SimuValue[m_matrixSize];
	m_X	= new SimuValue[m_matrixSize];
	m_R	= new SimuValue[m_matrixSize];
	m_Z	= new SimuValue[m_matrixSize];
	m_P	= new SimuValue[m_matrixSize];
	m_Q	= new SimuValue[m_matrixSize];
}

void CSimuCGSolver::AssertSymmetricSparseMatrix(SimuValue epsilon)
{
	char location[] = "CSimuCGSolver::AssertSymmetricSparseMatrix(epsilon)";

	bool bPositiveDefiniteMatrix = false;
	for (int row=0; row < m_matrixSize; row++)
	{
		// row is row number 
		if (CSimuUtility::IsInvalidSimuValue(m_valueArray[row]))
		{
			CSimuMsg::ExitWithMessage(location, "Invalid diagonal element.");
			return;
		}
		SimuValue offDiagSum = 0;
		for (int aryId=m_indexArray[row]; aryId<m_indexArray[row+1]; aryId++)
		{
			int col = m_indexArray[aryId];
			if (col >= m_matrixSize)
			{
				CSimuMsg::ExitWithMessage(location, "Column index is out of range.");
				return;
			}
			// current element is at [row, col] and its value=m_valueArray[aryId]
			if (CSimuUtility::IsInvalidSimuValue(m_valueArray[aryId]))
			{
				CSimuMsg::ExitWithMessage(location, "Invalid off-diagonal element.");
				return;
			}
			SimuValue elemValue = m_valueArray[aryId];
			offDiagSum += elemValue;
			// the symmetric element is at [col, row]
			int symEleRow = col; int symEleCol = row;
			// and check all the elements in symRow
			int k;
			for (k=m_indexArray[symEleRow]; k<m_indexArray[symEleRow+1]; k++)
			{
				int curCol = m_indexArray[k];
				// current element is at [symEleRow, curCol] and its value=m_valueArray[k]
				SimuValue symElemValue = m_valueArray[k];
				if (curCol == symEleCol)
				{
					SimuValue delta = elemValue - symElemValue;
					if (fabs(delta) > epsilon)
					{
						// value[row, col] != value[symEleRow, symEleCol]
						CSimuMsg::ExitWithMessage(location, "Symmetric values are not equal.");
					}
					break;
				}
			}
			if (k == m_indexArray[symEleRow+1])
			{
				CSimuMsg::ExitWithMessage(location, "Symmetric element is not found.");
				// redo the search for debug
				int symElemArrayId = GetValueArrayIndexForElementAt(symEleRow, symEleCol);
				SimuValue delta = elemValue - m_valueArray[symElemArrayId];
				if (fabs(delta) > epsilon)
					CSimuMsg::ExitWithMessage(location, "Symmetric values are not equal.");
				return;
			}
		}
		if (fabs(m_valueArray[row] + offDiagSum) > SIMU_VALUE_EPSILON)
			bPositiveDefiniteMatrix = true;
	}
	if (!bPositiveDefiniteMatrix)
		CSimuMsg::ExitWithMessage(location, "Diagonal element is equal to the sum of off-diagonal elements.");
}

int CSimuCGSolver::GetValueArrayIndexForElementAt(int row, int column)
{
	int startIndexForRow = m_indexArray[row];
	int endIndexForRow = m_indexArray[row+1]; // non-inclusive ending
	// search column number
	for (int i=startIndexForRow; i < endIndexForRow; i++)
	{
		if (m_indexArray[i] == column)
			return i;
	}
	return -1;
}

SimuValue CSimuCGSolver::HugeVectorNormal(SimuValue* V1)
{
	SimuValue normal = 0;
	for(int i=0; i<m_matrixSize; i++)
		normal += V1[i]*V1[i];

	return sqrt(normal);
}

SimuValue CSimuCGSolver::HugeVectorDotProduct(SimuValue* V1, SimuValue* V2)
{
	SimuValue result = 0;
	for(int i=0; i<m_matrixSize; i++)
	{
		result += V1[i]*V2[i];
	}

	return result;
}

SimuValue CSimuCGSolver::HugeVectorLargestComponentMagnitude(SimuValue* V1)
{
	SimuValue tmpFabs;
	SimuValue result = 0;
	for(int i=0; i<m_matrixSize; i++)
	{
		tmpFabs = fabs(V1[i]);
		if (result < tmpFabs)
			result = tmpFabs;
	}
	return result;
}

void CSimuCGSolver::HugeVectorAssignment(SimuValue* V1, SimuValue* V2)
{
	for (int i=0; i<m_matrixSize; i++)
		V1[i] = V2[i];
}

void CSimuCGSolver::HugeVectorAddition(SimuValue* V1, SimuValue* V2, SimuValue scalar)
{
	for (int i=0; i<m_matrixSize; i++)
	{
		V1[i] += V2[i]*scalar;
	}
}

void CSimuCGSolver::HugeVectorAddition(SimuValue* V1, SimuValue scalar, SimuValue* V2)
{
	for (int i=0; i<m_matrixSize; i++)
	{
		V1[i] = V1[i]*scalar + V2[i];
	}
}

void CSimuCGSolver::MatrixMultiply(SimuValue* multiplier, SimuValue* result)
{
	for (int i=0; i < m_matrixSize; i++)
	{
		result[i] = m_valueArray[i]*multiplier[i];
		for (int j = m_indexArray[i]; j < m_indexArray[i+1]; j++)
		{
			if (m_indexArray[j] >= m_matrixSize)
			{
				CSimuMsg::ExitWithMessage("CSimuCGSolver::MatrixMultiply(multiplier, result)",
										"Wrong index");
			}
			result[i] += m_valueArray[j]*multiplier[m_indexArray[j]];
		}
	}
}

void CSimuCGSolver::TransposeMatrixMultiply(SimuValue* multiplier, SimuValue* result)
{
	for (int i=0; i<m_matrixSize; i++)
	{
		result[i] = m_valueArray[i]*multiplier[i];
		for (int j=m_indexArray[i]; j<m_indexArray[i+1]; j++)
			result[m_indexArray[j]] += m_valueArray[j]*multiplier[i];
	}
}

void CSimuCGSolver::SolveDiagonalMatrix(SimuValue* variable, SimuValue* constant)
{
	for (int i=0; i<m_matrixSize; i++)
	{
		variable[i] = constant[i]/m_valueArray[i];
//		CSimuUtility::AssertSimuValue(variable[i]);
	}
}

void CSimuCGSolver::ImproveMatrixSolution(SimuValue* adjuster, SimuValue scale)
{
	for (int i=0; i<m_matrixSize; i++)
	{
		m_X[i] += adjuster[i]*scale;
	}
}
