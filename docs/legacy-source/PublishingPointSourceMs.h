#pragma once

#ifdef _MANAGED
#include <vcclr.h>
using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;
using namespace System::EnterpriseServices;
using namespace System::Runtime::InteropServices;
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "PacketInfoStructs.h"

class CPublishingPointSourceMs
{
public:
	CPublishingPointSourceMs( LPWSTR szConnectionString );
	~CPublishingPointSourceMs(void);
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
	static const UINT16			PPS_MAX_COMMAND_ENTRIES = 6;

#ifdef _MANAGED
	gcroot<System::String^>				m_ConnStr;
#else
		intptr_t				m_ConnStr;
#endif

enum PPS_COMMAND_ENTRIES : UINT16
{
	PPS_ESTABLISH_VIDEO_SOURCE				= 0
	, PPS_ESTABLISH_AUDIO_SOURCE			= 1
	, PPS_READ_PPT_VIDEO_SOURCES			= 2
	, PPS_READ_PPT_AUDIO_SOURCES			= 3
	, PPS_REMOVE_PPT_VIDEO_SOURCE			= 4
	, PPS_REMOVE_PPT_AUDIO_SOURCE			= 5
};

#if defined(_MANAGED)
	gcroot<cli::array< ::System::Data::IDbCommand^ >^ > m_commandCollection;
#else
	intptr_t	m_commandCollection;
#endif
};

