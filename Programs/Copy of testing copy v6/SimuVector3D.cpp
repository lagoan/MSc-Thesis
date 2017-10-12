// SimuVector3D.cpp: implementation of the CSimuVector3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuVector3D.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		ASSERT_SIMU_VECTOR_3D

CSimuVector3D* CSimuVector3D::m_staticV3D1 = NULL;
CSimuVector3D* CSimuVector3D::m_staticV3D2 = NULL;
CSimuVector3D* CSimuVector3D::m_staticV3D3 = NULL;
CSimuVector3D* CSimuVector3D::m_staticV3D4 = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuVector3D::CSimuVector3D()
{
	ResetMemberVariables();
}

CSimuVector3D::~CSimuVector3D()
{

}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void CSimuVector3D::ResetMemberVariables()
{
	v[X] = 0;	v[Y] = 0;	v[Z] = 0;
}


void CSimuVector3D::SetValue(SimuValue scalar)
{
	v[X] = scalar;
	v[Y] = scalar;
	v[Z] = scalar;
}

void CSimuVector3D::SetValue(CSimuVector3D* vector)
{
	v[X] = vector->v[X];
	v[Y] = vector->v[Y];
	v[Z] = vector->v[Z];
}

void CSimuVector3D::SetValue(CSimuVector3D* vector, SimuValue scalar)
{
	v[X] = vector->v[X]*scalar;
	v[Y] = vector->v[Y]*scalar;
	v[Z] = vector->v[Z]*scalar;
}

void CSimuVector3D::SetValueAsAddition(CSimuVector3D* first, CSimuVector3D* second)
{
	v[0] = first->v[0] + second->v[0];
	v[1] = first->v[1] + second->v[1];
	v[2] = first->v[2] + second->v[2];
}

/*
this = first - second
*/
void CSimuVector3D::SetValueAsSubstraction(CSimuVector3D* first, CSimuVector3D* second)
{
	v[0] = first->v[0] - second->v[0];
	v[1] = first->v[1] - second->v[1];
	v[2] = first->v[2] - second->v[2];
}

// compute this = first x second
void CSimuVector3D::SetValueAsCrossProduct(CSimuVector3D* first, CSimuVector3D* second)
{
	v[0] = first->v[1]*second->v[2] - first->v[2]*second->v[1];
	v[1] = first->v[2]*second->v[0] - first->v[0]*second->v[2];
	v[2] = first->v[0]*second->v[1] - first->v[1]*second->v[0];
}

void CSimuVector3D::SetValueAsAverage(CSimuVector3D* v0, CSimuVector3D* v1, CSimuVector3D* v2)
{
	SetValue(v0);
	AddedBy(v1);
	AddedBy(v2);
	ScaledBy(1.0/3.0);
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void CSimuVector3D::SetElements(SimuValue elem1, SimuValue elem2, SimuValue elem3)
{
	v[0] = elem1;
	v[1] = elem2;
	v[2] = elem3;
}

/////////////////////////////////////////////////////////////////////////////////////
// str contains ',' or ' ' seperated double numbers. They may be enclosed by '(' and ')'.
// Continuous ',' and/or ' ' are treated as one seperator
// The result values are stored in array
/////////////////////////////////////////////////////////////////////////////////////
bool CSimuVector3D::ParseStringForValue(CString str)
{
	char location[] = "CSimuVector3D::ParseStringForValue(str)";

	if (str.GetLength() > LEN_DATA_LINE)
	{
		CSimuMsg::ExitWithMessage(location, "Value string is too long.");
		return false;
	}
	// if data_pointer is enclosed by ( and ), take them off
	str.TrimLeft("(");
	str.TrimRight(")");
	if (str.GetLength() <= 0)
	{
		CSimuMsg::ExitWithMessage(location, "Value string is empty.");
		return false;
	}

	char* buf = new char[LEN_DATA_LINE];
	char* data_pointer;
	char* scalar_data = new char[LEN_DATA_LINE];
	int len = 0;
	int head = 0;

	strcpy(buf, str);
	data_pointer = buf;

	for(int i=0; i<DIMENSION_3; i++)
	{
		if (head >= (int)strlen(data_pointer))
		{
			CSimuMsg::ExitWithMessage(location, "Char array ended unexpectedly.");
			return false;
		}
		len = strcspn(&data_pointer[head], ", ");
		if (len >= LEN_DATA_LINE-head)
		{
			CSimuMsg::ExitWithMessage(location, "Scalar data is too long.");
			return false;
		}
		else if (len > 0)
		{
			strncpy(scalar_data, &data_pointer[head], len);
			scalar_data[len] = '\0';	// add terminating null char
			v[i] = 0;
			v[i] = atof(scalar_data);
		}
		else
			i--;	// hasn't found number for element i, so descrease it for next loop
		head += len + 1;
	}
	if (i < DIMENSION_3)
	{
		CSimuMsg::ExitWithMessage(location, "At lease one scalar data is missing.");
		return false;
	}

	delete buf;
	delete scalar_data;

	return true;
}

void CSimuVector3D::MakeStringFromValue(CString &str)
{
	char valueStr[LEN_VECTOR_SCALAR*3];
	sprintf(valueStr, "(%.3f, %.3f, %.3f)", v[0], v[1], v[2]);
	str = valueStr;
}

bool CSimuVector3D::IsZeroVector()
{
	for (int i=0; i < DIMENSION_3; i++)
		if (fabs(v[i]) > SIMU_VALUE_EPSILON)
			return false;
	return true;
}

bool CSimuVector3D::IsPositiveVector()
{
	for (int i=0; i<DIMENSION_3; i++)
		if (v[i] <= SIMU_VALUE_EPSILON)
			return false;
	return true;
}

bool CSimuVector3D::IsNonNegativeVector()
{
	for (int i=0; i<DIMENSION_3; i++)
		if (v[i] < 0)
			return false;
	return true;
}

bool CSimuVector3D::IsUnitVector(SimuValue unitFactor)
{
	for (int i=0; i<DIMENSION_3; i++)
		if (v[i] != unitFactor)
			return false;
	return true;
}

bool CSimuVector3D::IsSameLocationAs(CSimuVector3D* pPos, SimuValue minDistance)
{
	for (int i=0; i<DIMENSION_3; i++)
		if (fabs(v[i] - pPos->v[i]) > minDistance)
			return false;
	return true;
}

SimuValue CSimuVector3D::Length()
{
	SimuValue d = (SimuValue)sqrt(LengthSquare());
	return d;
}

SimuValue CSimuVector3D::LengthSquare()
{
	SimuValue d = 0.0;
	for (int i=0; i<DIMENSION_3; i++)
		d += v[i]*v[i];
	return d;
}

SimuValue CSimuVector3D::SelfDotProduct()
{
	#ifdef	ASSERT_SIMU_VECTOR_3D
	#endif

	SimuValue d = 0.0;
	for (int i=0; i<DIMENSION_3; i++)
		d += v[i]*v[i];
	return d;
}

bool CSimuVector3D::Normalize(SimuValue length)
{
	#ifdef	ASSERT_SIMU_VECTOR_3D
	#endif

	int i;
	SimuValue d = 0.0;
	for (i=0; i<DIMENSION_3; i++)
		d += v[i]*v[i];
	d = (SimuValue)sqrt(d);

	if (fabs(d) <= SIMU_VALUE_EPSILON)
		return false;

	for (i=0; i<DIMENSION_3; i++)
		v[i] = v[i]*length/d;

	return true;
}

void CSimuVector3D::NormalizeOntoOval(SimuValue radius[Z+1])
{
	SimuValue t = 0;
	for (int i=0; i <= Z; i++)
		t += pow(v[i]/radius[i], 2);
	t = sqrt(1/t);
	for (i=0; i <= Z; i++)
		v[i] *= t;
}

void CSimuVector3D::Reverse(SimuValue reverseFactor)
{
	#ifdef	ASSERT_SIMU_VECTOR_3D
	#endif

	for (int i=0; i<DIMENSION_3; i++)
	{
		if (v[i] != 0.0)
			v[i] = reverseFactor/v[i];
		else
		{
			CSimuMsg::ExitWithMessage("CSimuVector3D::Reverse(reverseFactor)",
									"One of elements is ZERO for vector(%d).");
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// current vector is scaled by value in scale
/////////////////////////////////////////////////////////////////////////////////////
void CSimuVector3D::ScaledBy(SimuValue scale)
{
	#ifdef	ASSERT_SIMU_VECTOR_3D
	#endif

	for (int i=0; i<DIMENSION_3; i++)
		v[i] *= scale;
}

/////////////////////////////////////////////////////////////////////////////////////
// current vector is added by vector adder
/////////////////////////////////////////////////////////////////////////////////////

void CSimuVector3D::AddedBy(SimuValue v0, SimuValue v1, SimuValue v2)
{
	v[0] += v0;
	v[1] += v1;
	v[2] += v2;
}

void CSimuVector3D::AddedBy(CSimuVector3D* adder)
{
	v[0] += adder->v[0];
	v[1] += adder->v[1];
	v[2] += adder->v[2];
}

void CSimuVector3D::AddedBy(CSimuVector3D* adder, SimuValue scalar)
{
	v[0] += adder->v[0]*scalar;
	v[1] += adder->v[1]*scalar;
	v[2] += adder->v[2]*scalar;
}

void CSimuVector3D::AddedBy(SimuValue scalar, CSimuVector3D* adder)
{
	v[0] = v[0]*scalar + adder->v[0];
	v[1] = v[1]*scalar + adder->v[1];
	v[2] = v[2]*scalar + adder->v[2];
}

/////////////////////////////////////////////////////////////////////////////////////
// current vector is substracted by vector substracter
/////////////////////////////////////////////////////////////////////////////////////
void CSimuVector3D::SubstractedBy(CSimuVector3D* substracter)
{
	v[0] -= substracter->v[0];
	v[1] -= substracter->v[1];
	v[2] -= substracter->v[2];
}

/////////////////////////////////////////////////////////////////////////////////////
// current vector is multiplied by vector multiplier with dot product
/////////////////////////////////////////////////////////////////////////////////////
void CSimuVector3D::DirectProductBy(CSimuVector3D* multiplier)
{
	#ifdef	ASSERT_SIMU_VECTOR_3D
	#endif

	for (int i=0; i<DIMENSION_3; i++)
		v[i] *= multiplier->v[i];
}

void CSimuVector3D::DividedBy(SimuValue divider)
{
	#ifdef	ASSERT_SIMU_VECTOR_3D
	#endif

	for (int i=0; i<DIMENSION_3; i++)
	{
		if (divider != 0.0)
			v[i] /= divider;
		else if (v[i] != 0.0)
		{
			CSimuMsg::ExitWithMessage("CSimuVector3D::DividedBy(divider)", "Divider is Zero.");
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// current vector is divided by vector divider
/////////////////////////////////////////////////////////////////////////////////////
void CSimuVector3D::DividedBy(CSimuVector3D* divider)
{
	#ifdef	ASSERT_SIMU_VECTOR_3D
	#endif

	for (int i=0; i<DIMENSION_3; i++)
	{
		#ifdef	ASSERT_SIMU_VECTOR_3D
		if (fabs(divider->v[i]) <= SIMU_VALUE_EPSILON
			&& fabs(v[i]) <= SIMU_VALUE_EPSILON)
		{
			CSimuMsg::ExitWithMessage("CSimuVector3D::DividedBy(divider)",
									"Divider is Zero.");
			return;
		}
		#endif

		if (divider->v[i] != 0.0)
			v[i] /= divider->v[i];
	}
}

SimuValue CSimuVector3D::GetDistanceSquareToLocation(SimuValue x, SimuValue y, SimuValue z)
{
	SimuValue tmp;
	SimuValue d = 0.0;
	tmp = v[X] - x; d += tmp*tmp;
	tmp = v[Y] - y; d += tmp*tmp;
	tmp = v[Z] - z; d += tmp*tmp;
	return d;
}

SimuValue CSimuVector3D::GetDistanceSquareToVector(CSimuVector3D* pVector)
{
	SimuValue tmp;
	SimuValue d = 0.0;
	for (int i=0; i<DIMENSION_3; i++)
	{
		tmp = v[i] - pVector->v[i];
		d += tmp*tmp;
	}
	return d;
}

SimuValue CSimuVector3D::GetDistanceToLocation(SimuValue x, SimuValue y, SimuValue z)
{
	SimuValue d = GetDistanceSquareToLocation(x, y, z);
	return (SimuValue)sqrt(d);
}

SimuValue CSimuVector3D::GetDistanceToVector(CSimuVector3D* pVector)
{
	return sqrt(GetDistanceSquareToVector(pVector));
}

/////////////////////////////////////////////////////////////////////////////////////
// If this vector falls within segment (p1, p2), return the distance to the line;
// otherwise, return the minimum distance of those to p1 and p2, in which case,
// the returned distance is negated.
/////////////////////////////////////////////////////////////////////////////////////
SimuValue CSimuVector3D::GetDistanceToSegment(CSimuVector3D* p1, CSimuVector3D* p2)
{
	SimuValue tmpDist;
	if (FindProjectionPointOnSegment(p1, p2, m_staticV3D2))
		// m_staticV3D1 is used in FindProjectionPointOnSegment(...)
	{
		tmpDist = GetDistanceToVector(m_staticV3D2);
	}
	else
	{
		tmpDist = GetDistanceToVector(p1);
		SimuValue tmpDist1 = GetDistanceToVector(p2);
		if (tmpDist > tmpDist1)
			tmpDist = tmpDist1;
		// negate the distance to indicate falling outside the segment
		tmpDist = -tmpDist;
	}
	return tmpDist;
}

/////////////////////////////////////////////////////////////////////////////////////
// If the returned value is 0, this point is on the plane.
// The absolute returned value is the distance to the plane.
// If the returned value is positive, which means this point is
// on the side to which the normal is pointing.
// If the returned value is negative, which means this point is
// on the other side.
/////////////////////////////////////////////////////////////////////////////////////
SimuValue CSimuVector3D::GetDistanceToPlane(CSimuVector3D* aPointOnPlane, CSimuVector3D* normal)
{
	// d = normal * (this - aPointOnPlane)
	SimuValue d = 0;
	for (int i=0; i < DIMENSION_3; i++)
	{
		SimuValue tmp = v[i] - aPointOnPlane->v[i];
		tmp *= normal->v[i];
		d += tmp;
//		d += normal->v[i]*(v[i] - aPointOnPlane->v[i]);
	}
	return d;
}

SimuValue CSimuVector3D::DotProduct(CSimuVector3D* multiplier)
{
	SimuValue result = 0.0;
	for (int i=0; i<DIMENSION_3; i++)
		result += v[i] * multiplier->v[i];

	return result;
}

void CSimuVector3D::DotProduct(CSimuVector3D* multiplier, SimuValue* result)
{
	#ifdef	ASSERT_SIMU_VECTOR_3D
	#endif

	*result = 0.0;
	for (int i=0; i<DIMENSION_3; i++)
		*result += v[i] * multiplier->v[i];
}

/////////////////////////////////////////////////////////////////////////////////////
// If this point is on the plane, return 0;
// If this point is on the side of the plane pointed by the normal, return 1;
// Otherwise, return -1.
/////////////////////////////////////////////////////////////////////////////////////
int CSimuVector3D::GetProjectionPointOnPlane(CSimuVector3D* pPosOnPlane,
											 CSimuVector3D* pNormal,
											 CSimuVector3D* projPos)
{
	SimuValue tmpDist = GetDistanceToPlane(pPosOnPlane, pNormal);
	projPos->SetValue(this);
	projPos->AddedBy(pNormal, -tmpDist);
	if (fabs(tmpDist) <= SIMU_VALUE_EPSILON)
		return 0;	// on the plane
	else if (tmpDist > 0)
		return 1;
	else
		return -1;
}

/////////////////////////////////////////////////////////////////////////////////////
// If this vector falls between segment (p1, p2), return true
// and the project point is in projP; otherwise, return false.
/////////////////////////////////////////////////////////////////////////////////////
bool CSimuVector3D::FindProjectionPointOnSegment(CSimuVector3D* p1,
												 CSimuVector3D* p2,
												 CSimuVector3D* projP)
{
	projP->SetValueAsSubstraction(p1, p2);
	m_staticV3D1->SetValueAsSubstraction(this, p2);
	SimuValue tmpLenSquare = projP->LengthSquare();
	SimuValue t = CSimuVector3D::DotProduct(projP, m_staticV3D1)/tmpLenSquare;

	if (t <= SIMU_VALUE_EPSILON
	 || t >= 1+SIMU_VALUE_EPSILON)
		return false;
	projP->AddedBy(t, p2);
	return true;
}

SimuValue CSimuVector3D::ComputeDistanceSquare(CSimuVector3D* v1, CSimuVector3D* v2)
{
	SimuValue tmp;
	SimuValue d = 0.0;
	for (int i=0; i<DIMENSION_3; i++)
	{
		tmp = v1->v[i] - v2->v[i];
		d += tmp*tmp;
	}
	return d;
}

/////////////////////////////////////////////////////////////////////////////////////
// if tmp > 0, the four positions follow the right hand rule;
// if tmp < 0, then not; otherwise, the four poistions are coplanar.
/////////////////////////////////////////////////////////////////////////////////////
SimuValue CSimuVector3D::FollowRightHandRule(CSimuVector3D* v1, CSimuVector3D* v2, CSimuVector3D* v3, CSimuVector3D* v4)
{
	m_staticV3D1->SetValueAsSubstraction(v2, v1);
	m_staticV3D2->SetValueAsSubstraction(v3, v2);
	m_staticV3D3->SetValueAsSubstraction(v4, v3);

	m_staticV3D4->SetValueAsCrossProduct(m_staticV3D1, m_staticV3D2);
	SimuValue tmp = m_staticV3D4->DotProduct(m_staticV3D3);

	return tmp;
}

// normal is pointing outward
void CSimuVector3D::ComputeNormalForSegment(CSimuVector3D* p1, CSimuVector3D* p2,
											CSimuVector3D* in, CSimuVector3D* n)
{
	n->v[X] = p1->v[Y] - p2->v[Y];
	n->v[Y] = - (p1->v[X] - p2->v[X]);
	SimuValue d = (n->v[X]*in->v[X] + n->v[Y]*in->v[Y])
				- (n->v[X]*p1->v[X] + n->v[Y]*p1->v[Y]);
	if (fabs(d) <= SIMU_VALUE_EPSILON)
	{
		CSimuMsg::ExitWithMessage("CSimuVector3D::ComputeNormalForSegment(...)",
								"Reference point is on the line of p1 and p2.");
		return;
	}
	// following condition makes the normal pointing outward
	if ( d > 0)
	{
		n->v[X] *= -1;
		n->v[Y] *= -1;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// aPoint's projection point falls on line specified by 2 points, lp1 and lp2.
// The projection point = lp2 + t*(lp1 - lp2)
// This function returns value t. If t is [0, 1], it means
// the projection point falls on segment (lp1, lp2); Otherwise not.
/////////////////////////////////////////////////////////////////////////////////////
SimuValue CSimuVector3D::ProjectOntoLineBy(CSimuVector3D* aPoint, CSimuVector3D* lp1, CSimuVector3D* lp2)
{
	m_staticV3D1->SetValueAsSubstraction(lp1, lp2);
	m_staticV3D2->SetValueAsSubstraction(aPoint, lp2);
	SimuValue tmpLenSquare = m_staticV3D1->LengthSquare();
	SimuValue tmp = DotProduct(m_staticV3D1, m_staticV3D2)/tmpLenSquare;
	return tmp;
}

SimuValue CSimuVector3D::ComputeDistanceSquareToLine(CSimuVector3D* pos,
													 CSimuVector3D* posOnLine,
													 CSimuVector3D* vectorOnLine)
{
	m_staticV3D1->SetValueAsSubstraction(pos, posOnLine);
	m_staticV3D2->SetValueAsCrossProduct(m_staticV3D1, vectorOnLine);
	SimuValue areaSquare = m_staticV3D2->LengthSquare();
	SimuValue distSquare = areaSquare/vectorOnLine->LengthSquare();
	return distSquare;
}

SimuValue CSimuVector3D::ComputeTriangleArea(CSimuVector3D* p1, CSimuVector3D* p2, CSimuVector3D* p3)
{
	SimuValue s1 = p1->GetDistanceToVector(p2);
	SimuValue s2 = p1->GetDistanceToVector(p3);
	SimuValue s3 = p2->GetDistanceToVector(p3);
	SimuValue s = (s1+s2+s3)/2;
	return sqrt(s*(s-s1)*(s-s2)*(s-s3));
}

SimuValue CSimuVector3D::DotProduct(CSimuVector3D* p1, CSimuVector3D* p2)
{
	return p1->DotProduct(p2);
}

SimuValue CSimuVector3D::DotProduct(CSimuVector3D* pCentralPoint, CSimuVector3D* pPoint1, CSimuVector3D* pPoint2)
{
	m_staticV3D1->SetValueAsSubstraction(pPoint1, pCentralPoint);
	m_staticV3D2->SetValueAsSubstraction(pPoint2, pCentralPoint);
	SimuValue tmp = m_staticV3D1->DotProduct(m_staticV3D2);
	return tmp;
}

/////////////////////////////////////////////////////////////////////////////////////
// the result normal is in <code>normal</code> and
// the three points and the normal forms right-hand order system
/////////////////////////////////////////////////////////////////////////////////////
void CSimuVector3D::ComputeNormalFromThreePoints(CSimuVector3D* p1,
												 CSimuVector3D* p2,
												 CSimuVector3D* p3,
												 CSimuVector3D* normal)
{
	m_staticV3D1->SetValueAsSubstraction(p1, p3);
	m_staticV3D2->SetValueAsSubstraction(p2, p3);
	normal->SetValueAsCrossProduct(m_staticV3D1, m_staticV3D2);
	normal->Normalize();
}

SimuValue CSimuVector3D::HugeVectorLargestComponentMagnitude(CSimuVector3D** V1, int num)
{
	SimuValue result = 0;
	for(int i=0; i<num; i++)
		if (result < V1[i]->Length())
			result = V1[i]->Length();

	return result;
}

SimuValue CSimuVector3D::HugeVectorMagnitude(CSimuVector3D** V1, int num)
{
	SimuValue result = 0;
	for(int i=0; i<num; i++)
		for(int j=0; j < DIMENSION_3; j++)
			result += V1[i]->v[j]*V1[i]->v[j];

	return sqrt(result);
}

SimuValue CSimuVector3D::HugeVectorDotProduct(CSimuVector3D** V1, CSimuVector3D** V2, int num)
{
	SimuValue result = 0;
	for(int i=0; i<num; i++)
		result += V1[i]->DotProduct(V2[i]);

	return result;
}

void CSimuVector3D::HugeVectorAddition(CSimuVector3D** V1, CSimuVector3D** V2, SimuValue scalar, int num)
{
	for (int i=0; i<num; i++)
		V1[i]->AddedBy(V2[i], scalar);
}

void CSimuVector3D::HugeVectorAddition(SimuValue scalar, CSimuVector3D** V1, CSimuVector3D** V2, int num)
{
	for (int i=0; i<num; i++)
		V1[i]->AddedBy(scalar, V2[i]);
}
