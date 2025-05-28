#pragma once

#ifdef _MANAGED
#include <vcclr.h>
using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;
using namespace System::Runtime::InteropServices;
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "PacketInfoStructs.h"

class CPublishingPointDestinationMs
{
public:
	CPublishingPointDestinationMs( LPWSTR szConnectionString );
	~CPublishingPointDestinationMs(void);

	DWORD EstablishVideoSubscription(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PESTABLISH_VIDEO_SUBSCRIPTION pEstablishVideoSubscription
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
	static const UINT16			PPS_MAX_COMMAND_ENTRIES = 3;

#ifdef _MANAGED
	gcroot<System::String^>				m_ConnStr;
#else
		intptr_t				m_ConnStr;
#endif

enum PPS_COMMAND_ENTRIES : UINT16
{
	PPS_ESTABLISH_VIDEO_SUBSCRIPTION			= 0
	, PPS_READ_PPT_VIDEO_SUBSCRIPTIONS			= 1
	, PPS_REMOVE_PPT_VIDEO_SUBSCRIPTIONS		= 2
};

#if defined(_MANAGED)
	gcroot<cli::array< ::System::Data::IDbCommand^ >^ > m_commandCollection;
#else
	intptr_t	m_commandCollection;
#endif
};

