#pragma once
#include "BlxContentServiceData.h"

#include "PacketInfoStructs.h"
//#include "PublishingPointDestinationMs.h"
class CPublishingPointDestinationMs;

class BLXCONTENTSERVICEDATA_API CPublishingPointDestination
{
public:
	CPublishingPointDestination( LPWSTR szConnectionString );
	~CPublishingPointDestination(void);

	DWORD EstablishVideoSubscription(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_VIDEO_SUBSCRIPTION pEstablishMediaDestination
			);

	DWORD ReadVideoSubscription(		
			GUID ID
			, PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_VIDEO_SUBSCRIPTION pEstablishVideoSubscription
			);

	DWORD RemoveMediaDestination(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			);

private:
	CPublishingPointDestinationMs* m_pPublishingPointDestinationMs;
};
