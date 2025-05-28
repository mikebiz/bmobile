#include "StdAfx.h"
#include <iphlpapi.h>
#include "ServerNetwork.h"

#include "PublishingPointSource.h"

#include "SharedUtilities.h"

CServerNetwork::CServerNetwork( PBCS_SERVER_NETWORK_CONTEXT snc )
	: m_fRun( TRUE )
{
	::CopyMemory(
		&m_ServerNetworkContext
		, snc
		, sizeof( BCS_SERVER_NETWORK_CONTEXT )
		);

	m_tSendThread = m_ServerNetworkContext.m_uiSendNwBuffer;

}

CServerNetwork::~CServerNetwork(void)
{
	m_pRcvBuffer.Free();
	m_pSendBuffer.Free();
}

VOID CServerNetwork::StartServiceThread( CServerNetwork* pServerNetwork )
{
	pServerNetwork->m_fRun = TRUE;
	HANDLE hThread = 
		( HANDLE )_beginthreadex(
			NULL
			, 0
			, &ServiceWorker
			, pServerNetwork
			, NULL
			, &pServerNetwork->m_ServerNetworkContext.m_ServerContext.m_uiThreadId
			);

	pServerNetwork->m_hSocketThread.Attach( hThread );
	return;
}

VOID CServerNetwork::EndServiceThread( CServerNetwork* pServerNetwork )
{
	pServerNetwork->m_fRun = FALSE;
	WaitForSingleObject( pServerNetwork->m_hSocketThread, INFINITE );
	pServerNetwork->m_hSocketThread.Close();
	return;
}

CClientManagementEventData* CServerNetwork::GetCurrentManagementThreadData( CClientManagementEventData* pServerNetwork )
{
	CClientManagementEventData* pData = NULL;
#ifdef _USE_ARRAY_CACHE_
	
//
//
////	pServerNetwork->m_csSendThreadData.LockShared();
//
//	InterlockedIncrement( &pServerNetwork->m_lSendThreadPhase );
//	LONG iThread = pServerNetwork->m_lSendThreadPhase;
//
//
////	pServerNetwork->m_csSendThreadData.ReleaseShared();
//
//	if( iThread > pServerNetwork->m_tSendThread - 2 )
//	{
//		CServerSendThreadData* pThreadOldest 
//			= pServerNetwork->m_pQCServerSendThreadData[ ( iThread + 1 ) % pServerNetwork->m_tSendThread ];
//
//		WaitForSingleObject(
//			pThreadOldest->hEvent 
//			, WSA_INFINITE
//			);
//		WSAResetEvent( pThreadOldest->hEvent );
//	}
//
//	pData = pServerNetwork->m_pQCServerSendThreadData[ iThread % pServerNetwork->m_tSendThread ];
//	WSAResetEvent( pData );

#else
	//pServerNetwork->m_pQClientManagementEventData->poll_timed(
	//	pData
	//	, 1e6
	//	);
#endif
	return pData;
}

VOID CServerNetwork::ReturnManagementThreadData( CClientManagementEventData* ThreadData, CServerNetwork* pServerNetwork )
{
#ifdef _USE_ARRAY_CACHE_
	WSASetEvent( ThreadData->hEvent );
#elif _USE_PQ_
	WSAResetEvent( ThreadData->hEvent );
	pServerNetwork->m_pQCServerSendThreadData->offer_timed(
		ThreadData
		, 5e6
		);
#else
	WSAResetEvent( ThreadData->hEvent );
	pServerNetwork->m_pQClientManagementEventData->push_timed(
		ThreadData
		, 1000 //1e6
		);
#endif
	return;
}

VOID CServerNetwork::AllocateReceiveThreadpool()
{
	m_pVideoThreadPool = CreateThreadpool(NULL);

	if( NULL != m_pVideoThreadPool ) 
	{
		SetThreadpoolThreadMaximum( m_pVideoThreadPool, m_ServerNetworkContext.m_uiReceiveTpMax );
		if ( SetThreadpoolThreadMinimum( m_pVideoThreadPool, m_ServerNetworkContext.m_uiReceiveTpMin ) ) 
		{
			InitializeThreadpoolEnvironment( &m_VideoCallbackEnvironment );
			m_pVideoCleanupGroup = m_VideoCallbackEnvironment.CleanupGroup;
			SetThreadpoolCallbackPool( 
				&m_VideoCallbackEnvironment
				, m_pVideoThreadPool
			);
			m_pVideoCleanupGroup = CreateThreadpoolCleanupGroup();
			SetThreadpoolCallbackCleanupGroup(
				&m_VideoCallbackEnvironment
				, m_pVideoCleanupGroup
				, NULL
			);
		}
	}
	else
	{
		return;
	}


	m_pVideoIo = CreateThreadpoolIo( 
		( HANDLE )m_ServerVideoSocket
		, ( PTP_WIN32_IO_CALLBACK )ReceiveCompleteRoutine
		, NULL
		, &m_VideoCallbackEnvironment
		);
}

VOID CServerNetwork::AllocateSendThreadpool()
{
	m_pClientThreadPool = CreateThreadpool(NULL);

	if( NULL != m_pClientThreadPool ) 
	{
		SetThreadpoolThreadMaximum( m_pClientThreadPool, m_ServerNetworkContext.m_uiSendTpMax );
		if ( SetThreadpoolThreadMinimum( m_pClientThreadPool, m_ServerNetworkContext.m_uiSendTpMin ) ) 
		{
			InitializeThreadpoolEnvironment( &m_ClientCallbackEnvironment );
			m_pClientCleanupGroup = m_ClientCallbackEnvironment.CleanupGroup;
			SetThreadpoolCallbackPool( 
				&m_ClientCallbackEnvironment
				, m_pClientThreadPool
			);
			m_pClientCleanupGroup = CreateThreadpoolCleanupGroup();
			SetThreadpoolCallbackCleanupGroup(
				&m_ClientCallbackEnvironment
				, m_pClientCleanupGroup
				, NULL
			);
		}
	}
	else
	{
		return;
	}
	
	m_pClientIo = CreateThreadpoolIo( 
		( HANDLE )m_ClientSocket
		, ( PTP_WIN32_IO_CALLBACK )SubmitVideoToClientCompleteRoutine
		, NULL
		, &m_ClientCallbackEnvironment
		);

}

VOID CServerNetwork::AllocateManagementThreadpool()
{
	m_pVideoThreadPool = CreateThreadpool(NULL);

	if( NULL != m_pVideoThreadPool ) 
	{
		SetThreadpoolThreadMaximum( m_pVideoThreadPool, m_ServerNetworkContext.m_uiReceiveTpMax );
		if ( SetThreadpoolThreadMinimum( m_pVideoThreadPool, m_ServerNetworkContext.m_uiReceiveTpMin ) ) 
		{
			InitializeThreadpoolEnvironment( &m_VideoCallbackEnvironment );
			m_pVideoCleanupGroup = m_VideoCallbackEnvironment.CleanupGroup;
			SetThreadpoolCallbackPool( 
				&m_VideoCallbackEnvironment
				, m_pVideoThreadPool
			);
			m_pVideoCleanupGroup = CreateThreadpoolCleanupGroup();
			SetThreadpoolCallbackCleanupGroup(
				&m_VideoCallbackEnvironment
				, m_pVideoCleanupGroup
				, NULL
			);
		}
	}
	else
	{
		return;
	}


	m_pVideoIo = CreateThreadpoolIo( 
		( HANDLE )m_ServerVideoSocket
		, ( PTP_WIN32_IO_CALLBACK )ReceiveCompleteRoutine
		, NULL
		, &m_VideoCallbackEnvironment
		);
}

VOID CServerNetwork::DeallocateReceiveThreadpool()
{
	WaitForThreadpoolIoCallbacks(
		m_pVideoIo
		, FALSE
		);
	return;

}

VOID CServerNetwork::DeallocateSendThreadpool()
{
}


VOID CServerNetwork::DeallocateManagementThreadpool()
{
}

VOID CServerNetwork::ProcessManagementRequest( 
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	//OutputDebugStringf( TEXT( "%s - %s -%s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pEventHeader->eventType );

	switch( pEventHeader->eventType )
	{
		case BE_ESTABLISH_VIDEO_SUBSCRIPTION:
		case BE_REMOVE_VIDEO_SUBSCRIPTION:
			OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );
			ProcessVideoSubscriptionRequest(
				pIpHeader
				, pUdpHeader
				, pEventHeader
				, pEventPacketHeader
				, pPacketDataBlob
				, pServerNetwork
				);
			break;
		case BE_ESTABLISH_VIDEO_SOURCE:
		case BE_REMOVE_VIDEO_SOURCE:
			OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );
			pServerNetwork->ProcessVideoSourceRequest(
				pIpHeader
				, pUdpHeader
				, pEventHeader
				, pEventPacketHeader
				, pPacketDataBlob
				, pServerNetwork
				);
			break;
		case BE_ESTABLISH_AUDIO_SOURCE:
		case BE_REMOVE_AUDIO_SOURCE:
			OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );
			pServerNetwork->ProcessAudioSourceRequest(
				pIpHeader
				, pUdpHeader
				, pEventHeader
				, pEventPacketHeader
				, pPacketDataBlob
				, pServerNetwork
				);
			break;
		case BE_KEEP_ALIVE:
			pServerNetwork->ProcessKeepAliveRequest(
				pIpHeader
				, pUdpHeader
				, pEventHeader
				, pEventPacketHeader
				, pPacketDataBlob
				, pServerNetwork
				);
			break;
		default:
			goto ProcessManagementRequest_Exit;
			break;
	}
ProcessManagementRequest_Exit:
	return;
}

VOID CServerNetwork::ProcessVideoMultimediaPublication( 
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	if( pEventHeader->eventType != BE_PUBLISH_VIDEO_FRAME )
	{
		goto ProcessVideoMultimediaPublication_Exit;
	}

	DWORD dwBytesSent = 0
		, dwFlags = 0
		;

	INT32 m_iLastError = 0;

	CServerVideoSource* pSourceIpHeader = NULL;
	CIpHeader* pDestIpHeader = NULL;

	PUBLISHING_POINT_SOURCE_VIDEO_ENTRIES* pTmpSourceClient = NULL;
	PUBLISHING_POINT_CLIENTS* pTmpDestClient = NULL;

	POSITION posSource;
	POSITION posDest;

	BOOL bSource		= FALSE
		, bSourceClient = FALSE
		, bDest			= FALSE
		, bDestClient	= FALSE
		;

	pServerNetwork->m_csVideoPubSourceLock.LockShared();
	bSource = pServerNetwork->m_pubPointVideoSources.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpSourceClient );
	pServerNetwork->m_csVideoPubSourceLock.ReleaseShared();

	if( !bSource )
	{
		goto ProcessVideoMultimediaPublication_Exit;
	}

	pServerNetwork->m_csVideoSubscriberLock.LockShared();
	bSourceClient = pTmpSourceClient->Lookup( 
		CString( pEventHeader->eventMacAddress  ).Trim()
		, pSourceIpHeader 
		);
	pServerNetwork->m_csVideoSubscriberLock.ReleaseShared();

	if( !bSourceClient )
	{
		pSourceIpHeader = NULL;
		goto ProcessVideoMultimediaPublication_Exit;
	}

	pServerNetwork->m_csPubDestinationLock.LockShared();
	bDest = pServerNetwork->m_pubPointDestinations.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpDestClient );
	pServerNetwork->m_csPubDestinationLock.ReleaseShared();

	if( !bDest )
	{
		goto ProcessVideoMultimediaPublication_Exit;
	}

	CIpHeader* tmpDestIpHeader = NULL;

	UINT uiTmpIpSrc = pIpHeader->ip_srcaddr;
	USHORT usTmpUdpSrc = pUdpHeader->src_portno;

	posDest = pTmpDestClient->GetHeadPosition();

	
	USHORT usLength = MAX_IP_HEADER
						+ MAX_UDP_HEADER
						+ MAX_EVENT_HEADER
						+ MAX_EVENT_PACKET_HEADER
						+ pEventPacketHeader->dataBlobSize
						;

	while( NULL != posDest )
	{
		CIpHeader* pTmpHeader = pTmpDestClient->GetNextValue( posDest );

#ifndef ENABLE_SELF_ROUTING
		if( 0 == strcmp( pTmpHeader->m_szMACAddress, pEventHeader->eventMacAddress ) )  
		{
			continue;
		}
#endif

		CServerSendThreadData* pServerSendThreadData =
			pServerNetwork->m_pSendBuffer->GetCurrentPacketThreadData();
		if( NULL == pServerSendThreadData )
		{
			continue;
		}

		pIpHeader->ip_destaddr = pTmpHeader->m_ipHeader.ip_srcaddr;
		pUdpHeader->dst_portno = pTmpHeader->m_udpHeader.src_portno;
		pIpHeader->ip_srcaddr = pServerNetwork->m_ClientSocketAddr.sin_addr.s_addr;//ipHeaderDests[ i ]->m_ipHeader.ip_destaddr;
		pUdpHeader->src_portno = pServerNetwork->m_ClientSocketAddr.sin_port; //ipHeaderDests[ i ]->m_udpHeader.dst_portno;

		pUdpHeader->udp_checksum = 0;
		
		pServerSendThreadData->m_socketAddrSrvVideo.sin_family = AF_INET;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_addr.s_addr = pIpHeader->ip_destaddr;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_port = pUdpHeader->dst_portno;

		pServerSendThreadData->IngestNetworkPacket( 
			reinterpret_cast< PCHAR >( pIpHeader )
			, usLength
			);

		//ComputeUdpPseudoHeaderChecksumV4(
		//	pServerSendThreadData->get_IpHeader()
		//	, pServerSendThreadData->get_UdpHeader()
		//	, reinterpret_cast<PCHAR>( pServerSendThreadData->get_EventHeader() )
		//	, MAX_EVENT_HEADER
		//		+ MAX_EVENT_PACKET_HEADER
		//		+ pServerSendThreadData->get_EventPacketHeader()->dataBlobSize 
		//	);

		StartThreadpoolIo( pServerNetwork->m_pClientIo );
		int rc = WSASendTo(
					pServerNetwork->m_ClientSocket
					, pServerSendThreadData->get_WSABUF()
					, 1
					, &dwBytesSent
					, dwFlags
					, ( SOCKADDR* ) &pServerSendThreadData->m_socketAddrSrvVideo
					, ( int )sizeof( SOCKADDR_IN )
					, pServerSendThreadData
					, NULL//SubmitVideoToClientCompleteRoutine
					);

		//if( 0 != rc )
		//{
		//	INT32 wsaError = WSAGetLastError();
		//	
		//	switch( wsaError )
		//	{
		//	case ERROR_IO_PENDING:
		//		break;
		//	default:
		//		CancelThreadpoolIo( pServerNetwork->m_pClientIo );
		//		ReturnServerSendThreadData( pServerSendThreadData, pServerNetwork  );
		//		break;
		//	}
		//}
	/* end */

	};

ProcessVideoMultimediaPublication_Exit:

	return;
}


VOID CServerNetwork::ProcessAudioMultimediaPublication( 
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	ATLASSERT( pEventHeader != NULL
		&& pEventHeader->eventType == BE_PUBLISH_AUDIO_SAMPLE );

	if( pEventHeader->eventType != BE_PUBLISH_AUDIO_SAMPLE )
	{
		//log error message.
		goto ProcessAudioMultimediaPublication_Exit;
	}

	DWORD dwBytesSent = 0
		, dwFlags = 0
		;

	INT32 m_iLastError = 0;

	CServerAudioSource* pSourceIpHeader = NULL;
	CIpHeader* pDestIpHeader = NULL;

	PUBLISHING_POINT_SOURCE_AUDIO_ENTRIES* pTmpSourceClient = NULL;
	PUBLISHING_POINT_CLIENTS* pTmpDestClient = NULL;

	POSITION posSource;
	POSITION posDest;

	BOOL bSource		= FALSE
		, bSourceClient = FALSE
		, bDest			= FALSE
		, bDestClient	= FALSE
		;

	pServerNetwork->m_csAudioPubSourceLock.LockShared();
	bSource = pServerNetwork->m_pubPointAudioSources.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpSourceClient );
	pServerNetwork->m_csAudioPubSourceLock.ReleaseShared();

	if( !bSource )
	{
		goto ProcessAudioMultimediaPublication_Exit;
	}

	pServerNetwork->m_csAudioSubscriberLock.LockShared();
	bSourceClient = pTmpSourceClient->Lookup( 
		CString( pEventHeader->eventMacAddress  ).Trim()
		, pSourceIpHeader 
		);
	pServerNetwork->m_csAudioSubscriberLock.ReleaseShared();

	if( !bSourceClient )
	{
		pSourceIpHeader = NULL;
		goto ProcessAudioMultimediaPublication_Exit;
	}

	pServerNetwork->m_csPubDestinationLock.LockShared();
	bDest = pServerNetwork->m_pubPointDestinations.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpDestClient );
	pServerNetwork->m_csPubDestinationLock.ReleaseShared();

	if( !bDest )
	{
		goto ProcessAudioMultimediaPublication_Exit;
	}

	CIpHeader* tmpDestIpHeader = NULL;

	UINT uiTmpIpSrc = pIpHeader->ip_srcaddr;
	USHORT usTmpUdpSrc = pUdpHeader->src_portno;

	posDest = pTmpDestClient->GetHeadPosition();

	
	USHORT usLength = MAX_IP_HEADER
						+ MAX_UDP_HEADER
						+ MAX_EVENT_HEADER
						+ MAX_EVENT_PACKET_HEADER
						+ pEventPacketHeader->dataBlobSize
						;

	while( NULL != posDest )
	{
		CIpHeader* pTmpHeader = pTmpDestClient->GetNextValue( posDest );

		if( 0 == strcmp( pTmpHeader->m_szMACAddress, pEventHeader->eventMacAddress ) )  
		{
			continue;
		}

		CServerSendThreadData* pServerSendThreadData =
			pServerNetwork->m_pSendBuffer->GetCurrentPacketThreadData();
		if( NULL == pServerSendThreadData )
		{
			continue;
		}

		pIpHeader->ip_destaddr = pTmpHeader->m_ipHeader.ip_srcaddr;
		pUdpHeader->dst_portno = pTmpHeader->m_udpHeader.src_portno;
		pIpHeader->ip_srcaddr = pServerNetwork->m_ClientSocketAddr.sin_addr.s_addr;//ipHeaderDests[ i ]->m_ipHeader.ip_destaddr;
		pUdpHeader->src_portno = pServerNetwork->m_ClientSocketAddr.sin_port; //ipHeaderDests[ i ]->m_udpHeader.dst_portno;

		pUdpHeader->udp_checksum = 0;
		
		pServerSendThreadData->m_socketAddrSrvVideo.sin_family = AF_INET;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_addr.s_addr = pIpHeader->ip_destaddr;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_port = pUdpHeader->dst_portno;

		pServerSendThreadData->IngestNetworkPacket( 
			reinterpret_cast< PCHAR >( pIpHeader )
			, usLength
			);

		//ComputeUdpPseudoHeaderChecksumV4(
		//	pServerSendThreadData->get_IpHeader()
		//	, pServerSendThreadData->get_UdpHeader()
		//	, reinterpret_cast<PCHAR>( pServerSendThreadData->get_EventHeader() )
		//	, MAX_EVENT_HEADER
		//		+ MAX_EVENT_PACKET_HEADER
		//		+ pServerSendThreadData->get_EventPacketHeader()->dataBlobSize 
		//	);

		StartThreadpoolIo( pServerNetwork->m_pClientIo );
		WSASendTo(
			pServerNetwork->m_ClientSocket
			, pServerSendThreadData->get_WSABUF()
			, 1
			, &dwBytesSent
			, dwFlags
			, ( SOCKADDR* ) &pServerSendThreadData->m_socketAddrSrvVideo
			, ( int )sizeof( SOCKADDR_IN )
			, pServerSendThreadData
			, NULL//SubmitVideoToClientCompleteRoutine
			);
	/* end */

	};

ProcessAudioMultimediaPublication_Exit:

	return;
}

VOID CServerNetwork::ProcessAudioSourceRequest(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	DWORD dwReturnValue = 0;
	OutputDebugStringf( TEXT( "%s :  %s " ), __TFILE__ , __TFUNCTION__);//, __LINE__,  pEventHeader->eventType );

	if( pEventHeader->eventType == BE_ESTABLISH_AUDIO_SOURCE  )
	{
		PESTABLISH_AUDIO_SOURCE pEstablishAudioSource = 
			reinterpret_cast< PESTABLISH_AUDIO_SOURCE >( pPacketDataBlob );

		dwReturnValue = m_pPublishingPointSource->EstablishAudioSource(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pEstablishAudioSource
							);
	}
	else if( pEventHeader->eventType == BE_REMOVE_AUDIO_SOURCE )
	{

		dwReturnValue = m_pPublishingPointSource->RemoveAudioSource(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pPacketDataBlob
							);
	}

	//TODO: Check error code and decide how to handle in static async call...
	return;
}

VOID CServerNetwork::ProcessVideoSourceRequest(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	DWORD dwReturnValue = 0;
	//OutputDebugStringf( TEXT( "%s - %s - %s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pEventHeader->eventType );
	OutputDebugStringf( TEXT( "%s : %s " ), __TFILE__ , __TFUNCTION__);//, __LINE__,  pEventHeader->eventType );

	if( pEventHeader->eventType == BE_ESTABLISH_VIDEO_SOURCE  )
	{
		PESTABLISH_VIDEO_SOURCE pEstablishVideoSource = 
			reinterpret_cast< PESTABLISH_VIDEO_SOURCE >( pPacketDataBlob );

			dwReturnValue = pServerNetwork->m_pPublishingPointSource->EstablishVideoSource(
								pIpHeader
								, pUdpHeader
								, pEventHeader
								, pEventPacketHeader
								, pEstablishVideoSource
								//, m_pTxnObj
								);
	}
	else if( pEventHeader->eventType == BE_REMOVE_VIDEO_SOURCE )
	{

		dwReturnValue = pServerNetwork->m_pPublishingPointSource->RemoveVideoSource(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pPacketDataBlob
							);
	}

	//TODO: Check error code and decide how to handle in static async call...
	return;
}

VOID CServerNetwork::ProcessVideoSubscriptionRequest(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	DWORD dwReturnValue = 0;

	//OutputDebugStringf( TEXT( "%s - %s - %s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pEventHeader->eventType );

	if( pEventHeader->eventType == BE_ESTABLISH_VIDEO_SUBSCRIPTION  )
	{
		PESTABLISH_VIDEO_SUBSCRIPTION pEstablishVideoSubscription = 
			reinterpret_cast< PESTABLISH_VIDEO_SUBSCRIPTION >( pPacketDataBlob );

		dwReturnValue = pServerNetwork->m_pPublishingPointDestination->EstablishVideoSubscription(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, pEstablishVideoSubscription
							);
	}
	else if( pEventHeader->eventType == BE_REMOVE_VIDEO_SUBSCRIPTION )
	{
		dwReturnValue = pServerNetwork->m_pPublishingPointDestination->RemoveMediaDestination(
							pIpHeader
							, pUdpHeader
							, pEventHeader
							, pEventPacketHeader
							, NULL
							);
	}

	//TODO: Check error code and decide how to handle in static async call...
	return;
}

VOID CServerNetwork::ProcessKeepAliveRequest(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	DWORD dwReturnValue = 0;
	dwReturnValue = pServerNetwork->m_pManagement->LogKeepAlive(
						pIpHeader
						, pUdpHeader
						, pEventHeader
						, pEventPacketHeader
						, pPacketDataBlob
						);

	return;
}

VOID CServerNetwork::ServerEventVideoSource(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	)
{
	PUBLISHING_POINT_MACADDR szEventMacAddress( pEventHeader->eventMacAddress );
	szEventMacAddress.TrimRight( ' ' );
	//OutputDebugStringf( TEXT( "%s - %s - %s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pEventHeader->eventType );
	OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );

	CIpHeader* pCacheIpHeader = NULL;
	CServerVideoSource* pCacheMediaSource = NULL;

	PUBLISHING_POINT_SOURCE_VIDEO_ENTRIES* pTmpSourceClient = NULL;

	PUBLISHING_POINT_CLIENTS* pTmpDestClient = NULL;

	POSITION posDest;

	BOOL bDest			= FALSE
		, bDestClient	= FALSE
		;

	m_csPubDestinationLock.LockShared();
	bDest = m_pubPointDestinations.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpDestClient );
	m_csPubDestinationLock.ReleaseShared();

	if( !bDest )
	{
		goto ServerEventVideoSource_Exit;
	}

	OutputDebugStringf( TEXT( "%s :  %s : %d : %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "destinations found" ) );

	CIpHeader* tmpDestIpHeader = NULL;

	UINT uiTmpIpSrc = pIpHeader->ip_srcaddr;
	USHORT usTmpUdpSrc = pUdpHeader->src_portno;

	posDest = pTmpDestClient->GetHeadPosition();
	
	USHORT usLength = ntohs( pIpHeader->ip_totallength );

	DWORD dwBytesSent = 0
			, dwFlags = 0;

	while( NULL != posDest )
	{
		CIpHeader* pTmpHeader = pTmpDestClient->GetNextValue( posDest );

		if( 0 == strcmp( pTmpHeader->m_szMACAddress, CW2A( szEventMacAddress.GetString() ) ) )  
		{
			continue;
		}
		OutputDebugStringf( TEXT( "%s :  %s : %d : %s - %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "dest macaddress found" ), ( CString( pTmpHeader->m_szMACAddress ) ) );

		CServerSendThreadData* pServerSendThreadData =
			m_pSendBuffer->GetCurrentPacketThreadData();
		if( NULL == pServerSendThreadData )
		{
			continue;
		}

		pIpHeader->ip_destaddr = pTmpHeader->m_ipHeader.ip_srcaddr;
		pUdpHeader->dst_portno = pTmpHeader->m_udpHeader.src_portno;
		pIpHeader->ip_srcaddr = m_ClientSocketAddr.sin_addr.s_addr;//ipHeaderDests[ i ]->m_ipHeader.ip_destaddr;
		pUdpHeader->src_portno = m_ClientSocketAddr.sin_port; //ipHeaderDests[ i ]->m_udpHeader.dst_portno;
		pUdpHeader->udp_checksum = 0;

		pServerSendThreadData->m_socketAddrSrvVideo.sin_family = AF_INET;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_addr.s_addr = pIpHeader->ip_destaddr;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_port = pUdpHeader->dst_portno;

		pServerSendThreadData->IngestNetworkPacket( 
			reinterpret_cast< PCHAR >( pIpHeader )
			, usLength
			);
		pUdpHeader->udp_checksum = 0;

		//ComputeUdpPseudoHeaderChecksumV4(
		//	pServerSendThreadData->get_IpHeader()
		//	, pServerSendThreadData->get_UdpHeader()
		//	, reinterpret_cast<PCHAR>( pServerSendThreadData->get_EventHeader() )
		//	, MAX_EVENT_HEADER
		//		+ MAX_EVENT_PACKET_HEADER
		//		+ pServerSendThreadData->get_EventPacketHeader()->dataBlobSize 
		//	);

		StartThreadpoolIo( m_pClientIo );
		
		WSASendTo(
			m_ClientSocket
			, pServerSendThreadData->get_WSABUF()
			, 1
			, &dwBytesSent
			, dwFlags
			, ( SOCKADDR* ) &pServerSendThreadData->m_socketAddrSrvVideo
			, ( int )sizeof( SOCKADDR_IN )
			, pServerSendThreadData
			, NULL//SubmitVideoToClientCompleteRoutine
			);
			OutputDebugStringf( TEXT( "%s :  %s : %d : %s - %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "packet sent" ), ( CString( pTmpHeader->m_szMACAddress ).GetString() ) );

	};

ServerEventVideoSource_Exit:
	return;
}

VOID CServerNetwork::ServerEventVideoSourceRt(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	)
{
	PUBLISHING_POINT_MACADDR szEventMacAddress( pEventHeader->eventMacAddress );
	szEventMacAddress.TrimRight( ' ' );
	//OutputDebugStringf( TEXT( "%s - %s - %s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pEventHeader->eventType );
	OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );

	CIpHeader* pCacheIpHeader = NULL;
	CServerVideoSource* pCacheMediaSource = NULL;

	PUBLISHING_POINT_SOURCE_VIDEO_ENTRIES* pTmpSourceClient = NULL;
    BOOL pubPoinVideoSourceExist = FALSE;
    BOOL tmpSourceClientExist = FALSE;

	if( pEventHeader->eventType == BE_ESTABLISH_VIDEO_SOURCE  )
	{
		pCacheIpHeader = EnsureClient(
			pIpHeader
			, pUdpHeader
			, szEventMacAddress
			, pEventPacketHeader
			, pPacketDataBlob
			, CT_VIDEO_SOURCE
			, this
			);


		PESTABLISH_VIDEO_SOURCE pEstablishVideoSource = 
			reinterpret_cast< PESTABLISH_VIDEO_SOURCE >( pPacketDataBlob );

		if( !m_pubPointVideoSources.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpSourceClient ) )
		{

			pTmpSourceClient = new PUBLISHING_POINT_SOURCE_VIDEO_ENTRIES( );
			pCacheMediaSource = new CServerVideoSource( 
				pCacheIpHeader
				, *pEventHeader
				, *pEventPacketHeader
				, *pEstablishVideoSource 
				);

			m_csVideoSubscriberLock.Lock();
			pTmpSourceClient->SetAt( 
				szEventMacAddress
				, pCacheMediaSource 
				);
			m_csVideoSubscriberLock.Release();

			m_csVideoPubSourceLock.Lock();
			m_pubPointVideoSources.SetAt(  
				pEventPacketHeader->eventPublishingPoint
				, pTmpSourceClient 
				);
			m_csVideoPubSourceLock.Release();
			OutputDebugStringf( TEXT( "%s :  %s : video source added for ppt %d - %s" ), __TFILE__ , __TFUNCTION__,  pEventPacketHeader->eventPublishingPoint, szEventMacAddress.GetString() );

			goto ServerEventVideoSource_Exit;
		}
		else
		{
			if( !pTmpSourceClient->Lookup( szEventMacAddress ) )
			{
				pCacheMediaSource = new CServerVideoSource( 
					pCacheIpHeader
					, *pEventHeader
					, *pEventPacketHeader
					, *pEstablishVideoSource 
					);

				m_csVideoSubscriberLock.Lock();
				pTmpSourceClient->SetAt( 
					szEventMacAddress
					, pCacheMediaSource 
					);
				m_csVideoSubscriberLock.Release();
				OutputDebugStringf( TEXT( "%s :  %s : video source added for ppt %d - %s" ), __TFILE__ , __TFUNCTION__,  pEventPacketHeader->eventPublishingPoint, szEventMacAddress.GetString() );
				
				goto ServerEventVideoSource_Exit;
			}
		}
	}
	else if( pEventHeader->eventType == BE_REMOVE_VIDEO_SOURCE )
	{
		if( m_pubPointVideoSources.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpSourceClient ) )
		{
			m_csVideoPubSourceLock.Lock();

			if( pTmpSourceClient->Lookup( szEventMacAddress, pCacheMediaSource ) )
			{
				pTmpSourceClient->RemoveKey( szEventMacAddress );
				delete pCacheMediaSource;
			}
			if( 0 == pTmpSourceClient->GetCount() )
			{
				m_pubPointVideoSources.RemoveKey( pEventPacketHeader->eventPublishingPoint );
				delete pTmpSourceClient;
			}
	
			m_csVideoPubSourceLock.Release();
			goto ServerEventVideoSource_Exit;
		}
		else
		{
			goto ServerEventVideoSource_Exit;
		}
	}
	else
	{
		goto ServerEventVideoSource_Exit;
	}

ServerEventVideoSource_Exit:
	return;
}

VOID CServerNetwork::ServerEventAudioSource(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	PUBLISHING_POINT_MACADDR szEventMacAddress( pEventHeader->eventMacAddress );
	szEventMacAddress.TrimRight( ' ' );
	//OutputDebugStringf( TEXT( "%s - %s - %s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pEventHeader->eventType );
	OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );

	CIpHeader* pCacheIpHeader = NULL;
	CServerAudioSource* pCacheMediaSource = NULL;

	PUBLISHING_POINT_SOURCE_AUDIO_ENTRIES* pTmpSourceClient = NULL;
	PUBLISHING_POINT_CLIENTS* pTmpDestClient = NULL;

	POSITION posDest;

	BOOL bDest			= FALSE
		, bDestClient	= FALSE
		;

	m_csPubDestinationLock.LockShared();
	bDest = m_pubPointDestinations.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpDestClient );
	m_csPubDestinationLock.ReleaseShared();

	if( !bDest )
	{
		goto ServerEventAudioSource_Exit;
	}

	OutputDebugStringf( TEXT( "%s :  %s : %d : %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "destinations found" ) );

	CIpHeader* tmpDestIpHeader = NULL;

	UINT uiTmpIpSrc = pIpHeader->ip_srcaddr;
	USHORT usTmpUdpSrc = pUdpHeader->src_portno;

	posDest = pTmpDestClient->GetHeadPosition();
	
	USHORT usLength = ntohs( pIpHeader->ip_totallength );

	DWORD dwBytesSent = 0
			, dwFlags = 0;

	while( NULL != posDest )
	{
		CIpHeader* pTmpHeader = pTmpDestClient->GetNextValue( posDest );

		if( 0 == strcmp( pTmpHeader->m_szMACAddress, CW2A( szEventMacAddress.GetString() ) ) )  
		{
			continue;
		}
		OutputDebugStringf( TEXT( "%s :  %s : %d : %s - %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "dest macaddress found" ), ( CString( pTmpHeader->m_szMACAddress ) ) );

		CServerSendThreadData* pServerSendThreadData =
			m_pSendBuffer->GetCurrentPacketThreadData();
		if( NULL == pServerSendThreadData )
		{
			continue;
		}

		pIpHeader->ip_destaddr = pTmpHeader->m_ipHeader.ip_srcaddr;
		pUdpHeader->dst_portno = pTmpHeader->m_udpHeader.src_portno;
		pIpHeader->ip_srcaddr = m_ClientSocketAddr.sin_addr.s_addr;//ipHeaderDests[ i ]->m_ipHeader.ip_destaddr;
		pUdpHeader->src_portno = m_ClientSocketAddr.sin_port; //ipHeaderDests[ i ]->m_udpHeader.dst_portno;
		pUdpHeader->udp_checksum = 0;

		pServerSendThreadData->m_socketAddrSrvVideo.sin_family = AF_INET;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_addr.s_addr = pIpHeader->ip_destaddr;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_port = pUdpHeader->dst_portno;

		pServerSendThreadData->IngestNetworkPacket( 
			reinterpret_cast< PCHAR >( pIpHeader )
			, usLength
			);
		pUdpHeader->udp_checksum = 0;

		//ComputeUdpPseudoHeaderChecksumV4(
		//	pServerSendThreadData->get_IpHeader()
		//	, pServerSendThreadData->get_UdpHeader()
		//	, reinterpret_cast<PCHAR>( pServerSendThreadData->get_EventHeader() )
		//	, MAX_EVENT_HEADER
		//		+ MAX_EVENT_PACKET_HEADER
		//		+ pServerSendThreadData->get_EventPacketHeader()->dataBlobSize 
		//	);

		StartThreadpoolIo( m_pClientIo );
		
		WSASendTo(
			m_ClientSocket
			, pServerSendThreadData->get_WSABUF()
			, 1
			, &dwBytesSent
			, dwFlags
			, ( SOCKADDR* ) &pServerSendThreadData->m_socketAddrSrvVideo
			, ( int )sizeof( SOCKADDR_IN )
			, pServerSendThreadData
			, NULL//SubmitVideoToClientCompleteRoutine
			);
			OutputDebugStringf( TEXT( "%s :  %s : %d : %s - %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "packet sent" ), ( CString( pTmpHeader->m_szMACAddress ).GetString() ) );

	};



ServerEventAudioSource_Exit:
	return;
}


VOID CServerNetwork::ServerEventAudioSourceRt(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	PUBLISHING_POINT_MACADDR szEventMacAddress( pEventHeader->eventMacAddress );
	szEventMacAddress.TrimRight( ' ' );
	//OutputDebugStringf( TEXT( "%s - %s - %s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pEventHeader->eventType );
	OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );

	CIpHeader* pCacheIpHeader = NULL;
	CServerAudioSource* pCacheMediaSource = NULL;

	PUBLISHING_POINT_SOURCE_AUDIO_ENTRIES* pTmpSourceClient = NULL;

	if( pEventHeader->eventType == BE_ESTABLISH_AUDIO_SOURCE  )
	{
		pCacheIpHeader = EnsureClient(
			pIpHeader
			, pUdpHeader
			, szEventMacAddress
			, pEventPacketHeader
			, pPacketDataBlob
			, CT_AUDIO_SOURCE
			, pServerNetwork
			);

		PESTABLISH_AUDIO_SOURCE pEstablishAudioSource = 
			reinterpret_cast< PESTABLISH_AUDIO_SOURCE >( pPacketDataBlob );

		if( !m_pubPointAudioSources.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpSourceClient ) )
		{
			pTmpSourceClient = new PUBLISHING_POINT_SOURCE_AUDIO_ENTRIES( );
			pCacheMediaSource = new CServerAudioSource( 
				pCacheIpHeader
				, *pEventHeader
				, *pEventPacketHeader
				, *pEstablishAudioSource );

			m_csAudioSubscriberLock.Lock();
			pTmpSourceClient->SetAt( 
				szEventMacAddress
				, pCacheMediaSource 
				);
			m_csAudioSubscriberLock.Release();

			m_csAudioPubSourceLock.Lock();
			m_pubPointAudioSources.SetAt(  
				pEventPacketHeader->eventPublishingPoint
				, pTmpSourceClient 
				);
			m_csAudioPubSourceLock.Release();
			OutputDebugStringf( TEXT( "%s :  %s : audio source added for ppt %d - %s" ), __TFILE__ , __TFUNCTION__,  pEventPacketHeader->eventPublishingPoint, szEventMacAddress.GetString() );

			goto ServerEventAudioSourceRt_Exit;
		}
		else
		{
			if( !pTmpSourceClient->Lookup( szEventMacAddress ) )
			{
				pCacheMediaSource = new CServerAudioSource( 
					pCacheIpHeader
					, *pEventHeader
					, *pEventPacketHeader
					, *pEstablishAudioSource 
					);

				m_csAudioSubscriberLock.Lock();
				pTmpSourceClient->SetAt( 
					szEventMacAddress
					, pCacheMediaSource 
					);
				m_csAudioSubscriberLock.Release();
				OutputDebugStringf( TEXT( "%s :  %s : video source added for ppt %d - %s" ), __TFILE__ , __TFUNCTION__,  pEventPacketHeader->eventPublishingPoint, szEventMacAddress.GetString() );
				
				goto ServerEventAudioSourceRt_Exit;
			}
		}
	}
	else if( pEventHeader->eventType == BE_REMOVE_AUDIO_SOURCE )
	{
		if( m_pubPointAudioSources.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpSourceClient ) )
		{
			m_csAudioPubSourceLock.Lock();

			if( pTmpSourceClient->Lookup( szEventMacAddress, pCacheMediaSource ) )
			{
				pTmpSourceClient->RemoveKey( szEventMacAddress );
				delete pCacheMediaSource;
			}
			if( 0 == pTmpSourceClient->GetCount() )
			{
				m_pubPointAudioSources.RemoveKey( pEventPacketHeader->eventPublishingPoint );
				delete pTmpSourceClient;
			}
	
			m_csAudioPubSourceLock.Release();
			goto ServerEventAudioSourceRt_Exit;
		}
		else
		{
			goto ServerEventAudioSourceRt_Exit;
		}
	}

ServerEventAudioSourceRt_Exit:
	return;
}

VOID CServerNetwork::ServerEventVideoSubscription(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	PUBLISHING_POINT_MACADDR szEventMacAddress( pEventHeader->eventMacAddress );
	szEventMacAddress.TrimRight( ' ' );
	//OutputDebugStringf( TEXT( "%s - %s - %s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pEventHeader->eventType );
	OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );

	CIpHeader* pCacheIpHeader = NULL;
	PUBLISHING_POINT_CLIENTS* pTmpDestClient = NULL;
	if( pEventHeader->eventType == BE_ESTABLISH_VIDEO_SUBSCRIPTION  )
	{
		//Video sources

		PUBLISHING_POINT_SOURCE_VIDEO_ENTRIES* pTmpSourceClient = NULL;

		POSITION posSrc;

		BOOL bSource		= FALSE
			, bSourceClient = FALSE
			;

		pServerNetwork->m_csVideoPubSourceLock.LockShared();
		bSource = pServerNetwork->m_pubPointVideoSources.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpSourceClient );
		pServerNetwork->m_csVideoPubSourceLock.ReleaseShared();

		if( !bSource )
		{
			goto check_audio;
		}
		OutputDebugStringf( TEXT( "%s :  %s : %d : %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "video sources found" ) );

		CServerVideoSource* tmpSrcVideoSource = NULL;

		posSrc = pTmpSourceClient->GetHeadPosition();
		
		while( NULL != posSrc )
		{
			DWORD dwBytesSent = 0
					, dwFlags = 0;

			CServerVideoSource* pTmpVideoSource = pTmpSourceClient->GetNextValue( posSrc );

#ifndef ENABLE_SELF_ROUTING
			if( 0 == strcmp( pTmpVideoSource->m_pIpHeader->m_szMACAddress, CW2A( szEventMacAddress.GetString() ) ) )  
			{
				continue;
			}
#endif
			OutputDebugStringf( TEXT( "%s :  %s : %d : %s - %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "source macaddress found" ), ( CString( pTmpVideoSource->m_pIpHeader->m_szMACAddress ) ) );

			CServerSendThreadData* pServerSendThreadData =
				m_pSendBuffer->GetCurrentPacketThreadData();
			if( NULL == pServerSendThreadData )
			{
				continue;
			}

			USHORT usLength = ntohs( pTmpVideoSource->m_pIpHeader->m_ipHeader.ip_totallength );
			pServerSendThreadData->ReallocBuffer( usLength );

			PIP_HEADER pIpHeaderO =
				pServerSendThreadData->get_IpHeader();
			PUDP_HEADER pUdpHeaderO =
				pServerSendThreadData->get_UdpHeader();
			PEVENT_HEADER pEventHeaderO =
				pServerSendThreadData->get_EventHeader();
			PEVENT_PACKET_HEADER pEventPacketHeaderO =
				pServerSendThreadData->get_EventPacketHeader();
			PESTABLISH_VIDEO_SOURCE pEstablishVideoSourceO =
				reinterpret_cast<PESTABLISH_VIDEO_SOURCE>( pServerSendThreadData->get_PacketDataBlob() );

			::CopyMemory(
				pIpHeaderO
				, &pTmpVideoSource->m_pIpHeader->m_ipHeader
				, MAX_IP_HEADER
				);

			::CopyMemory(
				pUdpHeaderO
				, &pTmpVideoSource->m_pIpHeader->m_udpHeader
				, MAX_UDP_HEADER
				);

			::CopyMemory(
				pEventHeaderO
				, &pTmpVideoSource->m_eventHeader
				, MAX_EVENT_HEADER
				);

			::CopyMemory(
				pEventPacketHeaderO
				, &pTmpVideoSource->m_eventPacketHeader
				, MAX_EVENT_PACKET_HEADER
				);

			::CopyMemory(
				pEstablishVideoSourceO
				, &pTmpVideoSource->m_videoSource
				, MAX_ESTABLISH_VIDEO_SOURCE
				);

			pIpHeaderO->ip_destaddr = pIpHeader->ip_srcaddr;
			pUdpHeaderO->dst_portno = pUdpHeader->src_portno;
			pIpHeaderO->ip_srcaddr = m_ClientSocketAddr.sin_addr.s_addr;//ipHeaderDests[ i ]->m_ipHeader.ip_destaddr;
			pUdpHeaderO->src_portno = m_ClientSocketAddr.sin_port; //ipHeaderDests[ i ]->m_udpHeader.dst_portno;
			pUdpHeaderO->udp_checksum = 0;

			pServerSendThreadData->m_socketAddrSrvVideo.sin_family = AF_INET;
			pServerSendThreadData->m_socketAddrSrvVideo.sin_addr.s_addr = pIpHeaderO->ip_destaddr;
			pServerSendThreadData->m_socketAddrSrvVideo.sin_port = pUdpHeaderO->dst_portno;

			pUdpHeader->udp_checksum = 0;

			//ComputeUdpPseudoHeaderChecksumV4(
			//	pServerSendThreadData->get_IpHeader()
			//	, pServerSendThreadData->get_UdpHeader()
			//	, reinterpret_cast<PCHAR>( pServerSendThreadData->get_EventHeader() )
			//	, MAX_EVENT_HEADER
			//		+ MAX_EVENT_PACKET_HEADER
			//		+ pServerSendThreadData->get_EventPacketHeader()->dataBlobSize 
			//		);

			StartThreadpoolIo( m_pClientIo );
			
			WSASendTo(
				m_ClientSocket
				, pServerSendThreadData->get_WSABUF()
				, 1
				, &dwBytesSent
				, dwFlags
				, ( SOCKADDR* ) &pServerSendThreadData->m_socketAddrSrvVideo
				, ( int )sizeof( SOCKADDR_IN )
				, pServerSendThreadData
				, NULL//SubmitVideoToClientCompleteRoutine
				);
			OutputDebugStringf( TEXT( "%s :  %s : %d : %s - %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "source macaddress sent" ), ( CString(  pTmpVideoSource->m_pIpHeader->m_szMACAddress ) ) );
		};

check_audio:
		//Audio sources

		PUBLISHING_POINT_SOURCE_AUDIO_ENTRIES* pTmpAudioSourceClient = NULL;

		bSource	= FALSE;
		bSourceClient = FALSE;

		pServerNetwork->m_csAudioPubSourceLock.LockShared();
		bSource = pServerNetwork->m_pubPointAudioSources.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpAudioSourceClient );
		pServerNetwork->m_csAudioPubSourceLock.ReleaseShared();

		if( !bSource )
		{
			goto ServerEventVideoSubscription_Exit;
		}
		OutputDebugStringf( TEXT( "%s :  %s : %d : %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "audio sources found" ) );

		CServerAudioSource* tmpSrcAudioSource = NULL;

		posSrc = pTmpAudioSourceClient->GetHeadPosition();
		
		while( NULL != posSrc )
		{
			DWORD dwBytesSent = 0
					, dwFlags = 0;

			CServerAudioSource* pTmpAudioSource = pTmpAudioSourceClient->GetNextValue( posSrc );

			if( 0 == strcmp( pTmpAudioSource->m_pIpHeader->m_szMACAddress, CW2A( szEventMacAddress.GetString() ) ) )  
			{
				continue;
			}
			OutputDebugStringf( TEXT( "%s :  %s : %d : %s - %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "source macaddress found" ), ( CString( pTmpAudioSource->m_pIpHeader->m_szMACAddress ) ) );

			CServerSendThreadData* pServerSendThreadData =
				m_pSendBuffer->GetCurrentPacketThreadData();
			if( NULL == pServerSendThreadData )
			{
				continue;
			}

			USHORT usLength = ntohs( pTmpAudioSource->m_pIpHeader->m_ipHeader.ip_totallength );
			pServerSendThreadData->ReallocBuffer( usLength );

			PIP_HEADER pIpHeaderO =
				pServerSendThreadData->get_IpHeader();
			PUDP_HEADER pUdpHeaderO =
				pServerSendThreadData->get_UdpHeader();
			PEVENT_HEADER pEventHeaderO =
				pServerSendThreadData->get_EventHeader();
			PEVENT_PACKET_HEADER pEventPacketHeaderO =
				pServerSendThreadData->get_EventPacketHeader();
			PESTABLISH_AUDIO_SOURCE pEstablishAudioSourceO =
				reinterpret_cast<PESTABLISH_AUDIO_SOURCE>( pServerSendThreadData->get_PacketDataBlob() );

			::CopyMemory(
				pIpHeaderO
				, &pTmpAudioSource->m_pIpHeader->m_ipHeader
				, MAX_IP_HEADER
				);

			::CopyMemory(
				pUdpHeaderO
				, &pTmpAudioSource->m_pIpHeader->m_udpHeader
				, MAX_UDP_HEADER
				);

			::CopyMemory(
				pEventHeaderO
				, &pTmpAudioSource->m_eventHeader
				, MAX_EVENT_HEADER
				);

			::CopyMemory(
				pEventPacketHeaderO
				, &pTmpAudioSource->m_eventPacketHeader
				, MAX_EVENT_PACKET_HEADER
				);

			::CopyMemory(
				pEstablishAudioSourceO
				, &pTmpAudioSource->m_audioSource
				, MAX_ESTABLISH_AUDIO_SOURCE
				);

			pIpHeaderO->ip_destaddr = pIpHeader->ip_srcaddr;
			pUdpHeaderO->dst_portno = pUdpHeader->src_portno;
			pIpHeaderO->ip_srcaddr = m_ClientSocketAddr.sin_addr.s_addr;//ipHeaderDests[ i ]->m_ipHeader.ip_destaddr;
			pUdpHeaderO->src_portno = m_ClientSocketAddr.sin_port; //ipHeaderDests[ i ]->m_udpHeader.dst_portno;
			pUdpHeaderO->udp_checksum = 0;

			pServerSendThreadData->m_socketAddrSrvVideo.sin_family = AF_INET;
			pServerSendThreadData->m_socketAddrSrvVideo.sin_addr.s_addr = pIpHeaderO->ip_destaddr;
			pServerSendThreadData->m_socketAddrSrvVideo.sin_port = pUdpHeaderO->dst_portno;

			pUdpHeader->udp_checksum = 0;
			//ComputeUdpPseudoHeaderChecksumV4(
			//	pServerSendThreadData->get_IpHeader()
			//	, pServerSendThreadData->get_UdpHeader()
			//	, reinterpret_cast<PCHAR>( pServerSendThreadData->get_EventHeader() )
			//	, MAX_EVENT_HEADER
			//		+ MAX_EVENT_PACKET_HEADER
			//		+ pServerSendThreadData->get_EventPacketHeader()->dataBlobSize 
			//		);

			StartThreadpoolIo( m_pClientIo );
			
			WSASendTo(
				m_ClientSocket
				, pServerSendThreadData->get_WSABUF()
				, 1
				, &dwBytesSent
				, dwFlags
				, ( SOCKADDR* ) &pServerSendThreadData->m_socketAddrSrvVideo
				, ( int )sizeof( SOCKADDR_IN )
				, pServerSendThreadData
				, NULL//SubmitVideoToClientCompleteRoutine
				);
			OutputDebugStringf( TEXT( "%s :  %s : %d : %s - %s" ), __TFILE__ , __TFUNCTION__, __LINE__,  TEXT( "source macaddress sent" ), ( CString(  pTmpAudioSource->m_pIpHeader->m_szMACAddress ) ) );
		};
	}

ServerEventVideoSubscription_Exit:
	return;
}


VOID CServerNetwork::ServerEventVideoSubscriptionRt(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CServerNetwork* pServerNetwork
	)
{
	PUBLISHING_POINT_MACADDR szEventMacAddress( pEventHeader->eventMacAddress );
	szEventMacAddress.TrimRight( ' ' );
	//OutputDebugStringf( TEXT( "%s - %s - %s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pEventHeader->eventType );
	OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );

	CIpHeader* pCacheIpHeader = NULL;
	PUBLISHING_POINT_CLIENTS* pTmpDestClient = NULL;

	if( pEventHeader->eventType == BE_ESTABLISH_VIDEO_SUBSCRIPTION  )
	{
		pCacheIpHeader = EnsureClient(
			pIpHeader
			, pUdpHeader
			, szEventMacAddress
			, pEventPacketHeader
			, pPacketDataBlob
			, CT_DESTINATION
			, pServerNetwork
			);

		if( !m_pubPointDestinations.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpDestClient ) )
		{
			pTmpDestClient = new PUBLISHING_POINT_CLIENTS( );

			m_csVideoSubscriberLock.Lock();
			pTmpDestClient->SetAt( 
				szEventMacAddress
				, pCacheIpHeader 
				);
			m_csVideoSubscriberLock.Release();

			m_csPubDestinationLock.Lock();
			m_pubPointDestinations.SetAt(  
				pEventPacketHeader->eventPublishingPoint
				, pTmpDestClient 
				);
			m_csPubDestinationLock.Release();
			OutputDebugStringf( TEXT( "%s :  %s : video destination added for ppt %d - %s" ), __TFILE__ , __TFUNCTION__,  pEventPacketHeader->eventPublishingPoint, szEventMacAddress.GetString() );

			goto ServerEventVideoSubscriptionRt_Exit;
		}
		else
		{
			if( !pTmpDestClient->Lookup( szEventMacAddress ) )
			{
				m_csVideoSubscriberLock.Lock();
				pTmpDestClient->SetAt( 
					szEventMacAddress
					, pCacheIpHeader 
					);
				m_csVideoSubscriberLock.Release();
				OutputDebugStringf( TEXT( "%s :  %s : video destination added for ppt %d - %s" ), __TFILE__ , __TFUNCTION__,  pEventPacketHeader->eventPublishingPoint, szEventMacAddress.GetString() );

				goto ServerEventVideoSubscriptionRt_Exit;
			}
		}
	}
	else if( pEventHeader->eventType == BE_REMOVE_VIDEO_SUBSCRIPTION )
	{
		if( m_pubPointDestinations.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpDestClient ) )
		{

			m_csVideoSubscriberLock.Lock();
			if( pTmpDestClient->Lookup( szEventMacAddress ) )
			{
				pTmpDestClient->RemoveKey( szEventMacAddress );
			}
			m_csVideoSubscriberLock.Release();

			m_csPubDestinationLock.Lock();
			if( 0 == pTmpDestClient->GetCount() )
			{
				m_pubPointDestinations.RemoveKey( pEventPacketHeader->eventPublishingPoint );
			}
	
			m_csPubDestinationLock.Release();
			goto ServerEventVideoSubscriptionRt_Exit;
		}
		else
		{
			goto ServerEventVideoSubscriptionRt_Exit;
		}
	}

ServerEventVideoSubscriptionRt_Exit:
	return;
}

VOID CServerNetwork::ServerEventDropUser(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	)
{
	PUBLISHING_POINT_MACADDR szEventMacAddress( pEventHeader->eventMacAddress );
	szEventMacAddress.TrimRight( ' ' );
	OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );

	
	USHORT usLength = ntohs( pIpHeader->ip_totallength );

	DWORD dwBytesSent = 0
			, dwFlags = 0;

	CServerSendThreadData* pServerSendThreadData =
		m_pSendBuffer->GetCurrentPacketThreadData();
	if( NULL == pServerSendThreadData )
	{
		goto ServerEventDropUser_Exit;
	}

	pIpHeader->ip_destaddr = pIpHeader->ip_srcaddr;
	pUdpHeader->dst_portno = pUdpHeader->src_portno;
	pIpHeader->ip_srcaddr = m_ClientSocketAddr.sin_addr.s_addr;//ipHeaderDests[ i ]->m_ipHeader.ip_destaddr;
	pUdpHeader->src_portno = m_ClientSocketAddr.sin_port; //ipHeaderDests[ i ]->m_udpHeader.dst_portno;
	pUdpHeader->udp_checksum = 0;

	pServerSendThreadData->m_socketAddrSrvVideo.sin_family = AF_INET;
	pServerSendThreadData->m_socketAddrSrvVideo.sin_addr.s_addr = pIpHeader->ip_destaddr;
	pServerSendThreadData->m_socketAddrSrvVideo.sin_port = pUdpHeader->dst_portno;

	pServerSendThreadData->IngestNetworkPacket( 
		reinterpret_cast< PCHAR >( pIpHeader )
		, usLength
		);

	pUdpHeader->udp_checksum = 0;
	//ComputeUdpPseudoHeaderChecksumV4(
	//	pServerSendThreadData->get_IpHeader()
	//	, pServerSendThreadData->get_UdpHeader()
	//	, reinterpret_cast<PCHAR>( pServerSendThreadData->get_EventHeader() )
	//	, MAX_EVENT_HEADER
	//		+ MAX_EVENT_PACKET_HEADER
	//		+ pServerSendThreadData->get_EventPacketHeader()->dataBlobSize 
	//	);

	StartThreadpoolIo( m_pClientIo );
	
	WSASendTo(
		m_ClientSocket
		, pServerSendThreadData->get_WSABUF()
		, 1
		, &dwBytesSent
		, dwFlags
		, ( SOCKADDR* ) &pServerSendThreadData->m_socketAddrSrvVideo
		, ( int )sizeof( SOCKADDR_IN )
		, pServerSendThreadData
		, NULL//SubmitVideoToClientCompleteRoutine
		);
	OutputDebugStringf( TEXT( " drop user set %s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventType );

ServerEventDropUser_Exit:
	return;
}

CIpHeader* CServerNetwork::EnsureClient(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PUBLISHING_POINT_MACADDR szEventMacAddress
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	, CLIENT_TYPE clientType
	, CServerNetwork* pServerNetwork
	)
{
	
	CIpHeader* pCacheIpHeader = NULL;

	switch( clientType )
	{
	case CT_VIDEO_SOURCE:
		if( !pServerNetwork->m_pVideoSourceClients.Lookup( szEventMacAddress, pCacheIpHeader ) )
		{
			pCacheIpHeader = new CIpHeader( *pIpHeader, *pUdpHeader, CW2A( szEventMacAddress.GetString() ) );

			pServerNetwork->m_csVideoSubscriberLock.Lock();
			pServerNetwork->m_pVideoSourceClients.SetAt( 
				szEventMacAddress
				, pCacheIpHeader 
				);
			pServerNetwork->m_csVideoSubscriberLock.Release();
		}
		else
		{
			if( pCacheIpHeader->m_udpHeader.src_portno != pUdpHeader->src_portno 
				|| pCacheIpHeader->m_ipHeader.ip_srcaddr !=  pIpHeader->ip_srcaddr )
			{
				pServerNetwork->m_csVideoSubscriberLock.Lock();
				pCacheIpHeader->m_ipHeader.ip_srcaddr = pIpHeader->ip_srcaddr;
				pCacheIpHeader->m_udpHeader.src_portno = pUdpHeader->src_portno;
				pServerNetwork->m_csVideoSubscriberLock.Release();
			}
		}
	break;
	case CT_AUDIO_SOURCE:
		if( !pServerNetwork->m_pAudioSourceClients.Lookup( szEventMacAddress, pCacheIpHeader ) )
		{
			pCacheIpHeader = new CIpHeader( *pIpHeader, *pUdpHeader, CW2A( szEventMacAddress.GetString() ) );

			pServerNetwork->m_csAudioSubscriberLock.Lock();
			pServerNetwork->m_pAudioSourceClients.SetAt( 
				szEventMacAddress
				, pCacheIpHeader 
				);
			pServerNetwork->m_csAudioSubscriberLock.Release();
		}
		else
		{
			if( pCacheIpHeader->m_udpHeader.src_portno != pUdpHeader->src_portno 
				|| pCacheIpHeader->m_ipHeader.ip_srcaddr !=  pIpHeader->ip_srcaddr )
			{
				pServerNetwork->m_csAudioSubscriberLock.Lock();
				pCacheIpHeader->m_ipHeader.ip_srcaddr = pIpHeader->ip_srcaddr;
				pCacheIpHeader->m_udpHeader.src_portno = pUdpHeader->src_portno;
				pServerNetwork->m_csAudioSubscriberLock.Release();
			}
		}
	break;
	case CT_DESTINATION:
			if( !pServerNetwork->m_pDestinationClients.Lookup( szEventMacAddress, pCacheIpHeader ) )
			{
				pCacheIpHeader = new CIpHeader( *pIpHeader, *pUdpHeader, CW2A( szEventMacAddress.GetString() ) );

				pServerNetwork->m_csPubDestinationLock.Lock();
				pServerNetwork->m_pDestinationClients.SetAt( 
					szEventMacAddress
					, pCacheIpHeader 
					);
				pServerNetwork->m_csPubDestinationLock.Release();
			}
			else
			{
				if( pCacheIpHeader->m_udpHeader.src_portno != pUdpHeader->src_portno 
					|| pCacheIpHeader->m_ipHeader.ip_srcaddr !=  pIpHeader->ip_srcaddr )
				{
					pServerNetwork->m_csPubDestinationLock.Lock();
					pCacheIpHeader->m_ipHeader.ip_srcaddr = pIpHeader->ip_srcaddr;
					pCacheIpHeader->m_udpHeader.src_portno = pUdpHeader->src_portno;
					pServerNetwork->m_csPubDestinationLock.Release();
				}
			}
	break;
	}


EnsureClient_Exit:
	return pCacheIpHeader;
}

UINT WINAPI CServerNetwork::ServiceWorker( PVOID context )
{
	UINT retValue = 0;
	CServerNetwork* pServerNetwork = static_cast< CServerNetwork* >( context );

	HRESULT hr = 
		DtcGetTransactionManager(
			NULL
			, NULL
			, IID_ITransactionDispenser
			, 0 ,0 ,0
			, (PVOID*)&pServerNetwork->m_pTxnDisp
			);

	if( FAILED( hr ) )
	{
		//TODO:: log error
		goto cleanup;
	}

    WSADATA wsd;
    int i, j;
	int iFromLength = 0;

	
    if( ( retValue = WSAStartup( MAKEWORD( 2, 2 ), &wsd ) ) != 0 )
    {
        printf("WSAStartup() failed: %d\n", retValue);
		goto cleanup;
    }


	BOOL fReuse = TRUE;
	BOOL fExUse = FALSE;
	ULONG ulRcvBuf = 65536;

	pServerNetwork->m_ServerVideoSocket = WSASocket(
		AF_INET
		, SOCK_RAW 
		, IPPROTO_UDP 
		, NULL
		, 0
		, WSA_FLAG_OVERLAPPED
		);
	pServerNetwork->m_iServerVideoLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iServerVideoLastError )
	{
		goto cleanup;
	}

	setsockopt( 
		pServerNetwork->m_ServerVideoSocket
		, SOL_SOCKET
		, SO_REUSEADDR
		, ( PCHAR ) &fReuse
		, sizeof( BOOL )
		);
	pServerNetwork->m_iServerVideoLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iServerVideoLastError )
	{
		goto cleanup;
	}

	setsockopt( 
		pServerNetwork->m_ServerVideoSocket
		, SOL_SOCKET
		, SO_EXCLUSIVEADDRUSE
		, ( PCHAR ) &fExUse
		, sizeof( BOOL )
		);
	pServerNetwork->m_iServerVideoLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iServerVideoLastError )
	{
		goto cleanup;
	}

	//setsockopt( 
	//	pServerNetwork->m_ServerVideoSocket
	//	, SOL_SOCKET
	//	, SO_RCVBUF
	//	, ( PCHAR ) &ulRcvBuf
	//	, sizeof( ULONG )
	//	);
	//pServerNetwork->m_iServerVideoLastError = WSAGetLastError();
	//if( 0 != pServerNetwork->m_iServerVideoLastError )
	//{
	//	goto cleanup;
	//}
	
	BOOL fHeaderIncluded = TRUE;
	setsockopt(
		pServerNetwork->m_ServerVideoSocket
		, IPPROTO_IP
		, IP_HDRINCL
		, ( PCHAR )&fHeaderIncluded
		, sizeof( BOOL )
		);
	pServerNetwork->m_iServerVideoLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iServerVideoLastError )
	{
		goto cleanup;
	}

	pServerNetwork->m_ServerVideoSocketAddr.sin_family = AF_INET;
	pServerNetwork->m_ServerVideoSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	pServerNetwork->m_ServerVideoSocketAddr.sin_port = htons( pServerNetwork->m_ServerNetworkContext.m_usPort );

	retValue = bind(
			pServerNetwork->m_ServerVideoSocket, 
			( const sockaddr* )&pServerNetwork->m_ServerVideoSocketAddr,
			sizeof( SOCKADDR_IN )
			);

	if( retValue == SOCKET_ERROR )
	{
		fprintf(stderr, "bind failed: %d\n", WSAGetLastError());
		goto cleanup;
	}

//	hr = SetupSend();

	pServerNetwork->m_hServerVideoEvent = WSACreateEvent();
    if( pServerNetwork->m_hServerVideoEvent == NULL )
    {
        goto cleanup;
    }

	retValue = WSAEventSelect(
		pServerNetwork->m_ServerVideoSocket
		, pServerNetwork->m_hServerVideoEvent
		, FD_READ
		);

	hr = SetupSend( pServerNetwork );

	//Setup Threadpools
	pServerNetwork->AllocateReceiveThreadpool();
	pServerNetwork->AllocateSendThreadpool();

	int iLoopCnt = 0;
	
	pServerNetwork->m_pRcvBuffer.Attach( 
		new CBlxBuffer<CServerPacketThreadData, CServerNetwork>( 
			pServerNetwork->m_ServerNetworkContext.m_uiReceiveNwBuffer 
			, MAX_PACKET_PAYLOAD
			, pServerNetwork->m_ServerNetworkContext.m_uiReceiveNwBuffer
			, 0
			, pServerNetwork
            , 1000 //1e6
			)
		);
	
	pServerNetwork->m_pSendBuffer.Attach( 
		new CBlxBuffer<CServerSendThreadData, CServerNetwork>( 
			pServerNetwork->m_ServerNetworkContext.m_uiSendNwBuffer 
			, MAX_VIDEO_WSASEND_BUFFER_SIZE_BYTES
			, pServerNetwork->m_ServerNetworkContext.m_uiSendNwBuffer
			, 0
			, pServerNetwork
            , 1000 //1e6
			)
		);

	pServerNetwork->m_pPublishingPointSource.Attach( new CPublishingPointSource( pServerNetwork->m_ServerNetworkContext.m_szDbConnString ) );
	pServerNetwork->m_pPublishingPointDestination.Attach( new CPublishingPointDestination( pServerNetwork->m_ServerNetworkContext.m_szDbConnString ) );
	pServerNetwork->m_pManagement.Attach( new CManagement( pServerNetwork->m_ServerNetworkContext.m_szDbConnString ) );

	pServerNetwork->m_pServerManagementHandler.Attach( 
		new CServerManagementHandler<CServerNetwork>( &pServerNetwork->m_ServerNetworkContext, pServerNetwork ) );
	if( NULL != pServerNetwork->m_pServerManagementHandler )
	{
		CServerManagementHandler<CServerNetwork>::StartServiceThread( pServerNetwork->m_pServerManagementHandler );
	}
	else
	{
		//TODO:  Log error and return
		goto cleanup;
	}

	SOCKET_THREAD_CONTEXT stc;
	stc.m_pServerNetwork = pServerNetwork;

	HANDLE hThread = 
		( HANDLE )_beginthreadex(
			NULL
			, 0
			, &CServerNetwork::VideoSocketWorker
			, &stc
			, 0
			, &stc.m_uiThreadId 
			);

	WaitForMultipleObjects(
		1
		, ( const HANDLE* )&hThread
		, TRUE
		, INFINITE
		);

cleanup:
	if( NULL != pServerNetwork->m_pServerManagementHandler )
	{
		CServerManagementHandler<CServerNetwork>::EndServiceThread( pServerNetwork->m_pServerManagementHandler );
	}

    if (pServerNetwork->m_aiServerVideoRessrc)
        freeaddrinfo(pServerNetwork->m_aiServerVideoRessrc);

    if (pServerNetwork->m_ServerVideoSocket != INVALID_SOCKET)
        closesocket( pServerNetwork->m_ServerVideoSocket );

    WSACleanup();

	_endthreadex( NULL );

	return retValue;

}
	
UINT WINAPI CServerNetwork::VideoSocketWorker( PVOID context )
{
	PSOCKET_THREAD_CONTEXT pSocketThreadContext =
		static_cast< PSOCKET_THREAD_CONTEXT >( context );

	CServerNetwork* pServerNetwork = 
		pSocketThreadContext->m_pServerNetwork;

	DWORD dwTimeout = 5000
		, dwEvent = 0
		;

	SHORT sCnt = 0;
	DWORD dwBytesRecv = 0
		, dwFlags = 0
		, BytesTransferred = 0
		;

	while ( pServerNetwork->m_fRun )
	{

		dwEvent = WSAWaitForMultipleEvents( 
			1 
			, &pServerNetwork->m_hServerVideoEvent  
			, TRUE
			, dwTimeout
			, TRUE
			);

		switch( dwEvent )
		{
		case WSA_WAIT_FAILED:
   //         fprintf(stderr, "WSAWaitForMultipleEvents Video failed: %d\n", GetLastError());
   //         goto cleanup;
			//break;
		case WSA_WAIT_TIMEOUT:
            //fprintf(stderr, "WSAWaitForMultipleEvents Video timed out: %d\n", GetLastError());
            //goto cleanup;
			continue;
		default:
			break;
		}

		CServerPacketThreadData* pThreadCurrent
			= pServerNetwork->m_pRcvBuffer->GetCurrentPacketThreadData();

		if( NULL == pThreadCurrent )
		{
			continue;
		}

		pThreadCurrent->m_fromlen = sizeof( pThreadCurrent->m_safrom );

		StartThreadpoolIo( pServerNetwork->m_pVideoIo );
		LPWSABUF x = pThreadCurrent->get_WSABUF(); // mxb -delete me
		DWORD rc = 0; 
		rc = WSARecvFrom(
			pServerNetwork->m_ServerVideoSocket
			, pThreadCurrent->get_WSABUF()
			, 1 
			, &dwBytesRecv
			, &dwFlags
			, (SOCKADDR *) &pThreadCurrent->m_safrom
			, &pThreadCurrent->m_fromlen
			, pThreadCurrent
			, NULL
			);

		if( 0 != rc )
		{
			INT32 wsaError = WSAGetLastError();
			
			switch( wsaError )
			{
				case ERROR_IO_PENDING:
					break;
				default:
					CancelThreadpoolIo( pServerNetwork->m_pVideoIo );
					pServerNetwork->m_pRcvBuffer->ReturnPacketThreadData( pThreadCurrent );
					break;
			}
		}
    }

cleanup:
	_endthreadex( NULL );
	return NOERROR;

}

UINT WINAPI CServerNetwork::SetupSend( CServerNetwork* pServerNetwork )
{
	HRESULT hr = S_OK;

	//g_wVersionRequested = MAKEWORD( 2, 2 );

	//g_iClientLastError = WSAStartup( 
	//	g_wVersionRequested
	//	, &g_ClientData
	//	);
	//if( 0 != g_iClientLastError )
	//{
	//	hr = E_FAIL;
	//	return;
	//}

	pServerNetwork->m_ClientSocket = WSASocket( 
		AF_INET
		, SOCK_RAW
		, IPPROTO_UDP 
		, NULL
		, 0
		, WSA_FLAG_OVERLAPPED
		);
	pServerNetwork->m_iClientLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iClientLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	BOOL fBroadcast = FALSE;
	setsockopt( 
		pServerNetwork->m_ClientSocket
		, SOL_SOCKET
		, SO_BROADCAST
		, ( CHAR* ) &fBroadcast
		, sizeof( BOOL )
		);
	pServerNetwork->m_iClientLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iClientLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	INT32 zero = 0;
	setsockopt(
		pServerNetwork->m_ClientSocket
		, SOL_SOCKET
		, SO_SNDBUF
		, ( PCHAR )&zero
		, sizeof( INT32 )
		);
	pServerNetwork->m_iClientLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iClientLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	BOOL fReuse = TRUE;
	setsockopt(
		pServerNetwork->m_ClientSocket
		, SOL_SOCKET
		, SO_REUSEADDR
		, ( PCHAR )&fReuse
		, sizeof( BOOL )
		);
	pServerNetwork->m_iClientLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iClientLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	BOOL fExUse = FALSE;
	setsockopt(
		pServerNetwork->m_ClientSocket
		, SOL_SOCKET
		, SO_EXCLUSIVEADDRUSE
		, ( PCHAR )&fExUse
		, sizeof( BOOL )
		);
	pServerNetwork->m_iClientLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iClientLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	BOOL fHeaderIncluded = TRUE;
	setsockopt(
		pServerNetwork->m_ClientSocket
		, IPPROTO_IP
		, IP_HDRINCL
		, ( PCHAR )&fHeaderIncluded
		, sizeof( BOOL )
		);
	pServerNetwork->m_iClientLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iClientLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	PMIB_UDPTABLE udpTable = ( PMIB_UDPTABLE )new BYTE[ sizeof( MIB_UDPTABLE ) * 250 ];
	ULONG dwSize = sizeof( MIB_UDPTABLE ) * 250;
	BOOL bOrder = TRUE;
	ULONG ulAf = AF_INET;
	UDP_TABLE_CLASS TableClass = UDP_TABLE_BASIC;
	DWORD dwRc =
		GetExtendedUdpTable(
			udpTable,
			&dwSize,
			bOrder,
			ulAf,
			TableClass,
			0UL
			);

	USHORT dwPort[ 2 ];
	INT8 iPort = 0;

	for( INT32 i = 0 ; i < udpTable->dwNumEntries ; i++ )
	{
		if( 0 < udpTable->table[ i ].dwLocalAddr )
		{
			if( 24000 < udpTable->table[ i ].dwLocalPort )
			{
				dwPort[ iPort ] = udpTable->table[ i ].dwLocalPort;
				iPort++;
				if( 1 < iPort )
				{
					break;
				}
			}
		}
	}

    IN_ADDR hostInAddr;

    pServerNetwork->m_iClientLastError = GetBestIpAddress(
        CString( L"www.microsoft.com" )
        , &hostInAddr
        );

	pServerNetwork->m_ClientSocketAddr.sin_family = AF_INET;
	hostent* thisHost = ::gethostbyname(  CW2A( pServerNetwork->m_ServerNetworkContext.m_szDNSTarget ) );
	pServerNetwork->m_ClientSocketAddr.sin_addr.s_addr = inet_addr( inet_ntoa (*(struct in_addr *)*thisHost->h_addr_list) );
	pServerNetwork->m_ClientSocketAddr.sin_port = htons( pServerNetwork->m_ServerNetworkContext.m_usPort );

    pServerNetwork->m_ClientLocalSocketAddr.sin_family = AF_INET;
	pServerNetwork->m_ClientLocalSocketAddr.sin_addr.s_addr = inet_addr( inet_ntoa( hostInAddr ) );
	pServerNetwork->m_ClientLocalSocketAddr.sin_port = htons( pServerNetwork->m_ServerNetworkContext.m_usPort );

	bind(
		pServerNetwork->m_ClientSocket
		, ( const sockaddr* )&pServerNetwork->m_ClientLocalSocketAddr 
		, sizeof( SOCKADDR_IN )
		);
	pServerNetwork->m_iClientLastError = WSAGetLastError();
	if( 0 != pServerNetwork->m_iClientLastError )
	{
		hr = E_FAIL;
		return hr;
	}

	pServerNetwork->m_hCliSocketOut = WSACreateEvent();

	WSAEventSelect(
	pServerNetwork->m_ClientSocket
	, pServerNetwork->m_hCliSocketOut
	, FD_WRITE
	);

	return hr;
}

VOID CALLBACK CServerNetwork::ReceiveCompleteRoutine( 
						PTP_CALLBACK_INSTANCE pInstance 
						, PVOID pContext 
						, PVOID pOverlapped 
						, ULONG lIoResult
						, ULONG_PTR pNumberOfBytesTransferred
						, PTP_IO pIo
					)
{
	CServerPacketThreadData* pThreadData = NULL;
	CServerNetwork* pServerNetwork = NULL;
	if( NULL != pOverlapped )
	{
		pThreadData = static_cast<CServerPacketThreadData*>( pOverlapped );
		pServerNetwork = static_cast<CServerNetwork*>( pThreadData->m_pContext );
	}
	else
	{
		return;
	}

	if( NO_ERROR != lIoResult )
	{
		//set some type of global error or throw exception ...
		OutputDebugStringf( TEXT( "Error in IOCP CServerNetwork::ReceiveCompleteRoutine %d: %d \n" ), lIoResult, pNumberOfBytesTransferred );
		fprintf(stderr, "Error in IOCP CServerNetwork::ReceiveCompleteRoutine %d: %d \n", lIoResult, pNumberOfBytesTransferred );
		pServerNetwork->m_pRcvBuffer->ReturnPacketThreadData( pThreadData );
		return;
	}

	PIP_HEADER pIpHeader = pThreadData->get_IpHeader();
	PUDP_HEADER pUdpHeader = pThreadData->get_UdpHeader(); 

	if( HOST_VIDEO_PORT != ntohs( pUdpHeader->dst_portno ) )
	{
		goto ReceiveCompleteRoutine_Cleanup;
	}

	PEVENT_HEADER pEventHeader = pThreadData->get_EventHeader(); 
	PEVENT_PACKET_HEADER pEventPacketHeader = pThreadData->get_EventPacketHeader();
	PPACKET_DATA_BLOB pPacketDataBlob = pThreadData->get_PacketDataBlob();

#ifdef _VISTA_
//	CallbackMayRunLong( pInstance );
#endif
	//OutputDebugStringf( TEXT( "\nEventTypeId %d - EventType %d: " ),  pEventHeader->eventTypeId,  pEventHeader->eventType );
//	OutputDebugStringf( TEXT( "%s :  %s : %d : %d" ), __TFILE__ , __TFUNCTION__,  pEventHeader->eventTypeId, pEventHeader->eventType );

	switch( pEventHeader->eventTypeId )
	{
		case BET_MANAGEMENT:
			pServerNetwork->ProcessManagementRequest(
				pIpHeader
				, pUdpHeader
				, pEventHeader
				, pEventPacketHeader
				, pPacketDataBlob
				, pServerNetwork
				);
			break;
		case BET_VIDEO_STREAM:
			pServerNetwork->ProcessVideoMultimediaPublication(
				pIpHeader
				, pUdpHeader
				, pEventHeader
				, pEventPacketHeader
				, pPacketDataBlob
				, pServerNetwork
				);
			break;
		case BET_AUDIO_STREAM:
			pServerNetwork->ProcessAudioMultimediaPublication(
				pIpHeader
				, pUdpHeader
				, pEventHeader
				, pEventPacketHeader
				, pPacketDataBlob
				, pServerNetwork
				);
			break;
		default:
			goto ReceiveCompleteRoutine_Cleanup;
	}


ReceiveCompleteRoutine_Cleanup:

	pServerNetwork->m_pRcvBuffer->ReturnPacketThreadData( pThreadData );

	Receive( pServerNetwork );

ReceiveCompleteRoutine_Exit:

	return;
}

VOID CALLBACK CServerNetwork::SubmitVideoToClientCompleteRoutine( 
						PTP_CALLBACK_INSTANCE pInstance 
						, PVOID pContext 
						, PVOID pOverlapped 
						, ULONG lIoResult
						, ULONG_PTR pNumberOfBytesTransferred
						, PTP_IO pIo
					)
{
	if( NO_ERROR != lIoResult )
	{
		//set some type of global error or throw exception ...
		return;
	}

	CServerSendThreadData* pThreadData = static_cast<CServerSendThreadData*>( pOverlapped );
	CServerNetwork*	pServerNetwork = static_cast<CServerNetwork*>( pThreadData->m_pContext );
	pServerNetwork->m_pSendBuffer->ReturnPacketThreadData( pThreadData );
	return;
}

VOID CALLBACK CServerNetwork::Receive( CServerNetwork* pServerNetwork )
{
	DWORD dwEvent;
	SHORT sCnt = 0;
	DWORD dwBytesRecv = 0
		, dwFlags = 0
		, BytesTransferred = 0
		;

	int iLoopCnt = 0;
	DWORD dwTimeout = 5000;

	dwEvent = WSAWaitForMultipleEvents( 
		1 
		, &pServerNetwork->m_hServerVideoEvent  
		, FALSE
		, dwTimeout
		, TRUE
		);

	switch( dwEvent )
	{
	case WSA_WAIT_FAILED:
        fprintf(stderr, "WSAWaitForMultipleEvents Video failed: %d\n", GetLastError());
        goto cleanup;
		break;
	case WSA_WAIT_TIMEOUT:
        fprintf(stderr, "WSAWaitForMultipleEvents Video timed out: %d\n", GetLastError());
        goto cleanup;
		break;
	}

	CServerPacketThreadData* pThreadCurrent
		= pServerNetwork->m_pRcvBuffer->GetCurrentPacketThreadData();

	if( NULL == pThreadCurrent )
	{
        goto cleanup;
	}

	pThreadCurrent->m_fromlen = sizeof( pThreadCurrent->m_safrom );

	StartThreadpoolIo( pServerNetwork->m_pVideoIo );

	DWORD rc = 0; 
	rc = WSARecvFrom(
		pServerNetwork->m_ServerVideoSocket
		, pThreadCurrent->get_WSABUF()
		, 1 
		, &dwBytesRecv
		, &dwFlags
		, (SOCKADDR *) &pThreadCurrent->m_safrom
		, &pThreadCurrent->m_fromlen
		, pThreadCurrent
		, NULL
		);

	if( 0 != rc )
	{
		INT32 wsaError = WSAGetLastError();
		
		switch( wsaError )
		{
		case ERROR_IO_PENDING:
			break;
		default:
			CancelThreadpoolIo( pServerNetwork->m_pVideoIo );
			pServerNetwork->m_pRcvBuffer->ReturnPacketThreadData( pThreadCurrent );
			break;
		}
	}

cleanup:
	return;
}

VOID CServerNetwork::ServerEventKeepAlive(
	PIP_HEADER pIpHeader
	, PUDP_HEADER pUdpHeader
	, PEVENT_HEADER pEventHeader
	, PEVENT_PACKET_HEADER pEventPacketHeader
	, PPACKET_DATA_BLOB pPacketDataBlob
	)
{
	PUBLISHING_POINT_MACADDR szEventMacAddress( pEventHeader->eventMacAddress );
	szEventMacAddress.TrimRight( ' ' );

	DWORD dwBytesSent = 0
		, dwFlags = 0
		;

	INT32 m_iLastError = 0;

	CServerVideoSource* pSourceIpHeader = NULL;
	CIpHeader* pDestIpHeader = NULL;

	PUBLISHING_POINT_SOURCE_VIDEO_ENTRIES* pTmpSourceClient = NULL;
	PUBLISHING_POINT_CLIENTS* pTmpDestClient = NULL;

	POSITION posSource;
	POSITION posDest;

	BOOL bSource		= FALSE
		, bSourceClient = FALSE
		, bDest			= FALSE
		, bDestClient	= FALSE
		;

	this->m_csVideoPubSourceLock.LockShared();
	bSource = this->m_pubPointVideoSources.Lookup( pEventPacketHeader->eventPublishingPoint, pTmpSourceClient );
	this->m_csVideoPubSourceLock.ReleaseShared();

	USHORT usLength = MAX_IP_HEADER
						+ MAX_UDP_HEADER
						+ MAX_EVENT_HEADER
						+ MAX_EVENT_PACKET_HEADER
						+ pEventPacketHeader->dataBlobSize
						;

	while( NULL != posDest )
	{
		CIpHeader* pTmpHeader = pTmpDestClient->GetNextValue( posDest );

		if( 0 == strcmp( pTmpHeader->m_szMACAddress, CW2A( szEventMacAddress.GetString() ) ) )  
		{
			continue;
		}

		CServerSendThreadData* pServerSendThreadData =
			m_pSendBuffer->GetCurrentPacketThreadData();
		if( NULL == pServerSendThreadData )
		{
			continue;
		}

		pIpHeader->ip_destaddr = pTmpHeader->m_ipHeader.ip_srcaddr;
		pUdpHeader->dst_portno = pTmpHeader->m_udpHeader.src_portno;
		pIpHeader->ip_srcaddr = m_ClientSocketAddr.sin_addr.s_addr;//ipHeaderDests[ i ]->m_ipHeader.ip_destaddr;
		pUdpHeader->src_portno = m_ClientSocketAddr.sin_port; //ipHeaderDests[ i ]->m_udpHeader.dst_portno;
		pUdpHeader->udp_checksum = 0;

		pServerSendThreadData->m_socketAddrSrvVideo.sin_family = AF_INET;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_addr.s_addr = pIpHeader->ip_destaddr;
		pServerSendThreadData->m_socketAddrSrvVideo.sin_port = pUdpHeader->dst_portno;

		pServerSendThreadData->IngestNetworkPacket( 
			reinterpret_cast< PCHAR >( pIpHeader )
			, usLength
			);

		pUdpHeader->udp_checksum = 0;

		//ComputeUdpPseudoHeaderChecksumV4(
		//	pServerSendThreadData->get_IpHeader()
		//	, pServerSendThreadData->get_UdpHeader()
		//	, reinterpret_cast<PCHAR>( pServerSendThreadData->get_EventHeader() )
		//	, MAX_EVENT_HEADER
		//		+ MAX_EVENT_PACKET_HEADER
		//		+ pServerSendThreadData->get_EventPacketHeader()->dataBlobSize 
		//	);

		StartThreadpoolIo( m_pClientIo );
		
		WSASendTo(
			m_ClientSocket
			, pServerSendThreadData->get_WSABUF()
			, 1
			, &dwBytesSent
			, dwFlags
			, ( SOCKADDR* ) &pServerSendThreadData->m_socketAddrSrvVideo
			, ( int )sizeof( SOCKADDR_IN )
			, pServerSendThreadData
			, NULL//SubmitVideoToClientCompleteRoutine
			);
	};

	return;
}


VOID CServerNetwork::ComputeUdpPseudoHeaderChecksumV4(
	PVOID iphdr,
	UDP_HEADER *udphdr,
	PCHAR payload,
	INT32 payloadlen
)
{
	char pseudobuf[65536];
	IP_HEADER *v4hdr=NULL;
	unsigned long zero=0;
	char *ptr=NULL;
	int chksumlen=0,
	i;

	ptr = pseudobuf;
	v4hdr = (IP_HEADER*)iphdr;
	// Include the source and destination IP addresses
	memcpy(ptr, &v4hdr->ip_srcaddr, sizeof(v4hdr->ip_srcaddr)); 
	ptr += sizeof(v4hdr->ip_srcaddr);
	chksumlen += sizeof(v4hdr->ip_srcaddr);
	memcpy(ptr, &v4hdr->ip_destaddr, sizeof(v4hdr->ip_destaddr)); 
	ptr += sizeof(v4hdr->ip_destaddr);
	chksumlen += sizeof(v4hdr->ip_destaddr);

	// Include the 8 bit zero field
	memcpy(ptr, &zero, 1);
	ptr++;
	chksumlen += 1;
	// Protocol
	memcpy(ptr, &v4hdr->ip_protocol, sizeof(v4hdr->ip_protocol)); 
	ptr += sizeof(v4hdr->ip_protocol);
	chksumlen += sizeof(v4hdr->ip_protocol);
	// UDP length
	memcpy(ptr, &udphdr->udp_length, sizeof(udphdr->udp_length)); 
	ptr += sizeof(udphdr->udp_length);
	chksumlen += sizeof(udphdr->udp_length);
	// UDP source port
	memcpy(ptr, &udphdr->src_portno, sizeof(udphdr->src_portno)); 
	ptr += sizeof(udphdr->src_portno);
	chksumlen += sizeof(udphdr->src_portno);
	// UDP destination port
	memcpy(ptr, &udphdr->dst_portno, sizeof(udphdr->dst_portno)); 
	ptr += sizeof(udphdr->dst_portno);
	chksumlen += sizeof(udphdr->dst_portno);
	// UDP length again
	memcpy(ptr, &udphdr->udp_length, sizeof(udphdr->udp_length)); 
	ptr += sizeof(udphdr->udp_length);
	chksumlen += sizeof(udphdr->udp_length);

	// 16-bit UDP checksum, zero 
	memcpy(ptr, &zero, sizeof(unsigned short));
	ptr += sizeof(unsigned short);
	chksumlen += sizeof(unsigned short);
	// payload
	memcpy(ptr, payload, payloadlen);
	ptr += payloadlen;
	chksumlen += payloadlen;
	// pad to next 16-bit boundary
	for(i=0 ; i < payloadlen%2 ; i++, ptr++)
	{
//		printf("pad one byte\n");
		*ptr = 0;
		ptr++;
		chksumlen++;
	}

	// Compute the checksum and put it in the UDP header
	udphdr->udp_checksum = checksum((USHORT *)pseudobuf, chksumlen);
//	udphdr->udp_checksum = 0; // Use 0 to allow WSA to determine checksum. (If you see checksum errors, use 0'0')
	return;
}

USHORT CServerNetwork::checksum(USHORT *buffer, INT32 size)
{
	unsigned long cksum=0;
	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT); 
	}

	// If the buffer was not a multiple of 16-bits, add the last byte

	if (size)
	{
		cksum += *(UCHAR*)buffer; 
	}

	// Add the low order 16-bits to the high order 16-bits
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16); 
	// Take the 1's complement

	return (USHORT)(~cksum); 
}
