#pragma once

#ifdef BLXWARE_EXPORTS
#define BLXWARE_API __declspec(dllexport)
#define BLXWARE_LINKAGE extern
#else
#define BLXWARE_API __declspec(dllimport)
#define BLXWARE_LINKAGE extern 
#endif

#include "BlxCommonLib.h"

class BLXWARE_API CBlxMutex
{
public:
	CBlxMutex( HANDLE& mutex, DWORD timeout );
    CBlxMutex( HANDLE& mutex, HANDLE& semaphone, DWORD timeout );
	~CBlxMutex(void);

private:
    HANDLE& m_mutex;
    HANDLE& m_semaphore;
};
