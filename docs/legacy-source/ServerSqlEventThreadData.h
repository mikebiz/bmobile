#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

#include "BlxHeap2.hpp"

#include <mq.h>
#include <transact.h>

class BLXWARE_API CServerSqlEventThreadData
    : public WSAOVERLAPPED
    , public CBlxHeap2< CServerSqlEventThreadData >
{
    friend class CBlxHeap2< CServerSqlEventThreadData >;
public:
    explicit CServerSqlEventThreadData( );
    explicit CServerSqlEventThreadData( USHORT BufferLength );
    explicit CServerSqlEventThreadData( USHORT BufferCount, DWORD Flag );
    explicit CServerSqlEventThreadData( PIP_HEADER pIpHeader
        , PUDP_HEADER pUdpHeader
        , PEVENT_HEADER pEventHeader
        , PEVENT_PACKET_HEADER pEventPacketHeader
        , PPACKET_DATA_BLOB pPacketDataBlob
        );
    explicit CServerSqlEventThreadData( UINT32 BufferLength );
    explicit CServerSqlEventThreadData( UINT32 BufferLength, DWORD dwN1, DWORD dwN2 );
    ~CServerSqlEventThreadData();

    PVOID						m_pContext;
    INT32			            m_iPktCnt;
    UINT32						m_uiHeapPriority;
    SOCKADDR_STORAGE			m_safrom;
    INT32						m_fromlen;
    SQL_SERVER_TRIGGER_EVENT	m_sqlTriggerEvent;
    ITransaction*				m_pTxnObj;

    MQMSGPROPS					m_msgprops;
    MSGPROPID					m_propidarray[PROP_NUM];
    MQPROPVARIANT				m_propvararray[PROP_NUM];
    HRESULT						m_status[PROP_NUM];

#ifdef _VISTA_
    PTP_WORK		m_pWorkItem;
#endif

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

    VOID ReallocBuffer( USHORT usNewSize );

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
    CServerSqlEventThreadData( CServerSqlEventThreadData& in ){};
    VOID InitializeQueue();
    VOID reallocBuffer( UINT32 length );
};
