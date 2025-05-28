#include "ServerVideoSource.h"
#include "IpHeader.h"

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
HANDLE CServerVideoSource::get_Heap()
{
    return m_hHeap;
}
void CServerVideoSource::set_Heap( HANDLE hHeap)
{
    m_hHeap = hHeap;
}
HANDLE CServerVideoSource::delete_Heap()
{
    m_hHeap = NULL;
    return m_hHeap;
}

UINT CServerVideoSource::get_NumAllocsInHeap()
{
    return m_uNumAllocsInHeap;
}
void CServerVideoSource::set_NumAllocsInHeap( UINT uNumAllocsInHeap )
{
    m_uNumAllocsInHeap = uNumAllocsInHeap;
}
void CServerVideoSource::increment_NumAllocsInHeap( )
{
    m_uNumAllocsInHeap++;
}
void CServerVideoSource::decrement_NumAllocsInHeap( )
{
    m_uNumAllocsInHeap--;
}
