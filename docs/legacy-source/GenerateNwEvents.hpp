#pragma once

#include "StdAfx.h"

#include "PublishingPointSource.h"
#include "PacketInfoStructs.h"
#include "ServerSqlEventThreadData.h"
#include "ClientManagementEventData.h"
#include "IpHeader.h"
#include "ServerSendThreadData.h"
#include "ServerAudioSource.h"
#include "ServerVideoSource.h"

template <class T_ServerNetwork>
class CGenerateNwEvents
{
public:
	CGenerateNwEvents( T_ServerNetwork* pServerNetwork );
	~CGenerateNwEvents(void);

	INT32 GeneratePptVideoSource( 
				CServerSqlEventThreadData* pServerSqlEventThreadData
				);
	INT32 GeneratePptVideoSourceRemove( 
				CServerSqlEventThreadData* pServerSqlEventThreadData
				);

	INT32 GeneratePptAudioSource( 
				CServerSqlEventThreadData* pServerSqlEventThreadData
				);
	INT32 GeneratePptAudioSourceRemove( 
				CServerSqlEventThreadData* pServerSqlEventThreadData
				);

	INT32 GeneratePptVideoSubscription( 
				CServerSqlEventThreadData* pServerSqlEventThreadData
				);
	INT32 GeneratePptVideoSubscriptionRemove( 
				CServerSqlEventThreadData* pServerSqlEventThreadData
				);

	INT32 GenerateKeepAlive( 
				CServerSqlEventThreadData* pServerSqlEventThreadData
				);

	INT32 GenerateDropUser( 
				CServerSqlEventThreadData* pServerSqlEventThreadData 
				);

private:
	typedef T_ServerNetwork		SERVER_NETWORK;
	SERVER_NETWORK*				m_pServerNetwork;
	CGenerateNwEvents( const CGenerateNwEvents& c );
};

template <class T_ServerNetwork>
CGenerateNwEvents<T_ServerNetwork>::CGenerateNwEvents( SERVER_NETWORK* pServerNetwork )
{
	m_pServerNetwork = pServerNetwork;
}

template <class T_ServerNetwork>
CGenerateNwEvents<T_ServerNetwork>::~CGenerateNwEvents(void)
{
}

template <class T_ServerNetwork>
INT32 CGenerateNwEvents<T_ServerNetwork>::GeneratePptVideoSource( CServerSqlEventThreadData* pServerSqlEventThreadData )
{
	INT32 iRc = 0;

	PSQL_SERVER_TRIGGER_EVENT pSqlServerTriggerEvent =
		&pServerSqlEventThreadData->m_sqlTriggerEvent;
	OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pSqlServerTriggerEvent->eventTypeId );

	pServerSqlEventThreadData->ReallocBuffer(
		MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ MAX_ESTABLISH_VIDEO_SOURCE
		);

	PIP_HEADER pIpHeader = pServerSqlEventThreadData->get_IpHeader();
	PUDP_HEADER pUdpHeader = pServerSqlEventThreadData->get_UdpHeader();
	PEVENT_HEADER pEventHeader = pServerSqlEventThreadData->get_EventHeader();
	PEVENT_PACKET_HEADER pEventPacketHeader = pServerSqlEventThreadData->get_EventPacketHeader();
	PESTABLISH_VIDEO_SOURCE pEstablishVideoSource =
		reinterpret_cast<PESTABLISH_VIDEO_SOURCE>( pServerSqlEventThreadData->get_PacketDataBlob() );

	DWORD dwRetVal = 0;
	dwRetVal = m_pServerNetwork->m_pPublishingPointSource->ReadVideoSource(
					pSqlServerTriggerEvent->rowId 
					, pIpHeader
					, pUdpHeader
					, pEventHeader
					, pEventPacketHeader
					, pEstablishVideoSource
					);
	switch( pSqlServerTriggerEvent->eventTypeId )
	{
		case BE_INSERT_PPT_VIDEO_SOURCE_RT:

			m_pServerNetwork->ServerEventVideoSourceRt(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, ( PPACKET_DATA_BLOB )pEstablishVideoSource
							);
			break;
		case BE_INSERT_PPT_VIDEO_SOURCE:

			m_pServerNetwork->ServerEventVideoSource(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, ( PPACKET_DATA_BLOB )pEstablishVideoSource
							);
			break;
	}
	return iRc;
}

template <class T_ServerNetwork>
INT32 CGenerateNwEvents<T_ServerNetwork>::GeneratePptVideoSourceRemove( CServerSqlEventThreadData* pServerSqlEventThreadData )
{
	INT32 iRc = 0;

	PSQL_SERVER_TRIGGER_EVENT pSqlServerTriggerEvent =
		&pServerSqlEventThreadData->m_sqlTriggerEvent;

	pServerSqlEventThreadData->ReallocBuffer(
		MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ 0
		);

	PIP_HEADER pIpHeader = pServerSqlEventThreadData->get_IpHeader();
	PUDP_HEADER pUdpHeader = pServerSqlEventThreadData->get_UdpHeader();
	PEVENT_HEADER pEventHeader = pServerSqlEventThreadData->get_EventHeader();
	PEVENT_PACKET_HEADER pEventPacketHeader = pServerSqlEventThreadData->get_EventPacketHeader();
	PPACKET_DATA_BLOB pPacketDataBlob =
		pServerSqlEventThreadData->get_PacketDataBlob();

	DWORD dwRetVal = 0;
	dwRetVal = m_pServerNetwork->m_pPublishingPointSource->ReadVideoSource(
					pSqlServerTriggerEvent->rowId 
					, pIpHeader
					, pUdpHeader
					, pEventHeader
					, pEventPacketHeader
					, NULL
					);

	pEventHeader->eventType			= BE_REMOVE_VIDEO_SOURCE;
	pEventHeader->eventTypeId		= BET_MANAGEMENT;

	pIpHeader->ip_totallength = htons(
					MAX_IP_HEADER
					+ MAX_UDP_HEADER
					+ MAX_EVENT_HEADER
					+ MAX_EVENT_PACKET_HEADER
					+ pEventPacketHeader->dataBlobSize 
					);

	pUdpHeader->udp_length = htons(
					MAX_UDP_HEADER
					+ MAX_EVENT_HEADER
					+ MAX_EVENT_PACKET_HEADER
					+ pEventPacketHeader->dataBlobSize
					);

	switch( pSqlServerTriggerEvent->eventTypeId )
	{
		case BE_REMOVE_PPT_VIDEO_SOURCE_RT:

			m_pServerNetwork->ServerEventVideoSourceRt(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pPacketDataBlob
							);
			break;
		case BE_REMOVE_PPT_VIDEO_SOURCE:

			m_pServerNetwork->ServerEventVideoSource(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pPacketDataBlob
							);
			break;
	}

	return iRc;
}

template <class T_ServerNetwork>
INT32 CGenerateNwEvents<T_ServerNetwork>::GeneratePptAudioSource( CServerSqlEventThreadData* pServerSqlEventThreadData )
{
	INT32 iRc = 0;

	PSQL_SERVER_TRIGGER_EVENT pSqlServerTriggerEvent =
		&pServerSqlEventThreadData->m_sqlTriggerEvent;

	pServerSqlEventThreadData->ReallocBuffer(
		MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ MAX_ESTABLISH_AUDIO_SOURCE
		);

	PIP_HEADER pIpHeader = pServerSqlEventThreadData->get_IpHeader();
	PUDP_HEADER pUdpHeader = pServerSqlEventThreadData->get_UdpHeader();
	PEVENT_HEADER pEventHeader = pServerSqlEventThreadData->get_EventHeader();
	PEVENT_PACKET_HEADER pEventPacketHeader = pServerSqlEventThreadData->get_EventPacketHeader();
	PESTABLISH_AUDIO_SOURCE pEstablishAudioSource =
		reinterpret_cast<PESTABLISH_AUDIO_SOURCE>( pServerSqlEventThreadData->get_PacketDataBlob() );

	DWORD dwRetVal = 0;
	dwRetVal = m_pServerNetwork->m_pPublishingPointSource->ReadAudioSource(
					pSqlServerTriggerEvent->rowId 
					, pIpHeader
					, pUdpHeader
					, pEventHeader
					, pEventPacketHeader
					, pEstablishAudioSource
					);
	switch( pSqlServerTriggerEvent->eventTypeId )
	{
		case BE_INSERT_PPT_AUDIO_SOURCE_RT:
			m_pServerNetwork->ServerEventAudioSourceRt(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, ( PPACKET_DATA_BLOB )pEstablishAudioSource
							, m_pServerNetwork
							);
			break;
		case BE_INSERT_PPT_AUDIO_SOURCE:
			m_pServerNetwork->ServerEventAudioSource(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, ( PPACKET_DATA_BLOB )pEstablishAudioSource
							, m_pServerNetwork
							);
			break;
	}

	return iRc;
}

template <class T_ServerNetwork>
INT32 CGenerateNwEvents<T_ServerNetwork>::GeneratePptAudioSourceRemove( CServerSqlEventThreadData* pServerSqlEventThreadData )
{
	INT32 iRc = 0;

	PSQL_SERVER_TRIGGER_EVENT pSqlServerTriggerEvent =
		&pServerSqlEventThreadData->m_sqlTriggerEvent;

	pServerSqlEventThreadData->ReallocBuffer(
		MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ 0
		);

	PIP_HEADER pIpHeader = pServerSqlEventThreadData->get_IpHeader();
	PUDP_HEADER pUdpHeader = pServerSqlEventThreadData->get_UdpHeader();
	PEVENT_HEADER pEventHeader = pServerSqlEventThreadData->get_EventHeader();
	PEVENT_PACKET_HEADER pEventPacketHeader = pServerSqlEventThreadData->get_EventPacketHeader();
	PPACKET_DATA_BLOB pPacketDataBlob =
		pServerSqlEventThreadData->get_PacketDataBlob();

	DWORD dwRetVal = 0;
	dwRetVal = m_pServerNetwork->m_pPublishingPointSource->ReadAudioSource(
					pSqlServerTriggerEvent->rowId 
					, pIpHeader
					, pUdpHeader
					, pEventHeader
					, pEventPacketHeader
					, NULL
					);

	pEventHeader->eventType			= BE_REMOVE_AUDIO_SOURCE;
	pEventHeader->eventTypeId		= BET_MANAGEMENT;

	pIpHeader->ip_totallength = htons(
					MAX_IP_HEADER
					+ MAX_UDP_HEADER
					+ MAX_EVENT_HEADER
					+ MAX_EVENT_PACKET_HEADER
					+ pEventPacketHeader->dataBlobSize 
					);

	pUdpHeader->udp_length = htons(
					MAX_UDP_HEADER
					+ MAX_EVENT_HEADER
					+ MAX_EVENT_PACKET_HEADER
					+ pEventPacketHeader->dataBlobSize
					);

	switch( pSqlServerTriggerEvent->eventTypeId )
	{
		case BE_REMOVE_PPT_AUDIO_SOURCE_RT:
			m_pServerNetwork->ServerEventAudioSourceRt(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pPacketDataBlob
							, m_pServerNetwork
							);
			break;
		case BE_REMOVE_PPT_AUDIO_SOURCE:
			m_pServerNetwork->ServerEventAudioSource(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pPacketDataBlob
							, m_pServerNetwork
							);
			break;
	}

	return iRc;
}

template <class T_ServerNetwork>
INT32 CGenerateNwEvents<T_ServerNetwork>::GeneratePptVideoSubscription( CServerSqlEventThreadData* pServerSqlEventThreadData )
{
	INT32 iRc = 0;

	PSQL_SERVER_TRIGGER_EVENT pSqlServerTriggerEvent =
		&pServerSqlEventThreadData->m_sqlTriggerEvent;

	pServerSqlEventThreadData->ReallocBuffer(
		MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ MAX_ESTABLISH_VIDEO_SUBSCRIPTION
		);

	PIP_HEADER pIpHeader = pServerSqlEventThreadData->get_IpHeader();
	PUDP_HEADER pUdpHeader = pServerSqlEventThreadData->get_UdpHeader();
	PEVENT_HEADER pEventHeader = pServerSqlEventThreadData->get_EventHeader();
	PEVENT_PACKET_HEADER pEventPacketHeader = pServerSqlEventThreadData->get_EventPacketHeader();
	PESTABLISH_VIDEO_SUBSCRIPTION pEstablishVideoSubscription =
		reinterpret_cast<PESTABLISH_VIDEO_SUBSCRIPTION>( pServerSqlEventThreadData->get_PacketDataBlob() );

	DWORD dwRetVal = 0;
	dwRetVal = m_pServerNetwork->m_pPublishingPointDestination->ReadVideoSubscription(
					pSqlServerTriggerEvent->rowId 
					, pIpHeader
					, pUdpHeader
					, pEventHeader
					, pEventPacketHeader
					, pEstablishVideoSubscription
					);
	switch( pSqlServerTriggerEvent->eventTypeId )
	{
		case BE_INSERT_PPT_MEDIA_DESTINATION_RT:
			m_pServerNetwork->ServerEventVideoSubscriptionRt(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, ( PPACKET_DATA_BLOB )pEstablishVideoSubscription
							, m_pServerNetwork
							);
			break;
		case BE_INSERT_PPT_MEDIA_DESTINATION:
			m_pServerNetwork->ServerEventVideoSubscription(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, ( PPACKET_DATA_BLOB )pEstablishVideoSubscription
							, m_pServerNetwork
							);
			break;
	}
	return iRc;
}

template <class T_ServerNetwork>
INT32 CGenerateNwEvents<T_ServerNetwork>::GeneratePptVideoSubscriptionRemove( CServerSqlEventThreadData* pServerSqlEventThreadData )
{
	INT32 iRc = 0;

	PSQL_SERVER_TRIGGER_EVENT pSqlServerTriggerEvent =
		&pServerSqlEventThreadData->m_sqlTriggerEvent;

	pServerSqlEventThreadData->ReallocBuffer(
		MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ 0
		);

	PIP_HEADER pIpHeader = pServerSqlEventThreadData->get_IpHeader();
	PUDP_HEADER pUdpHeader = pServerSqlEventThreadData->get_UdpHeader();
	PEVENT_HEADER pEventHeader = pServerSqlEventThreadData->get_EventHeader();
	PEVENT_PACKET_HEADER pEventPacketHeader = pServerSqlEventThreadData->get_EventPacketHeader();
	PPACKET_DATA_BLOB pPacketDataBlob =
		pServerSqlEventThreadData->get_PacketDataBlob();

	DWORD dwRetVal = 0;
	dwRetVal = m_pServerNetwork->m_pPublishingPointDestination->ReadVideoSubscription(
					pSqlServerTriggerEvent->rowId 
					, pIpHeader
					, pUdpHeader
					, pEventHeader
					, pEventPacketHeader
					, NULL
					);

	pEventHeader->eventType			= BE_REMOVE_VIDEO_SUBSCRIPTION;
	pEventHeader->eventTypeId		= BET_MANAGEMENT;

	pIpHeader->ip_totallength = htons(
					MAX_IP_HEADER
					+ MAX_UDP_HEADER
					+ MAX_EVENT_HEADER
					+ MAX_EVENT_PACKET_HEADER
					+ pEventPacketHeader->dataBlobSize 
					);

	pUdpHeader->udp_length = htons(
					MAX_UDP_HEADER
					+ MAX_EVENT_HEADER
					+ MAX_EVENT_PACKET_HEADER
					+ pEventPacketHeader->dataBlobSize
					);

	switch( pSqlServerTriggerEvent->eventTypeId )
	{
		case BE_REMOVE_PPT_MEDIA_DESTINATION_RT:
			m_pServerNetwork->ServerEventVideoSubscriptionRt(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pPacketDataBlob
							, m_pServerNetwork
							);
			break;
		case BE_REMOVE_PPT_MEDIA_DESTINATION:

			m_pServerNetwork->ServerEventVideoSubscription(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pPacketDataBlob
							, m_pServerNetwork
							);
			break;
	}
	return iRc;
}

template <class T_ServerNetwork>
INT32 CGenerateNwEvents<T_ServerNetwork>::GenerateKeepAlive( CServerSqlEventThreadData* pServerSqlEventThreadData )
{
	INT32 iRc = 0;

	PSQL_SERVER_TRIGGER_EVENT pSqlServerTriggerEvent =
		&pServerSqlEventThreadData->m_sqlTriggerEvent;

	pServerSqlEventThreadData->ReallocBuffer(
		MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ MAX_DELIVER_EVENT_KEEP_ALIVE
		);

	PIP_HEADER pIpHeader = pServerSqlEventThreadData->get_IpHeader();
	PUDP_HEADER pUdpHeader = pServerSqlEventThreadData->get_UdpHeader();
	PEVENT_HEADER pEventHeader = pServerSqlEventThreadData->get_EventHeader();
	PEVENT_PACKET_HEADER pEventPacketHeader = pServerSqlEventThreadData->get_EventPacketHeader();
	PDELIVER_EVENT_KEEP_ALIVE pDeliverEventKeepAlive =
		reinterpret_cast<PDELIVER_EVENT_KEEP_ALIVE>( pServerSqlEventThreadData->get_PacketDataBlob() );

	DWORD dwRetVal = 0;

	m_pServerNetwork->ServerEventKeepAlive(
					pIpHeader
					, pUdpHeader
					, pEventHeader
					, pEventPacketHeader
					, ( PPACKET_DATA_BLOB )pDeliverEventKeepAlive
					);
	return iRc;
}

template <class T_ServerNetwork>
INT32 CGenerateNwEvents<T_ServerNetwork>::GenerateDropUser( CServerSqlEventThreadData* pServerSqlEventThreadData )
{
	INT32 iRc = 0;

	PSQL_SERVER_TRIGGER_EVENT pSqlServerTriggerEvent =
		&pServerSqlEventThreadData->m_sqlTriggerEvent;

	pServerSqlEventThreadData->ReallocBuffer(
		MAX_IP_HEADER
		+ MAX_UDP_HEADER
		+ MAX_EVENT_HEADER
		+ MAX_EVENT_PACKET_HEADER
		+ 0
		);

	PIP_HEADER pIpHeader = pServerSqlEventThreadData->get_IpHeader();
	PUDP_HEADER pUdpHeader = pServerSqlEventThreadData->get_UdpHeader();
	PEVENT_HEADER pEventHeader = pServerSqlEventThreadData->get_EventHeader();
	PEVENT_PACKET_HEADER pEventPacketHeader = pServerSqlEventThreadData->get_EventPacketHeader();
	PPACKET_DATA_BLOB pPacketDataBlob =
		pServerSqlEventThreadData->get_PacketDataBlob();

	DWORD dwRetVal = 0;
	dwRetVal = m_pServerNetwork->m_pPublishingPointDestination->ReadVideoSubscription(
					pSqlServerTriggerEvent->rowId 
					, pIpHeader
					, pUdpHeader
					, pEventHeader
					, pEventPacketHeader
					, NULL
					);

	pEventHeader->eventType			= BE_DROP_USER_FROM_CONFERENCE;
	pEventHeader->eventTypeId		= BET_MANAGEMENT;

	pIpHeader->ip_totallength = htons(
					MAX_IP_HEADER
					+ MAX_UDP_HEADER
					+ MAX_EVENT_HEADER
					+ MAX_EVENT_PACKET_HEADER
					+ pEventPacketHeader->dataBlobSize 
					);

	pUdpHeader->udp_length = htons(
					MAX_UDP_HEADER
					+ MAX_EVENT_HEADER
					+ MAX_EVENT_PACKET_HEADER
					+ pEventPacketHeader->dataBlobSize
					);

	m_pServerNetwork->ServerEventDropUser(
					pIpHeader
					, pUdpHeader
					, pEventHeader
					, pEventPacketHeader
					, pPacketDataBlob
					);

	return iRc;
}
