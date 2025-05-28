#pragma once

#ifdef BLXWARE_EXPORTS
#define BLXWARE_API __declspec(dllexport)
#define BLXWARE_LINKAGE extern
#else
#define BLXWARE_API __declspec(dllimport)
#define BLXWARE_LINKAGE extern 
#endif

#include "BlxCommonLib.h"

#include "BlxLock.h"

class BLXWARE_API CScopedLock
{
public:
	CScopedLock( CBlxLock* BlxLock );
	~CScopedLock(void);
private:
	CBlxLock*			m_pLock;
};;
