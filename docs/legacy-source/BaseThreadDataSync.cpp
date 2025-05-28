#include "BaseThreadDataSync.h"

BLXWARE_API HANDLE CBaseThreadDataSync::m_hHeap = NULL;
BLXWARE_API LONG CBaseThreadDataSync::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CBaseThreadDataSync::m_csLock = NULL;

CBaseThreadDataSync::CBaseThreadDataSync()
: m_iPktCnt( 0 )
, m_uiHeapPriority( 0 )
{
	m_pDataBuf = NULL;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
	//hEvent = WSACreateEvent();
	//internalNew( 
	//	MAX_IP_HEADER
	//	, MAX_UDP_HEADER
	//	, MAX_EVENT_HEADER
	//	, MAX_EVENT_PACKET_HEADER
	//	, MAX_PACKET_DATA_BLOB
	//	);
}

CBaseThreadDataSync::CBaseThreadDataSync( PIP_HEADER pIpHeader
										 , PUDP_HEADER pUdpHeader
										 , PEVENT_HEADER pEventHeader
										 , PEVENT_PACKET_HEADER pEventPacketHeader
										 , PPACKET_DATA_BLOB pPacketDataBlob
										 )
										 : m_iPktCnt( 5 )
										 , m_uiHeapPriority( 0 )
{
	m_pDataBuf = NULL;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
	hEvent = WSACreateEvent();
	internalRefNew(
		pIpHeader
		, pUdpHeader
		, pEventHeader
		, pEventPacketHeader
		, pPacketDataBlob
		);
}

CBaseThreadDataSync::CBaseThreadDataSync( UINT32 BufferLength )
: m_iPktCnt( 0 )
, m_uiHeapPriority( 0 )
{
	m_pDataBuf = NULL;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
	hEvent = WSACreateEvent();
	initializeBuffer( BufferLength );
}

CBaseThreadDataSync::CBaseThreadDataSync( USHORT BufferCount, DWORD Flag )
: m_iPktCnt( BufferCount )
, m_uiHeapPriority( 0 )
{
	m_pDataBuf = NULL;
    m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc( CBaseThreadDataSync::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, ( sizeof( WSABUF ) * BufferCount ) ) );
//	m_pDataBuf = reinterpret_cast< WSABUF* >( new BYTE[ sizeof( WSABUF ) * BufferCount ] );
	CBaseThreadDataSync::increment_NumAllocsInHeap();

	for( INT8 i = 0 ; i < BufferCount ; i++ ) //mxb - 06/30/2009
	{
		m_pDataBuf[ i ].buf = NULL;
		m_pDataBuf[ i ].len = 0;
    	//CBaseThreadDataSync::increment_NumAllocsInHeap();
	}
}


CBaseThreadDataSync::CBaseThreadDataSync( UINT32 BufferLength, DWORD dwN1, DWORD dwN2 )
: m_iPktCnt( 0 )
, m_uiHeapPriority( 0 )
{
	m_pDataBuf = NULL;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
	hEvent = WSACreateEvent();
	initializeBuffer( BufferLength );
}

CBaseThreadDataSync::~CBaseThreadDataSync()
{
	//		OutputDebugString( L"\n~CBaseThreadDataSync begin ----------------------------------------------" );
	if( INVALID_HANDLE_VALUE != this->hEvent )
	{
		CloseHandle( this->hEvent );
	}

	if( 0 < m_iNumBuffers )
	{
		for( SHORT i = 0 ; i < m_iNumBuffers ; i++ )
		{
			if( HeapFree( 
				CBaseThreadDataSync::m_hHeap
				, 0 //0 //HEAP_NO_SERIALIZE
				, m_pDataBuf[ i ].buf 
				)
				)
			{
				CBaseThreadDataSync::decrement_NumAllocsInHeap();
			}
			else
			{
				OutputDebugStringf( L"\n~CBaseThreadDataSync HeapFree of buf[%d] failed", i );
			}
		}
		if(	HeapFree( 
			CBaseThreadDataSync::m_hHeap
			, 0 //HEAP_NO_SERIALIZE
			, m_pDataBuf 
			)
			)
		{
			CBaseThreadDataSync::decrement_NumAllocsInHeap();
		}
		else
		{
			OutputDebugString( L"\n~CBaseThreadDataSync HeapFree of wsabuf failed" );
		}
	}
	//_CrtDumpMemoryLeaks();
	//		OutputDebugString( L"\n~CBaseThreadDataSync end ----------------------------------------------" );
}

WSABUF* CBaseThreadDataSync::get_WSABUF( )
{
	return m_pDataBuf;
}

inline VOID CBaseThreadDataSync::internalRefNew( 
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	)
{
	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc( CBaseThreadDataSync::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) * 5 ) );
	CBaseThreadDataSync::increment_NumAllocsInHeap();

	m_pDataBuf[ PHS_IP_HEADER ].len = MAX_IP_HEADER;
	m_pDataBuf[ PHS_IP_HEADER ].buf = reinterpret_cast< PCHAR >( pIpHeader );
	m_pDataBuf[ PHS_UDP_HEADER ].len = MAX_UDP_HEADER;
	m_pDataBuf[ PHS_UDP_HEADER ].buf = reinterpret_cast< PCHAR >( pUdpHeader );
	m_pDataBuf[ PHS_EVENT_HEADER ].len = MAX_EVENT_HEADER;
	m_pDataBuf[ PHS_EVENT_HEADER ].buf = reinterpret_cast< PCHAR >( pEventHeader );
	m_pDataBuf[ PHS_EVENT_PACKET_HEADER ].len = MAX_EVENT_PACKET_HEADER;
	m_pDataBuf[ PHS_EVENT_PACKET_HEADER ].buf = reinterpret_cast< PCHAR >( pEventPacketHeader );
	m_pDataBuf[ PHS_PACKET_DATA_BLOB ].len = pEventPacketHeader->dataBlobSize;
	m_pDataBuf[ PHS_PACKET_DATA_BLOB ].buf = reinterpret_cast< PCHAR >( pPacketDataBlob );

	m_iNumBuffers = 0;
}

//CBaseThreadDataSync::CBaseThreadDataSync( UINT32 BufferLength )
//: m_iPktCnt( 0 )
//, m_uiHeapPriority( 0 )
//{
//	Internal = 0;
//	InternalHigh = 0;
//	Offset = 0;
//	OffsetHigh = 0;
//	hEvent = WSACreateEvent();
//	initializeBuffer( BufferLength, 0 );
//}

VOID CBaseThreadDataSync::internalNew( 
									  UINT32 BufferLength0
									  , UINT32 BufferLength1
									  , UINT32 BufferLength2
									  , UINT32 BufferLength3
									  , UINT32 BufferLength4
									  )
{
	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc( CBaseThreadDataSync::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) * 5 ) );
	CBaseThreadDataSync::increment_NumAllocsInHeap();

	initializeBuffer( BufferLength0, 0 );
	initializeBuffer( BufferLength1, 1 );
	initializeBuffer( BufferLength2, 2 );
	initializeBuffer( BufferLength3, 3 );
	initializeBuffer( BufferLength4, 4 );
	m_iNumBuffers = 5;
};

VOID CBaseThreadDataSync::initializeBuffer( UINT32 length, SHORT iteration )
{
	m_pDataBuf[ iteration ].len = length;

	m_pDataBuf[ iteration ].buf = reinterpret_cast< PCHAR >( HeapAlloc( CBaseThreadDataSync::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, length ) );
	CBaseThreadDataSync::increment_NumAllocsInHeap();

	m_iNumBuffers = 1;
	return;
}

VOID CBaseThreadDataSync::initializeBuffer( UINT32 length )
{
	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc( CBaseThreadDataSync::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) ) );
	CBaseThreadDataSync::increment_NumAllocsInHeap();

	m_pDataBuf->len = length;
	m_pDataBuf->buf = reinterpret_cast< PCHAR >( HeapAlloc( CBaseThreadDataSync::m_hHeap, HEAP_ZERO_MEMORY /*| HEAP_NO_SERIALIZE*/, length ) );
	CBaseThreadDataSync::increment_NumAllocsInHeap();

	m_iNumBuffers = 1;
	return;
};
