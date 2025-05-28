#pragma once

#include "BlxCommonLib.h"
#include "BaseClientPacketDataSync.h"
#include "PacketInfoStructs.h"

#include "BlxHeap2.hpp"
#include "SyncQueue.h"

class BLXWARE_API CClientSendThreadData : public CBaseClientPacketDataSync
	,public CBlxHeap2< CClientSendThreadData >
{
	friend class CBlxHeap2< CClientSendThreadData >;

public:
	enum DGRAM_PACKET_HEADER_SEQUENCE : UINT16
	{
		PHS_EVENT_HEADER			= 0
		, PHS_EVENT_PACKET_HEADER	= 1
		, PHS_PACKET_DATA_BLOB		= 2
	};

	explicit CClientSendThreadData();
	~CClientSendThreadData();
	WSABUF* get_WSABUF( );
	UINT8 get_numBufs();
	BOOL set_ActualPacketDataBlobLength(const USHORT Length);
	PEVENT_HEADER get_EventHeader( );
	PEVENT_PACKET_HEADER get_EventPacketHeader( );
	PPACKET_DATA_BLOB get_PacketDataBlob( );

protected:

	static HANDLE m_hHeap;
	static LONG m_uNumAllocsInHeap;
	static CComCriticalSection* m_csLock;
	WSABUF*			m_pDataBuf;
	INT32			m_iNumBuffers;

private:
	CClientSendThreadData( CClientSendThreadData& in ){};
	void initializeBuffer( UINT32 length, SHORT iteration );
	void internalNew( 
		UINT32 BufferLength0
		, UINT32 BufferLength1
		, UINT32 BufferLength2
		);
};


typedef Blxware::SyncQueue< CClientSendThreadData* > QClientSendThreadData;
typedef QClientSendThreadData* PQClientSendThreadData;