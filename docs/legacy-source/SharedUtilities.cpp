
#include "SharedUtilities.h"
#include "PacketThreadData.h"
#include "ClientReceiveThreadData.h"
#include "ClientAudioData.h"
#include "ClientVideoData.h"
#include "ClientManagementEventData.h"
#include "PacketThreadNetworkBuffer.h"

#include <iphlpapi.h>

//#include <wxdebug.h>

WORD g_wVersionRequested;
WSADATA g_WsaData;

SOCKET						g_SocketMultiMedia = INVALID_SOCKET;

HANDLE						g_hSocketMultMediaEvent;
SOCKADDR_IN					g_SocketAddrCliMultiMedia;
SOCKADDR_IN					g_SocketAddrSrvMultiMedia;

CStringA					g_szMACAddr;
CString						g_szConfDns;

//CSimpleArray<CPacketThreadData*> g_pVideoThreadData;
//CSimpleArray< g_videoThreadArray > g_threadsVideoArray;

CSimpleArray<CPacketThreadData*> g_pAudioThreadData;
CSimpleArray< g_audioThreadArray > g_threadsAudioArray;

CSimpleArray<CPacketThreadData*> g_pMgmtThreadData;
CSimpleArray< g_mgmtThreadArray > g_threadsMgmtArray;


PQClientReceiveThreadData				g_pQClientReceiveThreadData;
PQClientAudioData				        g_pQClientAudioData;
PQClientVideoData				        g_pQClientVideoData;
PQManagementEventData				    g_pQSyncManagementEventData;

PQCPacketThreadData				        g_pQCPacketThreadData;
PQCPacketThreadNetworkBuffer		    g_pVideoNetworkBuffer;
PQCPacketThreadNetworkBuffer		    g_pAudioNetworkBuffer;
PQCPacketThreadNetworkBuffer		    g_pMgmtNetworkBuffer;

volatile LONG g_uiCurrentVideoThread = 0;
volatile LONG g_uiMaxVideoThread = MAX_CLIENT_THREADS_READER;
volatile LONG g_uiOldestVideoThread = 0;

volatile LONG g_uiCurrentAudioThread = 0;
volatile LONG g_uiMaxAudioThread  = MAX_CLIENT_THREADS_READER;
volatile LONG g_uiOldestAudioThread = 0;

/*volatile*/ LONG g_uiCurrentManagementThread = 0;
volatile LONG g_uiMaxManagementThread = MAX_CLIENT_THREADS_READER;
volatile LONG g_uiOldestManagementThread = 0;

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

static CComCriticalSection* g_macAddressCs = NULL;
CComCriticalSection* GetMacAddressCs();
static GUID g_macAddress;
static BOOL g_macAddressCreated = false;


CComCriticalSection* GetMacAddressCs()
{
	if( g_macAddressCs == NULL )
	{
		g_macAddressCs = new CComCriticalSection();
		g_macAddressCs->Init();
	}
	return g_macAddressCs;
}



#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

VOID ReturnCurrentThreadData( CPacketThreadData* pThreadData );

VOID CALLBACK SubmitCompleteRoutine (
                                     IN DWORD dwError
                                     , IN DWORD cbTransferred
                                     , IN LPOVERLAPPED lpOverlapped
                                     )
{

    CClientSendThreadData* pBase = ( CClientSendThreadData* )lpOverlapped;
    WSASetEvent( pBase->hEvent );

    if( pBase->get_EventHeader()->eventType == BLXWARE_EVENT::BE_PUBLISH_VIDEO_FRAMES )
    {
        CPacketThreadNetworkBuffer* pNetworkBuffer = ( CPacketThreadNetworkBuffer* )lpOverlapped;
        ReturnCurrentVideoNetworkBuffer( pNetworkBuffer );
    }
#ifdef INCLUDE_AUDIO
    else if( pBase->get_EventHeader()->eventType == BLXWARE_EVENT::BE_PUBLISH_AUDIO_FRAMES )
    {
        CPacketThreadNetworkBuffer* pNetworkBuffer = ( CPacketThreadNetworkBuffer* )lpOverlapped;
        ReturnCurrentAudioNetworkBuffer( pNetworkBuffer );
    }
#endif
    else
    {
        CPacketThreadData* pThreadData = ( CPacketThreadData* )lpOverlapped;
        ReturnCurrentThreadData( pThreadData );
    }


    return;
}

HRESULT StartWinsock()
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
}

HRESULT InitializeSendWinsock()
{
    HRESULT hr = S_OK;
    INT32 iLastError = 0;

    SetLastError( 0 );

    hr = StartWinsock();
    if( FAILED( hr ) )
    {
        return hr;
    }

    g_SocketMultiMedia = WSASocket( 
        AF_INET
        , SOCK_DGRAM //SOCK_DGRAM  //SOCK_RAW
        , IPPROTO_UDP 
        , NULL
        , 0
        , WSA_FLAG_OVERLAPPED
        );
	if( g_SocketMultiMedia == INVALID_SOCKET ) {
		iLastError = WSAGetLastError();
		if( 0 != iLastError )
		{
			hr = E_FAIL;
			return hr;
		}
	}


    BOOL fBroadcast = FALSE;
#ifndef UNDER_CE
    INT32 _bufSize = 0;
#else
    INT32 _bufSize = 1;
#endif

    ULONG fReuse = FALSE;//TRUE;
    ULONG fExUse = TRUE;//FALSE;
    BOOL fHeaderIncluded = TRUE;

	WSASetLastError( 0 );
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
        , ( PCHAR )&_bufSize
        , sizeof( INT32 )
        );
    iLastError = WSAGetLastError();
    ATLTRACE2( 
        atlTraceGeneral
        , 0
        , TEXT( " WSAGetLastError = %d \n" )
        , iLastError
        );

    //ATLASSERT( 0 == iLastError );
    //ATLTRACE2( 
    //    atlTraceGeneral
    //    , 0
    //    , L" SO_SNDBUF = %d \n"
    //    , _bufSize
    //    );
    //if( 0 != iLastError )
    //{
    //    hr = E_FAIL;
    //    return hr;
    //}

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

#ifndef UNDER_CE
    setsockopt(
        g_SocketMultiMedia
        , SOL_SOCKET
        , SO_EXCLUSIVEADDRUSE
        , ( PCHAR )&fExUse
        , sizeof( ULONG )
        );
    iLastError = WSAGetLastError();
    if( 0 != iLastError )
    {
        hr = E_FAIL;
        return hr;
    }
#endif

    IN_ADDR hostInAddr;

    iLastError = GetBestIpAddress( 
        g_szConfDns
        , &hostInAddr
        );
    if( 0 != iLastError )
    {
        hr = E_FAIL;
        return hr;
    }
	
	short _sockOffset = 2;

try_bind:

    g_SocketAddrCliMultiMedia.sin_family = AF_INET;
#ifdef UNDER_CE
    g_SocketAddrCliMultiMedia.sin_addr.s_addr = inet_addr( inet_ntoa (hostInAddr) );
#else
    g_SocketAddrCliMultiMedia.sin_addr.s_addr = inet_addr( inet_ntoa (hostInAddr) );
    //g_SocketAddrCliMultiMedia.sin_addr.s_addr = inet_addr( "192.168.0.1" );
#endif

    bind(
        g_SocketMultiMedia
        , ( const sockaddr* )&g_SocketAddrCliMultiMedia 
        , sizeof( SOCKADDR_IN )
        );
    iLastError = WSAGetLastError();
    if( 0 != iLastError )
    {
		if( WSAEADDRINUSE == iLastError ) {
			_sockOffset++;
			goto try_bind;
		} else {
			hr = E_FAIL;
			return hr;
		}
    }
    g_hSocketMultMediaEvent = WSACreateEvent();
    INT rc = WSAEventSelect(
        g_SocketMultiMedia
        , g_hSocketMultMediaEvent
        , FD_WRITE
        );

#ifndef UNDER_CE
    BOOL fBioc = BindIoCompletionCallback(
        ( HANDLE )g_SocketMultiMedia
        , SubmitCompleteRoutine
        , 0
        );
#endif 

    //hostent* thisHost = ::gethostbyname( CW2A( g_szConfDns.GetString() ) );
    IN_ADDR _inaddr     = {0};

    GetBestHostName(        //need error handling here...  probably should return a HRESULT or something instead of void.
        g_szConfDns
        , &_inaddr 
        );
    
    //if( !thisHost )
    //{
    //    iLastError = WSAGetLastError();
    //}

    g_SocketAddrSrvMultiMedia.sin_family = AF_INET;
//    g_SocketAddrSrvMultiMedia.sin_addr.s_addr = inet_addr( inet_ntoa (*(struct in_addr *)*thisHost->h_addr_list) );
    g_SocketAddrSrvMultiMedia.sin_addr.s_addr = inet_addr( inet_ntoa ( _inaddr ) );
    g_SocketAddrSrvMultiMedia.sin_port = htons( 23499 );

    return hr;
}

BOOL AllocateClientSendBuffers()
{
    BOOL rc = TRUE;

    g_pQCPacketThreadData   = new QCPacketThreadData( MAX_CLIENT_THREADS_READER3 );
    g_pVideoNetworkBuffer   = new QCPacketThreadNetworkBuffer( MAX_CLIENT_THREADS_READER3 );
    g_pAudioNetworkBuffer   = new QCPacketThreadNetworkBuffer( MAX_CLIENT_THREADS_READER3 );
    g_pMgmtNetworkBuffer    = new QCPacketThreadNetworkBuffer( MAX_CLIENT_THREADS_READER3 );

    CPacketThreadData* pOverlapped = NULL;
    CPacketThreadNetworkBuffer* pNetworkBuffer = NULL;

    for( INT32 i = 0 ; i < MAX_CLIENT_THREADS_READER3/*MAX_CLIENT_THREADS_READER*/ ; i++ )
    {
        pOverlapped =
            new CPacketThreadData( );
        g_pQCPacketThreadData->push( pOverlapped );

        //pOverlapped =
        //    new CPacketThreadData( );
        //g_pAudioThreadData.Add( pOverlapped );

        //pOverlapped =
        //    new CPacketThreadData( );
        //g_pMgmtThreadData.Add( pOverlapped );
    
        pNetworkBuffer = 
            new CPacketThreadNetworkBuffer( MAX_VIDEO_NETWORK_PACKETS );
        g_pVideoNetworkBuffer->push( pNetworkBuffer );
    
        pNetworkBuffer = 
            new CPacketThreadNetworkBuffer( MAX_AUDIO_NETWORK_PACKETS );
        g_pAudioNetworkBuffer->push( pNetworkBuffer );
    
        pNetworkBuffer = 
            new CPacketThreadNetworkBuffer( MAX_MGMT_NETWORK_PACKETS );
        g_pMgmtNetworkBuffer->push( pNetworkBuffer );
    
    }
    return rc;
}

BOOL DeallocateClientSendBuffers()
{
    BOOL rc = TRUE;

    CPacketThreadData* pOverlapped = NULL;
    for( INT32 i = 0 ; i < MAX_CLIENT_THREADS_READER3/*MAX_CLIENT_THREADS_READER*/ ; i++ )
    {
        //pOverlapped =  g_pVideoThreadData[ i ];
        //CloseHandle( pOverlapped->hEvent );
        //delete pOverlapped;
        g_pQCPacketThreadData->clear();

        //pOverlapped =  g_pAudioThreadData[ i ];
        //CloseHandle( pOverlapped->hEvent );
        //delete pOverlapped;

        //pOverlapped =  g_pMgmtThreadData[ i ];
        //CloseHandle( pOverlapped->hEvent );
        //delete pOverlapped;

        //g_pVideoThreadData.RemoveAll();
        g_pAudioThreadData.RemoveAll();
        g_pMgmtThreadData.RemoveAll();

        g_pVideoNetworkBuffer->clear();
        g_pAudioNetworkBuffer->clear();
        g_pMgmtNetworkBuffer->clear();
    }
    return rc;
}

BOOL AllocateClientReceiveBuffers()
{
    BOOL rc = TRUE;

    CClientReceiveThreadData* pOverlapped = NULL;

    g_pQClientReceiveThreadData = new QClientReceiveThreadData( MAX_CLIENT_THREADS_READER2_CHUNCKS * MAX_CLIENT_THREADS_READER2 );
#ifdef INCLUDE_AUDIO
    g_pQClientAudioData = new QClientAudioData( MAX_CLIENT_THREADS_READER2_CHUNCKS * MAX_CLIENT_THREADS_POOLED );
#endif
#ifndef USE_NETWORK_PACKET
    g_pQClientVideoData = new QClientVideoData( MAX_CLIENT_THREADS_READER2_CHUNCKS * MAX_CLIENT_THREADS_POOLED );
#endif
    g_pQSyncManagementEventData = new QManagementEventData( MAX_CLIENT_THREADS_READER2_CHUNCKS * MAX_CLIENT_MGT_THREADS_POOLED );

    for( INT32 k = 0 ; k < MAX_CLIENT_THREADS_READER2_CHUNCKS ; k++ )
    {
        for( INT32 i = 0 ; i < MAX_CLIENT_THREADS_READER2 ; i++ )
        {
            pOverlapped =
                new CClientReceiveThreadData( OUT_MAX_PACKET_PAYLOAD);

            g_pQClientReceiveThreadData->push(
                pOverlapped
                );

        }
    }

#ifdef INCLUDE_AUDIO
    CClientAudioData* pAudioOverlapped = NULL;
#endif

    CClientVideoData* pVideoOverlapped = NULL;
    CClientManagementEventData* pManagementOverlapped = NULL;

    for( INT32 k = 0 ; k < MAX_CLIENT_THREADS_READER2_CHUNCKS ; k++ )
    {
        for( INT32 i = 0 ; i < MAX_CLIENT_MGT_THREADS_POOLED ; i++ )
        {
            pManagementOverlapped =
                new CClientManagementEventData( MAX_MANAGEMENT_PACKET_BLOB );
            g_pQSyncManagementEventData->push(
                pManagementOverlapped
                );
        }
    }

    for( INT32 k = 0 ; k < MAX_CLIENT_THREADS_READER2_CHUNCKS ; k++ )
    {
#ifndef USE_NETWORK_PACKET
        for( INT32 i = 0 ; i < MAX_CLIENT_THREADS_POOLED ; i++ )
        {
            pVideoOverlapped =
                new CClientVideoData( IN_MAX_PACKET_PAYLOAD );
            g_pQClientVideoData->push(
                pVideoOverlapped
                );
        }
#endif
    }

    for( INT32 k = 0 ; k < MAX_CLIENT_THREADS_READER2_CHUNCKS ; k++ )
    {
#ifdef INCLUDE_AUDIO
        for( INT32 i = 0 ; i < MAX_CLIENT_THREADS_POOLED ; i++ )
        {
            pAudioOverlapped =
                new CClientAudioData( IN_MAX_PACKET_PAYLOAD );
            g_pQClientAudioData->push(
                pAudioOverlapped
                );
        }
#endif
    }

    return rc;
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

CClientReceiveThreadData*
GetCurrentClientReceiveThreadData( void )
{
    CClientReceiveThreadData* pData = NULL;

    g_pQClientReceiveThreadData->poll(//poll_timed(
        pData
        //, 1000
        );

    //wprintf(
    //    L"g_pQClientReceiveThreadData->Count: %d \n"
    //    , g_pQClientReceiveThreadData->size()
    //    );

    return pData;
}

void
ReturnClientReceiveThreadData( CClientReceiveThreadData* ClientReceiveData )
{
    WSAResetEvent( ClientReceiveData->hEvent );

    g_pQClientReceiveThreadData->push(//push_timed(
        ClientReceiveData
        //, 1000
        );

    return;
}

CClientAudioData*
GetCurrentClientAudioThreadData( void )
{
	CClientAudioData* pData = NULL;
	g_pQClientAudioData->poll(//poll_timed(
		pData
		//, 1000 //1e6
		);

    if( NULL == pData )
    {
        return pData;
    }
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

CClientVideoData*
GetCurrentClientVideoThreadData( void )
{
    CClientVideoData* pData = NULL;
    g_pQClientVideoData->poll(
        pData
        );

    if( NULL == pData )
    {
        return pData;
    }
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

    g_pQClientVideoData->push(
        VideoThreadData
        );

    return;
}

CPacketThreadData*
GetCurrentThreadData( void )
{
    CPacketThreadData* pData = NULL;
    g_pQCPacketThreadData->poll(
        pData
        );

    return pData;
}


VOID
ReturnCurrentThreadData( CPacketThreadData* VideoThreadData )
{
    WSASetEvent( VideoThreadData->hEvent );

    g_pQCPacketThreadData->push(
        VideoThreadData
        );

    return;
}

CPacketThreadNetworkBuffer*
GetCurrentVideoNetworkBuffer( void )
{
    CPacketThreadNetworkBuffer* pData = NULL;
    g_pVideoNetworkBuffer->poll(
        pData
        );

    return pData;
}


VOID
ReturnCurrentVideoNetworkBuffer( CPacketThreadNetworkBuffer* VideoNetworkBuffer )
{
    WSASetEvent( VideoNetworkBuffer->hEvent );

    CPacketThreadData* pThreadData = NULL;
    if( VideoNetworkBuffer->get_currentBufferCount() > 0 )
    {
        for( int i = 0 ; i < VideoNetworkBuffer->get_currentBufferCount() ; i++ )
        {
            pThreadData = VideoNetworkBuffer->get_packetThreadData( i );
            ReturnCurrentThreadData( pThreadData );
        }

        VideoNetworkBuffer->ClearBuffer( );
    }

    g_pVideoNetworkBuffer->push(
        VideoNetworkBuffer
        );

    return;
}

CPacketThreadNetworkBuffer*
GetCurrentAudioNetworkBuffer( void )
{
    CPacketThreadNetworkBuffer* pData = NULL;
    g_pAudioNetworkBuffer->poll(
        pData
        );

    return pData;
}


VOID
ReturnCurrentAudioNetworkBuffer( CPacketThreadNetworkBuffer* AudioNetworkBuffer )
{
    WSASetEvent( AudioNetworkBuffer->hEvent );

    CPacketThreadData* pThreadData = NULL;
    if( AudioNetworkBuffer->get_currentBufferCount() > 0 )
    {
        for( int i = 0 ; i < AudioNetworkBuffer->get_currentBufferCount() ; i++ )
        {
            pThreadData = AudioNetworkBuffer->get_packetThreadData( i );
            ReturnCurrentThreadData( pThreadData );
        }

        AudioNetworkBuffer->ClearBuffer( );
    }

    g_pAudioNetworkBuffer->push(
        AudioNetworkBuffer
        );

    return;
}

CPacketThreadNetworkBuffer*
GetCurrentMgmtNetworkBuffer( void )
{
    CPacketThreadNetworkBuffer* pData = NULL;
    g_pMgmtNetworkBuffer->poll(
        pData
        );

    return pData;
}


VOID
ReturnCurrentMgmtNetworkBuffer( CPacketThreadNetworkBuffer* MgmtNetworkBuffer )
{
    WSASetEvent( MgmtNetworkBuffer->hEvent );

    g_pMgmtNetworkBuffer->push(
        MgmtNetworkBuffer
        );

    return;
}

//CPacketThreadData*
//GetCurrentVideoThreadData( void )
//{
//    CPacketThreadData* pThreadCurrent = NULL;
//    CPacketThreadData* pThreadOldest = NULL;
//    LONG _uiCurrentVideoThread = 0;
//
//#ifdef _VISTA_
//    AcquireSRWLockExclusive( &g_srwVideoThreadData );
//#else
//    EnterCriticalSection( &g_srwVideoThreadData );
//#endif
//    _uiCurrentVideoThread = g_uiCurrentVideoThread;
//    g_uiCurrentVideoThread++;
//
//#ifdef _VISTA_
//    ReleaseSRWLockExclusive( &g_srwVideoThreadData );
//#else
//    LeaveCriticalSection( &g_srwVideoThreadData );
//#endif
//
//    pThreadCurrent = g_pVideoThreadData[ _uiCurrentVideoThread % g_uiMaxVideoThread ];
//    pThreadOldest = g_pVideoThreadData[ ( _uiCurrentVideoThread + 1 ) % g_uiMaxVideoThread ];
//    WSAResetEvent( pThreadCurrent->hEvent );
//
//    ::SecureZeroMemory(
//        pThreadCurrent->get_PacketDataBlob()
//        , MAX_PACKET_DATA_BLOB
//        );
//
//    if( _uiCurrentVideoThread > MAX_CLIENT_THREADS_READER - 2 )
//    {
//        WSAWaitForMultipleEvents( 
//            1
//            , &pThreadOldest->hEvent
//            , FALSE
//            , WSA_INFINITE
//            , TRUE 
//            );
//        WSAResetEvent( pThreadOldest->hEvent );
//    }
//
//    return pThreadCurrent;
//}

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

    g_pQClientAudioData->push(//push_timed(
        AudioThreadData
        //, 5000 
        );
    return;
}

CClientManagementEventData*
GetCurrentManagementEventThreadData( void )
{
    CClientManagementEventData* pData = NULL;
    g_pQSyncManagementEventData->poll(//poll_timed(
        pData
        //, 1000 //10e6
        );

    return pData;
}


VOID
ReturnManagementEventThreadData( CClientManagementEventData* ManagementEventThreadData )
{
    WSASetEvent( ManagementEventThreadData->hEvent );
    g_pQSyncManagementEventData->push(//push_timed(
        ManagementEventThreadData
        //, 1000 //10e6
        );
    return;
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

VOID FormatMACAddress(BYTE Address[], LPSTR frmtMACAddress)
{
    CHAR t[ MACADDRESS_LENGTH ];
    ::SecureZeroMemory(
        &t
        , MACADDRESS_LENGTH
        );

#ifndef UNDER_CE
    int len = StringCchPrintfA( 
#else
    int len = StringCchPrintfA( 
#endif
        t
        , _countof( t )
        , "%02x_%02x_%02x_%02x_%02x_%02x\0"
        , Address[0]
        , Address[1]
        , Address[2]
        , Address[3]
        , Address[4]
        , Address[5]
        );

    StringCchCopyA(
        frmtMACAddress
        , MACADDRESS_LENGTH
        , t
        );
}

VOID GetMACAddress( LPSTR macAddress )
{
	/* Removed in favor of generating a unique GUID on each transmission as this get's replaced by participantID */
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
    FormatMACAddress(pAdapterInfo->Address, macAddress);
	//if( g_macAddressCreated ) {
	//	StringFromGUID2(
	//		g_macAddress
	//		, macAddress
	//		, 39
	//		);
	//	return;
	//}


 //   GetMacAddressCs()->Lock();

	//if( g_macAddressCreated ) {
	//	StringFromGUID2(
	//		g_macAddress
	//		, macAddress
	//		, 39
	//		);
	//} else {
	//	CoCreateGuid( &g_macAddress );
	//	StringFromGUID2(
	//		g_macAddress
	//		, macAddress
	//		, 39
	//		);
	//	g_macAddressCreated = true;
	//}

	//GetMacAddressCs()->Unlock();

    return;
}

//int GetBestIpAddress( const CString toHostName, PIN_ADDR ipAddress )
//{
//    SetLastError( 0 );
//    FILE * hFile;
//    char _ipAddr[255] = {0};
//
//    hFile = fopen( "./dump.txt", "w");
//
//    DWORD iLastError = 0;
//
//    DWORD biIndex = -1;
//
//    PMIB_IPADDRTABLE pIPAddrTable;
//    pIPAddrTable = (PMIB_IPADDRTABLE) MALLOC(sizeof (MIB_IPADDRTABLE));
//    if (pIPAddrTable == NULL) {
//        sprintf( 
//            _ipAddr
//            , "(PMIB_IPADDRTABLE) MALLOC(sizeof (MIB_IPADDRTABLE)) failed.\n"
//            );
//        fwrite(_ipAddr, sizeof(char), strlen( _ipAddr ), hFile);
//        memset(
//            _ipAddr
//            , NULL
//            , 255
//            );
//        fflush(hFile);
//        fclose(hFile);
//        return WSAGetLastError();
//    }
//
//    DWORD dwSize    = 0;
//    DWORD dwRetVal  = 0;
//    if( GetIpAddrTable( pIPAddrTable, &dwSize, 0 ) == ERROR_INSUFFICIENT_BUFFER ) {
//        FREE( pIPAddrTable );
//        pIPAddrTable = ( PMIB_IPADDRTABLE )MALLOC( dwSize );
//        if( pIPAddrTable == NULL ) {
//            return WSAGetLastError();
//        }
//    }
//
//    if ((dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) != NO_ERROR) {
//        if (pIPAddrTable) {
//            FREE(pIPAddrTable);
//        }
//        return WSAGetLastError();
//    }
//
//    int i = 0;
//    DWORD _ndx = 0;
//
//    for (i = 0; i < (int) pIPAddrTable->dwNumEntries; i++) {
//            ipAddress->S_un.S_addr = (u_long) pIPAddrTable->table[i].dwAddr;
//            _ndx = pIPAddrTable->table[i].dwIndex;
//            sprintf( 
//                _ipAddr
//                , "%d : %d : %d : %d\n"
//                , ipAddress->S_un.S_un_b.s_b1 
//                , ipAddress->S_un.S_un_b.s_b2 
//                , ipAddress->S_un.S_un_b.s_b3 
//                , ipAddress->S_un.S_un_b.s_b4
//                );
//
//            fwrite(_ipAddr, sizeof(char), strlen( _ipAddr ), hFile);
//            memset(
//                _ipAddr
//                , NULL
//                , 255
//                );
//    }
//    fflush(hFile);
//    fclose(hFile);
//
//    return iLastError;
//}

VOID GetBestHostName( const ATL::CString hostName, PIN_ADDR pipaddr )
{
    hostent* thisHost       = NULL;
    char _ipAddrMsg[255]    = {0};
    FILE * hFile;
    hFile = fopen( "./dump1.txt", "w");

#ifndef USE_IP_DNS
    thisHost = ::gethostbyname( CW2A( hostName.GetString() ) );

    if( NULL == thisHost )
    {
        sprintf( 
            _ipAddrMsg
            , "gethostbyname failed - WSAGetLastError: %d\n"
            , WSAGetLastError() 
            );
        fwrite(_ipAddrMsg, sizeof(char), strlen( _ipAddrMsg ), hFile);
        memset(
            _ipAddrMsg
            , NULL
            , 255
            );
        fflush(hFile);
        fclose(hFile);
        return;
    } else {
        sprintf( 
            _ipAddrMsg
            , "gethostbyname succeeded\n"
            );
        fwrite(_ipAddrMsg, sizeof(char), strlen( _ipAddrMsg ), hFile);
        memset(
            _ipAddrMsg
            , NULL
            , 255
            );
    }

    ULONG _ipaddr   = inet_addr( CW2A( hostName.GetString() ) );
    ::CopyMemory(
        pipaddr
        , &_ipaddr
        , sizeof( ULONG )
        );
#else
    ULONG _ipaddr   = inet_addr( CW2A( hostName.GetString() ) );
    ::CopyMemory(
        pipaddr
        , &_ipaddr
        , sizeof( ULONG )
        );
#endif

    return;
}

int GetBestIpAddress( const CString toHostName, PIN_ADDR ipAddress )
{
    SetLastError( 0 );
    FILE * hFile;
    char _ipAddr[255] = {0};

    hFile = fopen( "./dump2.txt", "w");

    DWORD iLastError    = 0;
    hostent* thisHost   = NULL;
    IN_ADDR ipaddr      = {0};

    GetBestHostName(
        toHostName
        , &ipaddr
        );

    DWORD biIndex = -1;

	DWORD rc = 0;
	//rc = GetBestInterface(
 //       *(IPAddr*)pipaddr
	//	//(IPAddr)ipaddr
 //       , &biIndex
 //       );
    SetLastError( 0 );
	rc = GetBestInterface(
        (IPAddr)ipaddr.S_un.S_addr
        , &biIndex
        );

    if( NO_ERROR != rc ) {
        sprintf( 
            _ipAddr
            , "GetBestInterface failed - rc: %d\n"
            , rc 
            );
        fwrite(_ipAddr, sizeof(char), strlen( _ipAddr ), hFile);
        memset(
            _ipAddr
            , NULL
            , 255
            );
        fflush(hFile);
        fclose(hFile);
        return rc;
    }

    PMIB_IPADDRTABLE pIPAddrTable;
    pIPAddrTable = (PMIB_IPADDRTABLE) MALLOC(sizeof (MIB_IPADDRTABLE));
    if (pIPAddrTable == NULL) {
        sprintf( 
            _ipAddr
            , "(PMIB_IPADDRTABLE) MALLOC(sizeof (MIB_IPADDRTABLE)) failed.\n"
            );
        fwrite(_ipAddr, sizeof(char), strlen( _ipAddr ), hFile);
        memset(
            _ipAddr
            , NULL
            , 255
            );
        fflush(hFile);
        fclose(hFile);
        return WSAGetLastError();
    }

    DWORD dwSize    = 0;
    DWORD dwRetVal  = 0;
    if( GetIpAddrTable( pIPAddrTable, &dwSize, 0 ) == ERROR_INSUFFICIENT_BUFFER ) {
        FREE( pIPAddrTable );
        pIPAddrTable = ( PMIB_IPADDRTABLE )MALLOC( dwSize );
        if( pIPAddrTable == NULL ) {
            return WSAGetLastError();
        }
    }

    if ((dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) != NO_ERROR) {
        if (pIPAddrTable) {
            FREE(pIPAddrTable);
        }
        return WSAGetLastError();
    }

    int i = 0;
    DWORD _ndx = 0;

    for (i = 0; i < (int) pIPAddrTable->dwNumEntries; i++) {
            ipAddress->S_un.S_addr = (u_long) pIPAddrTable->table[i].dwAddr;
            _ndx = pIPAddrTable->table[i].dwIndex;
            sprintf( 
                _ipAddr
                , "%d : %d : %d : %d\n"
                , ipAddress->S_un.S_un_b.s_b1 
                , ipAddress->S_un.S_un_b.s_b2 
                , ipAddress->S_un.S_un_b.s_b3 
                , ipAddress->S_un.S_un_b.s_b4
                );

            fwrite(_ipAddr, sizeof(char), strlen( _ipAddr ), hFile);
            memset(
                _ipAddr
                , NULL
                , 255
                );
    }
    fflush(hFile);
    fclose(hFile);

    for (i = 0; i < (int) pIPAddrTable->dwNumEntries; i++) {
        if( pIPAddrTable->table[i].dwIndex == biIndex ) {
            if( 0 != pIPAddrTable->table[i].dwAddr ) {
                ipAddress->S_un.S_addr = (u_long) pIPAddrTable->table[i].dwAddr;
                break;
            }
        }
    }

    if( pIPAddrTable ) {
        FREE( pIPAddrTable );
    }

    return iLastError;
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
    InitializeCriticalSection( &g_csClientReceiveThreadData );
#else
    InitializeCriticalSection( &g_srwClientReaderThreadData );
    InitializeCriticalSection( &g_srwVideoThreadData );
    InitializeCriticalSection( &g_srwServerSendThreadData );
    InitializeCriticalSection( &g_srwAudioThreadData );
    InitializeCriticalSection( &g_srwManagementThreadData );
    InitializeCriticalSection( &g_srwClientReceiveThreadData );
    InitializeCriticalSection( &g_srwMediaQueue );
    InitializeCriticalSection( &g_srwipcVideoQueue );
    InitializeCriticalSection( &g_srwipcAudioQueue );
#endif
    return rc;
}

BOOL InitializeClientReceive( PCHAR ConfDns )
{
    BOOL rc = TRUE;

    g_szConfDns = ConfDns;

    //	InitializeReceiveWinsock();
    InitializeSRWLocks();
    InitializeClientReceiveBuffers();

    //g_szMACAddr = GetMACAddress();
	GetMACAddress( g_szMACAddr.GetBuffer() );

    return rc;
}

LONG g_PostVideoCount = 0;
LONG g_PostAudioCount = 0;

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
        , g_szMACAddr.GetString()
        );

    DWORD dwBytesSent = 0
        , dwFlags = 0
        ;

    pThreadData->set_ActualPacketDataBlobLength( pThreadData->get_EventPacketHeader()->dataBlobSize );
    WSABUF* w = pThreadData->get_WSABUF();

    WSAResetEvent( pThreadData->hEvent );
    //	pThreadData->hEvent = ( HANDLE )( ( DWORD_PTR ) pThreadData->hEvent | 1 );

	WSASetLastError( 0 );
#ifndef UNDER_CE
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
#else
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
#endif

    InterlockedIncrement( &g_PostVideoCount );

    //_tprintf( 
    //    TEXT( "Pin.cpp g_PostVideoCount,%ld,g_PostAudioCount,%ld \n" )
    //    , g_PostVideoCount
    //    , g_PostAudioCount
    //    );
    //	pThreadData->hEvent = ( HANDLE )( ( DWORD_PTR ) pThreadData->hEvent & ~1 );
    //	WSASetEvent( pThreadData->hEvent );

    iLastError = WSAGetLastError();
    //if( NO_ERROR != iLastError )
    //{
    //	fRc = FALSE;
    //}
#ifdef UNDER_CE
	DWORD transfered	= 0;
	DWORD flags		= 0;

	WSASetLastError( 0 );
	BOOL rc = 
		WSAGetOverlappedResult(
			g_SocketMultiMedia
			, pThreadData
			, &transfered
			, true
			, &flags
			);
    iLastError = WSAGetLastError();
	SubmitCompleteRoutine (
		iLastError
		, transfered
		, pThreadData
		);
#endif

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
        , g_szMACAddr.GetString()
        );

    DWORD dwBytesSent = 0
        , dwFlags = 0
        ;

    pThreadData->set_ActualPacketDataBlobLength( pThreadData->get_EventPacketHeader()->dataBlobSize );

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
    InterlockedIncrement( &g_PostAudioCount );

    iLastError = WSAGetLastError();
    //if( NO_ERROR != iLastError )
    //{
    //	fRc = FALSE;
    //}

PostClientAudioEvent_Exit:
    return fRc;
}

BOOL PostNetworkBufferVideoEvent( CPacketThreadNetworkBuffer* pBaseNetworkBuffer )
{
    BOOL fRc = TRUE;

    CPacketThreadNetworkBuffer* pNetworkBuffer = 
        reinterpret_cast< CPacketThreadNetworkBuffer* >( pBaseNetworkBuffer );

    INT32 iLastError = 0;

    PEVENT_HEADER pEventHeader = 
        pNetworkBuffer->get_EventHeader();

    strcpy_s(
        pEventHeader->eventMacAddress 
        , _countof( pEventHeader->eventMacAddress )
        , g_szMACAddr.GetString()
        );

    CPacketThreadData* pThreadData = NULL;
    UINT8 bufferCount = pNetworkBuffer->get_currentBufferCount();

    for( UINT8 bfrCnt = 0 ; bfrCnt < bufferCount ; bfrCnt++ )
    {
        pThreadData = pNetworkBuffer->get_packetThreadData( bfrCnt );
        pEventHeader = pThreadData->get_EventHeader();
        strcpy_s(
            pEventHeader->eventMacAddress 
            , _countof( pEventHeader->eventMacAddress )
            , g_szMACAddr.GetString()
            );
        pThreadData->set_ActualPacketDataBlobLength( pThreadData->get_EventPacketHeader()->dataBlobSize );
    }

    DWORD dwBytesSent = 0
        , dwFlags = 0
        ;

    UINT32 _dataBlobSize = MAX_NETWORK_BUFFER_PACKETS;
    pNetworkBuffer->set_ActualPacketDataBlobLength( _dataBlobSize );

    WSABUF* w = pNetworkBuffer->get_WSABUF();
    WSAResetEvent( pNetworkBuffer->hEvent );

	WSASetLastError( 0 );
#ifndef UNDER_CE
    WSASendTo(
        g_SocketMultiMedia
        , pNetworkBuffer->get_WSABUF()
        , pNetworkBuffer->get_numBufs()
        , &dwBytesSent
        , dwFlags
        , ( SOCKADDR* ) &g_SocketAddrSrvMultiMedia
        , ( int )sizeof( SOCKADDR_IN )
        , pNetworkBuffer
		, NULL//SubmitCompleteRoutine
		);
#else
    WSASendTo(
        g_SocketMultiMedia
        , pNetworkBuffer->get_WSABUF()
        , pNetworkBuffer->get_numBufs()
        , &dwBytesSent
        , dwFlags
        , ( SOCKADDR* ) &g_SocketAddrSrvMultiMedia
        , ( int )sizeof( SOCKADDR_IN )
        , pThreadData
		, NULL//SubmitCompleteRoutine
        );
#endif

    InterlockedIncrement( &g_PostVideoCount );

 /*   _tprintf( 
        TEXT( "Pin.cpp g_PostVideoCount,%ld,g_PostAudioCount,%ld \n" )
        , g_PostVideoCount
        , g_PostAudioCount
        );*/

    iLastError = WSAGetLastError();

#ifdef UNDER_CE
	DWORD transfered	= 0;
	DWORD flags		= 0;

	WSASetLastError( 0 );
	BOOL rc = 
		WSAGetOverlappedResult(
			g_SocketMultiMedia
			, pNetworkBuffer
			, &transfered
			, true
			, &flags
			);
    iLastError = WSAGetLastError();
	SubmitCompleteRoutine (
		iLastError
		, transfered
		, pNetworkBuffer
		);
#endif

PostNetworkBufferVideoEvent_Exit:
    return fRc;
}


BOOL PostNetworkBufferAudioEvent( CPacketThreadNetworkBuffer* pBaseNetworkBuffer )
{
    BOOL fRc = TRUE;

    CPacketThreadNetworkBuffer* pNetworkBuffer = 
        reinterpret_cast< CPacketThreadNetworkBuffer* >( pBaseNetworkBuffer );

    INT32 iLastError = 0;

    PEVENT_HEADER pEventHeader = 
        pNetworkBuffer->get_EventHeader();

    strcpy_s(
        pEventHeader->eventMacAddress 
        , _countof( pEventHeader->eventMacAddress )
        , g_szMACAddr.GetString()
        );

    CPacketThreadData* pThreadData = NULL;
    UINT8 bufferCount = pNetworkBuffer->get_currentBufferCount();

    for( UINT8 bfrCnt = 0 ; bfrCnt < bufferCount ; bfrCnt++ )
    {
        pThreadData = pNetworkBuffer->get_packetThreadData( bfrCnt );
        pEventHeader = pThreadData->get_EventHeader();
        strcpy_s(
            pEventHeader->eventMacAddress 
            , _countof( pEventHeader->eventMacAddress )
            , g_szMACAddr.GetString()
            );
        pThreadData->set_ActualPacketDataBlobLength( pThreadData->get_EventPacketHeader()->dataBlobSize );
    }

    DWORD dwBytesSent = 0
        , dwFlags = 0
        ;

    UINT32 _dataBlobSize = MAX_NETWORK_BUFFER_PACKETS;
    pNetworkBuffer->set_ActualPacketDataBlobLength( _dataBlobSize );

    WSABUF* w = pNetworkBuffer->get_WSABUF();
    WSAResetEvent( pNetworkBuffer->hEvent );

	WSASetLastError( 0 );
#ifndef UNDER_CE
    WSASendTo(
        g_SocketMultiMedia
        , pNetworkBuffer->get_WSABUF()
        , pNetworkBuffer->get_numBufs()
        , &dwBytesSent
        , dwFlags
        , ( SOCKADDR* ) &g_SocketAddrSrvMultiMedia
        , ( int )sizeof( SOCKADDR_IN )
        , pNetworkBuffer
		, NULL//SubmitCompleteRoutine
		);
#else
    WSASendTo(
        g_SocketMultiMedia
        , pNetworkBuffer->get_WSABUF()
        , pNetworkBuffer->get_numBufs()
        , &dwBytesSent
        , dwFlags
        , ( SOCKADDR* ) &g_SocketAddrSrvMultiMedia
        , ( int )sizeof( SOCKADDR_IN )
        , pThreadData
		, NULL//SubmitCompleteRoutine
        );
#endif

    InterlockedIncrement( &g_PostVideoCount );

    //_tprintf( 
    //    TEXT( "Pin.cpp g_PostVideoCount,%ld,g_PostAudioCount,%ld \n" )
    //    , g_PostVideoCount
    //    , g_PostAudioCount
    //    );

    iLastError = WSAGetLastError();

#ifdef UNDER_CE
	DWORD transfered	= 0;
	DWORD flags		= 0;

	WSASetLastError( 0 );
	BOOL rc = 
		WSAGetOverlappedResult(
			g_SocketMultiMedia
			, pNetworkBuffer
			, &transfered
			, true
			, &flags
			);
    iLastError = WSAGetLastError();
	SubmitCompleteRoutine (
		iLastError
		, transfered
		, pNetworkBuffer
		);
#endif

PostNetworkBufferAudioEvent_Exit:
    return fRc;
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

	//g_szMACAddr = GetMACAddress();
	GetMACAddress( g_szMACAddr.GetBuffer(39) );

	return rc;
}