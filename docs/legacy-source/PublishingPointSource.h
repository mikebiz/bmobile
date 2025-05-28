#pragma once
#include "BlxContentServiceData.h"

#include "PacketInfoStructs.h"
//#include "PublishingPointSourceMs.h"
class CPublishingPointSourceMs;

class BLXCONTENTSERVICEDATA_API CPublishingPointSource
{
public:
	CPublishingPointSource( LPWSTR szConnectionString );
	~CPublishingPointSource(void);
	DWORD EstablishVideoSource(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_VIDEO_SOURCE pEstablishVideoSource
			//, ITransaction* pTxn
			);
	DWORD ReadVideoSource(		
			GUID ID
			, PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_VIDEO_SOURCE pEstablishVideoSource
			);
	DWORD RemoveVideoSource(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			);

	DWORD EstablishAudioSource(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_AUDIO_SOURCE pEstablishAudioSource
			);
	DWORD ReadAudioSource(
		GUID ID
		, PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PESTABLISH_AUDIO_SOURCE pEstablishAudioSource
		);
	DWORD RemoveAudioSource(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			);

private:
	CPublishingPointSourceMs* m_pPublishingPointSourceMs;

};

