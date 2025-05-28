#pragma once

#include "ServerContext.h"
#include "BlxCommonLock.h"

#include "PacketInfoStructs.h"
#include "PriorityQueueSync.h"
#include "SyncQueue.h"
#include "operators.h"

#include "BlxBuffer.hpp"
#include "BlxPqBuffer.hpp"

#include "PublishingPointSource.h"
#include "PublishingPointDestination.h"
#include "Management.h"
#include "ServerManagementHandler.hpp"

class CServerNetwork
{
public:
	CServerNetwork( PBCS_SERVER_NETWORK_CONTEXT snc );
	~CServerNetwork(void);

	static VOID StartServiceThread( CServerNetwork* pServerNetwork );
	static VOID EndServiceThread( CServerNetwork* pServerNetwork );
	
	VOID ServerEventVideoSource(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
						);

	VOID ServerEventVideoSourceRt(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
						);

	VOID ServerEventAudioSource(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);
	
	VOID ServerEventAudioSourceRt(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);

	VOID ServerEventVideoSubscription(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);

	VOID ServerEventVideoSubscriptionRt(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);

	VOID ServerEventKeepAlive(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
						);

	
	VOID ServerEventDropUser(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
						);

	CAutoPtr< CPublishingPointSource >			m_pPublishingPointSource;
	CAutoPtr< CPublishingPointDestination >		m_pPublishingPointDestination;
	CAutoPtr< CManagement >						m_pManagement;

private:

	typedef USHORT PUBLISHING_POINT_ID;
	typedef CString PUBLISHING_POINT_MACADDR;
	typedef CString CONFERENCE_PARTICIPANT_ID;

	CAutoPtr< ITransactionDispenser >									m_pTxnDisp;
	CAutoPtr< CBlxBuffer<CServerPacketThreadData, CServerNetwork > >	m_pRcvBuffer;
	CAutoPtr< CBlxBuffer<CServerSendThreadData, CServerNetwork > >		m_pSendBuffer;
	CAutoPtr< CBlxPqBuffer<CServerWorkerThreadData, CServerNetwork, test_server_worker_data > >		m_pWorkerBuffer;

	enum CLIENT_TYPE 
	{
		CT_AUDIO_SOURCE				= 0
		, CT_VIDEO_SOURCE			= 1
		, CT_DESTINATION			= 2
	};


	typedef Blxware::SyncQueue< CClientManagementEventData* > QClientManagementEventData;
	typedef QClientManagementEventData* PQClientManagementEventData;
	PQClientManagementEventData m_pQClientManagementEventData;

	typedef CRBMap< PUBLISHING_POINT_MACADDR, CIpHeader* > PUBLISHING_POINT_CLIENTS;
	typedef CRBMap< PUBLISHING_POINT_MACADDR, CServerVideoSource* > PUBLISHING_POINT_SOURCE_VIDEO_ENTRIES;
	typedef CRBMap< PUBLISHING_POINT_MACADDR, CServerAudioSource* > PUBLISHING_POINT_SOURCE_AUDIO_ENTRIES;

	typedef CRBMap< PUBLISHING_POINT_ID, PUBLISHING_POINT_SOURCE_VIDEO_ENTRIES* > PUBLISHING_POINT_VIDEO_SOURCES;
	typedef CRBMap< PUBLISHING_POINT_ID, PUBLISHING_POINT_SOURCE_AUDIO_ENTRIES* > PUBLISHING_POINT_AUDIO_SOURCES;

	typedef CRBMap< PUBLISHING_POINT_ID, PUBLISHING_POINT_CLIENTS* > PUBLISHING_POINT_DESTINATIONS;

	PUBLISHING_POINT_CLIENTS m_pAudioSourceClients;
	PUBLISHING_POINT_CLIENTS m_pVideoSourceClients;
	PUBLISHING_POINT_CLIENTS m_pDestinationClients;
	PUBLISHING_POINT_VIDEO_SOURCES m_pubPointVideoSources;
	PUBLISHING_POINT_AUDIO_SOURCES m_pubPointAudioSources;
	PUBLISHING_POINT_DESTINATIONS m_pubPointDestinations;

	CBlxLock m_csVideoSubscriberLock;
	CBlxLock m_csVideoPubSourceLock;
	CBlxLock m_csAudioSubscriberLock;
	CBlxLock m_csAudioPubSourceLock;
	CBlxLock m_csPubDestinationLock;
	CBlxLock m_csReceiveThreadData;
	CBlxLock m_csSendThreadData;

	UINT32						m_uiSendThreadId;
	LONG						m_lSendThreadPhase;
	LONG						m_tSendThread;

	CHandle						m_hSocketThread;
	HANDLE						m_hCliSocketOut;
	BOOL						m_fRun;

	BCS_SERVER_NETWORK_CONTEXT	m_ServerNetworkContext;

	/*for sending*/
	WSADATA					m_ClientData;
	SOCKET					m_ClientSocket;
	SOCKADDR_IN				m_ClientSocketAddr;
	SOCKADDR_IN				m_ClientLocalSocketAddr;
	INT32					m_iClientLastError;
	WSAEVENT				m_hClientEvent;

	/*for receiving video*/
	WSADATA					m_ServerVideoData;
	SOCKET					m_ServerVideoSocket;
	SOCKADDR_IN				m_ServerVideoSocketAddr;
	INT32					m_iServerVideoLastError;
	WSAEVENT				m_hServerVideoEvent;
	addrinfo*				m_aiServerVideoRessrc;
	
	PTP_POOL				m_pVideoThreadPool;
	PTP_IO					m_pVideoIo;
	TP_CALLBACK_ENVIRON		m_VideoCallbackEnvironment;
	PTP_CLEANUP_GROUP		m_pVideoCleanupGroup;
	PTP_CLEANUP_GROUP_CANCEL_CALLBACK m_pVideoCleanupGroupCancelCallback;

	PTP_POOL				m_pClientThreadPool;
	PTP_IO					m_pClientIo;
	TP_CALLBACK_ENVIRON		m_ClientCallbackEnvironment;
	PTP_CLEANUP_GROUP		m_pClientCleanupGroup;
	PTP_CLEANUP_GROUP_CANCEL_CALLBACK m_pClientCleanupGroupCancelCallback;

	PTP_POOL				m_pManagementThreadPool;
	TP_CALLBACK_ENVIRON		m_ManagementCallbackEnvironment;
	PTP_CLEANUP_GROUP		m_pManagementCleanupGroup;
	PTP_CLEANUP_GROUP_CANCEL_CALLBACK m_pManagementCleanupGroupCancelCallback;

	CAutoPtr< CServerManagementHandler< CServerNetwork > >		m_pServerManagementHandler;

	VOID AllocateManagementThreadData();
	VOID DeallocateManagementThreadData();
	inline static CClientManagementEventData* CALLBACK GetCurrentManagementThreadData(  CClientManagementEventData* pServerNetwork  );
	inline static VOID CALLBACK ReturnManagementThreadData( CClientManagementEventData* ThreadData,  CServerNetwork* pServerNetwork  );

	VOID AllocateReceiveThreadpool();
	VOID AllocateSendThreadpool();
	VOID DeallocateReceiveThreadpool();
	VOID DeallocateSendThreadpool();
	VOID AllocateManagementThreadpool();
	VOID DeallocateManagementThreadpool();

	
	VOID ProcessVideoSourceRequest(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);

	VOID ProcessAudioSourceRequest(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);

	VOID ProcessKeepAliveRequest(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);

	static UINT WINAPI ServiceWorker( PVOID context );
	static UINT WINAPI VideoSocketWorker( PVOID context );
	static UINT WINAPI SetupSend( CServerNetwork* pServerNetwork );
	static VOID CALLBACK ReceiveCompleteRoutine( 
							PTP_CALLBACK_INSTANCE pInstance 
							, PVOID pContext 
							, PVOID pOverlapped 
							, ULONG lIoResult
							, ULONG_PTR pNumberOfBytesTransferred
							, PTP_IO pIo
						);
	static VOID CALLBACK SubmitVideoToClientCompleteRoutine( 
							PTP_CALLBACK_INSTANCE pInstance 
							, PVOID pContext 
							, PVOID pOverlapped 
							, ULONG lIoResult
							, ULONG_PTR pNumberOfBytesTransferred
							, PTP_IO pIo
						);
	static VOID CALLBACK Receive( CServerNetwork* pServerNetwork );

	VOID ProcessManagementRequest( 
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);
	VOID ProcessVideoMultimediaPublication( 
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);
	VOID CServerNetwork::ProcessAudioMultimediaPublication( 
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);

	VOID ProcessVideoSubscriptionRequest(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PEVENT_HEADER pEventHeader
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CServerNetwork* pServerNetwork
						);
	CIpHeader* CALLBACK EnsureClient(
							PIP_HEADER pIpHeader
							, PUDP_HEADER pUdpHeader
							, PUBLISHING_POINT_MACADDR szEventMacAddress
							, PEVENT_PACKET_HEADER pEventPacketHeader
							, PPACKET_DATA_BLOB pPacketDataBlob
							, CLIENT_TYPE clientType
							, CServerNetwork* pServerNetwork
						);

	VOID ComputeUdpPseudoHeaderChecksumV4(
							PVOID iphdr
							, UDP_HEADER *udphdr
							, PCHAR payload
							, INT32 payloadlen
						);
	USHORT checksum(USHORT *buffer, INT32 size);
};
