#pragma once

#include <windows.h>
#include <tchar.h>
#include <StrSafe.h>


///////////////////////////////////////////////////////////////////////////////


class CQueue {
public:
   struct ELEMENT {
      int m_nThreadNum, m_nRequestNum;
      // Other element data should go here
   };
   typedef ELEMENT* PELEMENT;

private:
   PELEMENT m_pElements;        // Array of elements to be processed
   int      m_nMaxElements;     // Maximum # of elements in the array
   HANDLE   m_h[2];             // Mutex & semaphore handles
   HANDLE   &m_hmtxQ;           // Reference to m_h[0]
   HANDLE   &m_hsemNumElements; // Reference to m_h[1]

public:
   CQueue(int nMaxElements);
   ~CQueue();

   BOOL Append(PELEMENT pElement, DWORD dwMilliseconds);
   BOOL Remove(PELEMENT pElement, DWORD dwMilliseconds);
};


///////////////////////////////////////////////////////////////////////////////


CQueue::CQueue(int nMaxElements) 
   : m_hmtxQ(m_h[0]), m_hsemNumElements(m_h[1]) {

   m_pElements = (PELEMENT) 
      HeapAlloc(GetProcessHeap(), 0, sizeof(ELEMENT) * nMaxElements);
   m_nMaxElements = nMaxElements;
   m_hmtxQ = CreateMutex(NULL, FALSE, NULL);
   m_hsemNumElements = CreateSemaphore(NULL, 0, nMaxElements, NULL);
}


///////////////////////////////////////////////////////////////////////////////


CQueue::~CQueue() {

   CloseHandle(m_hsemNumElements);
   CloseHandle(m_hmtxQ);
   HeapFree(GetProcessHeap(), 0, m_pElements);
}


///////////////////////////////////////////////////////////////////////////////


BOOL CQueue::Append(PELEMENT pElement, DWORD dwTimeout) {

   BOOL fOk = FALSE;
   DWORD dw = WaitForSingleObject(m_hmtxQ, dwTimeout);

   if (dw == WAIT_OBJECT_0) {
      // This thread has exclusive access to the queue

      // Increment the number of elements in the queue
      LONG lPrevCount;
      fOk = ReleaseSemaphore(m_hsemNumElements, 1, &lPrevCount);
      if (fOk) {
         // The queue is not full, append the new element
         m_pElements[lPrevCount] = *pElement;
      } else {

         // The queue is full, set the error code and return failure
         SetLastError(ERROR_DATABASE_FULL);
      }

      // Allow other threads to access the queue
      ReleaseMutex(m_hmtxQ);

   } else {
      // Timeout, set error code and return failure
      SetLastError(ERROR_TIMEOUT);
   }

   return(fOk);   // Call GetLastError for more info
}


///////////////////////////////////////////////////////////////////////////////


BOOL CQueue::Remove(PELEMENT pElement, DWORD dwTimeout) {

   // Wait for exclusive access to queue and for queue to have element.
   BOOL fOk = (WaitForMultipleObjects(_countof(m_h), m_h, TRUE, dwTimeout) 
      == WAIT_OBJECT_0);

   if (fOk) {
      // The queue has an element, pull it from the queue
      *pElement = m_pElements[0];

      // Shift the remaining elements down
      MoveMemory(&m_pElements[0], &m_pElements[1], 
         sizeof(ELEMENT) * (m_nMaxElements - 1));

      // Allow other threads to access the queue
      ReleaseMutex(m_hmtxQ);

   } else {
      // Timeout, set error code and return failure
      SetLastError(ERROR_TIMEOUT);
   }

   return(fOk);   // Call GetLastError for more info
}
