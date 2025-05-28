#pragma once

#include "PriorityQueueSync.h"
#include "operators.h"

template<typename T, class SC, class TEST_LT>
class CBlxPqBuffer
{
public:
	CBlxPqBuffer( INT32 numBuffers, INT32 defaultBufferSize, INT32 maxThreshold, INT32 minThreshold, SC* serverContext, DWORD retryInterval );
	~CBlxPqBuffer();
	inline T* GetCurrentPacketThreadData( );
	inline VOID ReturnPacketThreadData( T* ThreadData  );

protected:
	CBlxPqBuffer( const CBlxPqBuffer& buf );
private:

	SC*							m_serverContext;
	INT32						m_numBuffers;
	INT32						m_defaultBufferSize;
	INT32						m_maxThreshold;
	INT32						m_minThreshold;
	DWORD						m_retryInterval;

	typedef Blxware::PriorityQueueSync<std::priority_queue< T*, std::vector< T* >, TEST_LT>	> QPacketThreadData;
	typedef QPacketThreadData*	PQPacketThreadData;
	PQPacketThreadData			m_pQPacketThreadData;

	VOID AllocateThreadData();
	VOID DeallocateThreadData();
};

	
template<typename T, class SC, class TEST_LT>
CBlxPqBuffer<T, SC, TEST_LT>::CBlxPqBuffer( INT32 numBuffers, INT32 defaultBufferSize, INT32 maxThreshold, INT32 minThreshold, SC* serverContext, DWORD retryInterval = 1000 )
	: m_numBuffers( numBuffers )
	, m_defaultBufferSize( defaultBufferSize )
	, m_maxThreshold( maxThreshold )
	, m_minThreshold( minThreshold )
	, m_retryInterval( retryInterval )
	, m_serverContext( serverContext )
{
	AllocateThreadData();
}

template<typename T, class SC, class TEST_LT>
CBlxPqBuffer<T, SC, TEST_LT>::~CBlxPqBuffer()
{
	return;
}

template<typename T, class SC, class TEST_LT>
VOID CBlxPqBuffer<T, SC, TEST_LT>::AllocateThreadData()
{
	T* pServerOverlapped;

	for( UINT32 j = 0 ; j < m_numBuffers ; j++ )
	{
		pServerOverlapped = new T( m_defaultBufferSize );
		pServerOverlapped->Internal = 0;
		pServerOverlapped->InternalHigh = 0;
		pServerOverlapped->Offset = 0;
		pServerOverlapped->OffsetHigh = 0;
		pServerOverlapped->m_iPktCnt = 0;
		pServerOverlapped->m_pContext = m_serverContext;
		WSASetEvent( pServerOverlapped->hEvent );
		
		m_pQPacketThreadData->offer(
			pServerOverlapped 
			, m_retrvInterval
			);
	}

	return;
}

template<typename T, class SC, class TEST_LT>
VOID CBlxPqBuffer<T, SC, TEST_LT>::DeallocateThreadData()
{
	//need to blow away the internal heap first.
	m_pQPacketThreadData->empty();
	return;
}

template<typename T, class SC, class TEST_LT>
T* CBlxPqBuffer<T, SC, TEST_LT>::GetCurrentPacketThreadData( )
{
	T* pData = NULL;
	 m_pQPacketThreadData->poll_timed(
		pData
		, m_retryInterval
		);
	return pData;
}

template<typename T, class SC, class TEST_LT>
VOID CBlxPqBuffer<T, SC, TEST_LT>::ReturnPacketThreadData( T* ThreadData )
{
	m_pQPacketThreadData->offer_timed(
		ThreadData
		, m_retryInterval
		);
	WSASetEvent( ThreadData->hEvent );
	return;
}
