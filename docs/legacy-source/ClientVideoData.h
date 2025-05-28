#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

#include "BaseClientPacketDataSync.h"

class CClientReceiveThreadData;

#include "BlxHeap2.hpp"
#include "SyncQueue.h"

class BLXWARE_API CClientVideoData
	: public CBaseClientPacketDataSync
	,public CBlxHeap2< CClientVideoData >
{
	friend class CBlxHeap2< CClientVideoData >;

public:
	CClientVideoData( );
	CClientVideoData( UINT32 BufferLength );
	~CClientVideoData();
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
    VOID MorphPackets( PEVENT_HEADER, PEVENT_PACKET_HEADER, PPACKET_DATA_BLOB, CClientReceiveThreadData* );

protected:

	static HANDLE m_hHeap;
	static LONG m_uNumAllocsInHeap;
	static CComCriticalSection* m_csLock;
	WSABUF*			m_pDataBuf;

	void reallocBuffer( UINT32 length );

private:

	void InternalNew( UINT32 BufferLength);
};

typedef Blxware::SyncQueue< CClientVideoData* > QClientVideoData;
typedef QClientVideoData* PQClientVideoData;