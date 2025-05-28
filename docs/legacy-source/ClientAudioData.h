#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

#include "BaseClientPacketDataSync.h"

class CClientReceiveThreadData;

#include "BlxHeap2.hpp"
#include "SyncQueue.h"

class BLXWARE_API CClientAudioData
	: public CBaseClientPacketDataSync
	,public CBlxHeap2< CClientAudioData >
{
	friend class CBlxHeap2< CClientAudioData >;

public:
	CClientAudioData( );
	CClientAudioData( UINT32 BufferLength );
	~CClientAudioData();
	PVOID			m_pBasePin;

	#ifdef _VISTA_
		PTP_WORK		m_pWorkItem;
	#endif

	WSABUF* get_WSABUF( );
	PEVENT_HEADER get_EventHeader( );
	PEVENT_PACKET_HEADER get_EventPacketHeader( );
	PPACKET_DATA_BLOB get_PacketDataBlob( ) const;
	UINT32 get_ActualPacketSize( );
	VOID Morph( CClientReceiveThreadData* pThreadpoolData );

protected:

	static HANDLE m_hHeap;
	static LONG m_uNumAllocsInHeap;
	static CComCriticalSection* m_csLock;
	WSABUF*			m_pDataBuf;

	void reallocBuffer( UINT32 length );

private:

	void InternalNew( UINT32 BufferLength);
};

typedef Blxware::SyncQueue< CClientAudioData* > QClientAudioData;
typedef QClientAudioData* PQClientAudioData;
