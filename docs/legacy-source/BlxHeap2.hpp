#pragma once

#include "BlxCommonLib.h"

#include "debug.h"

template<typename T>
class CBlxHeap2
{
public:
	PVOID operator new( size_t size );
	VOID operator delete( PVOID p );
protected:

	static CComCriticalSection* get_Cs()
	{
		if( T::m_csLock == NULL )
		{
			T::m_csLock = new CComCriticalSection();
			T::m_csLock->Init();
		}
		return T::m_csLock;
	}

	static HANDLE get_Heap()
	{
		return T::m_hHeap;
	}

	static VOID set_Heap( HANDLE hHeap)
	{
		T::m_hHeap = hHeap;
	}

	static VOID delete_Heap()
	{
		T::m_hHeap = NULL;
	}

	static LONG get_NumAllocsInHeap()
	{
		return T::m_uNumAllocsInHeap;
	}

	static VOID increment_NumAllocsInHeap( )
	{
		InterlockedIncrement( &T::m_uNumAllocsInHeap );
	}

	static VOID decrement_NumAllocsInHeap( )
	{
		InterlockedDecrement( &T::m_uNumAllocsInHeap );
	}

private:
	T* m_t;

};

#include "BlxHeap2.inl"