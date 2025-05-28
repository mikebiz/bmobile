#pragma once

#include <crtdbg.h>

#include "BlxCommonLib.h"

#include <queue>

template< typename T >
class /*BLXWARE_API*/ CPriorityQueue
{
public:
	CPriorityQueue( UINT32 MinBufferSize, UINT32 MaxBufferSize );

	BOOL Empty();
	size_t Size();
//	T& const Top() const;
	BOOL Push(const T& Pred);
	VOID Pop();
	T& Top();

protected:

private:
	typedef std::priority_queue< T > PriorityQueue;
	PriorityQueue				m_pq;
	
	CRITICAL_SECTION			m_SRWLock;
//	SRWLOCK						m_SRWLock;
	CONDITION_VARIABLE			m_cvReadyToRead;
	CONDITION_VARIABLE			m_cvReadyToWrite;
	HANDLE						m_hEvent;

	UINT32						m_uiMaxBufferSize;
	UINT32						m_uiMinBufferSize;
	UINT32						m_uiCurrentBufferSize;

	BOOL						IsFull();
	BOOL						IsBelowThreshold();
};

//#ifdef BLXWARE_EXPORTS
#include "PriorityQueue.inl"
//#endif