#include "IpHeader.h"

HANDLE CIpHeader::m_hHeap = NULL;
UINT CIpHeader::m_uNumAllocsInHeap = 0;

CIpHeader::CIpHeader(void)
{
}

CIpHeader::CIpHeader( const IP_HEADER& ipHeader, const UDP_HEADER& udpHeader, LPCSTR macAddress )
{
	this->Copy(
		ipHeader
		, udpHeader 
		, macAddress
		);
}

CIpHeader::CIpHeader( const CIpHeader& ipHeader )
{
	this->Copy(
		ipHeader.m_ipHeader
		, ipHeader.m_udpHeader 
		, ipHeader.m_szMACAddress 
		);
}

CIpHeader::~CIpHeader(void)
{
	return;
}

BOOL CIpHeader::Copy( const IP_HEADER& ipHeader, const UDP_HEADER& udpHeader, LPCSTR macAddress )
{
	BOOL fRc = TRUE;

	::CopyMemory(
		&this->m_ipHeader
		, &ipHeader
		, sizeof( IP_HEADER )
		);

	::CopyMemory(
		&this->m_udpHeader
		, &udpHeader
		, sizeof( UDP_HEADER )
		);

	sprintf_s(
		this->m_szMACAddress 
		, macAddress
		);

	return fRc;
}
HANDLE CIpHeader::get_Heap()
{
	return m_hHeap;
}
void CIpHeader::set_Heap( HANDLE hHeap)
{
	m_hHeap = hHeap;
}
HANDLE CIpHeader::delete_Heap()
{
	m_hHeap = NULL;
	return m_hHeap;
}

UINT CIpHeader::get_NumAllocsInHeap()
{
	return m_uNumAllocsInHeap;
}
void CIpHeader::set_NumAllocsInHeap( UINT uNumAllocsInHeap )
{
	m_uNumAllocsInHeap = uNumAllocsInHeap;
}
void CIpHeader::increment_NumAllocsInHeap( )
{
	m_uNumAllocsInHeap++;
}
void CIpHeader::decrement_NumAllocsInHeap( )
{
	m_uNumAllocsInHeap--;
}
