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

class CManagementMs
{
public:
	CManagementMs( LPWSTR szConnectionString );
	~CManagementMs(void);

	DWORD LogKeepAlive(
			PIP_HEADER pIpHeader
			, PUDP_HEADER pUdpHeader
			, PEVENT_HEADER pEventHeader
			, PEVENT_PACKET_HEADER pEventPacketHeader
			, PPACKET_DATA_BLOB pPacketDataBlob
			);
private:
	static const UINT16			MGT_MAX_COMMAND_ENTRIES = 1;

#ifdef _MANAGED
	gcroot<System::String^>				m_ConnStr;
#else
	intptr_t							m_ConnStr;
#endif

enum MGT_COMMAND_ENTRIES : UINT16
{
	MGT_LOG_KEEP_ALIVE							= 0
};

#if defined(_MANAGED)
	gcroot<cli::array< ::System::Data::IDbCommand^ >^ > m_commandCollection;
#else
	intptr_t	m_commandCollection;
#endif
};

