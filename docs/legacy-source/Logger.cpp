#include "Logger.h"

#include "BlxHeap2.hpp"
#include "SyncQueue.h"
#include "ScopedLock.h"
//#include <atlbase.h>
#include <boost\shared_ptr.hpp>

const size_t     MAX_FILE_PATH_NAME         = 255;
const size_t     MAX_FUNCTION_NAME          = 255;
const size_t     MAX_LOG_EVENT_MESSAGE      = 1024;
const size_t     MAX_LOGGER_BUFFER_SIZE     = 5;
const size_t     MAX_LOGGER_WQ_SIZE         = 5;

struct _LOGGER_EVENT  {
    CHAR        fileName[ MAX_FILE_PATH_NAME ];
    CHAR        functionName[ MAX_FUNCTION_NAME ];
    CHAR        eventMessage[ MAX_LOG_EVENT_MESSAGE ];
};

class CLoggerEvent
    : public _LOGGER_EVENT
    , public CBlxHeap2< CLoggerEvent >
{
    friend class CBlxHeap2< CLoggerEvent >;
public:

    CLoggerEvent( ){};
    ~CLoggerEvent(){};

protected:

    static HANDLE m_hHeap;
    static LONG m_uNumAllocsInHeap;
    static CComCriticalSection* m_csLock;

private:

};


typedef Blxware::SyncQueue< CLoggerEvent* > QCLoggerEvent;
typedef boost::shared_ptr<QCLoggerEvent> PQCLoggerEvent;

HANDLE CLoggerEvent::m_hHeap = NULL;
LONG CLoggerEvent::m_uNumAllocsInHeap = 0;
CComCriticalSection* CLoggerEvent::m_csLock = NULL;

class CLoggerImpl
{
private:
    CLoggerImpl( CLoggerImpl& ){};

    CBlxLock                   m_pLock;

    static HRESULT StartServiceThread( CLoggerImpl* );
    static HRESULT EndServiceThread( CLoggerImpl* );
#ifndef UNDER_CE
    static UINT 
#else
    static DWORD
#endif
        WINAPI ServiceWorker( PVOID );

    PQCLoggerEvent              m_pLoggerEventBuffer;
    PQCLoggerEvent              m_pLoggerEventWorkQueue;
    CHandle						m_hWqThread;
    BOOL                        m_run;
    //  Eventually, all file processing will be moved into a sync type of trait
    //  that the CLoggerImpl class will use.  This will enable us to create a
    //  UDP sync that writes to the service for better scalability
    FILE*                       m_hFile;

    HRESULT AllocateBuffer( );
    HRESULT InitializeWorkQueue();
    HRESULT GetCurrentLoggerBuffer( CLoggerEvent** );
    HRESULT ReturnCurrentLoggerBuffer( CLoggerEvent* );

public:
    CLoggerImpl();
    ~CLoggerImpl();

    VOID LogTraceEvent(
        LPCSTR pFileName
        , LPCSTR pFunctionName
        , LPCSTR pTraceMessage
        );
};

CLoggerImpl::CLoggerImpl()
    : m_hWqThread()
    , m_run(FALSE)
    , m_hFile(NULL)
{
    AllocateBuffer();
    InitializeWorkQueue();

    m_hFile = fopen( "./log.txt", "w");

    if( m_hFile == INVALID_HANDLE_VALUE ) 
    {
        goto exit;
    }

    if( FAILED( StartServiceThread( this )) ) {
        goto exit;
    }
exit:
    return;
}

CLoggerImpl::~CLoggerImpl()
{ 
    EndServiceThread( this ); 
    fflush( m_hFile );
    fclose( m_hFile );

}

HRESULT CLoggerImpl::AllocateBuffer()
{
    HRESULT hr = S_OK;

    CLoggerEvent* _pLoggerEvent = NULL;

    m_pLoggerEventBuffer.reset( new QCLoggerEvent( MAX_LOGGER_BUFFER_SIZE ) );

    for( size_t k = 0 ; k < MAX_LOGGER_BUFFER_SIZE ; k++ )
    {
        _pLoggerEvent =
            new CLoggerEvent();
        m_pLoggerEventBuffer->push(
            _pLoggerEvent
            );
    }

    return hr;
}

HRESULT CLoggerImpl::InitializeWorkQueue()
{
    HRESULT hr = S_OK;

    CLoggerEvent* _pLoggerEvent = NULL;

    m_pLoggerEventWorkQueue.reset( new QCLoggerEvent( MAX_LOGGER_WQ_SIZE ) );

    return hr;
}

HRESULT CLoggerImpl::StartServiceThread( CLoggerImpl* logger )
{
    HRESULT hr = S_OK;

    logger->m_run         = TRUE;
    HANDLE _wqThread = 
#ifndef UNDER_CE
        ( HANDLE )_beginthreadex(
        NULL
        , 0
        , &ServiceWorker
        , logger
        , NULL
        , NULL
#else
        ( HANDLE )CreateThread(
        NULL
        , 0
        , &ServiceWorker
        , logger
        , 2048
        , NULL
#endif
        );

    logger->m_hWqThread.Attach( _wqThread );
    return hr;
}

#ifndef UNDER_CE
UINT 
#else
DWORD 
#endif
WINAPI CLoggerImpl::ServiceWorker( PVOID context )
{
    CLoggerImpl* _pLoggerImpl = static_cast<CLoggerImpl*>( context );
    UINT rc = 0;

    const size_t    MAX_BUFFER          = MAX_FILE_PATH_NAME + MAX_FILE_PATH_NAME + MAX_LOG_EVENT_MESSAGE;
    CLoggerEvent* _pLoggerEventWqItem = NULL;

    while( _pLoggerImpl->m_run ) 
    {

        BOOL rv = FALSE;
        rv = _pLoggerImpl->m_pLoggerEventWorkQueue->poll_timed(
            _pLoggerEventWqItem
            , 10e4
            );

        if( !rv ) {

            continue;
        }


        //  *   write to the output file
        //      Going forward, we'll need to devise a parameterized templated solution
        //      where we have the concept of a sync that can either write to a file or
        //      to a network destination.  It will be up to the sync on where it writes
        //      and completely transparent to this code.  We will probably use the same
        //      udp destination that we do for everything else today and it will just be
        //      another event that will be handled in the pipeline.  Ofcourse, we need to
        //      correct the architecture of the service first so that it is more of a pipeline
        //      and not just alot of if statements ;-)

        //  *   Once the logger is complete, then we need to return the work queue item to the
        //      buffer for reuse.



        CHAR _tmpBuffer[ MAX_BUFFER ];
        ::ZeroMemory(
            _tmpBuffer
            , MAX_BUFFER
            );

        size_t _bufUsed             = 0;
        size_t _bytesWritten        = 0;

        StringCchPrintfA(
            _tmpBuffer
            , MAX_BUFFER
            , "%s,%s,%s\n"
            , _pLoggerEventWqItem->fileName
            , _pLoggerEventWqItem->functionName
            , _pLoggerEventWqItem->eventMessage
            );

        HRESULT hr =
        StringCchLengthA(
            _tmpBuffer
            , MAX_BUFFER
            , &_bufUsed
            );

        _bytesWritten = fwrite( _tmpBuffer, sizeof(CHAR), _bufUsed + 1 , _pLoggerImpl->m_hFile );

        _pLoggerImpl->ReturnCurrentLoggerBuffer( _pLoggerEventWqItem );
    }

    return rc;
}

HRESULT CLoggerImpl::EndServiceThread( CLoggerImpl* logger )
{
    HRESULT hr = S_OK; 

    logger->m_run = FALSE;
    WaitForSingleObject( logger->m_hWqThread, INFINITE );
    logger->m_hWqThread.Close();

    return hr;
}

VOID CLoggerImpl::LogTraceEvent(
                                LPCSTR pFileName
                                , LPCSTR pFunctionName
                                , LPCSTR pTraceMessage
                                )
{
    HRESULT hr = S_OK;

    CLoggerEvent* _pLoggerEvent = NULL;

    if( FAILED( GetCurrentLoggerBuffer( &_pLoggerEvent ) ) ) {
        goto exit;
    }
    /*
    -   Use new safe string functions to copy input params to loggerEvent Struct.
    -   Post loggerEvent to new Q that holds work items (different that Q with preallocated
    buffers.
    -   Need to spawn a worker thread when class is created that will read messages out
    of worker Q and write to file (for now).  This will eventually become a "sync" that
    is included as a paramaterized template.  Eventually, we'll be able to post these 
    messages to UDP and log in database!
    */
    StringCchCopyA( 
        _pLoggerEvent->fileName 
        , MAX_FILE_PATH_NAME
        , pFileName
        );

    StringCchCopyA( 
        _pLoggerEvent->functionName 
        , MAX_FUNCTION_NAME
        , pFunctionName
        );

    StringCchCopyA( 
        _pLoggerEvent->eventMessage  
        , MAX_LOG_EVENT_MESSAGE
        , pTraceMessage
        );

    m_pLoggerEventWorkQueue->push_timed(
        _pLoggerEvent
        , 10e4
        );

exit:
    return;
}

HRESULT CLoggerImpl::GetCurrentLoggerBuffer( CLoggerEvent** loggerEvent )
{
    HRESULT hr = S_OK;
    assert( NULL == loggerEvent );

    BOOL rv = FALSE;
    rv = m_pLoggerEventBuffer->poll_timed(
        *loggerEvent
        , 10e4
        );

    if( !rv ) {
        hr = E_FAIL;
    }
    return hr;
}

HRESULT CLoggerImpl::ReturnCurrentLoggerBuffer( CLoggerEvent* loggerEvent )
{
    HRESULT hr = S_OK;
    assert( NULL == loggerEvent );

    m_pLoggerEventBuffer->push_timed(
        loggerEvent
        , 10e4
        );
    return hr;
}

boost::shared_ptr< CLoggerImpl > _loggerImpl;
CBlxLock _lockLogger;

HRESULT 
InitializeTracing()
{
    HRESULT hr = S_OK;
    if( NULL == _loggerImpl.get()) {
        CScopedLock _lock( &_lockLogger );
        if( NULL == _loggerImpl.get() ) {
            _loggerImpl.reset( new CLoggerImpl() );
        }
    }

    return hr;
}

HRESULT 
FinalizeTracing()
{
    HRESULT hr = S_OK;
    if( NULL != _loggerImpl.get()) {
        CScopedLock _lock( &_lockLogger );
        if( NULL != _loggerImpl.get() ) {
            Sleep(1000);
            _loggerImpl.reset();
        }
    }

    return hr;
}

VOID 
LogTraceEvent(
              LPCSTR pFileName
              , LPCSTR pFunctionName
              , LPCSTR pTraceMessage
              )
{
    if( NULL == _loggerImpl.get()) {
        CScopedLock _lock( &_lockLogger );
        if( NULL == _loggerImpl.get() ) {
            _loggerImpl.reset( new CLoggerImpl() );
        }
    }

    _loggerImpl->LogTraceEvent(
        pFileName
        , pFunctionName
        , pTraceMessage
        );
    return;
}
