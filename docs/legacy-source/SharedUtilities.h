#pragma once

#include "BlxCommonLib.h"

class CClientAudioData;
class CClientVideoData;
class CClientManagementEventData;
class CPacketThreadData;
class CClientReceiveThreadData;
class CClientVideoData;
class CPacketThreadNetworkBuffer;

BLXWARE_API BOOL PostClientVideoEvent(
	CPacketThreadData* pBaseThreadPacket 
	);
BLXWARE_API BOOL PostClientAudioEvent(
	CPacketThreadData* pBaseThreadPacket 
	);
BLXWARE_API BOOL PostNetworkBufferVideoEvent( 
    CPacketThreadNetworkBuffer* pBaseNetworkBuffer 
    );
BLXWARE_API BOOL PostNetworkBufferAudioEvent( 
    CPacketThreadNetworkBuffer* pBaseNetworkBuffer 
    );

//BLXWARE_LINKAGE BLXWARE_API CSimpleArray<CPacketThreadData*> g_pVideoThreadData;
//typedef CSimpleArray<CPacketThreadData*> g_videoThreadArray;

BLXWARE_LINKAGE BLXWARE_API CSimpleArray<CPacketThreadData*> g_pAudioThreadData;
typedef CSimpleArray<CPacketThreadData*> g_audioThreadArray;

BLXWARE_LINKAGE BLXWARE_API CSimpleArray<CPacketThreadData*> g_pMgmtThreadData;
typedef CSimpleArray<CPacketThreadData*> g_mgmtThreadArray;

#ifdef _VISTA_
BLXWARE_LINKAGE BLXWARE_API SRWLOCK g_srwClientReaderThreadData;
BLXWARE_LINKAGE BLXWARE_API SRWLOCK g_srwVideoThreadData;
BLXWARE_LINKAGE BLXWARE_API SRWLOCK g_srwAudioThreadData;
BLXWARE_LINKAGE BLXWARE_API SRWLOCK g_srwManagementThreadData;
#else
BLXWARE_LINKAGE BLXWARE_API CRITICAL_SECTION g_srwClientReaderThreadData;
BLXWARE_LINKAGE BLXWARE_API CRITICAL_SECTION g_srwVideoThreadData;
BLXWARE_LINKAGE BLXWARE_API CRITICAL_SECTION g_srwAudioThreadData;
BLXWARE_LINKAGE BLXWARE_API CRITICAL_SECTION g_srwManagementThreadData;
BLXWARE_LINKAGE BLXWARE_API CRITICAL_SECTION g_srwipcVideoQueue;
BLXWARE_LINKAGE BLXWARE_API CRITICAL_SECTION g_srwipcAudioQueue;
#endif

BLXWARE_API VOID FormatMACAddress(BYTE Address[], ATL::CString* frmtMACAddress);
BLXWARE_API VOID GetMACAddress( LPSTR macAddress );
BLXWARE_API int GetBestIpAddress( const ATL::CString hostName, PIN_ADDR ipAddress );
BLXWARE_API VOID GetBestHostName( const ATL::CString hostName, PIN_ADDR pipaddr );
BLXWARE_API BOOL InitializeClientSend(  PCHAR ConfDns  );
BLXWARE_API VOID CloseClientSend();
BLXWARE_API BOOL InitializeClientReceive( PCHAR ConfDns );

BLXWARE_API CPacketThreadData* GetCurrentManagementThreadData( void );
BLXWARE_API CPacketThreadData* GetCurrentThreadData( void );
BLXWARE_API CPacketThreadData* GetCurrentAudioThreadData( void );

BLXWARE_API CPacketThreadNetworkBuffer* GetCurrentVideoNetworkBuffer( void );
BLXWARE_API VOID ReturnCurrentVideoNetworkBuffer( CPacketThreadNetworkBuffer*  );
BLXWARE_API CPacketThreadNetworkBuffer* GetCurrentAudioNetworkBuffer( void );
BLXWARE_API VOID ReturnCurrentAudioNetworkBuffer( CPacketThreadNetworkBuffer*  );
BLXWARE_API CPacketThreadNetworkBuffer* GetCurrentMgmtNetworkBuffer( void );
BLXWARE_API VOID ReturnCurrentMgmtNetworkBuffer( CPacketThreadNetworkBuffer* );

//BLXWARE_API VOID ReturnClientAudioThreadData( CClientAudioData* AudioThreadData );
//BLXWARE_API VOID ReturnClientVideoThreadData( CClientVideoData* VideoThreadData );

BLXWARE_API CClientReceiveThreadData* GetCurrentClientReceiveThreadData( void );
BLXWARE_API VOID ReturnClientReceiveThreadData( CClientReceiveThreadData* /*ClientThreadData*/ );

BLXWARE_API CClientVideoData* GetCurrentClientVideoThreadData( void );
BLXWARE_API VOID ReturnClientVideoThreadData( CClientVideoData* /*VideoThreadData*/ );

BLXWARE_API CClientAudioData* GetCurrentClientAudioThreadData( void );
BLXWARE_API VOID ReturnClientAudioThreadData( CClientAudioData* /*AudioThreadData*/ );

BLXWARE_API CClientManagementEventData* GetCurrentManagementEventThreadData( void );
BLXWARE_API VOID ReturnManagementEventThreadData( CClientManagementEventData* /*ManagementEventThreadData*/ );

BLXWARE_API BOOL InitializeClientSend(  PCHAR ConfDns  );
BLXWARE_API VOID CloseClientSend();

BLXWARE_API HRESULT StartWinsock();
