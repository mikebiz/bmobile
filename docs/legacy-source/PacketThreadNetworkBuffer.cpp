#pragma once

#include "PacketThreadNetworkBuffer.h"

#include "ScopedLock.h"

BLXWARE_API HANDLE CPacketThreadNetworkBuffer::m_hHeap = NULL;
BLXWARE_API LONG CPacketThreadNetworkBuffer::m_uNumAllocsInHeap = 0;
BLXWARE_API CComAutoCriticalSection* CPacketThreadNetworkBuffer::m_csLock = new CComAutoCriticalSection();

CPacketThreadNetworkBuffer::CPacketThreadNetworkBuffer( UINT8 NumberNetworkPackets )
{
	m_pDataBuf = NULL;
    m_threadData.reserve( NumberNetworkPackets );

	internalNew(			
		MAX_EVENT_HEADER
		, MAX_EVENT_PACKET_HEADER
		, MAX_NETWORK_BUFFER_PACKETS
        , NumberNetworkPackets
		);

    m_currentBuffer = 3; //number of buffers for each packet
    m_bufferPackets = 0;
}

CPacketThreadNetworkBuffer::~CPacketThreadNetworkBuffer()
{
	if( 0 < m_iNumBuffers )
	{
		for( SHORT i = 0 ; i < m_iNumBuffers ; i++ )
		{
			if( HeapFree( 
				m_hHeap
				, 0 //0 //HEAP_NO_SERIALIZE
				, m_pDataBuf[ i ].buf 
				)
				)
			{
				CPacketThreadNetworkBuffer::decrement_NumAllocsInHeap();
			}
			else
			{
				OutputDebugStringf( L"\n~CClientSendThreadData HeapFree of buf[%d] failed", i );
			}
		}
		if(	HeapFree( 
			m_hHeap
			, 0 //HEAP_NO_SERIALIZE
			, m_pDataBuf 
			)
			)
		{
			CPacketThreadNetworkBuffer::decrement_NumAllocsInHeap();
		}
		else
		{
			OutputDebugString( L"\n~CClientSendThreadData HeapFree of wsabuf failed" );
		}
	}
}

WSABUF* CPacketThreadNetworkBuffer::get_WSABUF( )
{
	return m_pDataBuf;
}

UINT8 CPacketThreadNetworkBuffer::get_numBufs()
{
	return m_iNumBuffers;
}

UINT8 CPacketThreadNetworkBuffer::get_currentBufferCount()
{
	return m_bufferPackets;
}

PEVENT_HEADER CPacketThreadNetworkBuffer::get_EventHeader( )
{
	if( m_iNumBuffers < CPacketThreadNetworkBuffer::DGRAM_PACKET_HEADER_SEQUENCE::PHS_EVENT_HEADER )
	{
		return NULL;
	}
	else
	{
		PEVENT_HEADER pEventHeader = reinterpret_cast< PEVENT_HEADER >(
			m_pDataBuf[ CPacketThreadNetworkBuffer::DGRAM_PACKET_HEADER_SEQUENCE::PHS_EVENT_HEADER ].buf 
			);

		::CoCreateGuid( &pEventHeader->eventId );

		return pEventHeader; 
	}
}

PEVENT_PACKET_HEADER CPacketThreadNetworkBuffer::get_EventPacketHeader( )
{
	if( m_iNumBuffers < CPacketThreadNetworkBuffer::DGRAM_PACKET_HEADER_SEQUENCE::PHS_EVENT_PACKET_HEADER )
	{
		return NULL;
	}
	else
	{
		return reinterpret_cast< PEVENT_PACKET_HEADER >(
			m_pDataBuf[ CPacketThreadNetworkBuffer::DGRAM_PACKET_HEADER_SEQUENCE::PHS_EVENT_PACKET_HEADER ].buf 
			);
	}
}

PNETWORK_BUFFER_PACKETS CPacketThreadNetworkBuffer::get_NetworkBufferPackets( )
{
	if( m_iNumBuffers < PHS_NETWORK_BUFFER_PACKETS )
	{
		return NULL;
	}
	else
	{
		PNETWORK_BUFFER_PACKETS pNetworkBufferPackets = 
			reinterpret_cast< PNETWORK_BUFFER_PACKETS >( m_pDataBuf[ CPacketThreadNetworkBuffer::DGRAM_PACKET_HEADER_SEQUENCE::PHS_NETWORK_BUFFER_PACKETS ].buf );
		return pNetworkBufferPackets;
	}
}

BOOL CPacketThreadNetworkBuffer::set_ActualPacketDataBlobLength(const USHORT Length)
{
	if( m_iNumBuffers < CPacketThreadNetworkBuffer::DGRAM_PACKET_HEADER_SEQUENCE::PHS_NETWORK_BUFFER_PACKETS )
	{
		return FALSE;
	}
	m_pDataBuf[ CPacketThreadNetworkBuffer::DGRAM_PACKET_HEADER_SEQUENCE::PHS_NETWORK_BUFFER_PACKETS ].len = Length;
	return TRUE;
}

void CPacketThreadNetworkBuffer::initializeBuffer( UINT32 length, SHORT iteration )
{
	m_pDataBuf[ iteration ].len = length;

	m_pDataBuf[ iteration ].buf = reinterpret_cast< PCHAR >( HeapAlloc( m_hHeap, HEAP_ZERO_MEMORY, length ) );
	increment_NumAllocsInHeap();

	return;
}

void CPacketThreadNetworkBuffer::internalNew( 
										UINT32 BufferLength0
										, UINT32 BufferLength1
										, UINT32 BufferLength2
                                        , UINT8 NumberNetworkPackets
										)
{
    UINT8 _maxWsaBuf = 3 + NumberNetworkPackets;
	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY, sizeof( WSABUF ) * _maxWsaBuf ) );

	initializeBuffer( BufferLength0, 0 );
	initializeBuffer( BufferLength1, 1 );
	initializeBuffer( BufferLength2, 2 );

	m_iNumBuffers = _maxWsaBuf;
}
    
BOOL CPacketThreadNetworkBuffer::PushBuffer( CPacketThreadData* ThreadData )
{
    CScopedLock _lock( &m_classLock );
    BOOL rc = FALSE;

    UINT32 _dataBlobSize = ThreadData->get_EventPacketHeader()->dataBlobSize;
    ThreadData->set_ActualPacketDataBlobLength( _dataBlobSize );
    m_pDataBuf[ m_currentBuffer++ ] = ThreadData->get_WSABUF()[ 2 ];
    get_NetworkBufferPackets()->packetInfo[ m_bufferPackets ].length = _dataBlobSize;
    get_NetworkBufferPackets()->dataSize += _dataBlobSize;
    get_EventPacketHeader()->dataBlobSize += _dataBlobSize;

    m_threadData.push_back( ThreadData );
    m_bufferPackets++;
    rc = TRUE;

    return rc;
}
    
BOOL CPacketThreadNetworkBuffer::PopBuffer( CPacketThreadData* ThreadData )
{
    CScopedLock _lock( &m_classLock );
    BOOL rc = FALSE;

    if( 0 == m_currentBuffer )
    {
        return rc;
    }

    m_pDataBuf[ --m_currentBuffer ].len = 0;
    m_pDataBuf[ m_currentBuffer ].buf = NULL;

    m_threadData.pop_back();
    m_bufferPackets--;

    rc = TRUE;

    return rc;
}
    
BOOL CPacketThreadNetworkBuffer::ClearBuffer()
{
    CScopedLock _lock( &m_classLock );
    BOOL rc = FALSE;

    if( 0 == m_currentBuffer )
    {
        return rc;
    }

    for( UINT8 i = 0 ; i < m_bufferPackets ; i++ )
    {
        m_pDataBuf[ --m_currentBuffer ].len = 0;
        m_pDataBuf[ m_currentBuffer ].buf = NULL;
    }

    m_threadData.clear();
    m_bufferPackets = 0;

    rc = TRUE;

    return rc;
}

CPacketThreadData* CPacketThreadNetworkBuffer::get_packetThreadData( UINT8 Packet )
{
    CScopedLock _lock( &m_classLock );
    return m_threadData.at( Packet );
}
