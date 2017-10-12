// SimuCGSolver3D.cpp: implementation of the CSimuCGSolver3D class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuCGSolver3D.h"



#define		SIMU_PARA_ASSERT_CSIMU_CG_SOLVER_3D

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuCGSolver3D::CSimuCGSolver3D()
{
	m_scgs3dBlockSymmetric = false;
}

CSimuCGSolver3D::~CSimuCGSolver3D()
{

}

void CSimuCGSolver3D::AllocateSparseMatrixStorage()
{
	m_indexArray = new int[m_arraySize];
	m_valueArray = new SimuValue[m_arraySize*9];

	int matrixSize3D = m_matrixSize*3;
	m_B	= new SimuValue[matrixSize3D];
	m_X	= new SimuValue[matrixSize3D];
	m_R	= new SimuValue[matrixSize3D];
	m_Z	= new SimuValue[matrixSize3D];
	m_P	= new SimuValue[matrixSize3D];
	m_Q	= new SimuValue[matrixSize3D];
}

void CSimuCGSolver3D::AssertSymmetricSparseMatrix(SimuValue epsilon)
{
	char location[] = "CSimuCGSolver3D::AssertSymmetricSparseMatrix(epsilon)";

	for (int row=0; row < m_matrixSize; row++)
	{
		// row is row number 
		AssertValueArrayElement(row);
		for (int aryId = m_indexArray[row]; aryId < m_indexArray[row+1]; aryId++)
		{
			int col = m_indexArray[aryId];
			if (col >= m_matrixSize)
			{
				CSimuMsg::ExitWithMessage(location, "Column index is out of range.");
				return;
			}
			// current element is at [row, col] and its value=m_valueArray[aryId]
			AssertValueArrayElement(aryId);

			// the symmetric element is at [col, row]
			int symmEleRow = col; int symmEleCol = row;
			// and check all the elements in symRow
			int k;
			for (k = m_indexArray[symmEleRow]; k < m_indexArray[symmEleRow+1]; k++)
			{
				int curCol = m_indexArray[k];
				// current element is at [symmEleRow, curCol] and its value=m_valueArray[k]
				if (curCol == symmEleCol)
				{
					Assert2EqualValueArrayElements(aryId, k, epsilon);
					break;
				}
			}
			if (k == m_indexArray[symmEleRow+1])
			{
				CSimuMsg::ExitWithMessage(location, "Symmetric element is not found.");
				// redo the search for debug
				int symmElemArrayId = GetValueArrayIndexForElementAt(symmEleRow, symmEleCol);
				Assert2EqualValueArrayElements(aryId, symmElemArrayId, epsilon);
				return;
			}
		}
	}
}

void CSimuCGSolver3D::AssertValueArrayElement(int valueArrayId)
{
	for (int i=0; i < 9; i++)
		if (CSimuUtility::IsInvalidSimuValue(m_valueArray[valueArrayId*9+i]))
		{
			CSimuMsg::ExitWithMessage("CSimuCGSolver3D::AssertValueArrayElement(valueArrayId)",
										"Invalid value array element.");
			return;
		}
}

void CSimuCGSolver3D::Assert2EqualValueArrayElements(int valueArrayId1, int valueArrayId2,
													 SimuValue epsilon)
{
	for (int i=0; i < 9; i++)
	{
		SimuValue delta = m_valueArray[valueArrayId1*9+i] - m_valueArray[valueArrayId2*9+i];
		if (fabs(delta) > epsilon)
		{
			CSimuMsg::ExitWithMessage("CSimuCGSolver3D::Assert2EqualValueArrayElements(...)",
										"2 value array elements are not equal.");
			return;
		}
	}
}

void CSimuCGSolver3D::SetConstantVectorElement(int id, SimuValue value)
{
	m_B[id*3] = value;
	m_B[id*3+1] = value;
	m_B[id*3+2] = value;
}

void CSimuCGSolver3D::SetConstantVectorElement(int id, CVector3D* vect, SimuValue scale)
{
	m_B[id*3] = vect->v[0]*scale;
	m_B[id*3+1] = vect->v[1]*scale;
	m_B[id*3+2] = vect->v[2]*scale;
}

void CSimuCGSolver3D::AddConstantVectorElement(int id, CVector3D* vect, SimuValue scale)
{
	m_B[id*3] += vect->v[0]*scale;
	m_B[id*3+1] += vect->v[1]*scale;
	m_B[id*3+2] += vect->v[2]*scale;
}

void CSimuCGSolver3D::SetVariableVectorElement(int id, SimuValue value)
{
	m_X[id*3] = value;
	m_X[id*3+1] = value;
	m_X[id*3+2] = value;
}

void CSimuCGSolver3D::GetVariableVectorElement(int id, CVector3D* vect)
{
	vect->v[0] = m_X[id*3];
	vect->v[1] = m_X[id*3+1];
	vect->v[2] = m_X[id*3+2];
}

void CSimuCGSolver3D::SetValueArrayElementAsUnitMatrix(int id)
{
	m_valueArray[id*9] = 1;
	m_valueArray[id*9+1] = 0;
	m_valueArray[id*9+2] = 0;
	m_valueArray[id*9+3] = 0;
	m_valueArray[id*9+4] = 1;
	m_valueArray[id*9+5] = 0;
	m_valueArray[id*9+6] = 0;
	m_valueArray[id*9+7] = 0;
	m_valueArray[id*9+8] = 1;
}

void CSimuCGSolver3D::SetValueArrayElement(int id, SimuValue* value, SimuValue scale)
{
	m_valueArray[id*9] = value[0]*scale;
	m_valueArray[id*9+1] = value[1]*scale;
	m_valueArray[id*9+2] = value[2]*scale;
	m_valueArray[id*9+3] = value[3]*scale;
	m_valueArray[id*9+4] = value[4]*scale;
	m_valueArray[id*9+5] = value[5]*scale;
	m_valueArray[id*9+6] = value[6]*scale;
	m_valueArray[id*9+7] = value[7]*scale;
	m_valueArray[id*9+8] = value[8]*scale;
}

void CSimuCGSolver3D::AddValueArrayElement(int id, SimuValue* value, SimuValue scale)
{
	m_valueArray[id*9] += value[0]*scale;
	m_valueArray[id*9+1] += value[1]*scale;
	m_valueArray[id*9+2] += value[2]*scale;
	m_valueArray[id*9+3] += value[3]*scale;
	m_valueArray[id*9+4] += value[4]*scale;
	m_valueArray[id*9+5] += value[5]*scale;
	m_valueArray[id*9+6] += value[6]*scale;
	m_valueArray[id*9+7] += value[7]*scale;
	m_valueArray[id*9+8] += value[8]*scale;
}

SimuValue CSimuCGSolver3D::HugeVectorNormal(SimuValue* V1)
{
	SimuValue normal = 0;
	for(int i=0; i<m_matrixSize*3; i++)
		normal += V1[i]*V1[i];

	return sqrt(normal);
}

SimuValue CSimuCGSolver3D::HugeVectorDotProduct(SimuValue* V1, SimuValue* V2)
{
	SimuValue result = 0;
	for(int i=0; i<m_matrixSize*3; i++)
		result += V1[i]*V2[i];

	return result;
}

SimuValue CSimuCGSolver3D::HugeVectorLargestComponentMagnitude(SimuValue* V1)
{
	SimuValue tmpFabs;
	SimuValue result = 0;
	for(int i=0; i<m_matrixSize*3; i++)
	{
		tmpFabs = fabs(V1[i]);
		if (result < tmpFabs)
			result = tmpFabs;
	}
	return result;
}

void CSimuCGSolver3D::HugeVectorAssignment(SimuValue* V1, SimuValue* V2)
{
	for (int i=0; i<m_matrixSize*3; i++)
		V1[i] = V2[i];
}

void CSimuCGSolver3D::HugeVectorAddition(SimuValue* V1, SimuValue* V2, SimuValue scalar)
{
	for (int i=0; i<m_matrixSize*3; i++)
		V1[i] += V2[i]*scalar;
}

void CSimuCGSolver3D::HugeVectorAddition(SimuValue* V1, SimuValue scalar, SimuValue* V2)
{
	for (int i=0; i<m_matrixSize*3; i++)
		V1[i] = V1[i]*scalar + V2[i];
}

void CSimuCGSolver3D::MatrixMultiply(SimuValue* multiplier, SimuValue* result)
{
	for (int i=0; i<m_matrixSize; i++)
	{
		int veId = i*3;
		int meId = i*9;
		// 3x3 element, starting at meId, multiplies vector element starting at veId
		for (int m=0; m < 3; m++)
		{
			result[veId+m] = 0;
			for (int n=0; n < 3; n++)
				result[veId+m] += m_valueArray[meId+m*3+n]*multiplier[veId+n];
		}
		for (int j=m_indexArray[i]; j<m_indexArray[i+1]; j++)
		{
			if (m_indexArray[j] >= m_matrixSize)
			{
				CSimuMsg::ExitWithMessage("CSimuCGSolver3D::MatrixMultiply(multiplier, result)",
										"Wrong index");
			}
			meId = j*9;
			int veId2 = m_indexArray[j]*3;
			// 3x3 element, starting at meId, multiplies vector element starting at veId2
			for (int m=0; m < 3; m++)
			for (int n=0; n < 3; n++)
				result[veId+m] += m_valueArray[meId+m*3+n]*multiplier[veId2+n];
		}
	}
}

void CSimuCGSolver3D::SolveDiagonalMatrix(SimuValue* variable, SimuValue* constant)
{
	if (m_scgs3dBlockSymmetric)
	{
		SimuValue matrix[9], inverse[9];
		for (int i=0; i<m_matrixSize; i++)
		{
			int veId = i*3; // vector element starting at veId
			int meId = i*9;	// 3x3 element starting at meId
			for (int j=0; j < 9; j++)
				matrix[j] = m_valueArray[meId+j];
			CSimuUtility::Inverse3x3Matrix(matrix, inverse);
			for (int m=0; m < 3; m++)
			{
				variable[veId+m] = 0;
				for (int n=0; n < 3; n++)
				{
					variable[veId+m] += inverse[m*3+n]*constant[veId+n];
				}
			}
		}
	}
	else
	{
		for (int i=0; i<m_matrixSize; i++)
		{
			int veId = i*3; // vector element starting at veId
			int meId = i*9;	// 3x3 element starting at meId
			variable[veId] = constant[veId]/m_valueArray[meId];
			variable[veId+1] = constant[veId+1]/m_valueArray[meId+4];
			variable[veId+2] = constant[veId+2]/m_valueArray[meId+8];
		}
	}
}
