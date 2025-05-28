#include "ClientThreadpoolData.h"

CClientThreadpoolData::CClientThreadpoolData( UINT32 BufferLength )
: m_iPktCnt( 0 )
{
    Internal = 0;
    InternalHigh = 0;
    Offset = 0;
    OffsetHigh = 0;
    hEvent = WSACreateEvent();
    initializeBuffer( BufferLength );
}

CClientThreadpoolData::~CClientThreadpoolData()
{
    CloseHandle( this->hEvent );
}

WSABUF* CClientThreadpoolData::get_WSABUF( )
{
    return &m_pDataBuf;
}

PEVENT_HEADER CClientThreadpoolData::get_EventHeader( )
{
    return reinterpret_cast< PEVENT_HEADER >(
        m_pDataBuf.buf 
        );
}
PEVENT_PACKET_HEADER CClientThreadpoolData::get_EventPacketHeader( )
{
    return reinterpret_cast< PEVENT_PACKET_HEADER >(
        m_pDataBuf.buf 
        + MAX_EVENT_HEADER
        );
}
PPACKET_DATA_BLOB CClientThreadpoolData::get_PacketDataBlob( ) const
{
    return reinterpret_cast< PPACKET_DATA_BLOB >(
        m_pDataBuf.buf 
        + MAX_EVENT_HEADER
        + MAX_EVENT_PACKET_HEADER
        );
}

UINT32 CClientThreadpoolData::get_ActualPacketSize( ) 
{
    PEVENT_PACKET_HEADER pPh =
        this->get_EventPacketHeader(); 

    UINT32 packetSize =
        MAX_EVENT_HEADER
        + MAX_EVENT_PACKET_HEADER
        + pPh->dataBlobSize;

    return packetSize;
}

BOOL CClientThreadpoolData::operator < ( const CClientThreadpoolData&  right ) const
{
    BOOL fRc = FALSE;

    PPUBLISH_VIDEO_FRAME pVideoFrame = 
        reinterpret_cast< const PPUBLISH_VIDEO_FRAME >( this->get_PacketDataBlob() );
    PPUBLISH_VIDEO_FRAME pVideoFrameRight = 
        reinterpret_cast< PPUBLISH_VIDEO_FRAME >( right.get_PacketDataBlob() );

    fRc = pVideoFrameRight->packetCount < pVideoFrame->packetCount;

    return fRc;
}

VOID CClientThreadpoolData::initializeBuffer( UINT32 length )
{

    m_pDataBuf.len = length;

    m_iNumBuffers = 1;
    return;
}
