#pragma once

#include "BlxCommonLib.h"

class CBaseClientPacketDataSync	:
					public WSAOVERLAPPED
{
public:
	CBaseClientPacketDataSync();
	~CBaseClientPacketDataSync();
	void initializeWsaOverlapped(WSAOVERLAPPED* pWsa, bool bReset);
};

