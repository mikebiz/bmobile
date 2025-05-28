#include "ServerAudioSource.h"
#include "IpHeader.h"

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
HANDLE CServerAudioSource::get_Heap()
{
	return m_hHeap;
}
void CServerAudioSource::set_Heap( HANDLE hHeap)
{
	m_hHeap = hHeap;
}
HANDLE CServerAudioSource::delete_Heap()
{
	m_hHeap = NULL;
	return m_hHeap;
}

UINT CServerAudioSource::get_NumAllocsInHeap()
{
	return m_uNumAllocsInHeap;
}
void CServerAudioSource::set_NumAllocsInHeap( UINT uNumAllocsInHeap )
{
	m_uNumAllocsInHeap = uNumAllocsInHeap;
}
void CServerAudioSource::increment_NumAllocsInHeap( )
{
	m_uNumAllocsInHeap++;
}
void CServerAudioSource::decrement_NumAllocsInHeap( )
{
	m_uNumAllocsInHeap--;
}
