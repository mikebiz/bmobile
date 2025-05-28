#include "ServerWorkerThreadData.h"

BLXWARE_API HANDLE CServerWorkerThreadData::m_hHeap = NULL;
BLXWARE_API LONG CServerWorkerThreadData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CServerWorkerThreadData::m_csLock = NULL;

CServerWorkerThreadData::CServerWorkerThreadData( UINT32 PacketSize )
: m_uiHeapPriority( 0 )
//		, m_hBufferEvent( WSACreateEvent() )
{
	initializeBuffer( PacketSize );
	hEvent = WSACreateEvent();
}

CServerWorkerThreadData::~CServerWorkerThreadData()
{
	if( INVALID_HANDLE_VALUE != this->hEvent )
	{
		CloseHandle( this->hEvent );
	}
	if( 0 < m_iNumBuffers )
	{
		for( SHORT i = 0 ; i < m_iNumBuffers ; i++ )
		{
			if( HeapFree( 
				CServerWorkerThreadData::m_hHeap
				, 0 //HEAP_NO_SERIALIZE
				, m_pDataBuf[ i ].buf 
				)
				)
			{
				CServerWorkerThreadData::decrement_NumAllocsInHeap();
			}
			else
			{
				OutputDebugStringf( L"\n~CServerWorkerThreadData HeapFree of buf[%d] failed", i );
			}
		}
		if(	HeapFree( 
			CServerWorkerThreadData::m_hHeap
			, 0 //HEAP_NO_SERIALIZE
			, m_pDataBuf 
			)
			)
		{
			CServerWorkerThreadData::decrement_NumAllocsInHeap();
		}
		else
		{
			OutputDebugString( L"\n~CServerWorkerThreadData HeapFree of wsabuf failed" );
		}
	}
}

inline VOID CServerWorkerThreadData::IngestNetworkPacket( PCHAR pReceiveData, USHORT usLength )
{
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
	reallocBuffer( usLength );
	::CopyMemory(
		m_pDataBuf->buf
		, pReceiveData 
		, m_pDataBuf->len
		);
	return;
}

inline VOID CServerWorkerThreadData::ReallocBuffer( USHORT usLength )
{
	reallocBuffer( usLength );
}

PIP_HEADER CServerWorkerThreadData::get_IpHeader( )
{
	return reinterpret_cast< PIP_HEADER >(
		m_pDataBuf[ 0 ].buf 
		);
}
PUDP_HEADER CServerWorkerThreadData::get_UdpHeader( )
{
	return reinterpret_cast< PUDP_HEADER >(
		m_pDataBuf[ 0 ].buf 
		+ MAX_IP_HEADER
		);
}
PEVENT_HEADER CServerWorkerThreadData::get_EventHeader( )
{
	return reinterpret_cast< PEVENT_HEADER >(
		m_pDataBuf[ 0 ].buf 
		+ MAX_IP_HEADER
		+ MAX_UDP_HEADER
		);
}
PEVENT_PACKET_HEADER CServerWorkerThreadData::get_EventPacketHeader( )
{
	return reinterpret_cast< PEVENT_PACKET_HEADER >(
		m_pDataBuf[ 0 ].buf 
		+ MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		);
}
PPACKET_DATA_BLOB CServerWorkerThreadData::get_PacketDataBlob( )
{
	return reinterpret_cast< PPACKET_DATA_BLOB >(
		m_pDataBuf[ 0 ].buf 
		+ MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		);
}

WSABUF* CServerWorkerThreadData::get_WSABUF( )
{
	return m_pDataBuf;
}

VOID CServerWorkerThreadData::initializeBuffer( UINT32 length )
{

	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc( CServerWorkerThreadData::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) ) );
	CServerWorkerThreadData::increment_NumAllocsInHeap();
	m_pDataBuf->len = length;

	m_pDataBuf->buf = reinterpret_cast< PCHAR >( HeapAlloc( CServerWorkerThreadData::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, length ) );
	CServerWorkerThreadData::increment_NumAllocsInHeap();

	m_iNumBuffers = 1;
	return;
}

VOID CServerWorkerThreadData::reallocBuffer( USHORT length )
{
	m_pDataBuf->len = length;

	m_pDataBuf->buf = reinterpret_cast< PCHAR >( HeapReAlloc( CServerWorkerThreadData::m_hHeap, HEAP_ZERO_MEMORY, m_pDataBuf->buf , length ) );

	return;
}
