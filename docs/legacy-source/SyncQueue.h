#pragma once

#ifndef __SYNC_QUEUE
#define __SYNC_QUEUE


#include "BlxMutex.h"

#include <queue>
#include <vector>


// A thread safe queue library
namespace Blxware 
{

template<typename T>
class SyncQueue {
public:
	const size_t    maxSize;
    const DWORD     internalWaitTimeout;
	SyncQueue(const size_t maximumSize) 
        : maxSize(maximumSize) 
        , internalWaitTimeout( 1000 )
        , m_hmtxQ( m_h[ 0 ] )
        , m_hsemNumElements( m_h[ 1 ] )
    { 
        m_hmtxQ = CreateMutex(
            NULL
            , FALSE
            , NULL
            );

        m_hsemNumElements = CreateSemaphore( 
            NULL
            , 0
            , maxSize
            , NULL
            );
        return;
    }
	virtual ~SyncQueue() 
    { 
        CloseHandle( m_hsemNumElements );
        CloseHandle( m_hmtxQ );
    }
	size_t size() const {

        return internalSize(internalWaitTimeout);
	}

    bool push(const T& x) {

        return internalPush( x, internalWaitTimeout );
    }

	//retrieves and removes an element from the front of the queue, false if empty
	bool poll(T& ret) {
		return internalPoll( ret, internalWaitTimeout );
	}

    T& front() {
        return internalFront( internalWaitTimeout );
    }

    const T& front() const {
        return internalFront( internalWaitTimeout );
    }

	bool isFull() const {
		//TODO TEST we should be able to lock this recursively
		return maxSize > 0 && Q.size() >= maxSize;
	}

    void clear() {
        //BOOL ok = ( WaitForMultipleObjects( _countof( m_h ), m_h, TRUE, internalWaitTimeout ) == WAIT_OBJECT_0 );
        CBlxMutex _mtx( m_hmtxQ, m_hsemNumElements, internalWaitTimeout );

        DWORD rc = GetLastError();

        if( ERROR_SUCCESS == rc ) {
            while(!Q.empty()) {
                Q.pop();
            }
            rc = 0;
        } else {
            SetLastError( ERROR_TIMEOUT );
            return;
        }  
    }

	// timed functions attempt once to execute the function and if 
	// unsuccessful it will attempt a second time after waiting approximatly at most nanoSec nanoseconds
	// it is possible for it wait less time if the appropriate condition is signaled
	// if both attempts fail these functions return false; true if succesful on one of the attempts

	bool poll_timed(T& v_out, const DWORD timeout);
	// a maxSize of zero indicates an unbounded size
	bool push_timed(const T& v, const DWORD timeout);

protected:
	std::queue<T> Q;

private:
    HANDLE m_h[ 2 ];
    HANDLE& m_hmtxQ;
    HANDLE& m_hsemNumElements;

    size_t internalSize(DWORD timout) const;
    bool internalPush(const T& x, DWORD timout );
    bool internalPush(const std::vector<T>& vec, DWORD timout);
    bool internalPush(const T* arrayOElements, const size_t sizeOfArray, DWORD timout);
    void internalPop( DWORD timout );
    bool internalPoll(T& ret, DWORD timout);
    T& internalFront(DWORD timout);
    bool internalIsFull(DWORD timout);
    bool internalClear(DWORD timout);
};

template<class T>
size_t SyncQueue<T>::internalSize(DWORD timout) const {
    SetLastError( NO_ERROR );
    CBlxMutex _mtx( m_hmtxQ, timout );

    DWORD rc = GetLastError();

    size_t retVal = 0;

    if( ERROR_SUCCESS == rc ) {
        retVal = Q.size();
    } else {
        retVal = 0;
    }

    return retVal;
}

template<class T>
bool SyncQueue<T>::internalPush(const T& x, DWORD timout) {
    SetLastError( NO_ERROR );
    CBlxMutex _mtx( m_hmtxQ, timout );

    BOOL isOk = FALSE;
    DWORD rc = GetLastError();

    if( ERROR_SUCCESS == rc ) {
        //don't push if the queue has exceeded its maximum size
        LONG prevCount = 0;
        isOk = ReleaseSemaphore( m_hsemNumElements, 1, &prevCount );
        if( isOk ) {
            Q.push(x);
        } else {
            SetLastError( ERROR_DATABASE_FULL );
        }
    } 

    return isOk;
}

//retrieves and removes an element from the front of the queue, false if empty
template<class T>
bool SyncQueue<T>::internalPoll(T& ret, DWORD timout) {
    SetLastError( NO_ERROR );
    CBlxMutex _mtx( m_hmtxQ, m_hsemNumElements, timout );

    DWORD rc = GetLastError();

    if( ERROR_SUCCESS == rc ) {
        if(Q.empty()) {
            return false;
        }
        ret = Q.front();
        Q.pop();

        return true;
    } else {
        return false;
    }
}

template<class T>
T& SyncQueue<T>::internalFront( DWORD timeout) {
    T& retVal = NULL;
    SetLastError( NO_ERROR );
    CBlxMutex _mtx( m_hmtxQ, timout );

    DWORD rc = GetLastError();

    if( ERROR_SUCCESS == rc ) {
        retVal = Q.front();
    } else {
        SetLastError( ERROR_TIMEOUT );
    }

    return retVal;
}

template<class T>
bool SyncQueue<T>::internalIsFull(DWORD timout) {
    return maxSize > 0 && Q.size() >= maxSize;
}

template<class T>
bool SyncQueue<T>::internalClear(DWORD timout) {
    SetLastError( NO_ERROR );

    CBlxMutex _mtx( m_hmtxQ, m_hsemNumElements, timout );

    DWORD rc = GetLastError();

    if( ERROR_SUCCESS == rc ) {
        while(!Q.empty()) {
            Q.pop();
        }
    }
}

// pops and returns highest-priority element through v_out
// returns true iff the poll was successful, false otherwise
// this will attempt once and if there is no element this will wait 
// approximatly nanoSec nanoseconds before attemping a second time, 
// if the second attempt fails this will return false, otherwise true
template<class T>
bool SyncQueue<T>::poll_timed(T& v_out, const DWORD timeout) {
    return internalPoll( v_out, timeout );
}

// v is synchronously added to the priority queue if this will not exceed maxSize
// returns true iff the add was successful and maxSize was not exceeded, false otherwise
// this will attempt once and if v cannot be added without exceeding maxSize this will wait 
// approximatly nanoSec nanoseconds before attemping a second time, 
// if the second attempt fails this will return false, otherwise true
template<class T>
bool SyncQueue<T>::push_timed(const T& v, const DWORD timeout) {
    return internalPush( v, timeout );
}

} //end namespace Blxware
#endif //#ifndef __SYNC_QUEUE