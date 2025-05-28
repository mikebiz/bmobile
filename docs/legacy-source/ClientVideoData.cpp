#pragma once

#include "ClientVideoData.h"
#include "ClientReceiveThreadData.h"

BLXWARE_API HANDLE CClientVideoData::m_hHeap = NULL;
BLXWARE_API LONG CClientVideoData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CClientVideoData::m_csLock = NULL;

CClientVideoData::CClientVideoData( ) 
{
	InternalNew(OUT_MAX_PACKET_PAYLOAD);
}

CClientVideoData::CClientVideoData( UINT32 BufferLength )
{
	InternalNew(BufferLength);
}

CClientVideoData::~CClientVideoData()
{
#ifdef _VISTA_
	if( NULL != m_pWorkItem )
	{
		WaitForThreadpoolWorkCallbacks(
			m_pWorkItem
			, FALSE
			);
		CloseThreadpoolWork( m_pWorkItem );
		m_pWorkItem = NULL;
	}
#endif

	if (m_pDataBuf != NULL)
	{
		if (m_pDataBuf->buf != NULL)
		{
			if(	HeapFree( 
				m_hHeap
				, 0 //HEAP_NO_SERIALIZE
				, m_pDataBuf->buf
				)
				)
			{
				decrement_NumAllocsInHeap();
			}
			else
			{
				OutputDebugString( L"\n~CClientAudioData HeapFree of wsabuf failed" );
			}
		}

		if(	HeapFree( 
			m_hHeap
			, 0 //HEAP_NO_SERIALIZE
			, m_pDataBuf
			)
			)
		{
			decrement_NumAllocsInHeap();
		}
		else
		{
			OutputDebugString( L"\n~CClientAudioData HeapFree of wsabuf failed" );
		}
	}
}

WSABUF* CClientVideoData::get_WSABUF( )
{
	return m_pDataBuf;
}

PEVENT_HEADER CClientVideoData::get_EventHeader( )
{
	return reinterpret_cast< PEVENT_HEADER >(
		m_pDataBuf->buf 
		);
}
PEVENT_PACKET_HEADER CClientVideoData::get_EventPacketHeader( )
{
	return reinterpret_cast< PEVENT_PACKET_HEADER >(
		m_pDataBuf->buf 
		+ MAX_EVENT_HEADER
		);
}
PPACKET_DATA_BLOB CClientVideoData::get_PacketDataBlob( ) const
{
	return reinterpret_cast< PPACKET_DATA_BLOB >(
		m_pDataBuf->buf 
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		);
}

UINT32 CClientVideoData::get_ActualPacketSize( ) 
{
	PEVENT_PACKET_HEADER pPh =
		this->get_EventPacketHeader(); 

	UINT32 packetSize =
		MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ pPh->dataBlobSize;

	return packetSize;
}

VOID CClientVideoData::Morph( CClientReceiveThreadData* pThreadpoolData )
{
#ifdef _VISTA_
	m_pWorkItem = NULL;
#endif
	initializeWsaOverlapped(this, true);

	WSABUF* pBuf = pThreadpoolData->get_WSABUF();

	reallocBuffer( pThreadpoolData->get_ActualPacketSize() );

	CopyMemory(
		m_pDataBuf->buf
		, pBuf->buf 
		, m_pDataBuf->len
		);

	m_pBasePin = pThreadpoolData->m_pBasePin;
	return;
}

VOID CClientVideoData::MorphPackets(  PEVENT_HEADER pEventHeader, PEVENT_PACKET_HEADER pEventPacketHeader, PPACKET_DATA_BLOB pDataBlob, CClientReceiveThreadData* pThreadpoolData )
{
#ifdef _VISTA_
	m_pWorkItem = NULL;
#endif
	initializeWsaOverlapped(this, true);

	//WSABUF* pBuf = pThreadpoolData->get_WSABUF();

    UINT32 actualBufferSize = 
        MAX_EVENT_HEADER
        + MAX_EVENT_PACKET_HEADER
        + pEventPacketHeader->dataBlobSize;

	reallocBuffer( actualBufferSize );

    ::CopyMemory(
        get_EventHeader()
        , pEventHeader
        , MAX_EVENT_HEADER
        );

    ::CopyMemory(
        get_EventPacketHeader()
        , pEventPacketHeader
        , MAX_EVENT_PACKET_HEADER
        );

    ::CopyMemory(
        get_PacketDataBlob()
        , pDataBlob
        , pEventPacketHeader->dataBlobSize
        );

	m_pBasePin = pThreadpoolData;
	return;
}

void CClientVideoData::InternalNew( UINT32 BufferLength)
{
#ifdef _VISTA_
	m_pWorkItem = NULL;
#endif


	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc( m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) ) );
	increment_NumAllocsInHeap();

	m_pDataBuf->len = BufferLength;

	m_pDataBuf->buf = reinterpret_cast< PCHAR >( HeapAlloc( m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, BufferLength ) );
	increment_NumAllocsInHeap();
}

void CClientVideoData::reallocBuffer( UINT32 length )
{
	m_pDataBuf->len = length;

	m_pDataBuf->buf = reinterpret_cast< PCHAR >( HeapReAlloc( m_hHeap, HEAP_ZERO_MEMORY, m_pDataBuf->buf , length ) );

	return;
}
