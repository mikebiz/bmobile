#pragma once

#include "SyncQueue.h"

template<typename T, class SC>
class CBlxBuffer
{
public:

	CBlxBuffer( INT32 numBuffers, INT32 defaultBufferSize, INT32 maxThreshold, INT32 minThreshold, SC* serverContext, DWORD retryInterval );
	~CBlxBuffer();
	inline T* GetCurrentPacketThreadData( );
	inline VOID ReturnPacketThreadData( T* ThreadData  );

protected:
	CBlxBuffer( const CBlxBuffer& buf );
private:

	SC*							m_serverContext;
	UINT32						m_numBuffers;
	UINT32						m_defaultBufferSize;
	UINT32						m_maxThreshold;
	UINT32						m_minThreshold;
	DWORD						m_retryInterval;

#ifdef _USE_ARRAY_CACHE_

	CSimpleArray<T*>			m_pQPacketThreadData;

	UINT32						m_receiveThreadId;
	LONG						m_lreceiveThreadPhase;
	LONG						m_treceiveThread;
	LONG						m_outstandingReads;

#else

	typedef Blxware::SyncQueue< T* > QPacketThreadData;
	typedef QPacketThreadData* PQPacketThreadData;
	PQPacketThreadData m_pQPacketThreadData;

#endif

	VOID AllocateThreadData();
	VOID DeallocateThreadData();
};
	
template<typename T, class SC>
CBlxBuffer<T, SC>::CBlxBuffer( INT32 numBuffers, INT32 defaultBufferSize, INT32 maxThreshold, INT32 minThreshold, SC* serverContext, DWORD retryInterval = 1000 )
	: m_numBuffers( numBuffers )
	, m_defaultBufferSize( defaultBufferSize )
	, m_maxThreshold( maxThreshold )
	, m_minThreshold( minThreshold )
	, m_retryInterval( retryInterval )
#ifdef _USE_ARRAY_CACHE_
	, m_receiveThreadId( 0 )
	, m_lreceiveThreadPhase( -1 )
	, m_outstandingReads( 0 )
	, m_treceiveThread( numBuffers )
#endif
	, m_serverContext( serverContext )
{

#ifndef _USE_ARRAY_CACHE_
	m_pQPacketThreadData = new QPacketThreadData( numBuffers );
#endif
	AllocateThreadData();
}

template<typename T, class SC>
CBlxBuffer<T, SC>::~CBlxBuffer()
{
	return;
}

template<typename T, class SC>
VOID CBlxBuffer<T, SC>::AllocateThreadData()
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
//		WSASetEvent( pServerOverlapped->m_hBufferEvent );
		
#ifdef _USE_ARRAY_CACHE_
		m_pQPacketThreadData.Add( pServerOverlapped );
#else
		m_pQPacketThreadData->push( pServerOverlapped );
#endif
	}

	return;
}

template<typename T, class SC>
VOID CBlxBuffer<T, SC>::DeallocateThreadData()
{
#ifdef _USE_ARRAY_CACHE_
	m_pQServerPacketThreadData.RemoveAll();
#else
	m_pQServerPacketThreadData->clear();
#endif
	return;
}

template<typename T, class SC>
T* CBlxBuffer<T, SC>::GetCurrentPacketThreadData( )
{
	T* pData = NULL;
#ifdef _USE_ARRAY_CACHE_

	InterlockedIncrement( &m_outstandingReads );
	if( m_numBuffers <= m_outstandingReads )
	{
		InterlockedDecrement( &m_outstandingReads );
		return pData;
	}

	InterlockedIncrement( &m_lreceiveThreadPhase );
	LONG iThread = m_lreceiveThreadPhase;

	//if( iThread > m_treceiveThread - 2 )
	//{
	//	T* pThreadOldest 
	//		= m_pQPacketThreadData[ ( iThread + 1 ) % m_treceiveThread ];

	//	WaitForSingleObject(
	//		pThreadOldest->m_hBufferEvent 
	//		, 5000
	//		);

	//	DWORD _error = GetLastError();
	//	if( WAIT_TIMEOUT == _error )
	//	{
	//		return pData;
	//	}
	//	WSAResetEvent( pThreadOldest->m_hBufferEvent);
	//	WSAResetEvent( pThreadOldest->hEvent);
	//}
	pData = m_pQPacketThreadData[ iThread % m_treceiveThread ];

	WaitForSingleObject(
		pData->m_hBufferEvent 
		, 1000
		);

	DWORD _error = GetLastError();
	if( WAIT_TIMEOUT == _error )
	{
		return NULL;
	}
	WSAResetEvent( pData->m_hBufferEvent);
	WSAResetEvent( pData->hEvent);

#else
	 m_pQPacketThreadData->poll_timed(
		pData
		, m_retryInterval
		);
#endif
	return pData;
}

template<typename T, class SC>
VOID CBlxBuffer<T, SC>::ReturnPacketThreadData( T* ThreadData )
{
#ifdef _USE_ARRAY_CACHE_
	if( 1 < m_outstandingReads )
	{
		InterlockedDecrement( &m_outstandingReads );
		WSASetEvent( ThreadData->m_hBufferEvent );
	}
#else
	m_pQPacketThreadData->push_timed(
		ThreadData
		, m_retryInterval
		);
#endif
	WSASetEvent( ThreadData->hEvent );
	return;
}
