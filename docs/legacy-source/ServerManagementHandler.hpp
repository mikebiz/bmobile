#pragma once

#include "StdAfx.h"

#include <xolehlp.h>
#include <transact.h>
#include <mq.h>

#include "ServerContext.h"
#include "BlxCommonLock.h"

#include "PacketInfoStructs.h"

#include "GenerateNwEvents.hpp"

template <class T_ServerNetwork>
class CServerManagementHandler
{
public:
	CServerManagementHandler( PBCS_SERVER_NETWORK_CONTEXT snc, T_ServerNetwork* pServerNetwork );
	~CServerManagementHandler(void);

	static VOID StartServiceThread( CServerManagementHandler* pServerManagementHandler );
	static VOID EndServiceThread( CServerManagementHandler* pServerManagementHandler );

private:

	typedef T_ServerNetwork SERVER_NETWORK;
	SERVER_NETWORK*				m_pServerNetwork;

	CGenerateNwEvents<SERVER_NETWORK>* m_pNwEvents;

	PBCS_SERVER_NETWORK_CONTEXT	m_ServerNetworkContext;
	CHandle						m_hThread;
	CHandle						m_hMgtQueue;
	BOOL						m_fRun;

	UINT32						m_uiReceiveThreadId;
	LONG						m_lReceiveThreadPhase;
	LONG						m_tReceiveThread;
	LONG						m_ulOutstandingReads;

	LONG						m_lQThreadPhase;
	LONG						m_tQThread;

	CBlxLock					m_csReceiveThreadData;
	CAutoPtr< ITransactionDispenser > m_pTxnDisp;

	typedef Blxware::SyncQueue< CServerSqlEventThreadData* > QServerPacketThreadData;
	typedef QServerPacketThreadData* PQServerPacketThreadData;
	PQServerPacketThreadData m_pQServerPacketThreadData;

	PTP_POOL					m_pMgtThreadPool;
	PTP_IO						m_pMgtIo;
	TP_CALLBACK_ENVIRON			m_MgtCallbackEnvironment;
	PTP_CLEANUP_GROUP			m_pMgtCleanupGroup;
	PTP_CLEANUP_GROUP_CANCEL_CALLBACK m_pMgtCleanupGroupCancelCallback;

	VOID AllocateMgtThreadpool();
	VOID DeallocateMgtThreadpool();
	HRESULT InitializeManagementQueue();

	VOID AllocateReceiveThreadData();
	VOID DeallocateReceiveThreadData();
	inline static CServerSqlEventThreadData* CALLBACK GetCurrentServerPacketThreadData(  CServerManagementHandler* pServerNetwork  );
	inline static VOID CALLBACK ReturnServerPacketThreadData( CServerSqlEventThreadData* ThreadData,  CServerManagementHandler* pServerNetwork  );

	static UINT WINAPI ServiceWorker( PVOID context );
	static VOID CALLBACK PostEventProcess(
							PTP_CALLBACK_INSTANCE Instance
							, PVOID Context
							, PTP_WORK Work
							);
};



template <class T_ServerNetwork>
CServerManagementHandler<T_ServerNetwork>::CServerManagementHandler( PBCS_SERVER_NETWORK_CONTEXT snc, SERVER_NETWORK* pServerNetwork  )
	: m_fRun( TRUE )
	, m_uiReceiveThreadId( 0 )
	, m_lReceiveThreadPhase( -1 )
	, m_ulOutstandingReads( 0 )
	, m_tQThread( 0 )
	, m_lQThreadPhase( -1 )
{
	m_ServerNetworkContext = snc;
	m_pServerNetwork = pServerNetwork;
	m_pNwEvents = new CGenerateNwEvents<SERVER_NETWORK>( m_pServerNetwork );

	m_tReceiveThread = m_ServerNetworkContext->m_uiManagementQBuffer;
	m_tQThread = m_ServerNetworkContext->m_uiManagementQBuffer;

	m_pQServerPacketThreadData = new QServerPacketThreadData( m_ServerNetworkContext->m_uiManagementQBuffer );

}

template <class T_ServerNetwork>
CServerManagementHandler<T_ServerNetwork>::~CServerManagementHandler(void)
{
}


template <class T_ServerNetwork>
VOID CServerManagementHandler<T_ServerNetwork>::StartServiceThread( CServerManagementHandler* pServerManagementHandler )
{
	pServerManagementHandler->m_fRun = TRUE;

	HANDLE hThread = 
		( HANDLE )_beginthreadex(
			NULL
			, 0
			, &ServiceWorker
			, pServerManagementHandler
			, NULL
			, &pServerManagementHandler->m_ServerNetworkContext->m_ServerContext.m_uiThreadId
			);

	pServerManagementHandler->m_hThread.Attach( hThread );
	return;
}

template <class T_ServerNetwork>
VOID CServerManagementHandler<T_ServerNetwork>::EndServiceThread( CServerManagementHandler* pServerManagementHandler )
{
	pServerManagementHandler->m_fRun = FALSE;
	WaitForSingleObject( pServerManagementHandler->m_hThread, INFINITE );
	pServerManagementHandler->m_hThread.Close();
	return;
}

template <class T_ServerNetwork>
HRESULT CServerManagementHandler<T_ServerNetwork>::InitializeManagementQueue()
{
	HRESULT hr = S_OK;
	
	hr = MQOpenQueue( 
			m_ServerNetworkContext->m_szManagementQName							
			, MQ_RECEIVE_ACCESS                   
			, MQ_DENY_NONE                      
			, (QUEUEHANDLE*)&m_hMgtQueue                          
			);
	return hr;
}
template <class T_ServerNetwork>
VOID CServerManagementHandler<T_ServerNetwork>::AllocateMgtThreadpool()
{
	m_pMgtThreadPool = CreateThreadpool(NULL);

	if( NULL != m_pMgtThreadPool ) 
	{
		SetThreadpoolThreadMaximum( m_pMgtThreadPool, m_ServerNetworkContext->m_uiManagementQTpMax );
		if ( SetThreadpoolThreadMinimum( m_pMgtThreadPool, m_ServerNetworkContext->m_uiManagementQTpMin ) ) 
		{
			InitializeThreadpoolEnvironment( &m_MgtCallbackEnvironment );
			m_pMgtCleanupGroup = m_MgtCallbackEnvironment.CleanupGroup;
			SetThreadpoolCallbackPool( 
				&m_MgtCallbackEnvironment
				, m_pMgtThreadPool
			);
			m_pMgtCleanupGroup = CreateThreadpoolCleanupGroup();
			SetThreadpoolCallbackCleanupGroup(
				&m_MgtCallbackEnvironment
				, m_pMgtCleanupGroup
				, NULL
			);
		}
	}
	else
	{
		return;
	}
	return;
}

template <class T_ServerNetwork>
VOID CServerManagementHandler<T_ServerNetwork>::DeallocateMgtThreadpool()
{
	WaitForThreadpoolIoCallbacks(
		m_pMgtIo
		, FALSE
		);
	return;

}

template <class T_ServerNetwork>
VOID CServerManagementHandler<T_ServerNetwork>::AllocateReceiveThreadData()
{
	UINT32 uiVideoThreadCount = 0;
	CServerSqlEventThreadData* pServerOverlapped;

	for( UINT32 j = 0 ; j < m_ServerNetworkContext->m_uiManagementQBuffer ; j++ )
	{
		pServerOverlapped = new CServerSqlEventThreadData( MAX_SQL_SERVER_TRIGGER_EVENT );
		pServerOverlapped->Internal = 0;
		pServerOverlapped->InternalHigh = 0;
		pServerOverlapped->Offset = 0;
		pServerOverlapped->OffsetHigh = 0;
		pServerOverlapped->hEvent = WSACreateEvent();
		pServerOverlapped->m_iPktCnt = 0;
		pServerOverlapped->m_pContext = this;
		pServerOverlapped->m_pWorkItem = NULL;
		

		m_pQServerPacketThreadData->push( pServerOverlapped );
	}

	return;
}

template <class T_ServerNetwork>
VOID CServerManagementHandler<T_ServerNetwork>::DeallocateReceiveThreadData()
{
	m_pQServerPacketThreadData->clear();
	return;
}

template <class T_ServerNetwork>
CServerSqlEventThreadData* CServerManagementHandler<T_ServerNetwork>::GetCurrentServerPacketThreadData( CServerManagementHandler* pServerManagementHandler )
{
	CServerSqlEventThreadData* pData = NULL;
	 pServerManagementHandler->m_pQServerPacketThreadData->poll_timed(
		pData
		, 1000 //1e6
		);
	 if( NULL != pData->m_pWorkItem )
	 {
		WaitForThreadpoolWorkCallbacks(
			pData->m_pWorkItem
			, FALSE
			);
		CloseThreadpoolWork( pData->m_pWorkItem );
		pData->m_pWorkItem = NULL;
	 }
	return pData;
}

template <class T_ServerNetwork>
VOID CServerManagementHandler<T_ServerNetwork>::ReturnServerPacketThreadData( CServerSqlEventThreadData* ThreadData,  CServerManagementHandler* pServerManagementHandler  )
{
	WSAResetEvent( ThreadData->hEvent );

	pServerManagementHandler->m_pQServerPacketThreadData->push_timed(
		ThreadData
		, 1000 //1e6
		);
	return;
}

template <class T_ServerNetwork>
UINT WINAPI CServerManagementHandler<T_ServerNetwork>::ServiceWorker( PVOID context )
{
	UINT uiRc = 0;

	CServerManagementHandler* pServerManagementHandler = static_cast< CServerManagementHandler* >( context );

	HRESULT hr = pServerManagementHandler->InitializeManagementQueue();
	if( FAILED( hr ) )
	{
		//TODO:  Log error and return...  SetLastError?
		return -1;
	}
	pServerManagementHandler->AllocateMgtThreadpool();
	pServerManagementHandler->AllocateReceiveThreadData();

	hr = DtcGetTransactionManager(
			NULL
			, NULL
			, IID_ITransactionDispenser
			, 0 ,0 ,0
			, (PVOID*)&pServerManagementHandler->m_pTxnDisp
			);

	while( pServerManagementHandler->m_fRun )
	{
		HRESULT hrOl = S_OK;

		CServerSqlEventThreadData* pThreadCurrent
			= GetCurrentServerPacketThreadData( pServerManagementHandler );

		if( NULL == pThreadCurrent )
		{
			OutputDebugStringf( TEXT( "GetCurrentServerPacketThreadData returns NULL!" ) );
			continue;
		}

		hrOl = pServerManagementHandler->m_pTxnDisp->BeginTransaction(
				NULL
				, 0
				, 0
				, NULL
				, &pThreadCurrent->m_pTxnObj
				);
		if( FAILED( hrOl ) )
		{
			continue;
		}

		hrOl = MQReceiveMessage(
				pServerManagementHandler->m_hMgtQueue
				, 5000
				, MQ_ACTION_RECEIVE
				, &pThreadCurrent->m_msgprops
				, NULL
				, NULL
				, NULL
				, pThreadCurrent->m_pTxnObj //NULL 
				);

		if( MQ_ERROR_IO_TIMEOUT  == hrOl )
		{
			pThreadCurrent->m_pTxnObj->Abort(
				FALSE
				, 0
				, FALSE
				);

			pThreadCurrent->m_pTxnObj = NULL;

			CServerManagementHandler::ReturnServerPacketThreadData(
				pThreadCurrent
				, pServerManagementHandler
				);
			continue;
		}
		else if( 0 != hrOl )
		{
			OutputDebugStringf( TEXT( "Error %x:" )
				, hrOl
				);
			pThreadCurrent->m_pTxnObj->Abort(
				FALSE
				, 0
				, FALSE
				);

			pThreadCurrent->m_pTxnObj = NULL;

			CServerManagementHandler::ReturnServerPacketThreadData(
				pThreadCurrent
				, pServerManagementHandler
				);
			continue;
		}

		pThreadCurrent->m_pWorkItem = 
			CreateThreadpoolWork(
				(PTP_WORK_CALLBACK)CServerManagementHandler::PostEventProcess
				, pThreadCurrent 
				, &pServerManagementHandler->m_MgtCallbackEnvironment
				);
		if( NULL != pThreadCurrent->m_pWorkItem )
		{
			SubmitThreadpoolWork( pThreadCurrent->m_pWorkItem );
		}
		else
		{
			if( NULL != pThreadCurrent )
			{
				CServerManagementHandler::ReturnServerPacketThreadData(
					pThreadCurrent
					, pServerManagementHandler
					);
			}
		}
	}
	return uiRc;
}

template <class T_ServerNetwork>
VOID CALLBACK CServerManagementHandler<T_ServerNetwork>::PostEventProcess(
	PTP_CALLBACK_INSTANCE Instance
	, PVOID Context
	, PTP_WORK Work
	)
{

	CServerSqlEventThreadData* pThreadCurrent = 
		static_cast< CServerSqlEventThreadData* >( Context );
	CServerManagementHandler* pServerManagementHandler = 
		static_cast< CServerManagementHandler* >( pThreadCurrent->m_pContext );
		
	PSQL_SERVER_TRIGGER_EVENT pSqlServerTriggerEvent =
		&pThreadCurrent->m_sqlTriggerEvent;

	//OutputDebugStringf( TEXT( "%s - %s - %s : eventtype=%d" ), __TFILE__, __FUNCTION__, __LINE__,  pSqlServerTriggerEvent->eventType );
	OutputDebugStringf( TEXT( "%s :  %s : %d" ), __TFILE__ , __TFUNCTION__,  pSqlServerTriggerEvent->eventTypeId );

	INT32 iRc = 0;

	try
	{
		switch( pSqlServerTriggerEvent->eventTypeId )
		{
			case BE_INSERT_PPT_VIDEO_SOURCE_RT:
			case BE_INSERT_PPT_VIDEO_SOURCE:

				iRc = pServerManagementHandler->m_pNwEvents->GeneratePptVideoSource( 
							pThreadCurrent
							);
				if( iRc == 0 )
				{
				}
				break;
			case BE_INSERT_PPT_AUDIO_SOURCE_RT:
			case BE_INSERT_PPT_AUDIO_SOURCE:

				iRc = pServerManagementHandler->m_pNwEvents->GeneratePptAudioSource( 
							pThreadCurrent
							);
				if( iRc == 0 )
				{
				}
				break;
			case BE_INSERT_PPT_MEDIA_DESTINATION_RT:
			case BE_INSERT_PPT_MEDIA_DESTINATION:

				iRc = pServerManagementHandler->m_pNwEvents->GeneratePptVideoSubscription( 
							pThreadCurrent
							);
				if( iRc == 0 )
				{
				}
				break;
			case BE_REMOVE_PPT_VIDEO_SOURCE_RT:
			case BE_REMOVE_PPT_VIDEO_SOURCE:

				iRc = pServerManagementHandler->m_pNwEvents->GeneratePptVideoSourceRemove( 
							pThreadCurrent
							);
				if( iRc == 0 )
				{
				}
				break;
			case BE_REMOVE_PPT_MEDIA_DESTINATION_RT:
			case BE_REMOVE_PPT_MEDIA_DESTINATION:

				iRc = pServerManagementHandler->m_pNwEvents->GeneratePptVideoSubscriptionRemove( 
							pThreadCurrent
							);
				if( iRc == 0 )
				{
				}
				break;
			case BE_REMOVE_PPT_AUDIO_SOURCE_RT:
			case BE_REMOVE_PPT_AUDIO_SOURCE:

				iRc = pServerManagementHandler->m_pNwEvents->GeneratePptAudioSourceRemove( 
							pThreadCurrent
							);
				if( iRc == 0 )
				{
				}
				break;
			case BE_DELIVER_EVENT_KEEP_ALIVE:

				iRc = pServerManagementHandler->m_pNwEvents->GenerateKeepAlive( 
							pThreadCurrent
							);
				if( iRc == 0 )
				{
				}
				break;
			case BE_DROP_USER:

				iRc = pServerManagementHandler->m_pNwEvents->GenerateDropUser( 
							pThreadCurrent
							);
				if( iRc == 0 )
				{
				}
				break;
			default:
				break;
		}

		//OutputDebugStringf( TEXT( "Post - PostEventProcess Eventtype %d:" )
		//		, pSqlServerTriggerEvent->eventTypeId
		//		);


		pThreadCurrent->m_pTxnObj->Commit(
			FALSE
			, 0
			, FALSE
			);
	}
	catch( ... )
	{
		pThreadCurrent->m_pTxnObj->Abort(
			NULL
			, FALSE
			, FALSE
			);
	}

	CServerManagementHandler::ReturnServerPacketThreadData(
		pThreadCurrent
		, pServerManagementHandler
		);
}