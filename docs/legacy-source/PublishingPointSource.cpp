
#include "PublishingPointSourceMs.h"
#include "PublishingPointSource.h"

CPublishingPointSource::CPublishingPointSource( LPWSTR szConnectionString )
{
	m_pPublishingPointSourceMs = new CPublishingPointSourceMs( szConnectionString );
}

CPublishingPointSource::~CPublishingPointSource(void)
{
}

DWORD CPublishingPointSource::EstablishVideoSource(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_VIDEO_SOURCE pEstablishVideoSource
			//, ITransaction* pTxn
			)
{
	DWORD dwErrorCode = 0;
	dwErrorCode = m_pPublishingPointSourceMs->EstablishVideoSource(
						pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pEstablishVideoSource
						//, pTxn
						);

	return dwErrorCode;
}

DWORD CPublishingPointSource::ReadVideoSource(
			GUID ID
			, PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_VIDEO_SOURCE pEstablishVideoSource
			)
{
	DWORD dwErrorCode = 0;

	dwErrorCode = m_pPublishingPointSourceMs->ReadVideoSource(
						ID
						, pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pEstablishVideoSource
						);

	return dwErrorCode;
}

DWORD CPublishingPointSource::RemoveVideoSource(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			)
{
	DWORD dwErrorCode = 0;

	dwErrorCode = m_pPublishingPointSourceMs->RemoveVideoSource(
						pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pPacketDataBlob
						);

	return dwErrorCode;
}

DWORD CPublishingPointSource::EstablishAudioSource(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_AUDIO_SOURCE pEstablishAudioSource
			)
{
	DWORD dwErrorCode = 0;

	dwErrorCode = m_pPublishingPointSourceMs->EstablishAudioSource(
						pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pEstablishAudioSource
						);


	return dwErrorCode;
}

DWORD CPublishingPointSource::ReadAudioSource(
			GUID ID
			, PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_AUDIO_SOURCE pEstablishAudioSource
			)
{
	DWORD dwErrorCode = 0;

	dwErrorCode = m_pPublishingPointSourceMs->ReadAudioSource(
						ID
						, pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pEstablishAudioSource
						);

	return dwErrorCode;
}

DWORD CPublishingPointSource::RemoveAudioSource(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			)
{
	DWORD dwErrorCode = 0;

	dwErrorCode = m_pPublishingPointSourceMs->RemoveAudioSource(
						pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pPacketDataBlob
						);

	return dwErrorCode;
}
