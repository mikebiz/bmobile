#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

#include "BlxHeap.hpp"

class CIpHeader;

class BLXWARE_API CServerAudioSource
	: public CBlxHeap< CServerAudioSource >
{
	friend class CBlxHeap< CServerAudioSource >;
public:
	const CIpHeader*			m_pIpHeader;
	EVENT_HEADER				m_eventHeader;
	EVENT_PACKET_HEADER			m_eventPacketHeader;
	ESTABLISH_AUDIO_SOURCE		m_audioSource;

	CServerAudioSource(void);
	CServerAudioSource( 
		const CIpHeader* pIpHeader
		, const EVENT_HEADER& eventHeader
		, const EVENT_PACKET_HEADER& eventPacketHeader
		, const ESTABLISH_AUDIO_SOURCE& estAudioSource 
		);
	CServerAudioSource( const CServerAudioSource& serverMediaSource );
	BOOL Copy( 
		const EVENT_HEADER& eventHeader
		, const EVENT_PACKET_HEADER& eventPacketHeader
		,const ESTABLISH_AUDIO_SOURCE& estAudioSource 
		);
	~CServerAudioSource(void);

	static HANDLE get_Heap();
	static void set_Heap( HANDLE hHeap);
	static HANDLE delete_Heap();
	static UINT get_NumAllocsInHeap();
	static void set_NumAllocsInHeap( UINT uNumAllocsInHeap );
	static void increment_NumAllocsInHeap( );
	static void decrement_NumAllocsInHeap( );

private:

	static HANDLE m_hHeap;
	static UINT m_uNumAllocsInHeap;

};
