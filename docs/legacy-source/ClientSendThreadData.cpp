#pragma once

#include "ClientSendThreadData.h"


BLXWARE_API HANDLE CClientSendThreadData::m_hHeap = NULL;
BLXWARE_API LONG CClientSendThreadData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CClientSendThreadData::m_csLock = NULL;

CClientSendThreadData::CClientSendThreadData()
{
	m_pDataBuf = NULL;

	internalNew(			
		MAX_EVENT_HEADER
		, MAX_EVENT_PACKET_HEADER
		, MAX_PACKET_DATA_BLOB
		);
}

CClientSendThreadData::~CClientSendThreadData()
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
				CClientSendThreadData::decrement_NumAllocsInHeap();
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
			CClientSendThreadData::decrement_NumAllocsInHeap();
		}
		else
		{
			OutputDebugString( L"\n~CClientSendThreadData HeapFree of wsabuf failed" );
		}
	}
}

WSABUF* CClientSendThreadData::get_WSABUF( )
{
	return m_pDataBuf;
}

UINT8 CClientSendThreadData::get_numBufs()
{
	return m_iNumBuffers;
}

BOOL CClientSendThreadData::set_ActualPacketDataBlobLength(const USHORT Length)
{
	if( m_iNumBuffers < CClientSendThreadData::DGRAM_PACKET_HEADER_SEQUENCE::PHS_PACKET_DATA_BLOB )
	{
		return FALSE;
	}
	m_pDataBuf[ CClientSendThreadData::DGRAM_PACKET_HEADER_SEQUENCE::PHS_PACKET_DATA_BLOB ].len = Length;
	return TRUE;
}

PEVENT_HEADER CClientSendThreadData::get_EventHeader( )
{
	if( m_iNumBuffers < CClientSendThreadData::DGRAM_PACKET_HEADER_SEQUENCE::PHS_EVENT_HEADER )
	{
		return NULL;
	}
	else
	{
		PEVENT_HEADER pEventHeader = reinterpret_cast< PEVENT_HEADER >(
			m_pDataBuf[ CClientSendThreadData::DGRAM_PACKET_HEADER_SEQUENCE::PHS_EVENT_HEADER ].buf 
			);

		::CoCreateGuid( &pEventHeader->eventId );

		return pEventHeader; 
	}
}

PEVENT_PACKET_HEADER CClientSendThreadData::get_EventPacketHeader( )
{
	if( m_iNumBuffers < CClientSendThreadData::DGRAM_PACKET_HEADER_SEQUENCE::PHS_EVENT_PACKET_HEADER )
	{
		return NULL;
	}
	else
	{
		return reinterpret_cast< PEVENT_PACKET_HEADER >(
			m_pDataBuf[ CClientSendThreadData::DGRAM_PACKET_HEADER_SEQUENCE::PHS_EVENT_PACKET_HEADER ].buf 
			);
	}
}

PPACKET_DATA_BLOB CClientSendThreadData::get_PacketDataBlob( )
{
	if( m_iNumBuffers < PHS_PACKET_DATA_BLOB )
	{
		return NULL;
	}
	else
	{
		PPACKET_DATA_BLOB pPacketDataBlob = 
			reinterpret_cast< PPACKET_DATA_BLOB >( m_pDataBuf[ CClientSendThreadData::DGRAM_PACKET_HEADER_SEQUENCE::PHS_PACKET_DATA_BLOB ].buf );
		return pPacketDataBlob;
	}
}

void CClientSendThreadData::initializeBuffer( UINT32 length, SHORT iteration )
{
	m_pDataBuf[ iteration ].len = length;

	m_pDataBuf[ iteration ].buf = reinterpret_cast< PCHAR >( HeapAlloc( m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, length ) );
	increment_NumAllocsInHeap();

	return;
}

void CClientSendThreadData::internalNew( 
										UINT32 BufferLength0
										, UINT32 BufferLength1
										, UINT32 BufferLength2
										)
{
	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY /*HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) * 3 ) );

	initializeBuffer( BufferLength0, 0 );
	initializeBuffer( BufferLength1, 1 );
	initializeBuffer( BufferLength2, 2 );

	m_iNumBuffers = 3;
}
