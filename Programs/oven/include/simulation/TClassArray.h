// TClassArray.h: interface for the TClassArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCLASSARRAY_H__8BFF76E0_85C8_410D_AEA8_B6BD57E5CD32__INCLUDED_)
#define AFX_TCLASSARRAY_H__8BFF76E0_85C8_410D_AEA8_B6BD57E5CD32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TPointerArray.h"

template <class Class_T> class TClassArray : public TPointerArray<Class_T>
{
public:
	
	//int m_paNum;
	//int m_paMax;
	//Class_T** m_paPtrs;
	
	TClassArray();
	TClassArray(bool bMasterArray, int extendSize);
	virtual ~TClassArray();
	void InitializeClassArray(bool bMasterArray, int extendSize);

	void SetMasterArrayFlag(bool bMasterArray);
	bool GetMasterArrayFlag();
	Class_T* CreateOneElement();
	void SwapWithClassArray(TClassArray<Class_T> &ptrArray);
	virtual void ResetArraySize(int newSize);

protected:
	bool m_caIsMasterArray;
};

//////////////////////////////////////////////////////////////////////
// Template Implementation
//////////////////////////////////////////////////////////////////////

template <class Class_T> 
TClassArray<Class_T>::TClassArray()
{
	SetMasterArrayFlag(false);
}

template <class Class_T> 
TClassArray<Class_T>::TClassArray(bool bMasterArray, int extendSize)
{
	InitializeClassArray(bMasterArray, extendSize);
}

template <class Class_T> 
TClassArray<Class_T>::~TClassArray()
{
	if (this->m_paMax > 0)
	{
		ResetArraySize(0);
		this->m_paMax = 0;
		delete this->m_paPtrs;
		this->m_paPtrs = NULL;
	}
	else if (this->m_paPtrs != NULL)
		CSimuMsg::ExitWithMessage("TClassArray<Class_T>::~TClassArray()",
									"Buffer is not NULL when max number is zero.");
}

template <class Class_T> 
void TClassArray<Class_T>::InitializeClassArray(bool bMasterArray, int extendSize)
{
	TPointerArray<Class_T>::InitializePointerArray(extendSize);
	SetMasterArrayFlag(bMasterArray);
}

template <class Class_T> 
void TClassArray<Class_T>::SetMasterArrayFlag(bool bMasterArray)
{
	m_caIsMasterArray = bMasterArray;
}

template <class Class_T> 
bool TClassArray<Class_T>::GetMasterArrayFlag()
{
	return m_caIsMasterArray;
}

template <class Class_T> 
Class_T* TClassArray<Class_T>::CreateOneElement()
{
	Class_T* newElmt = new Class_T();
	AppendOnePointer(newElmt);
	return newElmt;
}

template <class Class_T> 
void TClassArray<Class_T>::SwapWithClassArray(TClassArray<Class_T> &classArray)
{
	SwapWithPointerArray(classArray);

	bool tmpBool = classArray.m_caIsMasterArray;
	classArray.m_caIsMasterArray = m_caIsMasterArray;
	m_caIsMasterArray = tmpBool;
}

// since this function involves object creation and delete,
// it only applys to master pointer array
template <class Class_T> 
void TClassArray<Class_T>::ResetArraySize(int newSize)
{
	char location[] = "TClassArray<Class_T>::ResetArraySize(newSize)";

	if (!m_caIsMasterArray)
	{
		TPointerArray<Class_T>::ResetArraySize(newSize);
		return;
	}

	if (this->m_paNum == newSize)
	{
	}
	else if (this->m_paNum < newSize)
	{
		if (newSize > this->m_paMax)
			ExtendPointerBuffer<Class_T>(this->m_paPtrs, this->m_paMax, newSize - this->m_paMax);
		while (this->m_paNum < newSize)
			this->m_paPtrs[this->m_paNum++] = new Class_T();
	}
	else // (this->m_paNum > newSize)
	{
		while (this->m_paNum > newSize)
		{
			this->m_paNum --;
			delete this->m_paPtrs[this->m_paNum];
//			delete this->m_paPtrs[--this->m_paNum];
		}
	}
}

#endif // !defined(AFX_TCLASSARRAY_H__8BFF76E0_85C8_410D_AEA8_B6BD57E5CD32__INCLUDED_)
