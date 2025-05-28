#pragma once

#ifdef BLXWARE_EXPORTS
#define BLXWARE_API __declspec(dllexport)
#define BLXWARE_LINKAGE extern
#else
#define BLXWARE_API __declspec(dllimport)
#define BLXWARE_LINKAGE extern 
#endif

#include "BlxCommonLib.h"

class BLXWARE_API CBlxLock
{
public:
	CBlxLock(void);
	~CBlxLock(void);
	VOID Lock();
	VOID LockShared();
	VOID Release();
	VOID ReleaseShared();
#ifdef _VISTA_ && #ifndef _USE_CS_
	PSRWLOCK get_Lock();
#else
    CComCriticalSection* get_Lock();
#endif
private:
#ifdef _VISTA_ && #ifndef _USE_CS_
	SRWLOCK					m_Lock;
	CONDITION_VARIABLE		m_cvReadyToConsume;
	CONDITION_VARIABLE		m_cvReadyToProduce;
#else
	CComCriticalSection		m_Lock;
#ifndef UNDER_CE
	CONDITION_VARIABLE		m_cvReadyToConsume;
	CONDITION_VARIABLE		m_cvReadyToProduce;
#endif
#endif
};
