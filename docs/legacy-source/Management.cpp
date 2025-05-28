
#include "Management.h"
#include "ManagementMs.h"

CManagement::CManagement( LPWSTR szConnectionString )
{
	m_pManagementMs = new CManagementMs( szConnectionString );
}

CManagement::~CManagement(void)
{
	if( m_pManagementMs )
	{
		delete m_pManagementMs;
	}
}

DWORD CManagement::LogKeepAlive(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			)
{
	DWORD dwErrorCode = 0;
	dwErrorCode = m_pManagementMs->LogKeepAlive(
						pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pPacketDataBlob
						);

	return dwErrorCode;
}

