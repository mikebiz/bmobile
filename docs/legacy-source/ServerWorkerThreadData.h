#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

#include "BlxHeap2.hpp"

class CServerWorkerThreadData 
	: public WSAOVERLAPPED
	, public CBlxHeap2< CServerWorkerThreadData >
{
		friend class CBlxHeap2< CServerWorkerThreadData >;
public:
	CServerWorkerThreadData( UINT32 PacketSize );
	~CServerWorkerThreadData();
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

	BLXWARE_API static HANDLE m_hHeap;
	BLXWARE_API static LONG m_uNumAllocsInHeap;
	BLXWARE_API static CComCriticalSection* m_csLock;

	VOID CServerWorkerThreadData::initializeBuffer( UINT32 length );
	VOID CServerWorkerThreadData::reallocBuffer( USHORT length );
};
