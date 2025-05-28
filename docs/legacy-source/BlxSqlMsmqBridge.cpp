#include "BlxSqlMsmqBridge.h"
#include "xphelper.h"

CBlxLock blxLock;

HRESULT getQueueHandle ( PTCHAR szQueueName,
							SRV_PROC* pSrvProc,
							HANDLE* phQueue,
							DWORD dwAccessMode = MQ_SEND_ACCESS,                // Access mode of queue
				 			DWORD dwShareMode = MQ_DENY_NONE					// Share mode of queue
						)
{
    HRESULT hr = S_OK;   

	if( mqConn.Lookup( szQueueName, *phQueue ) )
	{
		return hr;
	}
	else
	{
		//Open the destination queue to send messages.  
		//If not previously opened, open and add to cache
		hr = MQOpenQueue( szQueueName ,           // Format name of queue
							dwAccessMode,                      // Access mode of queue
							dwShareMode,                       // Share mode of queue
							phQueue                            // OUT: Handle of queue
						);
		if( FAILED( hr ) )
		{
			printError( pSrvProc, "Open for Q failed" );
			return FAIL;
		}
		mqConn.SetAt( 
			szQueueName
			, *phQueue
			);
	}

	return hr;
}

// send szErrorMsg to client
void printError( SRV_PROC *pSrvProc, 
				CHAR* szErrorMsg
				)
{
	srv_sendmsg( pSrvProc, 
					SRV_MSG_ERROR, 
					XP_ERROR, 
					SRV_INFO, 
					1,
					NULL, 
					0, 
					(DBUSMALLINT) __LINE__, 
					szErrorMsg,
					SRV_NULLTERM
				);

	srv_senddone( pSrvProc, 
					( SRV_DONE_ERROR | SRV_DONE_MORE ), 
					0, 
					0
				);
}

// send XP usage info to client
void printUsage( SRV_PROC *pSrvProc )
{
	srv_sendmsg( pSrvProc, 
					SRV_MSG_ERROR, 
					XP_PARAM_ERROR, 
					SRV_INFO, 
					1,
					NULL, 
					0, 
					( DBUSMALLINT ) __LINE__, 
					"usage: exec xp_blx_sqltriggerevent @QueueName, @MessageLabel, plus info for specific event...",
					SRV_NULLTERM
				);
	srv_senddone( pSrvProc, 
					( SRV_DONE_ERROR | SRV_DONE_MORE ), 
					0, 
					0
				);

}
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	HRESULT hr = SUCCEED;
	bool rc = TRUE;
	switch( ul_reason_for_call ) 
    { 
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
			POSITION pos;
			pos = mqConn.GetHeadPosition();
			
			bool fRC = false;
			__try
			{
				while( NULL != pos )
				{
					HANDLE hQh = mqConn.GetNextValue( pos );
					hr =  MQCloseQueue( hQh );
					if( FAILED ( hr ) )
					{
						rc = FALSE;
						break;
					}
				}

				fRC = true;
			}
			__finally
			{
				if( fRC )
				{
					mqConn.RemoveAll( );
				}
				else
				{
					mqConn.RemoveAll( );
				}
			}

			DeleteCriticalSection( &critSec );
            break;
    }
    return rc;
}

