#include "ServerSendThreadData.h"

#include <atlcoll.h>

BLXWARE_API HANDLE CServerSendThreadData::m_hHeap = NULL;
BLXWARE_API LONG CServerSendThreadData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CServerSendThreadData::m_csLock = NULL;

CServerSendThreadData::CServerSendThreadData( UINT32 PacketSize )
: m_uiHeapPriority( 0 )
{
	initializeBuffer( PacketSize );
	hEvent = WSACreateEvent();
}

CServerSendThreadData::~CServerSendThreadData()
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
				CServerSendThreadData::m_hHeap
				, 0 //HEAP_NO_SERIALIZE
				, m_pDataBuf[ i ].buf 
				)
				)
			{
				CServerSendThreadData::decrement_NumAllocsInHeap();
			}
			else
			{
				OutputDebugStringf( L"\n~CServerSendThreadData HeapFree of buf[%d] failed", i );
			}
		}
		if(	HeapFree( 
			CServerSendThreadData::m_hHeap
			, 0 //HEAP_NO_SERIALIZE
			, m_pDataBuf 
			)
			)
		{
			CServerSendThreadData::decrement_NumAllocsInHeap();
		}
		else
		{
			OutputDebugString( L"\n~CServerSendThreadData HeapFree of wsabuf failed" );
		}
	}
}

VOID CServerSendThreadData::IngestNetworkPacket( PCHAR pReceiveData, USHORT usLength )
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

VOID CServerSendThreadData::ReallocBuffer( USHORT usLength )
{
	reallocBuffer( usLength );
}

PIP_HEADER CServerSendThreadData::get_IpHeader( )
{
	return reinterpret_cast< PIP_HEADER >(
		m_pDataBuf[ 0 ].buf 
		);
}
PUDP_HEADER CServerSendThreadData::get_UdpHeader( )
{
	return reinterpret_cast< PUDP_HEADER >(
		m_pDataBuf[ 0 ].buf 
		+ MAX_IP_HEADER
		);
}
PEVENT_HEADER CServerSendThreadData::get_EventHeader( )
{
	return reinterpret_cast< PEVENT_HEADER >(
		m_pDataBuf[ 0 ].buf 
		+ MAX_IP_HEADER
		+ MAX_UDP_HEADER
		);
}
PEVENT_PACKET_HEADER CServerSendThreadData::get_EventPacketHeader( )
{
	return reinterpret_cast< PEVENT_PACKET_HEADER >(
		m_pDataBuf[ 0 ].buf 
		+ MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		);
}
PPACKET_DATA_BLOB CServerSendThreadData::get_PacketDataBlob( )
{
	return reinterpret_cast< PPACKET_DATA_BLOB >(
		m_pDataBuf[ 0 ].buf 
		+ MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		);
}

WSABUF* CServerSendThreadData::get_WSABUF( )
{
	return m_pDataBuf;
}

VOID CServerSendThreadData::initializeBuffer( UINT32 length )
{

	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc( CServerSendThreadData::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) ) );
	CServerSendThreadData::increment_NumAllocsInHeap();
	m_pDataBuf->len = length;

	m_pDataBuf->buf = reinterpret_cast< PCHAR >( HeapAlloc( CServerSendThreadData::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, length ) );
	CServerSendThreadData::increment_NumAllocsInHeap();

	m_iNumBuffers = 1;
	return;
}

VOID CServerSendThreadData::reallocBuffer( USHORT length )
{
	m_pDataBuf->len = length;

	m_pDataBuf->buf = reinterpret_cast< PCHAR >( HeapReAlloc( CServerSendThreadData::m_hHeap, HEAP_ZERO_MEMORY, m_pDataBuf->buf , length ) );

	return;
}

