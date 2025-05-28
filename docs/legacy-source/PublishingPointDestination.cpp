
#include "PublishingPointDestination.h"
#include "PublishingPointDestinationMs.h"

CPublishingPointDestination::CPublishingPointDestination( LPWSTR szConnectionString )
{
	m_pPublishingPointDestinationMs = new CPublishingPointDestinationMs( szConnectionString );
}

CPublishingPointDestination::~CPublishingPointDestination(void)
{
	if( m_pPublishingPointDestinationMs )
	{
		delete m_pPublishingPointDestinationMs;
	}
}

DWORD CPublishingPointDestination::EstablishVideoSubscription(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_VIDEO_SUBSCRIPTION pEstablishVideoSubscription
			)
{
	DWORD dwErrorCode = 0;
	dwErrorCode = m_pPublishingPointDestinationMs->EstablishVideoSubscription(
						pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pEstablishVideoSubscription
						);

	return dwErrorCode;
}

DWORD CPublishingPointDestination::ReadVideoSubscription(
			GUID ID
			, PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_VIDEO_SUBSCRIPTION pEstablishVideoSubscription
			)
{
	DWORD dwErrorCode = 0;

	dwErrorCode = m_pPublishingPointDestinationMs->ReadVideoSubscription(
						ID
						, pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pEstablishVideoSubscription
						);

	return dwErrorCode;
}

DWORD CPublishingPointDestination::RemoveMediaDestination(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			)
{
	DWORD dwErrorCode = 0;
	dwErrorCode = m_pPublishingPointDestinationMs->RemoveMediaDestination(
						pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pPacketDataBlob
						);

	return dwErrorCode;
}
