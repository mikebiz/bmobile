#include "PacketThreadData.h"

class CClientPacketThreadData
	: public CPacketThreadData
{
public:

	PVOID			m_pFilter;
	HANDLE			m_hFrameEvent;

	CClientPacketThreadData();
	VOID set_Filter(
		PVOID Filter
		);
	PVOID get_Filter( );
};
