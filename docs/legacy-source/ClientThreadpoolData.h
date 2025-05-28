#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

class BLXWARE_API CClientThreadpoolData 
	: public WSAOVERLAPPED
{
public:

	INT32			m_iPktCnt;

	explicit CClientThreadpoolData( UINT32 BufferLength );
	~CClientThreadpoolData();
	WSABUF* get_WSABUF( );
	PEVENT_HEADER get_EventHeader( );
	PEVENT_PACKET_HEADER get_EventPacketHeader( );
	PPACKET_DATA_BLOB get_PacketDataBlob( ) const;
	UINT32 get_ActualPacketSize( ) ;
	BOOL operator < ( const CClientThreadpoolData&  right ) const;

private:
	WSABUF			m_pDataBuf;
	INT32			m_iNumBuffers;

	CClientThreadpoolData( CClientThreadpoolData& in ){};
	CClientThreadpoolData( ){};


	VOID initializeBuffer( UINT32 length );
};
