#pragma once

#include "ClientAudioData.h"
#include "ClientReceiveThreadData.h"

BLXWARE_API HANDLE CClientAudioData::m_hHeap = NULL;
BLXWARE_API LONG CClientAudioData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CClientAudioData::m_csLock = NULL;

CClientAudioData::CClientAudioData( ) 
{
	InternalNew(OUT_MAX_PACKET_PAYLOAD);
}

CClientAudioData::CClientAudioData( UINT32 BufferLength )
{
	InternalNew(BufferLength);
}

CClientAudioData::~CClientAudioData()
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

WSABUF* CClientAudioData::get_WSABUF( )
{
	return m_pDataBuf;
}

PEVENT_HEADER CClientAudioData::get_EventHeader( )
{
	return reinterpret_cast< PEVENT_HEADER >(
		m_pDataBuf->buf 
		);
}
PEVENT_PACKET_HEADER CClientAudioData::get_EventPacketHeader( )
{
	return reinterpret_cast< PEVENT_PACKET_HEADER >(
		m_pDataBuf->buf 
		+ MAX_EVENT_HEADER
		);
}
PPACKET_DATA_BLOB CClientAudioData::get_PacketDataBlob( ) const
{
	return reinterpret_cast< PPACKET_DATA_BLOB >(
		m_pDataBuf->buf 
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		);
}

UINT32 CClientAudioData::get_ActualPacketSize( ) 
{
	PEVENT_PACKET_HEADER pPh =
		this->get_EventPacketHeader(); 

	UINT32 packetSize =
		MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ pPh->dataBlobSize;

	return packetSize;
}

VOID CClientAudioData::Morph( CClientReceiveThreadData* pThreadpoolData )
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

void CClientAudioData::InternalNew( UINT32 BufferLength)
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

void CClientAudioData::reallocBuffer( UINT32 length )
{
	m_pDataBuf->len = length;

	m_pDataBuf->buf = reinterpret_cast< PCHAR >( HeapReAlloc( m_hHeap, HEAP_ZERO_MEMORY, m_pDataBuf->buf , length ) );

	return;
}
