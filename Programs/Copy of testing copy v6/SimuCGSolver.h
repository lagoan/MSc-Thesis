// SimuCGSolver.h: interface for the CSimuCGSolver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUCGSOLVER_H__90F1B037_EC1D_4D24_A761_ADDA523DA652__INCLUDED_)
#define AFX_SIMUCGSOLVER_H__90F1B037_EC1D_4D24_A761_ADDA523DA652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimuCGSolver  
{
public:
	int			m_matrixSize;
	int			m_arraySize;
	int*		m_indexArray;
	SimuValue*	m_valueArray;
	SimuValue*	m_B;	// constant vector
	SimuValue*	m_X;	// solution vector

	CSimuCGSolver();
	virtual ~CSimuCGSolver();

	int SolveSymmetricMatrix(int maxIterations, SimuValue tol);
	virtual void FreeSparseMatrixStorage();
	virtual void AllocateSparseMatrixStorage();
	virtual void AssertSymmetricSparseMatrix(SimuValue epsilon=SIMU_VALUE_EPSILON);
	int GetValueArrayIndexForElementAt(int row, int column);

protected:
	SimuValue*	m_R;
	SimuValue*	m_Z;
	SimuValue*	m_P;
	SimuValue*	m_Q;

	virtual SimuValue HugeVectorNormal(SimuValue* V1);
	virtual SimuValue HugeVectorDotProduct(SimuValue* V1, SimuValue* V2);
	virtual SimuValue HugeVectorLargestComponentMagnitude(SimuValue* V1);
	virtual	void HugeVectorAssignment(SimuValue* V1, SimuValue* V2);
	virtual void HugeVectorAddition(SimuValue* V1, SimuValue* V2, SimuValue scalar);
	virtual void HugeVectorAddition(SimuValue* V1, SimuValue scalar, SimuValue* V2);
	virtual void MatrixMultiply(SimuValue* multiplier, SimuValue* result);
	void TransposeMatrixMultiply(SimuValue* multiplier, SimuValue* result);
	virtual void SolveDiagonalMatrix(SimuValue* variable, SimuValue* constant);
	void ImproveMatrixSolution(SimuValue* adjuster, SimuValue scale);
};

#endif // !defined(AFX_SIMUCGSOLVER_H__90F1B037_EC1D_4D24_A761_ADDA523DA652__INCLUDED_)
