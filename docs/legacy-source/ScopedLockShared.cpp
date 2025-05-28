#include "ScopedLockShared.h"

CScopedLockShared::CScopedLockShared( CBlxLock* BlxLock )
	: m_pLock( BlxLock )
{
#ifdef _VISTA_
	m_pLock->LockShared();
#else
#endif
}

CScopedLockShared::~CScopedLockShared(void)
{
#ifdef _VISTA_
	m_pLock->ReleaseShared();
	m_pLock = NULL;
#else
#endif
}
