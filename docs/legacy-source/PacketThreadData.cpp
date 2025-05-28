#include "PacketThreadData.h"

DWORD CPacketThreadData::get_CommonHeaderSize()
{
	return MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		;
}
DWORD CPacketThreadData::get_DomainHeaderSize()
{
	return MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		;
}
