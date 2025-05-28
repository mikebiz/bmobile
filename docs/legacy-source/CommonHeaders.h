#pragma once

#ifndef __COMMON_HEADERS_H__
#define __COMMON_HEADERS_H__

#ifndef STRICT
#define STRICT
#endif

#include "osver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define ATL_NO_LEAN_AND_MEAN
#define _SECURE_ATL 1

#include <atlbase.h>

#ifdef UNDER_CE

#include <aygshell.h>
#pragma comment(lib, "aygshell.lib") 

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#ifndef _DEVICE_RESOLUTION_AWARE
#define _DEVICE_RESOLUTION_AWARE
#endif
#endif

#ifdef _DEVICE_RESOLUTION_AWARE
#include "DeviceResolutionAware.h"
#endif

#if _WIN32_WCE < 0x500 && ( defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) )
	#pragma comment(lib, "ccrtrtti.lib")
	#ifdef _X86_	
		#if defined(_DEBUG)
			#pragma comment(lib, "libcmtx86d.lib")
		#else
			#pragma comment(lib, "libcmtx86.lib")
		#endif
	#endif
#endif

#include <altcecrt.h>
#endif

#include <atlstr.h>
#include <strsafe.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#define _ATL_NO_AUTOMATIC_NAMESPACE

#endif //__COMMON_HEADERS_H__