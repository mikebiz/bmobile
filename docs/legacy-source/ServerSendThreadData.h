#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

#include "BlxHeap2.hpp"

class BLXWARE_API CServerSendThreadData 
	: public WSAOVERLAPPED
	, public CBlxHeap2< CServerSendThreadData >
{
		friend class CBlxHeap2< CServerSendThreadData >;
public:
	CServerSendThreadData( UINT32 PacketSize );
	~CServerSendThreadData();
	inline VOID IngestNetworkPacket( PCHAR pReceiveData, USHORT usLength );
	inline VOID ReallocBuffer( USHORT usLength );
	PIP_HEADER get_IpHeader( );
	PUDP_HEADER get_UdpHeader( );
	PEVENT_HEADER get_EventHeader( );
	PEVENT_PACKET_HEADER get_EventPacketHeader( );
	PPACKET_DATA_BLOB get_PacketDataBlob( );
	WSABUF* get_WSABUF( );

	PVOID			m_pContext;
	UINT32			m_uiHeapPriority;
	SOCKADDR_IN		m_socketAddrSrvVideo;

	INT32			m_iPktCnt;

protected:
	WSABUF*						m_pDataBuf;
	LONG						m_iNumBuffers;

	static HANDLE m_hHeap;
	static LONG m_uNumAllocsInHeap;
	static CComCriticalSection* m_csLock;

	VOID initializeBuffer( UINT32 length );
	VOID reallocBuffer( USHORT length );
};

