#include "ClientPacketThreadData.h"

CClientPacketThreadData::CClientPacketThreadData()
: m_pFilter( NULL )
{
	m_hFrameEvent = WSACreateEvent();
}

VOID CClientPacketThreadData::set_Filter(
	PVOID Filter
	)
{
	m_pFilter = Filter;
	return;
}

PVOID CClientPacketThreadData::get_Filter( )
{
	return m_pFilter;
}
