#pragma once
//
#ifdef BLXWARE_EXPORTS
#define BLXWARE_API __declspec(dllexport)
#define BLXWARE_LINKAGE extern
#else
#define BLXWARE_API __declspec(dllimport)
#define BLXWARE_LINKAGE extern 
#endif
//#include "version.h"
//
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//// Windows Header Files:
//#include <windows.h>
//
//
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
//
//#include <atlbase.h>
//#include <atlsocket.h>
//#include <atlstr.h>
//#include <strsafe.h>
//

#include "CommonHeaders.h"

#include <atlsocket.h>
#include <atlenc.h>
#include <atlstr.h>
#include <atlmem.h>

// 'argument' : CLR - Native code has different type in ATL::
#pragma warning(disable:4744)
