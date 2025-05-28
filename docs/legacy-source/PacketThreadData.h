#pragma once

#include "ClientSendThreadData.h"
#include "SyncQueue.h"

class BLXWARE_API CPacketThreadData
: public CClientSendThreadData
{
public:

	DWORD get_CommonHeaderSize();
	DWORD get_DomainHeaderSize();
};


typedef Blxware::SyncQueue< CPacketThreadData* > QCPacketThreadData;
typedef QCPacketThreadData* PQCPacketThreadData;