
#ifndef __PACKET_INFO_STRUCTS_H__
#define __PACKET_INFO_STRUCTS_H__

#pragma once

#include "CommonHeaders.h"
#include "Consts.h"
#include "ServiceEnums.h"

const UINT8 PHS_MAX = 5;
//#ifndef _MANAGED
#include <pshpack1.h>
//#endif

#ifndef UNDER_CE
const UINT32 OUT_MAX_PACKET_PAYLOAD = 8 * 1514; //32768; //2048; //32768; // 54000;// 32768 ;  
const UINT32 IN_MAX_PACKET_PAYLOAD = 1 * 1514; //32768; //2048; //32768; // 54000;// 32768 ;  
#else
const UINT32 OUT_MAX_PACKET_PAYLOAD = 5 * 1514; //32768; //2048; //32768; // 54000;// 32768 ;  
const UINT32 IN_MAX_PACKET_PAYLOAD = 1 * 1; //32768; //2048; //32768; // 54000;// 32768 ;  
#endif
const UINT32 MAX_AUDIO_PACKET_BLOB = 700;  
const UINT32 MAX_MANAGEMENT_PACKET_BLOB = 700;  

typedef struct _RECV_INFO
{
	SOCKET				sd;
	INT32				iThread;
#ifdef _VISTA_
	PTP_IO				pRecvIo;
#endif
	HANDLE				hEvents;
    SOCKADDR_STORAGE    safrom;
	HANDLE				hAcceptEvents;
}RECV_INFO, *PRECV_INFO;
const UINT32 MAX_RECV_INFO( sizeof( RECV_INFO ) );  


typedef struct _IPHDR 
{
    UCHAR				ip_verlen;
    UCHAR				ip_tos;
    USHORT				ip_totallength;
    USHORT				ip_id; 
    USHORT				ip_offset;
    UCHAR				ip_ttl;
    UCHAR				ip_protocol;
    USHORT				ip_checksum;
    UINT				ip_srcaddr;
    UINT				ip_destaddr;
}IP_HEADER, * PIP_HEADER;
const UINT32 MAX_IP_HEADER ( sizeof( IP_HEADER ) );

// The UDP header structure
//
typedef struct _UDPHDR
{
    USHORT				src_portno;
    USHORT				dst_portno;  
    USHORT				udp_length; 
    USHORT				udp_checksum;

}UDP_HEADER, * PUDP_HEADER;
const UINT32 MAX_UDP_HEADER ( sizeof( UDP_HEADER ) );

// The route entry header structure
//
typedef struct _tagROUTE_ENTRY
{
    UINT				dst_ipaddr;
    USHORT				dst_portno;  
}ROUTE_ENTRY, * PROUTE_ENTRY;
const UINT32 MAX_ROUTE_ENTRY ( sizeof( ROUTE_ENTRY ) );
const UINT32 MAX_ROUTE_TABLE_ENTRIES = 8;

typedef struct _tagEVENT_HEADER
{
	GUID				eventId;
	BLXWARE_EVENT		eventType;
	BLXWARE_EVENT_TYPE	eventTypeId;
	CHAR				eventMacAddress[ MACADDRESS_LENGTH ];
}EVENT_HEADER, *PEVENT_HEADER;
const UINT32 MAX_EVENT_HEADER ( sizeof( EVENT_HEADER ) );

typedef struct _tagSQL_SERVER_TRIGGER_EVENT
{
	GUID				triggerId;
	BLXWARE_EVENT		eventType;
	BLXWARE_EVENT_TYPE	eventTypeId;
	GUID				publishingPoint;
	GUID				pptParticipantId;
	GUID				rowId;
}SQL_SERVER_TRIGGER_EVENT, *PSQL_SERVER_TRIGGER_EVENT;
const UINT32 MAX_SQL_SERVER_TRIGGER_EVENT ( sizeof( SQL_SERVER_TRIGGER_EVENT ) );

typedef struct _tagEVENT_PACKET_HEADER
{
	GUID				securityToken;
	PACKET_DATA_BLOB_TYPE	eventWsaBufType;
	USHORT				eventPublishingPoint; //sessionID
	GUID				publishingPointId; 
	GUID				pptParticipantId;
	DWORD				dataBlobSize;
}EVENT_PACKET_HEADER, *PEVENT_PACKET_HEADER;
const UINT32 MAX_EVENT_PACKET_HEADER ( sizeof( EVENT_PACKET_HEADER ) );

typedef struct _tagPACKET_DATA_BLOB
{
}PACKET_DATA_BLOB, *PPACKET_DATA_BLOB;

const UINT32 MAX_PACKET_DATA_BLOB ( OUT_MAX_PACKET_PAYLOAD  
									- MAX_EVENT_HEADER
									- MAX_EVENT_PACKET_HEADER
									- MAX_IP_HEADER
									- MAX_UDP_HEADER
									);
const UINT32 MAX_PACKET_CLIENT_DATA_BLOB ( OUT_MAX_PACKET_PAYLOAD  
									- MAX_EVENT_HEADER
									- MAX_EVENT_PACKET_HEADER
									);

const UINT32 MAX_PACKET_AUDIO ( MAX_AUDIO_PACKET_BLOB  
									+ MAX_EVENT_HEADER
									+ MAX_EVENT_PACKET_HEADER
									);


typedef struct _tagPACKET_HEADER
{
	PACKET_SOURCE_EVENT_TYPE			packetType;
	DWORD				headerFlags;
	DWORD				packetCount;
	LONGLONG			startTime;
	LONGLONG			endTime;
	LONGLONG			startMediaTime;
	LONGLONG			endMediaTime;
	BOOL				isSyncPoint;
	BOOL				isDiscontinuity;
	BOOL				isPreroll;
    GUID				majortype;
    GUID				subtype;
    BOOL				fixedSizeSamples;
    BOOL				temporalCompression;
    ULONG				sampleSize;
    GUID				headerFormatType;
    ULONG				headerFormatSize;
    RECT				rcSource;         
    RECT				rcTarget;        
    DWORD				dwBitRate;        
    DWORD				dwBitErrorRate;   
    INT64				AvgTimePerFrame;  
    DWORD				biSize;
    LONG				biWidth;
    LONG				biHeight;
    WORD				biPlanes;
    WORD				biBitCount;
    DWORD				biCompression;
    DWORD				biSizeImage;
    LONG				biXPelsPerMeter;
    LONG				biYPelsPerMeter;
    DWORD				biClrUsed;
    DWORD				biClrImportant;
	DWORD				dataSize;
}PACKET_HEADER, *PPACKET_HEADER;
const UINT32 MAX_PACKET_HEADER ( sizeof( PACKET_HEADER ) );

typedef struct _tagNETWORK_BUFFER_PACKET_INFO
{
    UINT32              length;
}NETWORK_BUFFER_PACKET_INFO, *PNETWORK_BUFFER_PACKET_INFO;
const UINT32 MAX_NETWORK_BUFFER_PACKET_INFO ( sizeof( NETWORK_BUFFER_PACKET_INFO ) );

const UINT8 MAX_NETWORK_BUFFER_PACKET_INFO_INSTANCES( 12 );
typedef struct _tagNETWORK_BUFFER_PACKETS
{
    UINT8               numberPackets;
    UINT32              dataSize;
    NETWORK_BUFFER_PACKET_INFO  packetInfo[ MAX_NETWORK_BUFFER_PACKET_INFO_INSTANCES ];
}NETWORK_BUFFER_PACKETS, *PNETWORK_BUFFER_PACKETS;
const UINT32 MAX_NETWORK_BUFFER_PACKETS ( sizeof( NETWORK_BUFFER_PACKETS ) );

typedef struct _tagESTABLISH_VIDEO_SOURCE
{
	GUID				majortype;
    GUID				subtype;
    BOOL				fixedSizeSamples;
    BOOL				temporalCompression;
    ULONG				sampleSize;
    GUID				headerFormatType;
    ULONG				headerFormatSize;
    RECT				rcSource;          
    RECT				rcTarget;         
    DWORD				dwBitRate;        
    DWORD				dwBitErrorRate;   
    INT64				AvgTimePerFrame;  
    DWORD				biSize;
    LONG				biWidth;
    LONG				biHeight;
    WORD				biPlanes;
    WORD				biBitCount;
    DWORD				biCompression;
    DWORD				biSizeImage;
    LONG				biXPelsPerMeter;
    LONG				biYPelsPerMeter;
    DWORD				biClrUsed;
    DWORD				biClrImportant;
}ESTABLISH_VIDEO_SOURCE, *PESTABLISH_VIDEO_SOURCE;
const UINT32 MAX_ESTABLISH_VIDEO_SOURCE ( sizeof( ESTABLISH_VIDEO_SOURCE ) );

typedef struct _tagESTABLISH_VIDEO_SOURCES
{
	UINT				videoSourcesCount;
	CHAR				videoSources;
}ESTABLISH_VIDEO_SOURCES, *PESTABLISH_VIDEO_SOURCES;

typedef struct _tagESTABLISH_AUDIO_SOURCE
{
    WORD				wFormatTag;         /* format type */
    WORD				nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD				nSamplesPerSec;     /* sample rate */
    DWORD				nAvgBytesPerSec;    /* for buffer estimation */
    WORD				nBlockAlign;        /* block size of data */
    WORD				wBitsPerSample;     /* number of bits per sample of mono data */
    WORD				cbSize;             /* the count in bytes of the size of */
}ESTABLISH_AUDIO_SOURCE, *PESTABLISH_AUDIO_SOURCE;
const UINT32 MAX_ESTABLISH_AUDIO_SOURCE ( sizeof( ESTABLISH_AUDIO_SOURCE ) );

typedef struct _tagESTABLISH_AUDIO_SOURCES
{
	UINT				audioSourcesCount;
	CHAR				audioSources;
}ESTABLISH_AUDIO_SOURCES, *PESTABLISH_AUDIO_SOURCES;

typedef struct _tagESTABLISH_VIDEO_SUBSCRIPTION
{
}ESTABLISH_VIDEO_SUBSCRIPTION, *PESTABLISH_VIDEO_SUBSCRIPTION;
const UINT32 MAX_ESTABLISH_VIDEO_SUBSCRIPTION ( sizeof( ESTABLISH_VIDEO_SUBSCRIPTION ) );

typedef struct _tagPUBLISH_VIDEO_FRAME
{
	ULONGLONG			packetCount;
	BOOL				isMultipart;
	UINT8				multipartCount;
	LONGLONG			startTime;
	LONGLONG			endTime;
	LONGLONG			startMediaTime;
	LONGLONG			endMediaTime;
	BOOL				isSyncPoint;
	BOOL				isDiscontinuity;
	BOOL				isPreroll;
	DWORD				dataSize;
	CHAR				data;
}PUBLISH_VIDEO_FRAME, *PPUBLISH_VIDEO_FRAME;
const UINT32 MAX_PUBLISH_VIDEO_FRAME ( sizeof( PUBLISH_VIDEO_FRAME ) );
const UINT32 MAX_VIDEO_SIZE = MAX_PACKET_DATA_BLOB - MAX_PUBLISH_VIDEO_FRAME;

typedef struct _tagPUBLISH_VIDEO_FRAME_BUFFER
{
	USHORT					multipartCount;
	BOOL					isInitialized;
	PUBLISH_VIDEO_FRAME		videoFrame;
	CHAR					data[ MAX_VIDEO_SIZE * MAX_SAMPLE_SEQ_NBR_INCR ];
}PUBLISH_VIDEO_FRAME_BUFFER, *PPUBLISH_VIDEO_FRAME_BUFFER;
const UINT32 MAX_PUBLISH_VIDEO_FRAME_BUFFER ( sizeof( PUBLISH_VIDEO_FRAME_BUFFER ) );

typedef struct _tagPUBLISH_AUDIO_SAMPLE
{
	ULONGLONG			packetCount;
	BOOL				isMultipart;
	UINT8				multipartCount;
	LONGLONG			startTime;
	LONGLONG			endTime;
	LONGLONG			startMediaTime;
	LONGLONG			endMediaTime;
	BOOL				isSyncPoint;
	BOOL				isDiscontinuity;
	BOOL				isPreroll;
	DWORD				dataSize;
	CHAR				data;
}PUBLISH_AUDIO_SAMPLE, *PPUBLISH_AUDIO_SAMPLE;
const UINT32 MAX_PUBLISH_AUDIO_SAMPLE ( sizeof( PUBLISH_AUDIO_SAMPLE ) );
const UINT32 MAX_AUDIO_SIZE = MAX_PACKET_DATA_BLOB - MAX_PUBLISH_AUDIO_SAMPLE;

typedef struct _tagPUBLISH_AUDIO_SAMPLE_BUFFER
{
	USHORT					multipartCount;
	BOOL					isInitialized;
	PUBLISH_AUDIO_SAMPLE	audioSample;
	CHAR					data[ MAX_AUDIO_SIZE * MAX_SAMPLE_SEQ_NBR_INCR ];
}PUBLISH_AUDIO_SAMPLE_BUFFER, *PPUBLISH_AUDIO_SAMPLE_BUFFER;
const UINT32 MAX_PUBLISH_AUDIO_SAMPLE_BUFFER ( sizeof( PUBLISH_AUDIO_SAMPLE_BUFFER ) );

/************************************************************************************/
/* Server Based Event Structures													*/

typedef struct _tagESTABLISH_VIDEO_SOURCE_MSMQ_TE
{
	IP_HEADER					m_ipHeader;
	UDP_HEADER					m_udpHeader;
	EVENT_HEADER				m_EventHeader;
	EVENT_PACKET_HEADER			m_EventPacketHeader;
	ESTABLISH_VIDEO_SOURCE		m_EstablishVideoSource;
}ESTABLISH_VIDEO_SOURCE_MSMQ_TE, *PESTABLISH_VIDEO_SOURCE_MSMQ_TE;
const UINT32 MAX_ESTABLISH_VIDEO_SOURCE_MSMQ_TE ( sizeof( ESTABLISH_VIDEO_SOURCE_MSMQ_TE ) );

typedef struct _tagMSMQ_TE_SYNC_VIDEO_RT
{
	IP_HEADER					m_ipHeader;
	UDP_HEADER					m_udpHeader;
	EVENT_HEADER				m_EventHeader;
	EVENT_PACKET_HEADER			m_EventPacketHeader;
	ESTABLISH_VIDEO_SOURCE		m_EstablishVideoSource;
}MSMQ_TE_SYNC_VIDEO_RT, *PMSMQ_TE_SYNC_VIDEO_RT;
const UINT32 MAX_MSMQ_TE_SYNC_VIDEO_RT ( sizeof( MSMQ_TE_SYNC_VIDEO_RT ) );

typedef struct _tagDELIVER_EVENT_KEEP_ALIVE
{
    LONG				destIP;
	WORD				destPort;
	WORD				keepAlive;
}DELIVER_EVENT_KEEP_ALIVE, *PDELIVER_EVENT_KEEP_ALIVE;
const UINT32 MAX_DELIVER_EVENT_KEEP_ALIVE ( sizeof( DELIVER_EVENT_KEEP_ALIVE ) );

typedef struct _tagDELIVER_EVENT_KEEP_ALIVE_MSMQ_TE
{
	IP_HEADER					m_ipHeader;
	UDP_HEADER					m_udpHeader;
	EVENT_HEADER				m_EventHeader;
	EVENT_PACKET_HEADER			m_EventPacketHeader;
	DELIVER_EVENT_KEEP_ALIVE	m_EstablishVideoSource;
}DELIVER_EVENT_KEEP_ALIVE_MSMQ_TE, *PDELIVER_EVENT_KEEP_ALIVE_MSMQ_TE;
const UINT32 MAX_DELIVER_EVENT_KEEP_ALIVE_MSMQ_TE ( sizeof( DELIVER_EVENT_KEEP_ALIVE_MSMQ_TE ) );

/************************************************************************************/

//#ifndef _MANAGED
#include <poppack.h>
//#endif

#endif //__PACKET_INFO_STRUCTS_H__