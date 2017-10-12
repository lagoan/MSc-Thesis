// TPointerArray.h: interface for the TPointerArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TPOINTERARRAY_H__A988A1E8_94DB_48F8_8783_4206318290DA__INCLUDED_)
#define AFX_TPOINTERARRAY_H__A988A1E8_94DB_48F8_8783_4206318290DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuMsg.h"

template <class Obj_T> class TPointerArray  
{
public:
	int m_paNum;
	int m_paMax;
	Obj_T** m_paPtrs;

	TPointerArray();
	TPointerArray(int extendSize);
	virtual ~TPointerArray();
	void InitializePointerArray(int extendSize);

	Obj_T* &operator[] (int i);
	int SearchForOnePointer(Obj_T* pObj);
	bool RemoveOnePointer(Obj_T* pObj);
	Obj_T* RemoveOnePointerFromEnd();
	void AppendOnePointer(Obj_T* pObj);
	void AppendPointersFromArray(TPointerArray<Obj_T> &ptrArray);
	void TakeOverPointersFromArray(TPointerArray<Obj_T> &ptrArray);
	void SwapWithPointerArray(TPointerArray<Obj_T> &ptrArray);
	virtual void ResetArraySize(int newSize);

protected:
	int m_paExtendSize;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template <class Obj_T> 
TPointerArray<Obj_T>::TPointerArray()
{
	InitializePointerArray(SIMU_PARA_DEFAULT_ARRAY_ALLOC_SIZE);
}

template <class Obj_T> 
TPointerArray<Obj_T>::TPointerArray(int extendSize)
{
	InitializePointerArray(extendSize);
}

template <class Obj_T> 
TPointerArray<Obj_T>::~TPointerArray()
{
	if (m_paPtrs != NULL)
	{
		ResetArraySize(0);
		delete m_paPtrs;
		m_paPtrs = NULL;
	}
}

template <class Obj_T> 
void TPointerArray<Obj_T>::InitializePointerArray(int extendSize)
{
	m_paNum = 0;
	m_paMax = 0;
	m_paPtrs = NULL;
	m_paExtendSize = extendSize;
	if (extendSize <= 0)
		CSimuMsg::ExitWithMessage("TPointerArray<Obj_T>::InitializePointerArray(extendSize)",
								"Array extending size must be positive integer.");
}

template <class Obj_T> 
Obj_T* &TPointerArray<Obj_T>::operator[] (int i)
{
	if (i >= m_paNum)
		CSimuMsg::ExitWithMessage("TPointerArray<Obj_T>::operator[]",
									"Index out of range.");

	return m_paPtrs[i];
}

template <class Obj_T> 
int TPointerArray<Obj_T>::SearchForOnePointer(Obj_T* pObj)
{
	for (int i=0; i < m_paNum; i++)
	{
		if (m_paPtrs[i] == pObj)
			return i;
	}
	return -1; // not found
}

template <class Obj_T> 
bool TPointerArray<Obj_T>::RemoveOnePointer(Obj_T* pObj)
{
	int ptrId = SearchForOnePointer(pObj);

	if (ptrId == -1) return false; // not found

	for (int j=ptrId+1; j < m_paNum; j++)
		m_paPtrs[j-1] = m_paPtrs[j];

	m_paPtrs[m_paNum-1] = NULL;
	m_paNum --;
	return true;
}

template <class Obj_T> 
Obj_T* TPointerArray<Obj_T>::RemoveOnePointerFromEnd()
{
	Obj_T* removedPointer = m_paPtrs[m_paNum-1];
	m_paPtrs[m_paNum-1] = NULL;
	m_paNum --;
	return removedPointer;
}

template <class Obj_T> 
void TPointerArray<Obj_T>::AppendOnePointer(Obj_T* pType)
{
	CheckPointerBufferSize<Obj_T>(m_paPtrs, m_paNum, m_paMax, m_paExtendSize);
	m_paPtrs[m_paNum++] = pType;
}

// ptrArray is not empty after this call
template <class Obj_T> 
void TPointerArray<Obj_T>::AppendPointersFromArray(TPointerArray<Obj_T> &ptrArray)
{
	for (int i=0; i < ptrArray.m_paNum; i++)
		AppendOnePointer(ptrArray[i]);
}

// ptrArray is empty after this call
template <class Obj_T> 
void TPointerArray<Obj_T>::TakeOverPointersFromArray(TPointerArray<Obj_T> &ptrArray)
{
	while (ptrArray.m_paNum > 0)
	{
		AppendOnePointer(ptrArray[ptrArray.m_paNum-1]);
		ptrArray[ptrArray.m_paNum-1] = NULL;
		ptrArray.m_paNum --;
	}
}

template <class Obj_T> 
void TPointerArray<Obj_T>::SwapWithPointerArray(TPointerArray<Obj_T> &ptrArray)
{
	int tmpInt = ptrArray.m_paNum;
	ptrArray.m_paNum = m_paNum;
	m_paNum = tmpInt;

	tmpInt = ptrArray.m_paMax;
	ptrArray.m_paMax = m_paMax;
	m_paMax = tmpInt;

	tmpInt = ptrArray.m_paExtendSize;
	ptrArray.m_paExtendSize = m_paExtendSize;
	m_paExtendSize = tmpInt;

	Obj_T** tmpPtrs = ptrArray.m_paPtrs;
	ptrArray.m_paPtrs = m_paPtrs;
	m_paPtrs = tmpPtrs;
}

template <class Obj_T> 
void TPointerArray<Obj_T>::ResetArraySize(int newSize)
{
	if (m_paMax < newSize)
		ExtendPointerBuffer<Obj_T>(m_paPtrs, m_paMax, newSize - m_paMax);
	m_paNum = newSize;
}

#endif // !defined(AFX_TPOINTERARRAY_H__A988A1E8_94DB_48F8_8783_4206318290DA__INCLUDED_)
