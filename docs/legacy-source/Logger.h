#pragma once

#include "BlxCommonLib.h"


#ifdef	__cplusplus
extern "C" {
#endif

typedef struct _LOGGER_EVENT LOGGER_EVENT, *PLOGGER_EVENT, *LPLOGGER_EVENT;

BLXWARE_API 
HRESULT 
InitializeTracing();

BLXWARE_API 
HRESULT
FinalizeTracing();

BLXWARE_API 
VOID 
LogTraceEvent(
    LPCSTR pFileName
    , LPCSTR pFunctionName
    , LPCSTR pTraceMessage
    );

    
#ifdef	__cplusplus
}
#endif