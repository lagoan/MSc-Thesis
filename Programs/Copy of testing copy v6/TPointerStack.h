// TPointerStack.h: interface for the TPointerStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TPOINTERSTACK_H__CDCE9931_DACE_4E73_BFFB_84DFCFD9D9E9__INCLUDED_)
#define AFX_TPOINTERSTACK_H__CDCE9931_DACE_4E73_BFFB_84DFCFD9D9E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class Obj_T> class TPointerStack : public TPointerArray<Obj_T>
{
public:
	TPointerStack();
	virtual ~TPointerStack();

	void StackPush(Obj_T* pObj);
	Obj_T* StackPop();
	bool StackIsEmpty();
	void ClearStack();
};

//////////////////////////////////////////////////////////////////////
// Template Implementation
//////////////////////////////////////////////////////////////////////

template <class Obj_T> 
TPointerStack<Obj_T>::TPointerStack()
{
	InitializePointerArray(SIMU_PARA_DEFAULT_ARRAY_ALLOC_SIZE);
}

template <class Obj_T> 
TPointerStack<Obj_T>::~TPointerStack()
{
}

template <class Obj_T> 
void TPointerStack<Obj_T>::StackPush(Obj_T* pObj)
{
	AppendOnePointer(pObj);
}

template <class Obj_T> 
Obj_T* TPointerStack<Obj_T>::StackPop()
{
	return m_paPtrs[--m_paNum];
}

template <class Obj_T> 
bool TPointerStack<Obj_T>::StackIsEmpty()
{
	if (m_paNum > 0)
		return false;
	return true;
}

template <class Obj_T> 
void TPointerStack<Obj_T>::ClearStack()
{
	ResetArraySize(0);
}

#endif // !defined(AFX_TPOINTERSTACK_H__CDCE9931_DACE_4E73_BFFB_84DFCFD9D9E9__INCLUDED_)
