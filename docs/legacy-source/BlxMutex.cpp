#include "BlxMutex.h"

CBlxMutex::CBlxMutex(HANDLE& mutex, DWORD timeout)
: m_mutex( mutex )
, m_semaphore( mutex )
{
    SetLastError( ERROR_SUCCESS );
    DWORD rc = WaitForSingleObject( m_mutex, timeout );

    if( WAIT_OBJECT_0 == rc ) {
        return;
    } else {
        SetLastError( ERROR_TIMEOUT );
    }

    return;
}

CBlxMutex::CBlxMutex(HANDLE& mutex, HANDLE& semaphore, DWORD timeout)
: m_mutex( mutex )
, m_semaphore( semaphore )
{
    SetLastError( ERROR_SUCCESS );
    //HANDLE hko[] = { m_mutex, m_semaphore };
    HANDLE hko[] = { m_semaphore, m_mutex };  //mxb - hack to make Windows Mobile work
    BOOL ret = FALSE;
#ifndef UNDER_CE
    ret = ( WaitForMultipleObjects( _countof(hko), hko, TRUE, timeout ) == WAIT_OBJECT_0 );
#else
    //DWORD evtReturned = WaitForMultipleObjects( _countof(hko), hko, FALSE, timeout );
    ////BOOL ret = FALSE;
    ////if( evtReturned == 0
    ////    || evtReturned == 1
    ////    )
    ////{
    ////    ret = TRUE;
    ////}

    //if( 0 == evtReturned )
    //{
    //    if( WaitForSingleObject( m_mutex, timeout ) == WAIT_OBJECT_0 )
    //    {
    //        ret = TRUE;
    //    }
    //}
    //else if( 1 == evtReturned )
    //{
    //    if( WaitForSingleObject( m_semaphore, timeout ) == WAIT_OBJECT_0 )
    //    {
    //        ret = TRUE;
    //    }
    //}
    //else
    //{
    //    ret = FALSE;
    //}
    //DWORD rc = WaitForSingleObject( m_mutex, timeout );
    //if( WAIT_OBJECT_0 == rc ) {
    //    DWORD rcSem = WaitForSingleObject( m_semaphore, timeout );
    //    if( WAIT_OBJECT_0 == rcSem ) {
    //        ret = TRUE;
    //    } else {
    //        //ReleaseMutex( m_mutex );
    //        //m_mutex = INVALID_HANDLE_VALUE;
    //        ret = FALSE;
    //    }
    //} else {
    //    ret = FALSE;
    //}
    ret = ( WaitForMultipleObjects( _countof(hko), hko, FALSE, timeout ) == WAIT_OBJECT_0 );
#endif

    if( ret ) {
        return;
    } else {
        SetLastError( ERROR_TIMEOUT );
    }
    return;
}
CBlxMutex::~CBlxMutex(void)
{
    //if( INVALID_HANDLE_VALUE != m_mutex )
    //{
        ReleaseMutex( m_mutex );
    //}
}
