#pragma once

#include "BlxCommonLib.h"
#include "PacketInfoStructs.h"

#include "BlxHeap.hpp"

class BLXWARE_API CIpHeader
	: public CBlxHeap< CIpHeader >
{
		friend class CBlxHeap< CIpHeader >;
public:
	IP_HEADER					m_ipHeader;
	UDP_HEADER					m_udpHeader;
	CHAR						m_szMACAddress[ MACADDRESS_LENGTH ];

	CIpHeader(void);
	CIpHeader( const IP_HEADER& ipHeader, const UDP_HEADER& udpHeader, LPCSTR macAddress );
	CIpHeader( const CIpHeader& ipHeader );
	BOOL Copy( const IP_HEADER& ipHeader, const UDP_HEADER& udpHeader, LPCSTR macAddress );
	~CIpHeader(void);

	static HANDLE get_Heap();
	static void set_Heap( HANDLE hHeap);
	static HANDLE delete_Heap();
	static UINT get_NumAllocsInHeap();
	static void set_NumAllocsInHeap( UINT uNumAllocsInHeap );
	static void increment_NumAllocsInHeap( );
	static void decrement_NumAllocsInHeap( );

private:

	static HANDLE m_hHeap;
	static UINT m_uNumAllocsInHeap;

};
