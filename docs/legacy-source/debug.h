#pragma once


#include <stdio.h>
#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

static __inline void OutputDebugStringf(PTCHAR fmt, ...)
{
#ifndef UNDER_CE
	va_list args;
	TCHAR buf[256];

	va_start(args, fmt);
	vswprintf_s(buf, fmt, args);
	wcscat_s(buf, TEXT("\n"));
	OutputDebugString(buf);
	va_end(args);
#endif
}

#ifdef _DEBUG
#define DPRINTF OutputDebugStringf
#else
static __inline void 
DPRINTF(PTCHAR fmt, ...) { }
#endif

static __inline void DebugDump( PTCHAR fmt, ... )
{
    va_list args;
    TCHAR buf[1024];

	va_start(args, fmt);
	_stprintf(buf, fmt, args);
	_tcscat(buf, TEXT("\n"));

    _tprintf( 
        TEXT("%s")
        , buf
        );

   	va_end(args);

}

#ifdef TRACE_ALL
#define DTRACEF DebugDump
#else
static __inline void 
DTRACEF(PTCHAR fmt, ...) { }
#endif
#ifdef __cplusplus
}
#endif

