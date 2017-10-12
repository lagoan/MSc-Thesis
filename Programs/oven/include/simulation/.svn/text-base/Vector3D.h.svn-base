// Vector3D.h: interface for the CVector3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR3D_H__9CF1250A_9D8C_402B_A6B3_236A28CC427E__INCLUDED_)
#define AFX_VECTOR3D_H__9CF1250A_9D8C_402B_A6B3_236A28CC427E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVector3D
{
public:
	SimuValue	v[DIMENSION_3];

	CVector3D();
	CVector3D(SimuValue elem1, SimuValue elem2, SimuValue elem3);
	virtual ~CVector3D();

	SimuValue& operator[] (unsigned int d) { return v[d]; };

	void ResetMemberVariables();

	void SetValue(SimuValue scalar);
	void SetValue(CVector3D* vector);
	void SetValue(CVector3D* vector, SimuValue scalar);
	void SetValueAsAddition(CVector3D* first, CVector3D* second);
	void SetValueAsSubstraction(CVector3D* first, CVector3D* second);
	void SetValueAsCrossProduct(CVector3D* first, CVector3D* second);
	void SetValueAsAverage(CVector3D* v0, CVector3D* v1, CVector3D* v2);
	void SetElements(SimuValue elem1=0, SimuValue elem2=0, SimuValue elem3=0);
	bool ParseStringForValue(QString str);
	void MakeStringFromValue(QString &str);
	bool IsZeroVector();
	bool IsPositiveVector();
	bool IsNonNegativeVector();
	bool IsUnitVector(SimuValue unitFactor = 1.0);
	bool IsSameVectorAs(CVector3D* pPos, SimuValue delta=SIMU_VALUE_EPSILON);
	SimuValue Length();
	SimuValue LengthSquare();
	SimuValue SelfDotProduct();

	bool Normalize(SimuValue length = 1.0);
	void NormalizeOntoOval(SimuValue radius[Z+1]);
	void NormalizeOntoOval(SimuValue radiusOnX, SimuValue radiusOnY, SimuValue radiusOnZ);
	void Reverse(SimuValue reverseFactor = 1.0);
	void AddedBy(SimuValue v0, SimuValue v1, SimuValue v2);
	void AddedBy(CVector3D* adder);
	void AddedBy(CVector3D* adder, SimuValue scalar);
	void AddedBy(SimuValue scalar, CVector3D* adder);
	void SubstractedBy(CVector3D* substracter);
	void ScaledBy(SimuValue scale);
	void DirectProductBy(CVector3D* multiplier);
	void DividedBy(SimuValue divider);
	void DividedBy(CVector3D* divider);

	SimuValue GetDistanceSquareToLocation(SimuValue x, SimuValue y, SimuValue z);
	SimuValue GetDistanceSquareToVector(CVector3D* pVector);
	SimuValue GetDistanceToLocation(SimuValue x, SimuValue y, SimuValue z);
	SimuValue GetDistanceToVector(CVector3D* pVector);
	SimuValue GetDistanceToSegment(CVector3D* p1, CVector3D* p2);
	SimuValue GetDistanceToPlane(CVector3D* aPointOnPlane, CVector3D* normal);
	SimuValue DotProduct(CVector3D* multiplier);
	void DotProduct(CVector3D* multiplier, SimuValue* result);
	int GetProjectionPointOnPlane(CVector3D* pPosOnPlane, CVector3D* pNormal, CVector3D* projPos);
	bool FindProjectionPointOnSegment(CVector3D* p1, CVector3D* p2, CVector3D* projP);

	static CVector3D* m_staticV3D1;
	static CVector3D* m_staticV3D2;
	static CVector3D* m_staticV3D3;
	static CVector3D* m_staticV3D4;

	static SimuValue ComputeDistanceSquare(CVector3D* v1, CVector3D* v2);
	static SimuValue FollowRightHandRule(CVector3D* v1, CVector3D* v2, CVector3D* v3, CVector3D* v4);
	static void ComputeNormalForSegment(CVector3D* p1, CVector3D* p2, CVector3D* rp, CVector3D* n);
	static SimuValue ProjectOntoLineBy(CVector3D* aPoint, CVector3D* lp1, CVector3D* lp2);
	static SimuValue ComputeDistanceSquareToLine(CVector3D* pos, CVector3D* posOnLine, CVector3D* vectorOnLine);
	static SimuValue ComputeTriangleArea(CVector3D* p1, CVector3D* p2, CVector3D* p3);
	static SimuValue DotProduct(CVector3D* p1, CVector3D* p2);
	static SimuValue DotProduct(CVector3D* pCentralPoint, CVector3D* pPoint1, CVector3D* pPoint2);
	static void ComputeNormalFromThreePoints(CVector3D* p1, CVector3D* p2, CVector3D* p3, CVector3D* normal);

	static SimuValue HugeVectorLargestComponentMagnitude(CVector3D** V1, int num);
	static SimuValue HugeVectorMagnitude(CVector3D** V1, int num);
	static SimuValue HugeVectorDotProduct(CVector3D** V1, CVector3D** V2, int num);
	static void HugeVectorAddition(CVector3D** V1, CVector3D** V2, SimuValue scalar, int num);
	static void HugeVectorAddition(SimuValue scalar, CVector3D** V1, CVector3D** V2, int num);
};


/////////////////////////////////////////////////////////////////////////////////////
// current vector is added by vector adder
/////////////////////////////////////////////////////////////////////////////////////

inline void CVector3D::AddedBy(SimuValue v0, SimuValue v1, SimuValue v2)
{
	v[0] += v0;
	v[1] += v1;
	v[2] += v2;
}

inline void CVector3D::AddedBy(CVector3D* adder)
{
	v[0] += adder->v[0];
	v[1] += adder->v[1];
	v[2] += adder->v[2];
}

inline void CVector3D::AddedBy(CVector3D* adder, SimuValue scalar)
{
	v[0] += adder->v[0] * scalar;
	v[1] += adder->v[1] * scalar;
	v[2] += adder->v[2] * scalar;
}

inline void CVector3D::AddedBy(SimuValue scalar, CVector3D* adder)
{
	v[0] = v[0]*scalar + adder->v[0];
	v[1] = v[1]*scalar + adder->v[1];
	v[2] = v[2]*scalar + adder->v[2];
}

#endif // !defined(AFX_VECTOR3D_H__9CF1250A_9D8C_402B_A6B3_236A28CC427E__INCLUDED_)
