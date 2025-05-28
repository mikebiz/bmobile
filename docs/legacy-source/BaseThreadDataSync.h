#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"
#include "BaseClientPacketDataSync.h"

#include "BlxHeap2.hpp"

class BLXWARE_API CBaseThreadDataSync 
	: public WSAOVERLAPPED
	, public CBlxHeap2< CBaseThreadDataSync >
{
		friend class CBlxHeap2< CBaseThreadDataSync >;
public:

	INT32			m_iPktCnt;
	UINT32			m_uiHeapPriority;

	explicit CBaseThreadDataSync();
	explicit CBaseThreadDataSync( PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PPACKET_DATA_BLOB pPacketDataBlob
		);
	explicit CBaseThreadDataSync( UINT32 BufferLength );
	explicit CBaseThreadDataSync( USHORT BufferCount, DWORD Flag );
	explicit CBaseThreadDataSync( UINT32 BufferLength, DWORD dwN1, DWORD dwN2 );
	~CBaseThreadDataSync();
	WSABUF* get_WSABUF( );
	VOID internalRefNew( 
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			);
	PVOID			m_pBasePin;

protected:
	WSABUF*			m_pDataBuf;
	INT32			m_iNumBuffers;
	
	//explicit CBaseThreadDataSync( UINT32 BufferLength );

	static HANDLE m_hHeap;
	static LONG m_uNumAllocsInHeap;
	static CComCriticalSection* m_csLock;

private:
	CBaseThreadDataSync( CBaseThreadDataSync& in ){};

protected:
	VOID CBaseThreadDataSync::internalNew( 
		UINT32 BufferLength0
		, UINT32 BufferLength1
		, UINT32 BufferLength2
		, UINT32 BufferLength3
		, UINT32 BufferLength4
		);
	VOID CBaseThreadDataSync::initializeBuffer( UINT32 length, SHORT iteration );
	VOID CBaseThreadDataSync::initializeBuffer( UINT32 length );
};
