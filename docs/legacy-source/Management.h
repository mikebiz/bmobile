#pragma once
#include "BlxContentServiceData.h"

#include "PacketInfoStructs.h"
//#include "ManagementMs.h"
class CManagementMs;

class BLXCONTENTSERVICEDATA_API CManagement
{
public:
	CManagement( LPWSTR szConnectionString );
	~CManagement(void);

	DWORD LogKeepAlive(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			);

private:
	CManagementMs* m_pManagementMs;

};