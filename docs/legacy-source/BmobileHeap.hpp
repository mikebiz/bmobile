#pragma once

#include "BlxCommonLib.h"

#include "debug.h"

namespace Bmobile{
    namespace System{

template<typename T>
class CHeap
{
public:
	PVOID operator new( size_t size );
	VOID operator delete( PVOID p );
protected:

	static CComAutoCriticalSection* get_Cs()
	{
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

    }//namespace System
}//namespace Bmobile

#include "BmobileHeap.inl"