// Vector.h: interface for the CVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR_H__7F557342_CBC5_11D5_9B85_0050BADE013B__INCLUDED_)
#define AFX_VECTOR_H__7F557342_CBC5_11D5_9B85_0050BADE013B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl/gl.h>

class CVector
{
public:
	char name[LEN_VECTOR_NAME];
	int		dim;
	SimuValue*	v;

	CVector(int dimension=VECTOR_DIMENSION, char* strName=NULL);
	~CVector();
	void SetName(char* newName);
	char* GetName();

	bool SetValue(CVector* vector);
	bool SetElements(int dimension, SimuValue elem1=0, SimuValue elem2=0, SimuValue elem3=0, SimuValue elem4=0);
	bool ParseStringForValue(CString string);
	bool ToGLFloat(GLfloat* a, int dimension);
	SimuValue GetDistanceToVector(CVector* pVector);

/*
	bool SetElements(int dimension, ...);
*/
};

#endif // !defined(AFX_VECTOR_H__7F557342_CBC5_11D5_9B85_0050BADE013B__INCLUDED_)
