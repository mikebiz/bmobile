#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

#include "BlxHeap.hpp"

class CIpHeader;

class BLXWARE_API CServerVideoSource
	: public CBlxHeap< CServerVideoSource >
{
		friend class CBlxHeap< CServerVideoSource >;
public:
	const CIpHeader*			m_pIpHeader;
	EVENT_HEADER				m_eventHeader;
	EVENT_PACKET_HEADER			m_eventPacketHeader;
	ESTABLISH_VIDEO_SOURCE		m_videoSource;

	CServerVideoSource(void);
	CServerVideoSource( 
		const CIpHeader* pIpHeader
		, const EVENT_HEADER& eventHeader
		, const EVENT_PACKET_HEADER& eventPacketHeader
		, const ESTABLISH_VIDEO_SOURCE& estVideoSource 
		);
	CServerVideoSource( const CServerVideoSource& serverMediaSource );
	BOOL Copy( 
		const EVENT_HEADER& eventHeader
		, const EVENT_PACKET_HEADER& eventPacketHeader
		, const ESTABLISH_VIDEO_SOURCE& estVideoSource 
		);
	~CServerVideoSource(void);

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

