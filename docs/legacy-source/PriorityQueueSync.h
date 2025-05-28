#pragma once

/** 
* PriorityQueueSync provides a thread-safe wrapper around std::priority_queue 
* and adds additional functionality for timed polling and offering.
**/

#include "BlxMutex.h"

#include <queue>
#include <vector>

namespace Blxware {

    template<class PriorityQueueT>
    class PriorityQueueSync
    {
    public:
        PriorityQueueSync( const PriorityQueueT& initialPQ = PriorityQueueT() )
            : pq(initialPQ)
            , internalWaitTimeout( 1000 )
            , m_hmtxQ( m_h[ 0 ] )
            , m_hsemNumElements( m_h[ 1 ] )
            , m_maxSize( 1000 )
        { 
            m_hmtxQ = CreateMutex(
                NULL
                , FALSE
                , NULL
                );

            m_hsemNumElements = CreateSemaphore( 
                NULL
                , 0
                , m_maxSize
                , NULL
                );
            return;
        }

        virtual ~PriorityQueueSync() 
        { 
            CloseHandle( m_hsemNumElements );
            CloseHandle( m_hmtxQ );
        }

        // test if queue is empty
        bool empty() const {
            // don't need to lock
            return pq.empty();
        }

        // return length of queue
        typename PriorityQueueT::size_type size() const {
            // don't need to lock
            return pq.size();
        }

        // return highest-priority element
        typename PriorityQueueT::const_reference top() const {
            SetLastError( NO_ERROR );
            CBlxMutex _mtx( m_hmtxQ, internalTimeout );

            DWORD rc = GetLastError();

            if( ERROR_SUCCESS == rc ) {
                return pq.top();
            } else {
                SetLastError( ERROR_TIMEOUT );
            }		
        }

        // return mutable highest-priority element (retained)
        typename PriorityQueueT::reference top() {	
            SetLastError( NO_ERROR );
            CBlxMutex _mtx( m_hmtxQ, internalTimeout );

            DWORD rc = GetLastError();

            if( ERROR_SUCCESS == rc ) {
                return pq.top();
            } else {
                SetLastError( ERROR_TIMEOUT );
            }	
        }

        // insert value in priority order
        void push(const typename PriorityQueueT::value_type& v) {	

            SetLastError( NO_ERROR );
            CBlxMutex _mtx( m_hmtxQ, timout );

            BOOL isOk = FALSE;
            DWORD rc = GetLastError();

            if( ERROR_SUCCESS == rc ) {
                //don't push if the queue has exceeded its maximum size
                LONG prevCount = 0;
                isOk = ReleaseSemaphore( m_hsemNumElements, 1, &prevCount );
                if( isOk ) {
                    pq.push(v);
                } else {
                    SetLastError( ERROR_DATABASE_FULL );
                }
            } 
        }

        // erase highest-priority element
        void pop() {	

            SetLastError( NO_ERROR );
            CBlxMutex _mtx( m_hmtxQ, m_hsemNumElements, timout );

            DWORD rc = GetLastError();

            if( ERROR_SUCCESS == rc ) {
                pq.pop();
            }
        }

        // pops and returns highest-priority element through v_out
        // returns true if successful, false if no element is available to return
        bool poll(typename PriorityQueueT::value_type& v_out, const typename PriorityQueueT::size_type minSize = 0);
        // v is synchronously added to the priority queue if this will not exceed maxSize
        // a maxSize of zero indicates an unbounded size
        // returns true iff the add was successful and maxSize was not exceeded, false otherwise
        bool offer(const typename PriorityQueueT::value_type& v, const typename PriorityQueueT::size_type maxSize = 0);

        // timed functions attempt once to execute the function and if 
        // unsuccessful it will attempt a second time after waiting approximatly at most nanoSec nanoseconds
        // it is possible for it wait less time if the appropriate condition is signaled
        // if both attempts fail these functions return false; true if succesful on one of the attempts

        bool top_timed(typename PriorityQueueT::value_type& v_out, const DWORD timeout) const;
        bool poll_timed(typename PriorityQueueT::value_type& v_out, const DWORD timeout, const typename PriorityQueueT::size_type minSize = 0);
        // a maxSize of zero indicates an unbounded size
        bool offer_timed(const typename PriorityQueueT::value_type& v, const DWORD timeout, const typename PriorityQueueT::size_type maxSize = 0);

    private:
        PriorityQueueT				pq;
        const DWORD                 internalWaitTimeout;
        HANDLE m_h[ 2 ];
        HANDLE& m_hmtxQ;
        HANDLE& m_hsemNumElements;

        const int                   m_maxSize;
    };


    // returns highest-priority element through v_out if there is an element to return
    // this will attempt once to get the top and if an element is not available it will wait approximatly nanoSec nanoseconds 
    // before trying a second time, if an element is still not available this will return false, otherwise true
    // if this returns false it did not modify v_out
    template<class PriorityQueueT>
    bool PriorityQueueSync<PriorityQueueT>::top_timed(typename PriorityQueueT::value_type& v_out, const DWORD timeout) const {
        SetLastError( NO_ERROR );
        CBlxMutex _mtx( m_hmtxQ, m_hsemNumElements, timout );

        DWORD rc = GetLastError();

        if( ERROR_SUCCESS == rc ) {
            //try to get the top element
            if(pq.size() > 0) {
                v_out = pq.top();
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    // pops and returns highest-priority element through v_out
    template<class PriorityQueueT>
    bool PriorityQueueSync<PriorityQueueT>::poll(typename PriorityQueueT::value_type& v_out, const typename PriorityQueueT::size_type minSize) {

        SetLastError( NO_ERROR );
        CBlxMutex _mtx( m_hmtxQ, m_hsemNumElements, internalWaitTimeout );

        DWORD rc = GetLastError();

        if( ERROR_SUCCESS == rc ) {
            if(minSize == 0 && pq.size() > 0) {
                v_out = pq.top();
                pq.pop();
                return true;
            }else{
                if( minSize > 0 && pq.size() > minSize ){
                    v_out = pq.top();
                    pq.pop();
                    return true;
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
    }

    // pops and returns highest-priority element through v_out
    // returns true iff the poll was successful, false otherwise
    // this will attempt once and if there is no element this will wait 
    // approximatly nanoSec nanoseconds before attemping a second time, 
    // if the second attempt fails this will return false, otherwise true
    template<class PriorityQueueT>
    bool PriorityQueueSync<PriorityQueueT>::poll_timed(typename PriorityQueueT::value_type& v_out, const DWORD timeout, const typename PriorityQueueT::size_type minSize) {
        SetLastError( NO_ERROR );
        CBlxMutex _mtx( m_hmtxQ, m_hsemNumElements, timeout );

        DWORD rc = GetLastError();

        if( ERROR_SUCCESS == rc ) {
            if(minSize == 0 && pq.size() > 0) {
                v_out = pq.top();
                pq.pop();
                return true;
            }else{
                if( minSize > 0 && pq.size() > minSize ){
                    v_out = pq.top();
                    pq.pop();
                    return true;
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
    }

    // v is synchronously added to the priority queue if this will not exceed maxSize
    // returns true iff the add was successful and maxSize was not exceeded, false otherwise
    template<class PriorityQueueT>
    bool PriorityQueueSync<PriorityQueueT>::offer(const typename PriorityQueueT::value_type& v, const typename PriorityQueueT::size_type maxSize) {

        SetLastError( NO_ERROR );
        CBlxMutex _mtx( m_hmtxQ, internalWaitTimeout );

        BOOL isOk = FALSE;
        DWORD rc = GetLastError();

        if( ERROR_SUCCESS == rc ) {
            //don't push if the queue has exceeded its maximum size
            LONG prevCount = 0;
            isOk = ReleaseSemaphore( m_hsemNumElements, 1, &prevCount );
            if( isOk ) {
                if(maxSize == 0 || pq.size() < maxSize) {
                    pq.push(v);
                    isOk = true;
                } else {
                    isOk = false;
                }
            } else {
                SetLastError( ERROR_DATABASE_FULL );
            }
        } 

        return isOk;
    }

    // v is synchronously added to the priority queue if this will not exceed maxSize
    // returns true iff the add was successful and maxSize was not exceeded, false otherwise
    // this will attempt once and if v cannot be added without exceeding maxSize this will wait 
    // approximatly nanoSec nanoseconds before attemping a second time, 
    // if the second attempt fails this will return false, otherwise true
    template<class PriorityQueueT>
    bool PriorityQueueSync<PriorityQueueT>::offer_timed(const typename PriorityQueueT::value_type& v, const DWORD timeout, const typename PriorityQueueT::size_type maxSize) {

        SetLastError( NO_ERROR );
        CBlxMutex _mtx( m_hmtxQ, timeout );

        BOOL isOk = FALSE;
        DWORD rc = GetLastError();

        if( ERROR_SUCCESS == rc ) {
            //don't push if the queue has exceeded its maximum size
            LONG prevCount = 0;
            isOk = ReleaseSemaphore( m_hsemNumElements, 1, &prevCount );
            if( isOk ) {
                if(maxSize == 0 || pq.size() < maxSize) {
                    pq.push(v);
                    isOk = true;
                } else {
                    isOk = false;
                }
            } else {
                SetLastError( ERROR_DATABASE_FULL );
            }
        } 

        return isOk;
    }
} //end namespace Blxware

