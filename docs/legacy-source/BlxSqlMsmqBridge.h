// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "BlxCommonLib.h"
#include <atlcoll.h>
//// Insert your headers here
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//
//#include <windows.h>
//#include <stdio.h>
//
#include <transact.h>
#include <mq.h>
//


typedef CRBMap< PTCHAR, HANDLE > msmqConnections;
static msmqConnections mqConn;

#include "BlxLock.h"

static CRITICAL_SECTION critSec;
static BOOL fIsCritSecInitialized = FALSE;

//Include ODS headers
#ifdef __cplusplus
extern "C" {
#endif 

#include <Srv.h>		// Main header file that includes all other header files

#ifdef __cplusplus
}
#endif 

#define XP_NOERROR              0
#define XP_ERROR                1
#define MAXCOLNAME				25
#define MAXNAME					25
#define MAXTEXT					255

//This is where you control the number of parameters prior to where the message
//parameters will be.  In this example, the first two are simple ones for the message
//queue name and if its private of public.  The parameter XP_PARAM_MESSAGE_BEGIN
//indicates which parameter begins the message.  The assumption is that all remaining
//parameters will be included in the message that is written to the queue.  If one wishes
//to include additional parameters in the call, they can be included in front of  XP_
//PARAM_MESSAGE_BEGIN.  These parameters will be null terminiated once they are
//brought out of the Srv_Proc structure. 
#define XP_PARAM_QUEUE_NAME 0
#define XP_PARAM_QUEUE_TYPE 1
#define XP_PARAM_MESSAGE_LABEL 2
#define XP_PARAM_MESSAGE_BEGIN 3

#define MAX_SERVER_ERROR 20000
#define XP_PARAM_ERROR MAX_SERVER_ERROR+1

extern CBlxLock blxLock;

void printError( SRV_PROC* pSrvProc, 
					CHAR* szErrorMsg 
				);
void printUsage( SRV_PROC* pSrvProc );
HRESULT getQueueHandle (
						PTCHAR szQueueName,
						SRV_PROC* pSrvProc,
						HANDLE* phQueue,
						DWORD dwAccessMode,                // Access mode of queue
				 		DWORD dwShareMode					// Share mode of queue
						);




