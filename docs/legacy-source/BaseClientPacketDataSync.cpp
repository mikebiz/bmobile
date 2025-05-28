#include "BaseClientPacketDataSync.h"

CBaseClientPacketDataSync::CBaseClientPacketDataSync()
{
	initializeWsaOverlapped(this, false);
}

CBaseClientPacketDataSync::~CBaseClientPacketDataSync()
{
	if( INVALID_HANDLE_VALUE != hEvent )
	{
		CloseHandle( this->hEvent );
	}
}

void CBaseClientPacketDataSync::initializeWsaOverlapped(WSAOVERLAPPED* pWsa, bool bReset)
{
	pWsa->Internal = 0;
	pWsa->InternalHigh = 0;
	pWsa->Offset = 0;
	pWsa->OffsetHigh = 0;
	if (bReset)
		WSAResetEvent(pWsa->hEvent);
	else
		pWsa->hEvent = WSACreateEvent();

}
