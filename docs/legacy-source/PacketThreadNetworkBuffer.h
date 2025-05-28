#pragma once

#include "BlxCommonLib.h"
#include "BaseClientPacketDataSync.h"
#include "PacketInfoStructs.h"

#include "BmobileHeap.hpp"
#include "BlxLock.h"

#include "PacketThreadData.h"

class BLXWARE_API CPacketThreadNetworkBuffer : public CBaseClientPacketDataSync
	,public Bmobile::System::CHeap< CPacketThreadNetworkBuffer >
{
	friend class Bmobile::System::CHeap< CPacketThreadNetworkBuffer >;

public:

	enum DGRAM_PACKET_HEADER_SEQUENCE : UINT16
	{
		PHS_EVENT_HEADER			    = 0
		, PHS_EVENT_PACKET_HEADER	    = 1
		, PHS_NETWORK_BUFFER_PACKETS    = 2
	};

    explicit CPacketThreadNetworkBuffer( UINT8 NumberNetworkPackets );
	~CPacketThreadNetworkBuffer();
	WSABUF* get_WSABUF( );
	UINT8 get_numBufs();
    UINT8 get_currentBufferCount();
    CPacketThreadData* get_packetThreadData( UINT8 Packet );
    BOOL PushBuffer( CPacketThreadData* ThreadData );
    BOOL PopBuffer( CPacketThreadData* ThreadData );
    BOOL ClearBuffer( );

	PEVENT_HEADER get_EventHeader( );
	PEVENT_PACKET_HEADER get_EventPacketHeader( );
	PNETWORK_BUFFER_PACKETS get_NetworkBufferPackets( );
    BOOL set_ActualPacketDataBlobLength(const USHORT Length);

protected:

	static HANDLE               m_hHeap;
	static LONG                 m_uNumAllocsInHeap;
	static CComAutoCriticalSection* m_csLock;
	WSABUF*			            m_pDataBuf;
	INT32			            m_iNumBuffers;

private:
    CBlxLock                    m_classLock;
    UINT8                       m_currentBuffer;
    UINT8                       m_bufferPackets;

    std::vector<CPacketThreadData*> m_threadData;

	CPacketThreadNetworkBuffer( CPacketThreadNetworkBuffer& in ){};
	void initializeBuffer( UINT32 length, SHORT iteration );
	void internalNew( 
		UINT32 BufferLength0
		, UINT32 BufferLength1
		, UINT32 BufferLength2
        , UINT8 NumberNetworkPackets
		);
};

typedef Blxware::SyncQueue< CPacketThreadNetworkBuffer* > QCPacketThreadNetworkBuffer;
typedef QCPacketThreadNetworkBuffer* PQCPacketThreadNetworkBuffer;