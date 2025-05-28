#include "BlxSqlMsmqBridge.h"
#include "xphelper.h"

#include "PacketInfoStructs.h"
#include "debug.h"

/*
xp_SqlMqBridge:  Extended Stored Procedure SQL to MSMQ Bridge within a Distributed Transaction.
This XProc provides a way to send messages from SQL Server 2000 to MSMQ with the requirement
that the parent process is working under an MSDTC.  Should there be no MSDTC at the parent level,
the message will fail.  In this scenario, should the transaction fail within MSMQ, SQL, or any of 
the enlisted resource managers participating in the transaction, the message(s) will not be sent.
*/

#ifdef __cplusplus
extern "C" {
#endif

RETCODE __declspec( dllexport ) xp_blx_sqltriggerevent( SRV_PROC* pSrvProc );

#ifdef __cplusplus
}
#endif

RETCODE __declspec( dllexport ) xp_blx_sqltriggerevent( SRV_PROC* pSrvProc )
{
    SRVRETCODE					rc = XP_NOERROR;

	XP_PARAM_EX*				pParams			= NULL;

	XP_PARAM_EX*				pQueueName				= NULL;				//WCHAR[ 255 ]
	XP_PARAM_EX*				pMessageLabel			= NULL;				//WCHAR[ 255 ]
	XP_PARAM_EX*				pTriggerId				= NULL;				//GUID
	XP_PARAM_EX*				pEventType				= NULL;				//BLXWARE_EVENT
	XP_PARAM_EX*				pEventTypeId			= NULL;				//BLXWARE_EVENT_TYPE
	XP_PARAM_EX*				pPublishingPointId		= NULL;				//GUID
	XP_PARAM_EX*				pPptParticipantId		= NULL;				//GUID
	XP_PARAM_EX*				pRowId					= NULL;				//GUID

	ITransaction*				pTransaction			= NULL;
	TCHAR						szQueueName[ 255 ]		= {0};
	TCHAR						szMessageLabel[ 255 ]	= {0};
	BOOL						pIsInTransaction		= FALSE;
	SQL_SERVER_TRIGGER_EVENT	ssTriggerEvent;

	__try
	{
		HRESULT hr;

		int nParams = srv_rpcparams( pSrvProc );
		if( ! ( nParams >= 1 ) && ( nParams <= 5 ) )
		{
			if ( nParams == -1 )
			{
				printUsage ( pSrvProc );
				rc = XP_ERROR;
			}
			__leave;
		}

		pParams = new XP_PARAM_EX[ nParams ];
		if( NULL == pParams )
		{
			__leave;
		}

		for( int i = 0 ; i < nParams; i++ )
		{
			::ZeroMemory(
				&pParams[ i ]
				, sizeof( XP_PARAM_EX )
				);

			rc = ::getparam_ex(
					pSrvProc
					, ( i + 1 )
					, &pParams[ i ]
			);

			if( FAIL == rc )
			{
				__leave;
			}
			if( _tcsicmp( pParams[ i ].szName, _T( "@TriggerId" ) ) == 0 )
			{
				if( IS_GUID( pParams[ i ].bType ) )
				{
					pTriggerId = &pParams[ i ];
				}
				else
				{
					__leave;
				}
			}
			else if( _tcsicmp( pParams[ i ].szName, _T( "@EventType" ) ) == 0 )
			{
				if( IS_INT( pParams[ i ].bType ) )
				{
					pEventType = &pParams[ i ];
				}
				else
				{
					__leave;
				}
			}
			else if( _tcsicmp( pParams[ i ].szName, _T( "@EventTypeId" ) ) == 0 )
			{
				if( IS_INT( pParams[ i ].bType ) )
				{
					pEventTypeId = &pParams[ i ];
				}
				else
				{
					__leave;
				}
			}
			else if( _tcsicmp( pParams[ i ].szName, _T( "@PublishingPointId" ) ) == 0 )
			{
				if( IS_GUID( pParams[ i ].bType ) )
				{
					pPublishingPointId = &pParams[ i ];
				}
				else
				{
					__leave;
				}
			}
			else if( _tcsicmp( pParams[ i ].szName, _T( "@RowId" ) ) == 0 )
			{
				if( IS_GUID( pParams[ i ].bType ) )
				{
					pRowId = &pParams[ i ];
				}
				else
				{
					__leave;
				}
			}
			else if( _tcsicmp( pParams[ i ].szName, _T( "@PptParticipantId" ) ) == 0 )
			{
				if( IS_GUID( pParams[ i ].bType ) )
				{
					pPptParticipantId = &pParams[ i ];
				}
				else
				{
					__leave;
				}
			}
			else if( _tcsicmp( pParams[ i ].szName, _T( "@QueueName" ) ) == 0 )
			{
				if( IS_STRING( pParams[ i ].bType ) )
				{
					pQueueName = &pParams[ i ];
				}
				else
				{
					__leave;
				}
			}
			else if( _tcsicmp( pParams[ i ].szName, _T( "@MessageLabel" ) ) == 0 )
			{
				if( IS_STRING( pParams[ i ].bType ) )
				{
					pMessageLabel = &pParams[ i ];
				}
				else
				{
					__leave;
				}
			}
			else
			{
				__leave;
			}
		}

		rc = srv_getdtcxact( 
				pSrvProc
				, ( void** )&pTransaction
				);
		if( FAIL == rc )
		{
			printError( pSrvProc, 
						"No DTC for sqltriggerevent!"
						);
			__leave;
		}

		if( pTransaction )
		{
			if( pTriggerId->fNull == TRUE )
			{
				__leave;
			}
			rc = ::getparam_data_ex(
					pSrvProc
					, pTriggerId->iOrdinal
					, pTriggerId
					, ( PBYTE )&ssTriggerEvent.triggerId  
					);

			if( pEventType->fNull == TRUE )
			{
				__leave;
			}
			rc = ::getparam_data_ex(
					pSrvProc
					, pEventType->iOrdinal
					, pEventType
					, ( PBYTE )&ssTriggerEvent.eventType  
					);

			if( pEventTypeId->fNull == TRUE )
			{
				__leave;
			}
			rc = ::getparam_data_ex(
					pSrvProc
					, pEventTypeId->iOrdinal
					, pEventTypeId
					, ( PBYTE )&ssTriggerEvent.eventTypeId  
					);
		
			if( pPublishingPointId->fNull == TRUE )
			{
				__leave;
			}
			rc = ::getparam_data_ex(
					pSrvProc
					, pPublishingPointId->iOrdinal
					, pPublishingPointId
					, ( PBYTE )&ssTriggerEvent.publishingPoint 
					);

			if( pPptParticipantId->fNull == TRUE )
			{
				__leave;
			}
			rc = ::getparam_data_ex(
					pSrvProc
					, pPptParticipantId->iOrdinal
					, pPptParticipantId
					, ( PBYTE )&ssTriggerEvent.pptParticipantId  
					);

			if( pRowId->fNull == TRUE )
			{
				__leave;
			}
			rc = ::getparam_data_ex(
					pSrvProc
					, pRowId->iOrdinal
					, pRowId
					, ( PBYTE )&ssTriggerEvent.rowId  
					);

			if( pQueueName->fNull == TRUE )
			{
				__leave;
			}
			rc = ::getparam_data_ex(
					pSrvProc
					, pQueueName->iOrdinal
					, pQueueName
					, ( PBYTE )&szQueueName  
					);

			if( pMessageLabel->fNull == TRUE )
			{
				__leave;
			}
			rc = ::getparam_data_ex(
					pSrvProc
					, pMessageLabel->iOrdinal
					, pMessageLabel
					, ( PBYTE )&szMessageLabel  
					);
		}
		else
		{
			__leave;
		}

		// /////////////////////////////////////Begin - Send message to MSMQ
		////////////////////////////////////////////////////////////////
		//  Define the required structures.
		////////////////////////////////////////////////////////////////

		const INT32 NUMBEROFPROPERTIES = 4; //Number of message properties
		DWORD cPropId = 0; 

		MQMSGPROPS msgprops;
		MSGPROPID aMsgPropId[NUMBEROFPROPERTIES];     
		MQPROPVARIANT aMsgPropVar[NUMBEROFPROPERTIES];
		HRESULT aMsgStatus[NUMBEROFPROPERTIES];

		//////////////////////////////////////////////////////////////////
		// Specify message properties.
		//////////////////////////////////////////////////////////////////
		aMsgPropId[ cPropId ] = PROPID_M_BODY;     
		aMsgPropVar[ cPropId ].vt = VT_VECTOR | VT_UI1; 
		aMsgPropVar[ cPropId ].caub.pElems = ( LPBYTE )&ssTriggerEvent;
		aMsgPropVar[ cPropId ].caub.cElems = MAX_SQL_SERVER_TRIGGER_EVENT;
		cPropId++;      
		aMsgPropId[ cPropId ] = PROPID_M_LABEL;                  // PROPVARIENT Field
		aMsgPropVar[ cPropId ].vt = VT_LPWSTR;                   // Type Indicator
		aMsgPropVar[ cPropId ].pwszVal = ( LPWSTR )szMessageLabel; // Message label
		cPropId++;

		aMsgPropId[ cPropId ] = PROPID_M_DELIVERY;            // MQPROPVARIANT Field
		aMsgPropVar[ cPropId ].vt = VT_UI1;                   // Type Indicator
		aMsgPropVar[ cPropId ].bVal = MQMSG_DELIVERY_RECOVERABLE;
		cPropId++;

		aMsgPropId[ cPropId ] = PROPID_M_BODY_TYPE;
		aMsgPropVar[ cPropId ].vt = VT_UI4;
		aMsgPropVar[ cPropId ].ulVal = VT_ARRAY | VT_UI1;
		cPropId++;


		// Initialize the MQMSGPROPS structure.
		msgprops.cProp = cPropId;                           // Number of properties
		msgprops.aPropID = aMsgPropId;                      // Ids of properties
		msgprops.aPropVar = aMsgPropVar;                    // Values of properties
		msgprops.aStatus = aMsgStatus;                      // Error reports

		HANDLE hQueue = NULL;                               // Queue handle
		DWORD dwAccessMode = MQ_SEND_ACCESS;                // Access mode of queue
		DWORD dwShareMode = MQ_DENY_NONE;                   // Share mode of queue

		// Obtain format name of queue.
		// Open the destination queue to send messages.
		__try
		{
			blxLock.Lock();

			hr = getQueueHandle( szQueueName,
									pSrvProc,
									&hQueue,
									dwAccessMode,
									dwShareMode
								);
			// Send message to queue as recoverable.
			if( S_OK == hr )
			{
				hr = MQSendMessage( hQueue,                         // Handle to open queue
									&msgprops,                       // Properties of message
									pTransaction					// Current DTC to enlist MSMQ
									);  
			}

		}
		__finally
		{
				blxLock.Release();
		}

		if( FAILED( hr ) )
		{
			printError( pSrvProc, 
						"Send of message for Q failed"
						);
			rc = XP_ERROR;
			__leave;
		}
	}
	__finally
	{
		if( pParams )
		{
			delete[] pParams;
			pParams = NULL;
			pTransaction = NULL;
		}
	}

    return rc;
}

