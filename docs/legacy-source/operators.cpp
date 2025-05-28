#include "operators.h"

struct test_audio
	: std::binary_function<CClientAudioData*, CClientAudioData*, bool> 
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

struct test_video
	: std::binary_function<CClientVideoData*, CClientVideoData*, bool> 
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
