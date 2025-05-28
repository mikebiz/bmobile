#include "BlxLock.h"

CBlxLock::CBlxLock(void)
{
#ifdef _VISTA_ && #ifndef _USE_CS_
	InitializeSRWLock( &m_Lock );
#else
	m_Lock.Init();
#endif
}

CBlxLock::~CBlxLock(void)
{
#ifdef _VISTA_ && #ifndef _USE_CS_
#else
	m_Lock.Term();
#endif
}

VOID CBlxLock::Lock()
{
#ifdef _VISTA_ && #ifndef _USE_CS_
	AcquireSRWLockExclusive( &m_Lock );
#else
	m_Lock.Lock();
#endif
}

VOID CBlxLock::LockShared()
{
#ifdef _VISTA_ && #ifndef _USE_CS_
	AcquireSRWLockShared( &m_Lock );
#else
	m_Lock.Lock();
#endif
}

VOID CBlxLock::Release()
{
#ifdef _VISTA_ && #ifndef _USE_CS_
	ReleaseSRWLockExclusive( &m_Lock );
#else
	m_Lock.Unlock();
#endif
}

VOID CBlxLock::ReleaseShared()
{
#ifdef _VISTA_ && #ifndef _USE_CS_
	ReleaseSRWLockShared( &m_Lock );
#else
	m_Lock.Unlock();
#endif
}

#ifdef _VISTA_ && #ifndef _USE_CS_
PSRWLOCK CBlxLock::get_Lock()
{
	return &m_Lock;
 }
#else
#endif
