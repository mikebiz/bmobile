#pragma once

#ifdef BLXWARE_EXPORTS
#define BLXWARE_API __declspec(dllexport)
#define BLXWARE_LINKAGE extern
#else
#define BLXWARE_API __declspec(dllimport)
#define BLXWARE_LINKAGE extern 
#endif

#include <windows.h>
#include "BlxHeap.hpp"

inline void chMB(PCSTR szMsg) {
   char szTitle[MAX_PATH];
   GetModuleFileNameA(NULL, szTitle, _countof(szTitle));
   MessageBoxA(GetActiveWindow(), szMsg, szTitle, MB_OK);
}


inline void chFAIL(PSTR szMsg) {
   chMB(szMsg);
   DebugBreak();
}

// Put up an assertion failure message box.
inline void chASSERTFAIL(LPCSTR file, int line, PCSTR expr) {
   char sz[2*MAX_PATH];
   wsprintfA(sz, "File %s, line %d : %s", file, line, expr);
   chFAIL(sz);
}


// Put up a message box if an assertion fails in a debug build.
#ifdef _XDEBUG
   #define chASSERT(x) if (!(x)) chASSERTFAIL(__FILE__, __LINE__, #x)
#else
   #define chASSERT(x)
#endif


// Assert in debug builds, but don't remove the code in retail builds.
#ifdef _XDEBUG
   #define chVERIFY(x) chASSERT(x)
#else
   #define chVERIFY(x) (x)
#endif

class BLXWARE_API CIOCP 
	: public CBlxHeap< CIOCP >
{
public:
   CIOCP(int nMaxConcurrency = -1) { 
      m_hIOCP = NULL; 
      if (nMaxConcurrency != -1)
         (void) Create(nMaxConcurrency);
   }

   ~CIOCP() { 
      if (m_hIOCP != NULL) 
         chVERIFY(CloseHandle(m_hIOCP)); 
   }


   BOOL Close() {
      BOOL bResult = CloseHandle(m_hIOCP);
      m_hIOCP = NULL;
      return(bResult);
   }

   BOOL Create(int nMaxConcurrency = 0) {
      m_hIOCP = CreateIoCompletionPort(
         INVALID_HANDLE_VALUE, NULL, 0, nMaxConcurrency);
      chASSERT(m_hIOCP != NULL);
      return(m_hIOCP != NULL);
   }

   BOOL AssociateDevice(HANDLE hDevice, ULONG_PTR CompKey) {
      BOOL fOk = (CreateIoCompletionPort(hDevice, m_hIOCP, CompKey, 0) 
         == m_hIOCP);
      chASSERT(fOk);
      return(fOk);
   }

   BOOL AssociateSocket(SOCKET hSocket, ULONG_PTR CompKey) {
      return(AssociateDevice((HANDLE) hSocket, CompKey));
   }

   BOOL PostStatus(ULONG_PTR CompKey, DWORD dwNumBytes = 0, 
      OVERLAPPED* po = NULL) {

      BOOL fOk = PostQueuedCompletionStatus(m_hIOCP, dwNumBytes, CompKey, po);
      chASSERT(fOk);
      return(fOk);
   }

   BOOL GetStatus(ULONG_PTR* pCompKey, PDWORD pdwNumBytes,
      LPOVERLAPPED* ppo, DWORD dwMilliseconds = INFINITE) {

      return(GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, 
         pCompKey, ppo, dwMilliseconds));
   }
	
	static HANDLE get_Heap()
	{
		return m_hHeap;
	}
	static set_Heap( HANDLE hHeap)
	{
		m_hHeap = hHeap;
	}
	static HANDLE delete_Heap()
	{
		m_hHeap = NULL;
	}

	static UINT get_NumAllocsInHeap()
	{
		return m_uNumAllocsInHeap;
	}
	static set_NumAllocsInHeap( UINT uNumAllocsInHeap )
	{
		m_uNumAllocsInHeap = uNumAllocsInHeap;
	}
	static increment_NumAllocsInHeap( )
	{
		m_uNumAllocsInHeap++;
	}
	static decrement_NumAllocsInHeap( )
	{
		m_uNumAllocsInHeap--;
	}
protected:
	static HANDLE m_hHeap;
	static UINT m_uNumAllocsInHeap;

private:
   HANDLE m_hIOCP;
};
