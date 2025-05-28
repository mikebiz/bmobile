#pragma once

#ifdef BLXWARE_EXPORTS
#define BLXWARE_API __declspec(dllexport)
#define BLXWARE_LINKAGE extern
#else
#define BLXWARE_API __declspec(dllimport)
#define BLXWARE_LINKAGE extern 
#endif


#include <windows.h>


template<typename T>
class CBlxHeap
{
public:
	PVOID operator new( size_t size );
	VOID operator delete( PVOID p );
protected:

private:
	T* m_t;
};

#include "BlxHeap.inl"