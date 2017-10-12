// SimuCGSolver3D.h: interface for the CSimuCGSolver3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUCGSOLVER3D_H__2472B37F_AB2D_46ED_9458_0D3EA3EE320C__INCLUDED_)
#define AFX_SIMUCGSOLVER3D_H__2472B37F_AB2D_46ED_9458_0D3EA3EE320C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuCGSolver.h"

class CSimuCGSolver3D : public CSimuCGSolver  
{
public:
	bool m_scgs3dBlockSymmetric;

	CSimuCGSolver3D();
	virtual ~CSimuCGSolver3D();

	virtual void AllocateSparseMatrixStorage();
	virtual void AssertSymmetricSparseMatrix(SimuValue epsilon=SIMU_VALUE_EPSILON);
	void AssertValueArrayElement(int valueArrayId);
	void Assert2EqualValueArrayElements(int valueArrayId1, int valueArrayId2,
										SimuValue epsilon);

	void SetConstantVectorElement(int id, SimuValue value);
	void SetConstantVectorElement(int id, CVector3D* vect, SimuValue scale);
	void AddConstantVectorElement(int id, CVector3D* vect, SimuValue scale);
	void SetVariableVectorElement(int id, SimuValue value);
	void GetVariableVectorElement(int id, CVector3D* vect);
	void SetValueArrayElementAsUnitMatrix(int id);
	void SetValueArrayElement(int id, SimuValue* value, SimuValue scale);
	void AddValueArrayElement(int id, SimuValue* value, SimuValue scale);

protected:
	virtual SimuValue HugeVectorNormal(SimuValue* V1);
	virtual SimuValue HugeVectorDotProduct(SimuValue* V1, SimuValue* V2);
	virtual SimuValue HugeVectorLargestComponentMagnitude(SimuValue* V1);
	virtual	void HugeVectorAssignment(SimuValue* V1, SimuValue* V2);
	virtual void HugeVectorAddition(SimuValue* V1, SimuValue* V2, SimuValue scalar);
	virtual void HugeVectorAddition(SimuValue* V1, SimuValue scalar, SimuValue* V2);
	virtual void MatrixMultiply(SimuValue* multiplier, SimuValue* result);
	virtual void SolveDiagonalMatrix(SimuValue* variable, SimuValue* constant);
};

#endif // !defined(AFX_SIMUCGSOLVER3D_H__2472B37F_AB2D_46ED_9458_0D3EA3EE320C__INCLUDED_)
