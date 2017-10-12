// SimuVector3D.h: interface for the CSimuVector3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUVECTOR3D_H__FC0D521D_CE10_47F8_B2EE_08603A6CE6DF__INCLUDED_)
#define AFX_SIMUVECTOR3D_H__FC0D521D_CE10_47F8_B2EE_08603A6CE6DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimuVector3D
{
public:
	SimuValue	v[DIMENSION_3];

	CSimuVector3D();
	virtual ~CSimuVector3D();

	SimuValue& operator[] (unsigned int d) { return v[d]; };

	void ResetMemberVariables();

	void SetValue(SimuValue scalar);
	void SetValue(CSimuVector3D* vector);
	void SetValue(CSimuVector3D* vector, SimuValue scalar);
	void SetValueAsAddition(CSimuVector3D* first, CSimuVector3D* second);
	void SetValueAsSubstraction(CSimuVector3D* first, CSimuVector3D* second);
	void SetValueAsCrossProduct(CSimuVector3D* first, CSimuVector3D* second);
	void SetValueAsAverage(CSimuVector3D* v0, CSimuVector3D* v1, CSimuVector3D* v2);
	void SetElements(SimuValue elem1=0, SimuValue elem2=0, SimuValue elem3=0);
	bool ParseStringForValue(CString str);
	void MakeStringFromValue(CString &str);
	bool IsZeroVector();
	bool IsPositiveVector();
	bool IsNonNegativeVector();
	bool IsUnitVector(SimuValue unitFactor = 1.0);
	bool IsSameLocationAs(CSimuVector3D* pPos, SimuValue minDistance=SIMU_VALUE_EPSILON);
	SimuValue Length();
	SimuValue LengthSquare();
	SimuValue SelfDotProduct();

	bool Normalize(SimuValue length = 1.0);
	void NormalizeOntoOval(SimuValue radius[Z+1]);
	void Reverse(SimuValue reverseFactor = 1.0);
	void AddedBy(SimuValue v0, SimuValue v1, SimuValue v2);
	void AddedBy(CSimuVector3D* adder);
	void AddedBy(CSimuVector3D* adder, SimuValue scalar);
	void AddedBy(SimuValue scalar, CSimuVector3D* adder);
	void SubstractedBy(CSimuVector3D* substracter);
	void ScaledBy(SimuValue scale);
	void DirectProductBy(CSimuVector3D* multiplier);
	void DividedBy(SimuValue divider);
	void DividedBy(CSimuVector3D* divider);

	SimuValue GetDistanceSquareToLocation(SimuValue x, SimuValue y, SimuValue z);
	SimuValue GetDistanceSquareToVector(CSimuVector3D* pVector);
	SimuValue GetDistanceToLocation(SimuValue x, SimuValue y, SimuValue z);
	SimuValue GetDistanceToVector(CSimuVector3D* pVector);
	SimuValue GetDistanceToSegment(CSimuVector3D* p1, CSimuVector3D* p2);
	SimuValue GetDistanceToPlane(CSimuVector3D* aPointOnPlane, CSimuVector3D* normal);
	SimuValue DotProduct(CSimuVector3D* multiplier);
	void DotProduct(CSimuVector3D* multiplier, SimuValue* result);
	int GetProjectionPointOnPlane(CSimuVector3D* pPosOnPlane, CSimuVector3D* pNormal, CSimuVector3D* projPos);
	bool FindProjectionPointOnSegment(CSimuVector3D* p1, CSimuVector3D* p2, CSimuVector3D* projP);

	static CSimuVector3D* m_staticV3D1;
	static CSimuVector3D* m_staticV3D2;
	static CSimuVector3D* m_staticV3D3;
	static CSimuVector3D* m_staticV3D4;

	static SimuValue ComputeDistanceSquare(CSimuVector3D* v1, CSimuVector3D* v2);
	static SimuValue FollowRightHandRule(CSimuVector3D* v1, CSimuVector3D* v2, CSimuVector3D* v3, CSimuVector3D* v4);
	static void ComputeNormalForSegment(CSimuVector3D* p1, CSimuVector3D* p2, CSimuVector3D* rp, CSimuVector3D* n);
	static SimuValue ProjectOntoLineBy(CSimuVector3D* aPoint, CSimuVector3D* lp1, CSimuVector3D* lp2);
	static SimuValue ComputeDistanceSquareToLine(CSimuVector3D* pos, CSimuVector3D* posOnLine, CSimuVector3D* vectorOnLine);
	static SimuValue ComputeTriangleArea(CSimuVector3D* p1, CSimuVector3D* p2, CSimuVector3D* p3);
	static SimuValue DotProduct(CSimuVector3D* p1, CSimuVector3D* p2);
	static SimuValue DotProduct(CSimuVector3D* pCentralPoint, CSimuVector3D* pPoint1, CSimuVector3D* pPoint2);
	static void ComputeNormalFromThreePoints(CSimuVector3D* p1, CSimuVector3D* p2, CSimuVector3D* p3, CSimuVector3D* normal);

	static SimuValue HugeVectorLargestComponentMagnitude(CSimuVector3D** V1, int num);
	static SimuValue HugeVectorMagnitude(CSimuVector3D** V1, int num);
	static SimuValue HugeVectorDotProduct(CSimuVector3D** V1, CSimuVector3D** V2, int num);
	static void HugeVectorAddition(CSimuVector3D** V1, CSimuVector3D** V2, SimuValue scalar, int num);
	static void HugeVectorAddition(SimuValue scalar, CSimuVector3D** V1, CSimuVector3D** V2, int num);
};

#endif // !defined(AFX_SIMUVECTOR3D_H__FC0D521D_CE10_47F8_B2EE_08603A6CE6DF__INCLUDED_)
