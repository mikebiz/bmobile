#pragma once


#ifdef BLXWARE_EXPORTS
#define BLXWARE_API __declspec(dllexport)
#define BLXWARE_LINKAGE extern
#else
#define BLXWARE_API __declspec(dllimport)
#define BLXWARE_LINKAGE extern 
#endif

#include "CommonHeaders.h"

class CNetworkPacketSyncImpl;
class CPacketThreadData;

class BLXWARE_API CNetworkPacketSync
{
public:
    CNetworkPacketSync( UINT8 MaxVideoPackets, UINT8 MaxAudioPackets );
    virtual ~CNetworkPacketSync();

    STDMETHODIMP Initialize();
    STDMETHODIMP Deinitialize();
    CPacketThreadData* GetVideoThreadData( void );
    STDMETHODIMP PostVideoEvent( CPacketThreadData* VideoPacket);
    CPacketThreadData* GetManagementThreadData( void );
    STDMETHODIMP PostVideoMgmtEvent( CPacketThreadData* VideoPacket);
#ifdef INCLUDE_AUDIO
    CPacketThreadData* GetAudioThreadData( void );
    STDMETHODIMP PostAudioEvent( CPacketThreadData* AudioPacket);
    STDMETHODIMP PostAudioMgmtEvent( CPacketThreadData* VideoPacket);
#endif

protected:

private:
    CNetworkPacketSyncImpl*     m_pNpsImpl;

    CNetworkPacketSync( const CNetworkPacketSync& PacketSync );
};