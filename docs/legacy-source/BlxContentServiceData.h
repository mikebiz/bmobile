#pragma once

//#include "BlxCommonLib.h"
#pragma once

//#include "osver.h"
//#include "CommonHeaders.h"
//
//#include <atlstr.h>
#include "version.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlsocket.h>
#include <atlenc.h>
#include <atlstr.h>
#include <atlmem.h>

#include <stdio.h>
#include <strsafe.h>

// 'argument' : CLR - Native code has different type in ATL::
#pragma warning(disable:4744)

#ifdef BLXCONTENTSERVICEDATA_EXPORTS
#define BLXCONTENTSERVICEDATA_API __declspec(dllexport)
#define BLXCONTENTSERVICEDATA_LINKAGE
#else
#define BLXCONTENTSERVICEDATA_API __declspec(dllimport)
#define BLXCONTENTSERVICEDATA_LINKAGE extern 
#endif

const USHORT MAX_CONNECTION_STRING = 255;
