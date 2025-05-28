#include "ScopedLock.h"

CScopedLock::CScopedLock( CBlxLock* BlxLock )
	: m_pLock( BlxLock )
{
	m_pLock->Lock();
}

CScopedLock::~CScopedLock(void)
{
	m_pLock->Release();
	m_pLock = NULL;
}
