#pragma once

#include "BlxCommonLib.h"
#include "BaseClientPacketDataSync.h"
#include "PacketInfoStructs.h"
#include "SyncQueue.h"

#include "BlxHeap2.hpp"

class BLXWARE_API CClientReceiveThreadData
	: public CBaseClientPacketDataSync
	, public CBlxHeap2< CClientReceiveThreadData >
{
	friend class CBlxHeap2< CClientReceiveThreadData >;

public:
	CClientReceiveThreadData( );
	CClientReceiveThreadData( UINT32 BufferLength );
	~CClientReceiveThreadData();
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
protected:
};

typedef Blxware::SyncQueue< CClientReceiveThreadData* > QClientReceiveThreadData;
typedef QClientReceiveThreadData* PQClientReceiveThreadData;
