#include "EncoderProfiles.h"



void BuildProfile1080iDS( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "downsample\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\Silk,MP3_960x544,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 0;
	ep.iSource = 1;
	ep.bDeinterlace = false;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfile1080i( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "none\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\Silk,MP3_1920x1088,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 0;
	ep.iSource = 1;
	ep.bDeinterlace = false;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfileNTSCDS( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "downsample\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\Silk,MP3_320x240.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 0;
	ep.iSource = 1;
	ep.bDeinterlace = true;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = true;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfileNTSC( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "none\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\Silk,MP3_720x480,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 0;
	ep.iSource = 1;
	ep.bDeinterlace = true;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfile720pDS( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "downsample\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\Silk,MP3_640x368,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 0;
	ep.iSource = 1;
	ep.bDeinterlace = false;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfile720p( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "none\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\Silk,MP3_1280x720,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 0;
	ep.iSource = 1;
	ep.bDeinterlace = false;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfileX2641080iDS( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "downsample\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\X264,MP3_960x544,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 2;
	ep.iSource = 1;
	ep.bDeinterlace = false;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfileX264NTSC( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "none\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\X264,MP3_720x480,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 2;
	ep.iSource = 1;
	ep.bDeinterlace = true;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfileX264720pDS( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "downsample\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\X264,MP3_640x368,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 2;
	ep.iSource = 1;
	ep.bDeinterlace = false;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfileX264720p( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "none\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\X264,MP3_1280x720,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 2;
	ep.iSource = 1;
	ep.bDeinterlace = false;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfileATINTSC( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "none\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\Silk,MP3_720x480,48k.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 0;
	ep.iSource = 0;
	ep.bDeinterlace = true;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_ASF_WRITER;
}

void BuildProfileNTSCDS_UDP( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "downsample\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\Silk,MP3_320x2240.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 0;
	ep.iSource = 1;
	ep.bDeinterlace = false;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.bRefTimeMon = false;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_UPD_PUSH;
}

void BuildProfileNTSC_UDP( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "e:\\\\streamer\\FilterProperties\\MP3Properties_128_48.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "none\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "e:\\\\streamer\\Profiles\\Silk,MP3_720x480.prx\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "e:\\\\streamer\\stream.asf\0" ) );
	ep.iAudioCodec = 0;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 0;
	ep.iSource = 1;
	ep.bDeinterlace = false;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
	ep.bVideoOnly = false;
	ep.streamDestination = SD_UPD_PUSH;
}

void BuildProfileWebCam( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "MP3Properties_32_441.dat\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "none\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "\0" ) );
	ep.iAudioCodec = 1;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 2; //0; mxb - replace silk with x264
	ep.iSource = 4;
	ep.bDeinterlace = false; //true;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = false;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
#ifdef INCLUDE_AUDIO
	ep.bVideoOnly = false;
#else
	ep.bVideoOnly = true;
#endif
	ep.streamDestination = SD_UPD_PUSH;
}

void BuildProfileMobilePhone( ENCODER_PROFILE& ep )
{
	ep.dwSizeOfThisStruct = sizeof( ENCODER_PROFILE );
	ep.dwVersionOfThisStruct = ENCODER_PROFILE_CURRENT;
	wcscpy_s ( ep.szMP3PropPath, _countof( ep.szMP3PropPath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szScalerPropPath, _countof( ep.szScalerPropPath ), TEXT( "none\0" ) );
	wcscpy_s ( ep.szProfilePath, _countof( ep.szProfilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szInputFilePath, _countof( ep.szInputFilePath ), TEXT( "\0" ) );
	wcscpy_s ( ep.szOutputFilePath, _countof( ep.szOutputFilePath ), TEXT( "\0" ) );
	ep.iAudioCodec = 1;
	ep.iCrossbar = 1;
	ep.iVideoCodec = 2; //2; //0; mxb - replace silk with x264
	ep.iSource = 5;
	ep.bDeinterlace = false; //true;
	ep.bMutePreview = true;
	ep.bDisableVideoPreview = true;//false;
	ep.bOutputGraph = false;
	ep.bOutputFile = false;
	ep.bSaveMP3Prop = false;
	ep.bSaveScalerProp = false;
	ep.bRefTimeMon = false;
	ep.bMod16Fix = false;
	ep.iSyncTolerance = 30;
	ep.iRestartTimeHour = 3;
	ep.iRestartTimeMinute = 30;
#ifdef INCLUDE_AUDIO
	ep.bVideoOnly = false;
#else
	ep.bVideoOnly = true;
#endif
	ep.streamDestination = SD_UPD_PUSH;
}
