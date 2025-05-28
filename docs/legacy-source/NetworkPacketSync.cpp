#include "NetworkPacketSync.h"

#include <iphlpapi.h>

#include "SharedUtilities.h"
#include "PacketThreadNetworkBuffer.h"
#include "ScopedLock.h"

class CNetworkPacketSyncImpl
{
public:
    CNetworkPacketSyncImpl( UINT8 MaxVideoPackets, UINT8 MaxAudioPackets );
    virtual ~CNetworkPacketSyncImpl();
    STDMETHODIMP Initialize();
    STDMETHODIMP Deinitialize();
    CPacketThreadData* GetVideoThreadData( void );
    STDMETHODIMP PostVideoEvent( CPacketThreadData* VideoPacket);
    CPacketThreadData* GetManagementThreadData( void );
    STDMETHODIMP PostVideoMgmtEvent( CPacketThreadData* VideoPacket);
#ifdef INCLUDE_AUDIO
    CPacketThreadData* GetAudioThreadData( void );
    STDMETHODIMP PostAudioEvent( CPacketThreadData* AudioPacket);
    STDMETHODIMP PostAudioMgmtEvent( CPacketThreadData* VideoPacket);
#endif

protected:

private:
    BOOL                        m_isInitialized;
    UINT8                       m_maxVideoPackets;
#ifdef INCLUDE_AUDIO
    UINT8                       m_maxAudioPackets;
#endif
    CPacketThreadNetworkBuffer* m_pVideoNb;
    CPacketThreadNetworkBuffer* m_pAudioNb;
    CPacketThreadNetworkBuffer* m_pMgmtNb;

    HANDLE					    m_hWorkerThread;
    HANDLE                      m_workerThreadComplete;

    CBlxLock                    m_audioLock;
    CBlxLock                    m_videoLock;
    CBlxLock                    m_mgmtLock;

    BOOL                        m_dequeuePackets;

    static DWORD CALLBACK  StartReceive( PVOID /*context*/ );
};

//CNetworkPacketSyncImpl::------------------------------------------------------
CNetworkPacketSyncImpl::CNetworkPacketSyncImpl( UINT8 MaxVideoPackets, UINT8 MaxAudioPackets )
: m_isInitialized( FALSE )
, m_maxVideoPackets( MaxVideoPackets )
#ifdef INCLUDE_AUDIO
, m_maxAudioPackets( MaxAudioPackets )
#endif
, m_pVideoNb( NULL )
, m_pAudioNb( NULL )
, m_pMgmtNb( NULL )
, m_hWorkerThread( INVALID_HANDLE_VALUE )
, m_workerThreadComplete( INVALID_HANDLE_VALUE )
, m_dequeuePackets( TRUE )
{
    m_workerThreadComplete = 
        ::CreateEvent(
            NULL
            , TRUE
            , FALSE
            , TEXT( "CNetworkPacketSyncImpl_WorkerThreadEvent" )
            );

//#ifdef UNDER_CE
//    m_hWorkerThread = 
//        ( HANDLE )CreateThread(
//        NULL
//        , 0
//        , &StartReceive
//        , this
//        , 0 //CREATE_SUSPENDED //mxb
//        , NULL
//        );
//#else
//    m_hWorkerThread = 
//        ( HANDLE )_beginthreadex(
//        NULL
//        , 0
//        , &StartReceive
//        , this
//        , 0 //CREATE_SUSPENDED //mxb
//        , NULL
//        );
//#endif
}

CNetworkPacketSyncImpl::~CNetworkPacketSyncImpl()
{
    //m_dequeuePackets = FALSE;
    //WaitForSingleObject( 
    //    m_workerThreadComplete
    //    , INFINITE
    //    );
}

STDMETHODIMP CNetworkPacketSyncImpl::Initialize( )
{
    HRESULT hr = S_OK;

    return hr;
}

STDMETHODIMP CNetworkPacketSyncImpl::Deinitialize( )
{
    HRESULT hr = S_OK;

    return hr;
}

CPacketThreadData* CNetworkPacketSyncImpl::GetVideoThreadData( void )
{
    return GetCurrentThreadData();
}

STDMETHODIMP CNetworkPacketSyncImpl::PostVideoEvent( CPacketThreadData* VideoPacket)
{
    HRESULT hr = S_OK;
    CScopedLock _lock( &m_videoLock );

    if( NULL == m_pVideoNb )
    {
        m_pVideoNb = GetCurrentVideoNetworkBuffer();
        PEVENT_HEADER pEventHeader = m_pVideoNb->get_EventHeader();
        ::CoCreateGuid( &pEventHeader->eventId );
        pEventHeader->eventType = BLXWARE_EVENT::BE_PUBLISH_VIDEO_FRAMES;
        pEventHeader->eventTypeId = BLXWARE_EVENT_TYPE::BET_VIDEO_STREAM;

        PEVENT_PACKET_HEADER pEventPacketHeader = m_pVideoNb->get_EventPacketHeader();
        pEventPacketHeader->eventPublishingPoint = VideoPacket->get_EventPacketHeader()->eventPublishingPoint;
        pEventPacketHeader->eventWsaBufType = VideoPacket->get_EventPacketHeader()->eventWsaBufType;
        pEventPacketHeader->pptParticipantId = VideoPacket->get_EventPacketHeader()->pptParticipantId;
        pEventPacketHeader->publishingPointId = VideoPacket->get_EventPacketHeader()->publishingPointId;
        pEventPacketHeader->securityToken = VideoPacket->get_EventPacketHeader()->securityToken;
        pEventPacketHeader->dataBlobSize = MAX_NETWORK_BUFFER_PACKETS;

        m_pVideoNb->get_NetworkBufferPackets()->numberPackets = 0;
        m_pVideoNb->get_NetworkBufferPackets()->dataSize = 0;
        m_pVideoNb->ClearBuffer();
    }

    if( !m_pVideoNb->PushBuffer( VideoPacket ) )
    {
        hr = E_FAIL;
        goto PostVideoEvent_Exit;
    }

    if( MAX_VIDEO_NETWORK_PACKETS == m_pVideoNb->get_currentBufferCount() )
    {
        CPacketThreadNetworkBuffer* pNetworkBuffer = m_pVideoNb;
        pNetworkBuffer->get_NetworkBufferPackets()->numberPackets = m_pVideoNb->get_currentBufferCount();
        if( !PostNetworkBufferVideoEvent( pNetworkBuffer ) )
        {
            hr = E_FAIL;
        }
        m_pVideoNb = NULL;
    }


PostVideoEvent_Exit:
    return hr;
}

STDMETHODIMP CNetworkPacketSyncImpl::PostVideoMgmtEvent( CPacketThreadData* VideoPacket)
{
    HRESULT hr = S_OK;
    CScopedLock _lock( &m_mgmtLock );

    if( !PostClientVideoEvent( VideoPacket ) )
    {
        hr = E_FAIL;
    }

PostVideoEvent_Exit:
    return hr;
}

CPacketThreadData* CNetworkPacketSyncImpl::GetManagementThreadData( void )
{
    return GetCurrentThreadData();//GetCurrentManagementThreadData();
}

DWORD CALLBACK CNetworkPacketSyncImpl::StartReceive( PVOID context )
{
    DWORD rc = 0;
    CNetworkPacketSyncImpl* pContext = static_cast< CNetworkPacketSyncImpl* >( context );

    while( pContext->m_dequeuePackets )
    {
    }

    SetEvent( pContext->m_workerThreadComplete );
#ifdef UNDER_CE
    ExitThread( rc );
#else
    _endthreadex( rc );
#endif
    return rc;
}

#ifdef INCLUDE_AUDIO

CPacketThreadData* CNetworkPacketSyncImpl::GetAudioThreadData( void )
{
    return GetCurrentThreadData();
}

STDMETHODIMP CNetworkPacketSyncImpl::PostAudioEvent( CPacketThreadData* AudioPacket)
{
    HRESULT hr = S_OK;
    CScopedLock _lock( &m_audioLock );

    if( NULL == m_pAudioNb )
    {
        m_pAudioNb = GetCurrentAudioNetworkBuffer();
        PEVENT_HEADER pEventHeader = m_pAudioNb->get_EventHeader();
        ::CoCreateGuid( &pEventHeader->eventId );
        pEventHeader->eventType = BLXWARE_EVENT::BE_PUBLISH_AUDIO_FRAMES;
        pEventHeader->eventTypeId = BLXWARE_EVENT_TYPE::BET_AUDIO_STREAM;

        PEVENT_PACKET_HEADER pEventPacketHeader = m_pAudioNb->get_EventPacketHeader();
        pEventPacketHeader->eventPublishingPoint = AudioPacket->get_EventPacketHeader()->eventPublishingPoint;
        pEventPacketHeader->eventWsaBufType = AudioPacket->get_EventPacketHeader()->eventWsaBufType;
        pEventPacketHeader->pptParticipantId = AudioPacket->get_EventPacketHeader()->pptParticipantId;
        pEventPacketHeader->publishingPointId = AudioPacket->get_EventPacketHeader()->publishingPointId;
        pEventPacketHeader->securityToken = AudioPacket->get_EventPacketHeader()->securityToken;
        pEventPacketHeader->dataBlobSize = MAX_NETWORK_BUFFER_PACKETS;

        m_pAudioNb->get_NetworkBufferPackets()->numberPackets = 0;
        m_pAudioNb->get_NetworkBufferPackets()->dataSize = 0;
        m_pAudioNb->ClearBuffer();
    }

    if( !m_pAudioNb->PushBuffer( AudioPacket ) )
    {
        hr = E_FAIL;
        goto PostAudioEvent_Exit;
    }

    if( MAX_AUDIO_NETWORK_PACKETS == m_pAudioNb->get_currentBufferCount() )
    {
        CPacketThreadNetworkBuffer* pNetworkBuffer = m_pAudioNb;
        pNetworkBuffer->get_NetworkBufferPackets()->numberPackets = m_pAudioNb->get_currentBufferCount();
        if( !PostNetworkBufferAudioEvent( pNetworkBuffer ) )
        {
            hr = E_FAIL;
        }
        m_pAudioNb = NULL;
    }

PostAudioEvent_Exit:
    return hr;
}

STDMETHODIMP CNetworkPacketSyncImpl::PostAudioMgmtEvent( CPacketThreadData* AudioPacket)
{
    HRESULT hr = S_OK;
    CScopedLock _lock( &m_mgmtLock );

    if( !PostClientAudioEvent( AudioPacket ) )
    {
        hr = E_FAIL;
    }

PostVideoEvent_Exit:
    return hr;
}

#endif

//CNetworkPacketSync:: ---------------------------------------------------------
CNetworkPacketSync::CNetworkPacketSync( UINT8 MaxVideoPackets, UINT8 MaxAudioPackets )
{
    m_pNpsImpl = new CNetworkPacketSyncImpl( MaxVideoPackets, MaxAudioPackets );
}

CNetworkPacketSync::~CNetworkPacketSync()
{
}

CNetworkPacketSync::CNetworkPacketSync(const CNetworkPacketSync& PacketSync )
{
}

STDMETHODIMP CNetworkPacketSync::Initialize( )
{
    return m_pNpsImpl->Initialize();
}

STDMETHODIMP CNetworkPacketSync::Deinitialize( )
{
    return m_pNpsImpl->Deinitialize();
}

CPacketThreadData* CNetworkPacketSync::GetVideoThreadData( void )
{
    return m_pNpsImpl->GetVideoThreadData();
}

CPacketThreadData* CNetworkPacketSync::GetManagementThreadData( void )
{
    return m_pNpsImpl->GetVideoThreadData();
}

STDMETHODIMP CNetworkPacketSync::PostVideoEvent( CPacketThreadData* VideoPacket)
{
    return m_pNpsImpl->PostVideoEvent( VideoPacket );
}

STDMETHODIMP CNetworkPacketSync::PostVideoMgmtEvent( CPacketThreadData* VideoPacket)
{
    return m_pNpsImpl->PostVideoMgmtEvent( VideoPacket );
}

#ifdef INCLUDE_AUDIO

CPacketThreadData* CNetworkPacketSync::GetAudioThreadData( void )
{
    return m_pNpsImpl->GetAudioThreadData();
}

STDMETHODIMP CNetworkPacketSync::PostAudioEvent( CPacketThreadData* AudioPacket)
{
    return m_pNpsImpl->PostAudioEvent( AudioPacket );
}

STDMETHODIMP CNetworkPacketSync::PostAudioMgmtEvent( CPacketThreadData* VideoPacket)
{
    return m_pNpsImpl->PostVideoMgmtEvent( VideoPacket );
}

#endif
