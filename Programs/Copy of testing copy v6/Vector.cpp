// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuMsg.h"
#include "SimuUtility.h"
#include "Vector.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ASSERT_VECTOR

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// After this constructor, either simulation is in error or
// dim > 0, v != NULL, and v[i] = 0.0 (i=0..dim-1)
/////////////////////////////////////////////////////////////////////////////////////
CVector::CVector(int dimension, char* strName)
{
	if (dimension <= 0)
	{
		CSimuMsg::ExitWithMessage("CVector::CVector(dimension, name)",
								"dimension <= 0 for vector, %s.", name);
		return;
	}

	if (strName != NULL)
		strcpy(name, strName);
	else
		strcpy(name, "no name");

	dim = dimension;
	v = new SimuValue[dim];
	for(int i=0; i<dim; i++)
		v[i] = 0.0;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
CVector::~CVector()
{
	delete []v;
	v = NULL;
	dim = 0;
}

void CVector::SetName(char* newName)
{
	strcpy(name, newName);
}

char* CVector::GetName()
{
	return name;
}

bool CVector::SetValue(CVector* vector)
{
	#ifdef	ASSERT_VECTOR
	if (vector->v == NULL)
		return CSimuMsg::PushErrorInfo("Found vector->v == NULL for vector, %s, in %s.",
										name, "CVector::SetValue(vector)");
	else if (v == NULL)
		return CSimuMsg::PushErrorInfo("Found v == NULL for vector, %s, in %s.",
										name, "CVector::SetValue(vector)");
	else if (vector->dim != dim)
		return CSimuMsg::PushErrorInfo("Found vector->dim != dim for vector, %s, in %s.",
										name, "CVector::SetValue(vector)");
	#endif

	for (int i=0; i<dim; i++)
	{
		v[i] = vector->v[i];
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
bool CVector::SetElements(int dimension, SimuValue elem1, SimuValue elem2, SimuValue elem3, SimuValue elem4)
{
	#ifdef	ASSERT_VECTOR
	if (dimension != dim)
		return CSimuMsg::PushErrorInfo("Found dimension != dim for vector, %s, in %s.",
										name,
										"CVector::SetElements(dimension, elem1, elem2, elem3)");
	else if (v == NULL)
		return CSimuMsg::PushErrorInfo("Found v == NULL for vector, %s, in %s.",
										name,
										"CVector::SetElements(dimension, elem1, elem2, elem3)");
	#endif

	if (dimension > 0)
	{
		v[0] = elem1;
		if (dimension > 1)
		{
			v[1] = elem2;
			if (dimension > 2)
			{
				v[2] = elem3;
				if (dimension > 3)
				{
					v[3] = elem4;
				}
			}
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
// str contains ',' or ' ' seperated double numbers. They may be enclosed by '(' and ')'.
// Continuous ',' and/or ' ' are treated as one seperator
// The result values are stored in array
/////////////////////////////////////////////////////////////////////////////////////
bool CVector::ParseStringForValue(CString str)
{
	char location[] = "CVector::ParseStringForValue(str)";

	if (str.GetLength() > LEN_DATA_LINE)
	{
		sprintf(CSimuMsg::GetEmptyBuf(),
				"Value string, %s, is too long for vector, %s.", str, name);
		CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
		return false;
	}
	// if data_pointer is enclosed by ( and ), take them off
	str.TrimLeft("(");
	str.TrimRight(")");
	if (str.GetLength() <= 0)
	{
		sprintf(CSimuMsg::GetEmptyBuf(),
				"Value string, %s, is empty for vector, %s.", str, name);
		CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
		return false;
	}

	char* buf = new char[LEN_DATA_LINE];
	char* data_pointer;
	char* scalar_data = new char[LEN_DATA_LINE];
	int len = 0;
	int head = 0;

	strcpy(buf, str);
	data_pointer = buf;
	int i;
	for(i=0; i<dim; i++)
	{
		if (head >= (int)strlen(data_pointer))
		{
			sprintf(CSimuMsg::GetEmptyBuf(),
					"Char array(%s) ended unexpectedly for vector, %s.", str, name);
			CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
			return false;
		}
		len = strcspn(&data_pointer[head], ", ");
		if (len >= LEN_DATA_LINE-head)
		{
			sprintf(CSimuMsg::GetEmptyBuf(),
					"Scalar data is too long for vector, %s.", name);
			CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
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
	if (i < dim)
	{
		sprintf(CSimuMsg::GetEmptyBuf(),
				"At lease one scalar data is missing for vector, %s.", name);
		CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
		return false;
	}

	delete buf;
	delete scalar_data;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
bool CVector::ToGLFloat(GLfloat* a, int dimension)
{
	#ifdef	ASSERT_VECTOR
	if (a == NULL)
		return CSimuMsg::PushErrorInfo("Found a == NULL for vector, %s, in %s.",
										name, "CVector::ToGLFloat(a, dimension)");
	else if (dimension != dim)
		return CSimuMsg::PushErrorInfo("Found dimension != dim for vector, %s, in %s.",
										name, "CVector::ToGLFloat(a, dimension)");
	else if (v == NULL)
		return CSimuMsg::PushErrorInfo("Found v == NULL for vector, %s, in %s.",
										name, "CVector::ToGLFloat(a, dimension)");
	#endif

	for(int i=0; i<dim; i++)
		a[i] = (GLfloat)v[i];

	return true;
}

SimuValue CVector::GetDistanceToVector(CVector* pVector)
{
	#ifdef	ASSERT_VECTOR
	if (pVector == NULL)
		return CSimuMsg::PushErrorInfo("Found pVector == NULL for vector, %s, in %s.",
										name, "CVector::getDistanceToVector(pVector)");
	else if (pVector->v == NULL)
		return CSimuMsg::PushErrorInfo("Found pVector(%s)->v == NULL for vector, %s, in %s.",
										pVector->name,
										name, "CVector::getDistanceToVector(pVector)");
	else if (dim != pVector->dim)
		return CSimuMsg::PushErrorInfo("Found dim != pVector(%s)->dim for vector, %s, in %s.",
										pVector->name,
										name, "CVector::getDistanceToVector(pVector)");
	else if (v == NULL)
		return CSimuMsg::PushErrorInfo("Found v == NULL for vector, %s, in %s.",
										name, "CVector::getDistanceToVector(pVector)");
	#endif

	SimuValue tmp;
	SimuValue d = 0.0;
	for (int i=0; i<dim; i++)
	{
		tmp = v[i] - pVector->v[i];
		d += tmp*tmp;
	}
	return sqrt(d);
}

/*
bool CVector::SetElements(int dimension, ...)
{
	#ifdef	ASSERT_VECTOR
	if (dimension != dim)
		return CSimuMsg::PushErrorInfo("Found dimension != dim for vector, %s, in %s.",
										name,
										"CVector::SetElements(dimension, ...)");
	else if (v == NULL)
		return CSimuMsg::PushErrorInfo("Found v == NULL for vector, %s, in %s.",
										name,
										"CVector::SetElements(dimension, ...)");
	#endif

	va_list arg_addr;

	va_start(arg_addr, dimension);
	for (int i=0; i<dimension; i++)
	{
		#ifdef	DOUBLE_VALUE
		v[i] = va_arg(arg_addr, double);
		#else
		v[i] = va_arg(arg_addr, float);
		#endif
	}
	va_end(arg_addr);

	return true;
}
*/