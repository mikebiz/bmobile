#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

#include "BlxHeap2.hpp"

class BLXWARE_API CServerPacketThreadData
    : public WSAOVERLAPPED
    , public CBlxHeap2< CServerPacketThreadData >
{
    friend class CBlxHeap2< CServerPacketThreadData >;
public:

    explicit CServerPacketThreadData( );
    explicit CServerPacketThreadData( USHORT BufferLength );
    explicit CServerPacketThreadData( USHORT BufferCount, DWORD Flag );
    explicit CServerPacketThreadData( PIP_HEADER pIpHeader
        , PUDP_HEADER pUdpHeader
        , PEVENT_HEADER pEventHeader
        , PEVENT_PACKET_HEADER pEventPacketHeader
        , PPACKET_DATA_BLOB pPacketDataBlob
        );
    explicit CServerPacketThreadData( UINT32 BufferLength );
    explicit CServerPacketThreadData( UINT32 BufferLength, DWORD dwN1, DWORD dwN2 );
    ~CServerPacketThreadData();

    PVOID						m_pContext;
    INT32			            m_iPktCnt;
    UINT32						m_uiHeapPriority;
    SOCKADDR_STORAGE			m_safrom;
    INT32						m_fromlen;
    //	HANDLE						m_hBufferEvent;

    WSABUF* get_WSABUF( );
    VOID internalRefNew( 
        PIP_HEADER pIpHeader
        , PUDP_HEADER pUdpHeader
        , PEVENT_HEADER pEventHeader
        , PEVENT_PACKET_HEADER pEventPacketHeader
        , PPACKET_DATA_BLOB pPacketDataBlob
        );
    PVOID			m_pBasePin;
    PIP_HEADER get_IpHeader( );
    PUDP_HEADER get_UdpHeader( );
    PEVENT_HEADER get_EventHeader( );
    PEVENT_PACKET_HEADER get_EventPacketHeader( );
    PPACKET_DATA_BLOB get_PacketDataBlob( );
protected:
    WSABUF*			m_pDataBuf;
    INT32			m_iNumBuffers;

    //explicit CBaseThreadDataSync( UINT32 BufferLength );

    static HANDLE m_hHeap;
    static LONG m_uNumAllocsInHeap;
    static CComCriticalSection* m_csLock;

    VOID internalNew( 
        UINT32 BufferLength0
        , UINT32 BufferLength1
        , UINT32 BufferLength2
        , UINT32 BufferLength3
        , UINT32 BufferLength4
        );
    VOID initializeBuffer( UINT32 length );
    VOID initializeBuffer( UINT32 length, SHORT iteration );

private:
    CServerPacketThreadData( CServerPacketThreadData& in ){};

};
