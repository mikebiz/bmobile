
//#define BLXWARE_API __declspec(dllexport)

#include "PacketInfo.h"
#include "PriorityQueueSync.h"
#include "SyncQueue.h"
#include "operators.h"


USHORT checksum(USHORT *buffer, int size);
void ComputeUdpPseudoHeaderChecksumV4(
	void *iphdr,
	UDP_HEADER *udphdr,
	char *payload,
	int payloadlen
);

#pragma data_seg (".shared")


BLXWARE_API HANDLE CClientSendThreadData::m_hHeap = NULL;
BLXWARE_API LONG CClientSendThreadData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CClientSendThreadData::m_csLock = NULL;

BLXWARE_API HANDLE CClientReceiveThreadData::m_hHeap = NULL;
BLXWARE_API LONG CClientReceiveThreadData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CClientReceiveThreadData::m_csLock = NULL;

BLXWARE_API HANDLE CClientAudioData::m_hHeap = NULL;
BLXWARE_API LONG CClientAudioData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CClientAudioData::m_csLock = NULL;

BLXWARE_API HANDLE CClientVideoData::m_hHeap = NULL;
BLXWARE_API LONG CClientVideoData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CClientVideoData::m_csLock = NULL;

BLXWARE_API HANDLE CClientManagementEventData::m_hHeap = NULL;
BLXWARE_API LONG CClientManagementEventData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CClientManagementEventData::m_csLock = NULL;


typedef Blxware::SyncQueue< CClientReceiveThreadData* > QClientReceiveThreadData;
typedef QClientReceiveThreadData* PQClientReceiveThreadData;
PQClientReceiveThreadData				g_pQClientReceiveThreadData;

typedef Blxware::SyncQueue< CClientAudioData* > QClientAudioData;
typedef QClientAudioData* PQClientAudioData;
PQClientAudioData				g_pQClientAudioData;

typedef Blxware::SyncQueue< CClientVideoData* > QClientVideoData;
typedef QClientVideoData* PQClientVideoData;
PQClientVideoData				g_pQClientVideoData;

typedef Blxware::SyncQueue< CClientManagementEventData* > QManagementEventData;
typedef QManagementEventData* PQManagementEventData;
PQManagementEventData				g_pQSyncManagementEventData;

//
//BLXWARE_API HANDLE CBaseThreadData::m_hHeap = NULL;
//BLXWARE_API LONG CBaseThreadData::m_uNumAllocsInHeap = 0;

BLXWARE_API HANDLE CBaseThreadDataSync::m_hHeap = NULL;
BLXWARE_API LONG CBaseThreadDataSync::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CBaseThreadDataSync::m_csLock = NULL;


volatile LONG g_uiCurrentVideoThread = 0;
volatile LONG g_uiMaxVideoThread = MAX_CLIENT_THREADS_READER;
volatile LONG g_uiOldestVideoThread = 0;

volatile LONG g_uiCurrentServerSendThread = 0;
volatile LONG g_uiMaxServerSendThread = MAX_VIDEO_IOCP_SENDTO_THREADS;
volatile LONG g_uiOldestServerSendThread = 0;

volatile LONG g_uiCurrentAudioThread = 0;
volatile LONG g_uiMaxAudioThread  = MAX_CLIENT_THREADS_READER;
volatile LONG g_uiOldestAudioThread = 0;

volatile LONG g_uiCurrentManagementThread = 0;
volatile LONG g_uiMaxManagementThread = MAX_CLIENT_THREADS_READER;
volatile LONG g_uiOldestManagementThread = 0;

volatile LONG g_uiCurrentServerVideoThread = 0;
volatile LONG g_uiMaxServerVideoThread = MAX_CLIENT_THREADS_READER;
volatile LONG g_uiOldestServerVideoThread = 0;

//#pragma data_seg(".shared")

WORD g_wVersionRequested;
WSADATA g_WsaData;

SOCKET						g_SocketMultiMedia = INVALID_SOCKET;

HANDLE						g_hSocketMultMediaEvent;
SOCKADDR_IN					g_SocketAddrCliMultiMedia;
SOCKADDR_IN					g_SocketAddrSrvMultiMedia;

CString						g_szMACAddr;
CString						g_szConfDns;


#ifdef _VISTA_
SRWLOCK g_srwClientReaderThreadData;
SRWLOCK g_srwVideoThreadData;
SRWLOCK g_srwServerSendThreadData;
SRWLOCK g_srwAudioThreadData;
SRWLOCK g_srwManagementThreadData;
SRWLOCK g_srwClientReceiveThreadData;
SRWLOCK g_srwMediaQueue;
SRWLOCK g_srwipcVideoQueue;
SRWLOCK g_srwipcAudioQueue;
CRITICAL_SECTION g_csClientReceiveThreadData;
#else
CRITICAL_SECTION g_srwClientReaderThreadData;
CRITICAL_SECTION g_srwVideoThreadData;
CRITICAL_SECTION g_srwServerSendThreadData;
CRITICAL_SECTION g_srwAudioThreadData;
CRITICAL_SECTION g_srwManagementThreadData;
CRITICAL_SECTION g_srwClientReceiveThreadData;
CRITICAL_SECTION g_srwMediaQueue;
CRITICAL_SECTION g_srwipcVideoQueue;
CRITICAL_SECTION g_srwipcAudioQueue;
#endif

//#pragma data_seg()

CSimpleArray<CServerPacketThreadData*> g_pServerThreadData;
CSimpleArray< g_serverThreadArray > g_threadsServerArray;

CSimpleArray<CServerSendThreadData*> g_pServerSendThreadData;
CSimpleArray< g_serverSendThreadArray > g_threadsServerSendThreadArray;

CSimpleArray<CPacketThreadData*> g_pVideoThreadData;
CSimpleArray< g_videoThreadArray > g_threadsVideoArray;

CSimpleArray<CPacketThreadData*> g_pAudioThreadData;
CSimpleArray< g_audioThreadArray > g_threadsAudioArray;

CSimpleArray<CPacketThreadData*> g_pMgmtThreadData;
CSimpleArray< g_mgmtThreadArray > g_threadsMgmtArray;

VOID CALLBACK SubmitCompleteRoutine (
	  IN DWORD dwError
	  , IN DWORD cbTransferred
	  , IN LPOVERLAPPED lpOverlapped
	)
{

	CPacketThreadData* pThreadData = ( CPacketThreadData* )lpOverlapped;
	WSASetEvent( pThreadData->hEvent );
	return;
}


BOOL InitializeSRWLocks()
{
	BOOL rc = TRUE;

#ifdef _VISTA_
	InitializeSRWLock( &g_srwClientReaderThreadData );
	InitializeSRWLock( &g_srwVideoThreadData );
	InitializeSRWLock( &g_srwServerSendThreadData );
	InitializeSRWLock( &g_srwAudioThreadData );
	InitializeSRWLock( &g_srwVideoThreadData );
	InitializeSRWLock( &g_srwClientReceiveThreadData );
	InitializeSRWLock( &g_srwMediaQueue );
	InitializeSRWLock( &g_srwipcVideoQueue );
	InitializeSRWLock( &g_srwipcAudioQueue );
	InitializeCriticalSectionAndSpinCount( &g_csClientReceiveThreadData, 1 );
#else
	InitializeCriticalSectionAndSpinCount( &g_srwClientReaderThreadData, 2 );
	InitializeCriticalSectionAndSpinCount( &g_srwVideoThreadData, 2 );
	InitializeCriticalSectionAndSpinCount( &g_srwServerSendThreadData, 2 );
	InitializeCriticalSectionAndSpinCount( &g_srwAudioThreadData, 2 );
	InitializeCriticalSectionAndSpinCount( &g_srwManagementThreadData, 2 );
	InitializeCriticalSectionAndSpinCount( &g_srwClientReceiveThreadData, 2 );
	InitializeCriticalSectionAndSpinCount( &g_srwMediaQueue, 2 );
	InitializeCriticalSectionAndSpinCount( &g_srwipcVideoQueue, 2 );
	InitializeCriticalSectionAndSpinCount( &g_srwipcAudioQueue, 2 );
#endif
	return rc;
}



BOOL AllocateClientSendBuffers()
{
	BOOL rc = TRUE;

	CPacketThreadData* pOverlapped = NULL;
	for( INT32 i = 0 ; i < MAX_CLIENT_THREADS_READER ; i++ )
	{
		pOverlapped =
			new CPacketThreadData( );
		g_pVideoThreadData.Add( pOverlapped );

		pOverlapped =
			new CPacketThreadData( );
		g_pAudioThreadData.Add( pOverlapped );

		pOverlapped =
			new CPacketThreadData( );
		g_pMgmtThreadData.Add( pOverlapped );
	}
	return rc;
}

BOOL DeallocateClientSendBuffers()
{
	BOOL rc = TRUE;

	CPacketThreadData* pOverlapped = NULL;
	for( INT32 i = 0 ; i < MAX_CLIENT_THREADS_READER ; i++ )
	{
		pOverlapped =  g_pVideoThreadData[ i ];
		CloseHandle( pOverlapped->hEvent );
		delete pOverlapped;

		pOverlapped =  g_pAudioThreadData[ i ];
		CloseHandle( pOverlapped->hEvent );
		delete pOverlapped;

		pOverlapped =  g_pMgmtThreadData[ i ];
		CloseHandle( pOverlapped->hEvent );
		delete pOverlapped;

		g_pVideoThreadData.RemoveAll();
		g_pAudioThreadData.RemoveAll();
		g_pMgmtThreadData.RemoveAll();
	}
	return rc;
}

BOOL AllocateServerSendBuffers()
{
	BOOL rc = TRUE;

	//CServerSendThreadData* pOverlapped = NULL;
	//for( INT32 i = 0 ; i < MAX_VIDEO_IOCP_SENDTO_THREADS ; i++ )
	//{
	//	pOverlapped =
	//		new CServerSendThreadData( );
	//	pOverlapped->Internal = 0;
	//	pOverlapped->InternalHigh = 0;
	//	pOverlapped->Offset = 0;
	//	pOverlapped->OffsetHigh = 0;
	//	pOverlapped->hEvent = WSACreateEvent();
	//	g_pServerSendThreadData.Add( pOverlapped );
	//}
	return rc;
}

BOOL DeallocateServerSendBuffers()
{
	BOOL rc = TRUE;

	//CServerSendThreadData* pOverlapped = NULL;
	//for( INT32 i = 0 ; i < MAX_VIDEO_IOCP_SENDTO_THREADS ; i++ )
	//{
	//	pOverlapped =  g_pServerSendThreadData[ i ];
	//	CloseHandle( pOverlapped->hEvent );
	//	delete pOverlapped;

	//	g_pServerSendThreadData.RemoveAll();
	//}
	return rc;
}



CServerSendThreadData*
GetCurrentServerSendThreadData( void )
{
	CServerSendThreadData* pThreadCurrent = NULL;
	CServerSendThreadData* pThreadOldest = NULL;
	LONG _uiCurrentServerSendVideoThread = 0;

#ifdef _VISTA_
	AcquireSRWLockExclusive( &g_srwServerSendThreadData );
#else
	EnterCriticalSection( &g_srwServerSendThreadData );
#endif
	_uiCurrentServerSendVideoThread = g_uiCurrentServerSendThread;
	g_uiCurrentServerSendThread++;

#ifdef _VISTA_
	ReleaseSRWLockExclusive( &g_srwServerSendThreadData );
#else
	LeaveCriticalSection( &g_srwServerSendThreadData );
#endif

	pThreadCurrent = g_pServerSendThreadData[ _uiCurrentServerSendVideoThread % g_uiMaxServerSendThread ];
	pThreadOldest = g_pServerSendThreadData[ ( _uiCurrentServerSendVideoThread + 1 ) % g_uiMaxServerSendThread ];
	WSAResetEvent( pThreadCurrent->hEvent );

	if( _uiCurrentServerSendVideoThread > MAX_VIDEO_IOCP_SENDTO_THREADS - 2 )
	{
		WSAWaitForMultipleEvents( 
			1
			, &pThreadOldest->hEvent
			, FALSE
			, WSA_INFINITE
			, TRUE 
			);
		WSAResetEvent( pThreadOldest->hEvent );
	}

	return pThreadCurrent;
}


/**/
CPacketThreadData*
GetCurrentVideoThreadData( void )
{
	CPacketThreadData* pThreadCurrent = NULL;
	CPacketThreadData* pThreadOldest = NULL;
	LONG _uiCurrentVideoThread = 0;

#ifdef _VISTA_
	AcquireSRWLockExclusive( &g_srwVideoThreadData );
#else
	EnterCriticalSection( &g_srwVideoThreadData );
#endif
	_uiCurrentVideoThread = g_uiCurrentVideoThread;
	g_uiCurrentVideoThread++;

#ifdef _VISTA_
	ReleaseSRWLockExclusive( &g_srwVideoThreadData );
#else
	LeaveCriticalSection( &g_srwVideoThreadData );
#endif

	pThreadCurrent = g_pVideoThreadData[ _uiCurrentVideoThread % g_uiMaxVideoThread ];
	pThreadOldest = g_pVideoThreadData[ ( _uiCurrentVideoThread + 1 ) % g_uiMaxVideoThread ];
	WSAResetEvent( pThreadCurrent->hEvent );

	::SecureZeroMemory(
		pThreadCurrent->get_PacketDataBlob()
		, MAX_PACKET_DATA_BLOB
		);

	if( _uiCurrentVideoThread > MAX_CLIENT_THREADS_READER - 2 )
	{
		WSAWaitForMultipleEvents( 
			1
			, &pThreadOldest->hEvent
			, FALSE
			, WSA_INFINITE
			, TRUE 
			);
		WSAResetEvent( pThreadOldest->hEvent );
	}

	return pThreadCurrent;
}

CPacketThreadData*
GetCurrentAudioThreadData( void )
{
	CPacketThreadData* pThreadCurrent = NULL;
	CPacketThreadData* pThreadOldest = NULL;
	LONG _uiCurrentAudioThread = 0;

#ifdef _VISTA_
	AcquireSRWLockExclusive( &g_srwAudioThreadData );
#else
	EnterCriticalSection( &g_srwAudioThreadData );
#endif
	_uiCurrentAudioThread = g_uiCurrentAudioThread;
	g_uiCurrentAudioThread++;

#ifdef _VISTA_
	ReleaseSRWLockExclusive( &g_srwAudioThreadData );
#else
	LeaveCriticalSection( &g_srwAudioThreadData );
#endif

	pThreadCurrent = g_pAudioThreadData[ _uiCurrentAudioThread % g_uiMaxAudioThread ];
	pThreadOldest = g_pAudioThreadData[ ( _uiCurrentAudioThread + 1 ) % g_uiMaxAudioThread ];
	WSAResetEvent( pThreadCurrent->hEvent );

	::SecureZeroMemory(
		pThreadCurrent->get_PacketDataBlob()
		, MAX_PACKET_DATA_BLOB
		);

	if( _uiCurrentAudioThread > MAX_CLIENT_THREADS_READER - 2 )
	{
		WSAWaitForMultipleEvents( 
			1
			, &pThreadOldest->hEvent
			, FALSE
			, WSA_INFINITE
			, TRUE 
			);
		WSAResetEvent( pThreadOldest->hEvent );
	}

	return pThreadCurrent;
}

CPacketThreadData*
GetCurrentManagementThreadData( void )
{
	CPacketThreadData* pThreadCurrent;
	CPacketThreadData* pThreadOldest;

#ifdef _VISTA_
	AcquireSRWLockExclusive( &g_srwManagementThreadData );
#else
	EnterCriticalSection( &g_srwManagementThreadData );
#endif

	pThreadCurrent = g_pMgmtThreadData[ g_uiCurrentManagementThread % MAX_CLIENT_THREADS_READER3 ];
	
#ifdef _VISTA_
	ReleaseSRWLockExclusive( &g_srwManagementThreadData );
#else
	LeaveCriticalSection( &g_srwManagementThreadData );
#endif
	WSAResetEvent( pThreadCurrent->hEvent );

	::SecureZeroMemory(
		pThreadCurrent->get_PacketDataBlob()
		, sizeof( MAX_PACKET_DATA_BLOB )
		);

	if( g_uiCurrentManagementThread > MAX_CLIENT_THREADS_READER3 - 2 )
	{
#ifdef _VISTA_
		AcquireSRWLockExclusive( &g_srwManagementThreadData );
#else
		EnterCriticalSection( &g_srwManagementThreadData );
#endif
		pThreadOldest = g_pMgmtThreadData[ ( g_uiCurrentManagementThread + 1 ) % MAX_CLIENT_THREADS_READER3 ];
		
#ifdef _VISTA_
		ReleaseSRWLockExclusive( &g_srwManagementThreadData );
#else
		LeaveCriticalSection( &g_srwManagementThreadData );
#endif
		WSAWaitForMultipleEvents( 
			1
			, &pThreadOldest->hEvent
			, FALSE
			, WSA_INFINITE
			, TRUE 
			);
		WSAResetEvent( pThreadOldest->hEvent );
	}
	InterlockedIncrement( &g_uiCurrentManagementThread );

	return pThreadCurrent;
}



//
CServerPacketThreadData*
GetCurrentServerVideoThreadData(
		PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PPACKET_DATA_BLOB pPacketDataBlob
		)
{
	CServerPacketThreadData* pThreadCurrent = NULL;
	CServerPacketThreadData* pThreadOldest = NULL;
	LONG _uiCurrentServerVideoThread = 0;

#ifdef _VISTA_
	AcquireSRWLockExclusive( &g_srwVideoThreadData );
#else
	EnterCriticalSection( &g_srwVideoThreadData );
#endif	
	pThreadCurrent = 
		reinterpret_cast< CServerPacketThreadData* > (
			g_pVideoThreadData[ g_uiCurrentServerVideoThread % g_uiMaxServerVideoThread ] );
	pThreadOldest = reinterpret_cast< CServerPacketThreadData* > (
		g_pVideoThreadData[ ( g_uiCurrentServerVideoThread + 1 ) % g_uiMaxServerVideoThread ] );
	
	_uiCurrentServerVideoThread = g_uiCurrentServerVideoThread;
	g_uiCurrentServerVideoThread++;

#ifdef _VISTA_
	ReleaseSRWLockExclusive( &g_srwVideoThreadData );
#else
	LeaveCriticalSection( &g_srwVideoThreadData );
#endif

	pThreadCurrent->internalRefNew(
		pIpHeader
		, pUdpHeader
		, pEventHeader
		, pEventPacketHeader
		, pPacketDataBlob
		);

	if( _uiCurrentServerVideoThread > MAX_VIDEO_WSASENT_BUFFER_SIZE - 2 )
	{

		WSAWaitForMultipleEvents( 
			1
			, &pThreadOldest->hEvent
			, FALSE
			, WSA_INFINITE
			, TRUE 
			);
		WSAResetEvent( pThreadOldest->hEvent );
	}

	return pThreadCurrent;
}

WSABUF* CreateNetworkBuffer( PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							)
{
	WSABUF* networkBuffer = ( WSABUF* )new CHAR[ sizeof( WSABUF ) * PHS_MAX ];
	
	networkBuffer[ PHS_IP_HEADER ].len = MAX_IP_HEADER;
	networkBuffer[ PHS_IP_HEADER ].buf = reinterpret_cast< PCHAR >( pIpHeader );

	networkBuffer[ PHS_UDP_HEADER ].len = MAX_UDP_HEADER;
	networkBuffer[ PHS_UDP_HEADER ].buf = reinterpret_cast< PCHAR >( pUdpHeader );

	networkBuffer[ PHS_EVENT_HEADER ].len = MAX_EVENT_HEADER;
	networkBuffer[ PHS_EVENT_HEADER ].buf = reinterpret_cast< PCHAR >( pEventHeader );

	networkBuffer[ PHS_EVENT_PACKET_HEADER ].len = MAX_EVENT_PACKET_HEADER;
	networkBuffer[ PHS_EVENT_PACKET_HEADER ].buf = reinterpret_cast< PCHAR >( pEventPacketHeader );

	networkBuffer[ PHS_PACKET_DATA_BLOB ].len = pEventPacketHeader->dataBlobSize;
	networkBuffer[ PHS_PACKET_DATA_BLOB ].buf = reinterpret_cast< PCHAR >( pPacketDataBlob );

	return networkBuffer;
}


VOID FormatMACAddress(BYTE Address[], CString* frmtMACAddress)
{
	CHAR t[ MACADDRESS_LENGTH ];
	::SecureZeroMemory(
		&t
		, MACADDRESS_LENGTH
		);

	int len = sprintf_s( t
		, "%02x_%02x_%02x_%02x_%02x_%02x"
		, Address[0]
		, Address[1]
		, Address[2]
		, Address[3]
		, Address[4]
		, Address[5]
		);
	frmtMACAddress->Empty();
	frmtMACAddress->SetString( CA2W( t ) );
}

CString GetMACAddress( VOID )
{
	IP_ADAPTER_INFO AdapterInfo[16];      
	DWORD dwBufLen = sizeof(AdapterInfo); 

	DWORD dwStatus = GetAdaptersInfo(     
		AdapterInfo
		, &dwBufLen
		);                 

	PIP_ADAPTER_INFO pAdapterInfo;
	pAdapterInfo = &AdapterInfo[ 0 ]; 
	//for( int i = 0 ; i < 16 ; i++ )
	//{
	//	if( NULL != AdapterInfo[ i ].GatewayList.Context )
	//	{
	//		pAdapterInfo = &AdapterInfo[ i ]; 
	//		break;
	//	}
	//}
	CString s;
	FormatMACAddress(pAdapterInfo->Address, &s);
	return s;
}


HRESULT InitializeClientSendBuffers()
{
	HRESULT hr = S_OK;
	if( !AllocateClientSendBuffers() )
	{
		hr = E_FAIL;
	}
	return hr;
}

HRESULT InitializeClientReceiveBuffers()
{
	HRESULT hr = S_OK;
	//if( !AllocateClientReceiveBuffers() )
	//{
	//	hr = E_FAIL;
	//}
	if( !AllocateClientReceiveBuffers() )
	{
		hr = E_FAIL;
	}
	return hr;
}

VOID CloseClientSend()
{
	if (g_SocketMultiMedia != INVALID_SOCKET)
	{
		closesocket(g_SocketMultiMedia);
	}
}


BOOL InitializeClientSend( PCHAR ConfDns )
{
	BOOL rc = TRUE;

	g_szConfDns = ConfDns;

	InitializeSendWinsock();
	InitializeSRWLocks();
	InitializeClientSendBuffers();

	g_szMACAddr = GetMACAddress();

	return rc;
}


BOOL InitializeClientReceive( PCHAR ConfDns )
{
	BOOL rc = TRUE;

	g_szConfDns = ConfDns;

//	InitializeReceiveWinsock();
	InitializeSRWLocks();
	InitializeClientReceiveBuffers();

	g_szMACAddr = GetMACAddress();

	return rc;
}


HRESULT InitializeSendWinsock()
{
	HRESULT hr = S_OK;
	INT32 iLastError = 0;

	g_wVersionRequested = MAKEWORD( 2, 2 );

	iLastError = WSAStartup( 
		g_wVersionRequested
		, &g_WsaData
		);
	if( 0 != iLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	g_SocketMultiMedia = WSASocket( 
		AF_INET
		, SOCK_DGRAM  //SOCK_RAW
		, IPPROTO_UDP 
		, NULL
		, 0
		, WSA_FLAG_OVERLAPPED
		);
	iLastError = WSAGetLastError();
	if( 0 != iLastError )
	{
		hr = E_FAIL;
		return hr;
	}


	BOOL fBroadcast = FALSE;
	INT32 zero = 0;//32768;//SO_MAX_MSG_SIZE;
	ULONG fReuse = TRUE;
	ULONG fExUse = FALSE;
	BOOL fHeaderIncluded = TRUE;

	setsockopt( 
		g_SocketMultiMedia
		, SOL_SOCKET
		, SO_BROADCAST
		, ( CHAR* ) &fBroadcast
		, sizeof( BOOL )
		);
	iLastError = WSAGetLastError();
	if( 0 != iLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	setsockopt(
		g_SocketMultiMedia
		, SOL_SOCKET
		, SO_SNDBUF
		, ( PCHAR )&zero
		, sizeof( INT32 )
		);
	iLastError = WSAGetLastError();
	if( 0 != iLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	INT32 _sndBufSize = sizeof( INT32 );
	INT32 t;
	getsockopt(
		g_SocketMultiMedia
		, SOL_SOCKET
		, SO_SNDBUF
		, ( PCHAR )&t
		, &_sndBufSize
		);
	iLastError = WSAGetLastError();
	ATLTRACE2( 
		atlTraceGeneral
		, 0
		, L" WSAGetLastError = %d "
		, iLastError
		);

	ATLASSERT( 0 == iLastError );
	ATLTRACE2( 
		atlTraceGeneral
		, 0
		, L" SO_SNDBUF = %d "
		, t
		);
	if( 0 != iLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	setsockopt(
		g_SocketMultiMedia
		, SOL_SOCKET
		, SO_REUSEADDR
		, ( PCHAR )&fReuse
		, sizeof( ULONG )
		);
	iLastError = WSAGetLastError();
	if( 0 != iLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	setsockopt(
		g_SocketMultiMedia
		, SOL_SOCKET
		, SO_EXCLUSIVEADDRUSE
		, ( PCHAR )&fExUse
		, sizeof( BOOL )
		);
	iLastError = WSAGetLastError();
	if( 0 != iLastError )
	{
		hr = E_FAIL;
		return hr;
	}


	hostent* thisHost = NULL;
	thisHost = ::gethostbyname( NULL );
	INT32 iEntries = sizeof(*thisHost) % sizeof( hostent );
	if( !thisHost )
	{
		iLastError = WSAGetLastError();
		hr = E_FAIL;
		return hr;
	}

	PMIB_UDPTABLE udpTable = ( PMIB_UDPTABLE )new BYTE[ sizeof( MIB_UDPTABLE ) * 250 ];
	ULONG dwSize = sizeof( MIB_UDPTABLE ) * 250;
	BOOL bOrder = TRUE;
	ULONG ulAf = AF_INET;
	UDP_TABLE_CLASS TableClass = UDP_TABLE_BASIC;
	DWORD dwRc =
		GetExtendedUdpTable(
			udpTable,
			&dwSize,
			bOrder,
			ulAf,
			TableClass,
			0UL
			);

	if( ERROR_INVALID_PARAMETER == dwRc )
	{
		hr = E_FAIL;
		return hr;
	}
	else if( ERROR_INSUFFICIENT_BUFFER == dwRc )
	{
		hr = E_FAIL;
		return hr;
	}
	USHORT dwPort[ 2 ];
	INT8 iPort = 0;

	for( INT32 i = 0 ; i < udpTable->dwNumEntries ; i++ )
	{
		if( 0 < udpTable->table[ i ].dwLocalAddr )
		{
			if( 24000 < udpTable->table[ i ].dwLocalPort )
			{
				dwPort[ iPort ] = udpTable->table[ i ].dwLocalPort;
				iPort++;
				if( 1 < iPort )
				{
					break;
				}
			}
		}
	}

	g_SocketAddrCliMultiMedia.sin_family = AF_INET;
	g_SocketAddrCliMultiMedia.sin_addr.s_addr = inet_addr( inet_ntoa (*(struct in_addr *)*thisHost->h_addr_list) );
	g_SocketAddrCliMultiMedia.sin_port = htons(  dwPort[ 1 ] );

	bind(
		g_SocketMultiMedia
		, ( const sockaddr* )&g_SocketAddrCliMultiMedia 
		, sizeof( SOCKADDR_IN )
		);
	iLastError = WSAGetLastError();
	if( 0 != iLastError )
	{
		hr = E_FAIL;
		return hr;
	}
	g_hSocketMultMediaEvent = WSACreateEvent();
	INT rc = WSAEventSelect(
	g_SocketMultiMedia
	, g_hSocketMultMediaEvent
	, FD_WRITE
	);

	BOOL fBioc = BindIoCompletionCallback(
	( HANDLE )g_SocketMultiMedia
	, SubmitCompleteRoutine
	, 0
	);

	thisHost = ::gethostbyname( CW2A( g_szConfDns.GetString() ) );
	if( !thisHost )
	{
		iLastError = WSAGetLastError();
	}

	g_SocketAddrSrvMultiMedia.sin_family = AF_INET;
	g_SocketAddrSrvMultiMedia.sin_addr.s_addr = inet_addr( inet_ntoa (*(struct in_addr *)*thisHost->h_addr_list) );
	g_SocketAddrSrvMultiMedia.sin_port = htons( 23499 );

	return hr;
}



BOOL PostClientVideoEvent(
	CPacketThreadData* pBaseThreadPacket 
	)
{
	BOOL fRc = TRUE;

	CPacketThreadData* pThreadData = 
		reinterpret_cast< CPacketThreadData* >( pBaseThreadPacket );
	
	INT32 iLastError = 0;

	PEVENT_HEADER pEventHeader = 
		pThreadData->get_EventHeader();

	strcpy_s(
		pEventHeader->eventMacAddress 
		, _countof( pEventHeader->eventMacAddress )
		, CW2A( g_szMACAddr.GetString() )
		);

	DWORD dwBytesSent = 0
		, dwFlags = 0
		;

	pThreadData->set_ActualPacketDataBlobLength( pThreadData->get_EventPacketHeader()->dataBlobSize );
	WSABUF* w = pThreadData->get_WSABUF();

	WSAResetEvent( pThreadData->hEvent );
//	pThreadData->hEvent = ( HANDLE )( ( DWORD_PTR ) pThreadData->hEvent | 1 );

	WSASendTo(
		g_SocketMultiMedia
		, pThreadData->get_WSABUF()
		, pThreadData->get_numBufs()
		, &dwBytesSent
		, dwFlags
		, ( SOCKADDR* ) &g_SocketAddrSrvMultiMedia
		, ( int )sizeof( SOCKADDR_IN )
		, pThreadData
		, NULL//SubmitCompleteRoutine
		);


//	pThreadData->hEvent = ( HANDLE )( ( DWORD_PTR ) pThreadData->hEvent & ~1 );
//	WSASetEvent( pThreadData->hEvent );

	iLastError = WSAGetLastError();
	//if( NO_ERROR != iLastError )
	//{
	//	fRc = FALSE;
	//}

PostClientVideoEvent_Exit:
	return fRc;
}


BOOL PostClientAudioEvent(
	CPacketThreadData* pBaseThreadPacket 
	)
{
	BOOL fRc = TRUE;

	CPacketThreadData* pThreadData = 
		reinterpret_cast< CPacketThreadData* >( pBaseThreadPacket );
	
	INT32 iLastError = 0;

	PEVENT_HEADER pEventHeader = 
		pThreadData->get_EventHeader();

	strcpy_s(
		pEventHeader->eventMacAddress 
		, _countof( pEventHeader->eventMacAddress )
		, CW2A( g_szMACAddr.GetString() )
		);

	DWORD dwBytesSent = 0
		, dwFlags = 0
		;

	pThreadData->set_ActualPacketDataBlobLength( pThreadData->get_EventPacketHeader()->dataBlobSize );
	WSABUF* w = pThreadData->get_WSABUF();

	WSAResetEvent( pThreadData->hEvent );
//	pThreadData->hEvent = ( HANDLE )( ( DWORD_PTR ) pThreadData->hEvent | 1 );

	WSASendTo(
		g_SocketMultiMedia
		, pThreadData->get_WSABUF()
		, pThreadData->get_numBufs()
		, &dwBytesSent
		, dwFlags
		, ( SOCKADDR* ) &g_SocketAddrSrvMultiMedia
		, ( int )sizeof( SOCKADDR_IN )
		, pThreadData
		, NULL//SubmitCompleteRoutine
		);

//	pThreadData->hEvent = ( HANDLE )( ( DWORD_PTR ) pThreadData->hEvent & ~1 );
//	WSASetEvent( pThreadData->hEvent );

	iLastError = WSAGetLastError();
	//if( NO_ERROR != iLastError )
	//{
	//	fRc = FALSE;
	//}

PostClientAudioEvent_Exit:
	return fRc;
}

/***************************************************************************************/

//HANDLE CClientThreadpoolData::m_hHeap = NULL;
//UINT CClientThreadpoolData::m_uNumAllocsInHeap = 0;
//CClientThreadpoolData::CCachePtr CClientThreadpoolData::cc = CCache::getContiguousCache( 40000 );


VOID CClientThreadpoolData::initializeBuffer( UINT32 length )
{

	//m_pDataBuf = reinterpret_cast< WSABUF* >( new BYTE[ sizeof( WSABUF ) ] );
	m_pDataBuf.len = length;

	//bufSharedArray = cc->getArray();
	//m_pDataBuf.buf = bufSharedArray.get();
//	m_pDataBuf.buf = reinterpret_cast< PCHAR >( new CHAR[ length ] );

	m_iNumBuffers = 1;
	return;
}






/***************************************************************************************/

BLXWARE_API HANDLE CServerSendThreadData::m_hHeap = NULL;
BLXWARE_API LONG CServerSendThreadData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CServerSendThreadData::m_csLock = NULL;

BLXWARE_API HANDLE CServerWorkerThreadData::m_hHeap = NULL;
BLXWARE_API LONG CServerWorkerThreadData::m_uNumAllocsInHeap = 0;
BLXWARE_API CComCriticalSection* CServerWorkerThreadData::m_csLock = NULL;

//
//inline VOID CBaseThreadData::internalNew( 
//		UINT32 BufferLength0
//		, UINT32 BufferLength1
//		, UINT32 BufferLength2
//		, UINT32 BufferLength3
//		, UINT32 BufferLength4
//		)
//{
//	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY /*HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) * 5 ) );
//	initializeBuffer( BufferLength0, 0 );
//	initializeBuffer( BufferLength1, 1 );
//	initializeBuffer( BufferLength2, 2 );
//	initializeBuffer( BufferLength3, 3 );
//	initializeBuffer( BufferLength4, 4 );
//	m_iNumBuffers = 5;
//}
//
//
//inline VOID CBaseThreadData::internalRefNew( 
//		PIP_HEADER pIpHeader
//		, PUDP_HEADER pUdpHeader
//		, PEVENT_HEADER pEventHeader
//		, PEVENT_PACKET_HEADER pEventPacketHeader
//		, PPACKET_DATA_BLOB pPacketDataBlob
//		)
//{
//	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY /*HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) * 5 ) );
//	
//	m_pDataBuf[ PHS_IP_HEADER ].len = MAX_IP_HEADER;
//	m_pDataBuf[ PHS_IP_HEADER ].buf = reinterpret_cast< PCHAR >( pIpHeader );
//	m_pDataBuf[ PHS_UDP_HEADER ].len = MAX_UDP_HEADER;
//	m_pDataBuf[ PHS_UDP_HEADER ].buf = reinterpret_cast< PCHAR >( pUdpHeader );
//	m_pDataBuf[ PHS_EVENT_HEADER ].len = MAX_EVENT_HEADER;
//	m_pDataBuf[ PHS_EVENT_HEADER ].buf = reinterpret_cast< PCHAR >( pEventHeader );
//	m_pDataBuf[ PHS_EVENT_PACKET_HEADER ].len = MAX_EVENT_PACKET_HEADER;
//	m_pDataBuf[ PHS_EVENT_PACKET_HEADER ].buf = reinterpret_cast< PCHAR >( pEventPacketHeader );
//	m_pDataBuf[ PHS_PACKET_DATA_BLOB ].len = pEventPacketHeader->dataBlobSize;
//	m_pDataBuf[ PHS_PACKET_DATA_BLOB ].buf = reinterpret_cast< PCHAR >( pPacketDataBlob );
//	m_iNumBuffers = 5;
//}

//inline VOID CBaseThreadData::initializeBuffer( UINT32 length )
//{
//	m_pDataBuf = reinterpret_cast< WSABUF* >( HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY /*HEAP_NO_SERIALIZE*/, sizeof( WSABUF ) ) );
//	m_pDataBuf->len = length;
//	m_pDataBuf->buf = reinterpret_cast< PCHAR >( HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY /*HEAP_NO_SERIALIZE*/, length ) );
//	m_uNumAllocsInHeap++;
//	m_iNumBuffers = 1;
//	return;
//}
//
//
//inline VOID CBaseThreadData::initializeBuffer( UINT32 length, SHORT iteration )
//{
//	m_pDataBuf[ iteration ].len = length;
//	m_pDataBuf[ iteration ].buf = reinterpret_cast< PCHAR >( HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY /*HEAP_NO_SERIALIZE*/, length ) );
//	m_uNumAllocsInHeap++;
//	return;
//}



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


HANDLE CIpHeader::m_hHeap = NULL;
UINT CIpHeader::m_uNumAllocsInHeap = 0;

CIpHeader::CIpHeader(void)
{
}

CIpHeader::CIpHeader( const IP_HEADER& ipHeader, const UDP_HEADER& udpHeader, LPCSTR macAddress )
{
	this->Copy(
		ipHeader
		, udpHeader 
		, macAddress
		);
}

CIpHeader::CIpHeader( const CIpHeader& ipHeader )
{
	this->Copy(
		ipHeader.m_ipHeader
		, ipHeader.m_udpHeader 
		, ipHeader.m_szMACAddress 
		);
}

CIpHeader::~CIpHeader(void)
{
	return;
}

BOOL CIpHeader::Copy( const IP_HEADER& ipHeader, const UDP_HEADER& udpHeader, LPCSTR macAddress )
{
	BOOL fRc = TRUE;

	::CopyMemory(
	&this->m_ipHeader
	, &ipHeader
	, sizeof( IP_HEADER )
	);
	
	::CopyMemory(
	&this->m_udpHeader
	, &udpHeader
	, sizeof( UDP_HEADER )
	);

	sprintf_s(
		this->m_szMACAddress 
		, macAddress
		);

	return fRc;
}

HANDLE CServerVideoSource::m_hHeap = NULL;
UINT CServerVideoSource::m_uNumAllocsInHeap = 0;

CServerVideoSource::CServerVideoSource(void)
{
}

CServerVideoSource::CServerVideoSource( 
	const CIpHeader* pIpHeader
	, const EVENT_HEADER& eventHeader
	, const EVENT_PACKET_HEADER& eventPacketHeader
	, const ESTABLISH_VIDEO_SOURCE& estVideoSource
	)
{
	this->m_pIpHeader = pIpHeader; //new CIpHeader( *pIpHeader );
	this->Copy(
		eventHeader
		, eventPacketHeader
		, estVideoSource
		);
}

CServerVideoSource::CServerVideoSource( const CServerVideoSource& serverMediaSource )
{
	this->m_pIpHeader = serverMediaSource.m_pIpHeader; //new CIpHeader( *serverMediaSource.m_pIpHeader );
	this->Copy(
		serverMediaSource.m_eventHeader 
		, serverMediaSource.m_eventPacketHeader 
		, serverMediaSource.m_videoSource
		);
}

CServerVideoSource::~CServerVideoSource(void)
{
	return;
}

BOOL CServerVideoSource::Copy( 
	const EVENT_HEADER& eventHeader
	, const EVENT_PACKET_HEADER& eventPacketHeader
	, const ESTABLISH_VIDEO_SOURCE& estVideoSource
	)
{
	BOOL fRc = TRUE;

	::CopyMemory(
	&this->m_videoSource
	, &estVideoSource
	, MAX_ESTABLISH_VIDEO_SOURCE
	);

	::CopyMemory(
	&this->m_eventHeader
	, &eventHeader
	, MAX_EVENT_HEADER
	);

	::CopyMemory(
	&this->m_eventPacketHeader
	, &eventPacketHeader
	, sizeof( EVENT_PACKET_HEADER )
	);

	return fRc;
}


HANDLE CServerAudioSource::m_hHeap = NULL;
UINT CServerAudioSource::m_uNumAllocsInHeap = 0;

CServerAudioSource::CServerAudioSource(void)
{
}

CServerAudioSource::CServerAudioSource( 
	const CIpHeader* pIpHeader
	, const EVENT_HEADER& eventHeader
	, const EVENT_PACKET_HEADER& eventPacketHeader
	, const ESTABLISH_AUDIO_SOURCE& estAudioSource
	)
{
	this->m_pIpHeader = pIpHeader; //new CIpHeader( *pIpHeader );
	this->Copy(
		eventHeader
		, eventPacketHeader
		, estAudioSource
		);
}

CServerAudioSource::CServerAudioSource( const CServerAudioSource& serverMediaSource )
{
	this->m_pIpHeader = serverMediaSource.m_pIpHeader; //new CIpHeader( *serverMediaSource.m_pIpHeader );
	this->Copy(
		serverMediaSource.m_eventHeader 
		, serverMediaSource.m_eventPacketHeader 
		, serverMediaSource.m_audioSource
		);
}

CServerAudioSource::~CServerAudioSource(void)
{
	return;
}

BOOL CServerAudioSource::Copy( 
	const EVENT_HEADER& eventHeader
	, const EVENT_PACKET_HEADER& eventPacketHeader
	, const ESTABLISH_AUDIO_SOURCE& estAudioSource
	)
{
	BOOL fRc = TRUE;

	::CopyMemory(
	&this->m_audioSource
	, &estAudioSource
	, sizeof( ESTABLISH_AUDIO_SOURCE )
	);

	::CopyMemory(
	&this->m_eventHeader
	, &eventHeader
	, MAX_EVENT_HEADER
	);

	::CopyMemory(
	&this->m_eventPacketHeader
	, &eventPacketHeader
	, sizeof( EVENT_PACKET_HEADER )
	);

	return fRc;
}

USHORT checksum(USHORT *buffer, int size)
{
	unsigned long cksum=0;
	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT); 
	}

	// If the buffer was not a multiple of 16-bits, add the last byte

	if (size)
	{
		cksum += *(UCHAR*)buffer; 
	}

	// Add the low order 16-bits to the high order 16-bits
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16); 
	// Take the 1's complement

	return (USHORT)(~cksum); 
}

 

//
// Function: ComputeUdpPseudoHeaderChecksumV4
//
// Description:
// Compute the UDP pseudo header checksum. The UDP checksum is based
// on the following fields:
// o source IP address
// o destination IP address
// o 8-bit zero field
// o 8-bit protocol field
// o 16-bit UDP length
// o 16-bit source port
// o 16-bit destination port
// o 16-bit UDP packet length
// o 16-bit UDP checksum (zero)
// o UDP payload (padded to the next 16-bit boundary)
//
// This routine uses a temporary buffer passed into the function (pseudobuf)
// for computing the pseudo-checksum. It is assumed this buffer is large enough.
//

void ComputeUdpPseudoHeaderChecksumV4(
	void *iphdr,
	UDP_HEADER *udphdr,
	char *payload,
	int payloadlen
)
{
//	char pseudobuf[65536];
//	IP_HEADER *v4hdr=NULL;
//	unsigned long zero=0;
//	char *ptr=NULL;
//	int chksumlen=0,
//	i;
//
//	ptr = pseudobuf;
//	v4hdr = (IP_HEADER*)iphdr;
//	// Include the source and destination IP addresses
//	memcpy(ptr, &v4hdr->ip_srcaddr, sizeof(v4hdr->ip_srcaddr)); 
//	ptr += sizeof(v4hdr->ip_srcaddr);
//	chksumlen += sizeof(v4hdr->ip_srcaddr);
//	memcpy(ptr, &v4hdr->ip_destaddr, sizeof(v4hdr->ip_destaddr)); 
//	ptr += sizeof(v4hdr->ip_destaddr);
//	chksumlen += sizeof(v4hdr->ip_destaddr);
//
//	// Include the 8 bit zero field
//	memcpy(ptr, &zero, 1);
//	ptr++;
//	chksumlen += 1;
//	// Protocol
//	memcpy(ptr, &v4hdr->ip_protocol, sizeof(v4hdr->ip_protocol)); 
//	ptr += sizeof(v4hdr->ip_protocol);
//	chksumlen += sizeof(v4hdr->ip_protocol);
//	// UDP length
//	memcpy(ptr, &udphdr->udp_length, sizeof(udphdr->udp_length)); 
//	ptr += sizeof(udphdr->udp_length);
//	chksumlen += sizeof(udphdr->udp_length);
//	// UDP source port
//	memcpy(ptr, &udphdr->src_portno, sizeof(udphdr->src_portno)); 
//	ptr += sizeof(udphdr->src_portno);
//	chksumlen += sizeof(udphdr->src_portno);
//	// UDP destination port
//	memcpy(ptr, &udphdr->dst_portno, sizeof(udphdr->dst_portno)); 
//	ptr += sizeof(udphdr->dst_portno);
//	chksumlen += sizeof(udphdr->dst_portno);
//	// UDP length again
//	memcpy(ptr, &udphdr->udp_length, sizeof(udphdr->udp_length)); 
//	ptr += sizeof(udphdr->udp_length);
//	chksumlen += sizeof(udphdr->udp_length);
//
//	// 16-bit UDP checksum, zero 
//	memcpy(ptr, &zero, sizeof(unsigned short));
//	ptr += sizeof(unsigned short);
//	chksumlen += sizeof(unsigned short);
//	// payload
//	memcpy(ptr, payload, payloadlen);
//	ptr += payloadlen;
//	chksumlen += payloadlen;
//	// pad to next 16-bit boundary
//	for(i=0 ; i < payloadlen%2 ; i++, ptr++)
//	{
////		printf("pad one byte\n");
//		*ptr = 0;
//		ptr++;
//		chksumlen++;
//	}
//
//	// Compute the checksum and put it in the UDP header
//	udphdr->udp_checksum = checksum((USHORT *)pseudobuf, chksumlen);
	udphdr->udp_checksum = 0; // Use 0 to allow WSA to determine checksum. (If you see checksum errors, use 0'0')
	return;
}

 
//////#pragma data_seg()



CClientReceiveThreadData*
GetCurrentClientReceiveThreadData( void )
{
	CClientReceiveThreadData* pData = NULL;

	g_pQClientReceiveThreadData->poll_timed(
		pData
		, 10e6
		);


	return pData;
}

void
ReturnClientReceiveThreadData( CClientReceiveThreadData* ClientReceiveData )
{
	WSAResetEvent( ClientReceiveData->hEvent );

	g_pQClientReceiveThreadData->push(
		ClientReceiveData
		);

	return;
}


CClientVideoData*
GetCurrentClientVideoThreadData( void )
{
	CClientVideoData* pData = NULL;
	g_pQClientVideoData->poll_timed(
		pData
		, 10e6
		);
#ifdef _VISTA_
	try
	{
		if( NULL != pData->m_pWorkItem )
		{
			WaitForThreadpoolWorkCallbacks(
				pData->m_pWorkItem
				, FALSE
				);
			CloseThreadpoolWork( pData->m_pWorkItem );
			pData->m_pWorkItem = NULL;
		}
	}
	catch(...)
	{
		// Todo: Fix crash that happens intermitantly on shutdown
	}

#endif
	//g_pQClientReceiveThreadData->poll(
	//	pData
	//	);

	return pData;
}


VOID
ReturnClientVideoThreadData( CClientVideoData* VideoThreadData )
{
	WSASetEvent( VideoThreadData->hEvent );

#ifdef _VISTA_
	//try
	//{
	//	if( NULL != VideoThreadData->m_pWorkItem )
	//	{
	//		WaitForThreadpoolWorkCallbacks(
	//			VideoThreadData->m_pWorkItem
	//			, FALSE
	//			);
	//		CloseThreadpoolWork( VideoThreadData->m_pWorkItem );
	//		VideoThreadData->m_pWorkItem = NULL;
	//	}
	//}
	//catch(...)
	//{
	//	// Todo: Fix crash that happens intermitantly on shutdown
	//}

#endif

	g_pQClientVideoData->push_timed(
		VideoThreadData
		, 10e6
		);

	return;
}

CClientAudioData*
GetCurrentClientAudioThreadData( void )
{
	CClientAudioData* pData = NULL;
	g_pQClientAudioData->poll_timed(
		pData
		, 1e6
		);

#ifdef _VISTA_
	try
	{
		if( NULL != pData->m_pWorkItem )
		{
			WaitForThreadpoolWorkCallbacks(
				pData->m_pWorkItem
				, FALSE
				);
			CloseThreadpoolWork( pData->m_pWorkItem );
			pData->m_pWorkItem = NULL;
		}
	}
	catch(...)
	{
		// Todo: Fix crash that happens intermitantly on shutdown
	}
#endif

return pData;
}


VOID
ReturnClientAudioThreadData( CClientAudioData* AudioThreadData )
{
	WSASetEvent( AudioThreadData->hEvent );

#ifdef _VISTA_
	//try
	//{
	//	if( NULL != AudioThreadData->m_pWorkItem )
	//	{
	//		WaitForThreadpoolWorkCallbacks(
	//			AudioThreadData->m_pWorkItem
	//			, FALSE
	//			);
	//		CloseThreadpoolWork( AudioThreadData->m_pWorkItem );
	//		AudioThreadData->m_pWorkItem = NULL;
	//	}
	//}
	//catch(...)
	//{
	//	// Todo: Fix crash that happens intermitantly on shutdown
	//}
#endif

	g_pQClientAudioData->push_timed(
		AudioThreadData
		, 1e6
		);
	return;
}

CClientManagementEventData*
GetCurrentManagementEventThreadData( void )
{
	CClientManagementEventData* pData = NULL;
	g_pQSyncManagementEventData->poll_timed(
		pData
		, 10e6);

	return pData;
}


VOID
ReturnManagementEventThreadData( CClientManagementEventData* ManagementEventThreadData )
{
	WSASetEvent( ManagementEventThreadData->hEvent );
	g_pQSyncManagementEventData->push_timed(
		ManagementEventThreadData
		, 10e6
		);
	return;
}

BOOL AllocateClientReceiveBuffers()
{
	BOOL rc = TRUE;

	CClientReceiveThreadData* pOverlapped = NULL;

	g_pQClientReceiveThreadData = new QClientReceiveThreadData();
	g_pQClientAudioData = new QClientAudioData();
	g_pQClientVideoData = new QClientVideoData();
	g_pQSyncManagementEventData = new QManagementEventData( );

	for( INT32 k = 0 ; k < MAX_CLIENT_THREADS_READER2_CHUNCKS ; k++ )
	{
		for( INT32 i = 0 ; i < MAX_CLIENT_THREADS_READER2 ; i++ )
		{
			pOverlapped =
				new CClientReceiveThreadData( MAX_PACKET_PAYLOAD);

			g_pQClientReceiveThreadData->push(
				pOverlapped
				);

		}
	}

	CClientVideoData* pVideoOverlapped = NULL;
	CClientAudioData* pAudioOverlapped = NULL;
	CClientManagementEventData* pManagementOverlapped = NULL;

	for( INT32 k = 0 ; k < MAX_CLIENT_THREADS_READER2_CHUNCKS ; k++ )
	{
		for( INT32 i = 0 ; i < MAX_CLIENT_THREADS_POOLED ; i++ )
		{
			pVideoOverlapped =
				new CClientVideoData( MAX_AUDIO_PACKET_BLOB );
			g_pQClientVideoData->push(
				pVideoOverlapped
				);

			pAudioOverlapped =
				new CClientAudioData( MAX_AUDIO_PACKET_BLOB );
			g_pQClientAudioData->push(
				pAudioOverlapped
				);
			
			pManagementOverlapped =
				new CClientManagementEventData( MAX_AUDIO_PACKET_BLOB );
			g_pQSyncManagementEventData->push(
				pManagementOverlapped
				);
		}
	}

	return rc;
}

BOOL DeallocateClientReceiveBuffers()
{
	BOOL rc = TRUE;


	return rc;
}



