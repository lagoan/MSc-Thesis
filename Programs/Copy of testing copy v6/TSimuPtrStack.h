// TSimuPtrStack.h: interface for the TSimuPtrStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSIMUPTRSTACK_H__DA0EF56A_5C25_43E9_A82D_DF58A5C00A9A__INCLUDED_)
#define AFX_TSIMUPTRSTACK_H__DA0EF56A_5C25_43E9_A82D_DF58A5C00A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class Pointed_T> class TSimuPtrStack  
{
	Pointed_T**	m_buffer;
	int		m_maxPtrsInBuffer;
	int		m_numPtrsInBuffer;	// m_numPtrsInBuffer <= m_maxPtrsInBuffer

	int		m_numTotalPtrs;	// debug use only
	CString	m_name;	// debug use only

public:
	TSimuPtrStack(CString name, int iniSize=0);
	virtual ~TSimuPtrStack();

	void CheckBufferSize();
	void SetSize(int newSize);
	void Push(Pointed_T* aPtr);
	Pointed_T* Pop();
};

#define		ASSERT_TSIMU_PTR_STACK

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

template <class Pointed_T> TSimuPtrStack<Pointed_T>::TSimuPtrStack(CString name, int iniSize)
{
	m_buffer	= NULL;
	m_maxPtrsInBuffer	= 0;
	m_numPtrsInBuffer	= 0;

	m_numTotalPtrs	= 0;
	m_name = name;

	SetSize(iniSize);
};

template <class Pointed_T> TSimuPtrStack<Pointed_T>::~TSimuPtrStack()
{
	if (m_numTotalPtrs != m_numPtrsInBuffer)
	{
		char tmpMsg[LEN_DATA_LINE];
		sprintf(tmpMsg, "Missing %d allocated objects of %s when exit.",
						m_numTotalPtrs-m_numPtrsInBuffer, m_name);
		MessageBox(NULL, tmpMsg, "TSimuPtrStack<Pointed_T>::~TSimuPtrStack()", MB_ICONERROR);
	}

	// delete all objects pointed to by all pointers
	for (int i=0; i < m_numPtrsInBuffer; i++)
	{
		delete (Pointed_T*)m_buffer[i];
	}
	// delete the stack buffer
	if (m_maxPtrsInBuffer > 0)
	{
		delete []m_buffer;
	}
};

template <class Pointed_T> void TSimuPtrStack<Pointed_T>::CheckBufferSize()
{
	CheckPointerBufferSize<Pointed_T>(m_buffer, m_numPtrsInBuffer, m_maxPtrsInBuffer,
									  SIMU_PARA_MEM_ALLOC_SIZE);
};

//////////////////////////////////////////////////////////////////////
// This function allocates bigger memory block for the stack buffer,
// and copies the existing pointers from the old buffer into the new buffer.
//////////////////////////////////////////////////////////////////////
template <class Pointed_T> void TSimuPtrStack<Pointed_T>::SetSize(int newSize)
{
	if (newSize <= m_maxPtrsInBuffer)
	{
		CSimuMsg::ExitWithMessage("TSimuPtrStack<Pointed_T>::SetSize(newSize)",
								"Stack should not shrink.");
		return;
	}
	else if (newSize < 0)
	{
		CSimuMsg::ExitWithMessage("TSimuPtrStack<Pointed_T>::SetSize(newSize)",
								"New size should not be negative.");
		return;
	}
	Pointed_T** newBuffer = new Pointed_T*[newSize];
	for (int i=0; i < m_numPtrsInBuffer; i++)
	{
		newBuffer[i] = m_buffer[i];
	}

	if (m_maxPtrsInBuffer > 0)
	{
		delete []m_buffer;
	}

	m_buffer = newBuffer;
	m_maxPtrsInBuffer = newSize;
};

template <class Pointed_T> void TSimuPtrStack<Pointed_T>::Push(Pointed_T* aPtr)
{
	if (!aPtr->IsInUse())
	{
		CSimuMsg::ExitWithMessage("TSimuPtrStack<Pointed_T>::Push(aPtr)",
								"Object(%d) is already recycled.", aPtr->GetObjId());
		return;
	}
	aPtr->ResetMemberVariables();	// make the object ready to be popped up
	aPtr->SetInUse(false);
	aPtr->IncreaseRecycledTimes();
	CheckBufferSize();
	m_buffer[m_numPtrsInBuffer++] = aPtr;
};

template <class Pointed_T> Pointed_T* TSimuPtrStack<Pointed_T>::Pop()
{
	Pointed_T* aPtr = NULL;
	if (m_numPtrsInBuffer > 0)
	{
		aPtr = m_buffer[--m_numPtrsInBuffer];
		m_buffer[m_numPtrsInBuffer] = NULL;	// remove popped ptr from stack
	}
	else
	{
		aPtr = new Pointed_T();
		m_numTotalPtrs ++;
	}

	if (aPtr->IsInUse())
		CSimuMsg::ExitWithMessage("TSimuPtrStack<Pointed_T>::Push(aPtr)",
								"Object(%d) is already in use.", aPtr->GetObjId());
	aPtr->SetInUse(true);
	return aPtr;
};

#endif // !defined(AFX_TSIMUPTRSTACK_H__DA0EF56A_5C25_43E9_A82D_DF58A5C00A9A__INCLUDED_)
