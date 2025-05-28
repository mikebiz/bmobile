#pragma once

#include <functional>
#include "PacketInfoStructs.h"
#include "ClientAudioData.h"
#include "ClientVideoData.h"
#include "ServerPacketThreadData.h"
#include "ServerWorkerThreadData.h"
#include "ClientReceiveThreadData.h"

struct test_audio_datagram
{ 
	BOOL operator()(const CClientAudioData* t1, const CClientAudioData* t2) const 
	{ 
		PPUBLISH_AUDIO_SAMPLE pAudioSample = 
			reinterpret_cast< const PPUBLISH_AUDIO_SAMPLE >( t1->get_PacketDataBlob() );
		PPUBLISH_AUDIO_SAMPLE pAudioSampleRight = 
			reinterpret_cast< PPUBLISH_AUDIO_SAMPLE >( t2->get_PacketDataBlob() );

		BOOL fRc = pAudioSample->packetCount > pAudioSampleRight->packetCount;
		return fRc;
	} 
}; 

#ifndef USE_NETWORK_PACKET
struct test_video_datagram
{ 
	BOOL operator()(const CClientVideoData* t1, const CClientVideoData* t2) const 
	{ 
		PPUBLISH_VIDEO_FRAME pVideoFrame = 
			reinterpret_cast< const PPUBLISH_VIDEO_FRAME >( t1->get_PacketDataBlob() );
		PPUBLISH_VIDEO_FRAME pVideoFrameRight = 
			reinterpret_cast< PPUBLISH_VIDEO_FRAME >( t2->get_PacketDataBlob() );

		BOOL fRc = pVideoFrame->packetCount > pVideoFrameRight->packetCount;
		return fRc;
	} 
};
#else
struct test_video_datagram
{ 
	BOOL operator()(const CClientReceiveThreadData* t1, const CClientReceiveThreadData* t2) const 
	{ 
		PPUBLISH_VIDEO_FRAME pVideoFrame = 
			reinterpret_cast< const PPUBLISH_VIDEO_FRAME >( t1->get_PacketDataBlob() );
		PPUBLISH_VIDEO_FRAME pVideoFrameRight = 
			reinterpret_cast< PPUBLISH_VIDEO_FRAME >( t2->get_PacketDataBlob() );

		BOOL fRc = pVideoFrame->packetCount > pVideoFrameRight->packetCount;
		return fRc;
	} 
};
#endif



struct test_server_receive_data
{ 
	BOOL operator()(const CServerPacketThreadData* t1, const CServerPacketThreadData* t2) const 
	{ 
		return( t1->m_uiHeapPriority > t2->m_uiHeapPriority );
	} 
}; 

struct test_server_worker_data
{ 
	BOOL operator()(const CServerWorkerThreadData* t1, const CServerWorkerThreadData* t2) const 
	{ 
		return( t1->m_uiHeapPriority > t2->m_uiHeapPriority );
	} 
}; 
