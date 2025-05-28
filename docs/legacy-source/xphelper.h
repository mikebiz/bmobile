// ***************************************************************************
// copyright (C) 2000 SQLDEV.net
//
// @file@: xphelper.h
//
// purpose: XP help classes
//
//     
// notes:
//     
// update history:
//
//   @Version@:
//   00000 GED  11/07/2000	lib created
//
// @EndHeader@
// ***************************************************************************
#pragma once

#ifndef _XP_HELPER_H
#define _XP_HELPER_H

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINDOWS 0x0500	

#include <windows.h>
#include <tchar.h>
#include <comdef.h>			// COM Compiler support for _bstr_t
#include <assert.h>

#include <atlbase.h>
#include <atlstr.h>

#ifdef SQL70
#include "..\common\inc\70\srv.h"
#endif

#ifdef SQL80
#include "srv.h"
#endif

// globals
//
extern HINSTANCE g_hinstDLL;
extern HINSTANCE g_hResDLL;

// Macros
//
#define	NUMELEMENTS(array) (sizeof(array) / sizeof(array[0]))
#define	ROUND_UP(size, amount) (((DWORD)(size) +  ((amount) - 1)) & ~((amount) - 1))
#define	SAFE_DELETE(pv) if (pv) { delete [] (pv); pv = NULL; }  
#define	SAFE_RELEASE(pv) if (pv) { (pv)->Release(); pv = NULL; }  
#define	SAFE_FREE(pv) if (pv) { SysFreeString(pv); pv = NULL; }  

#ifdef SQL70
#define IS_INT(bType)		(	bType == SRVINTN		|| \
								bType == SRVINT4		|| \
								bType == SRVINT2		|| \
								bType == SRVINT1		)
#endif // SQL70

#ifdef SQL80
#define IS_INT(bType)		(	bType == SRVINTN		|| \
								bType == SRVINT8		|| \
								bType == SRVINT4		|| \
								bType == SRVINT2		|| \
								bType == SRVINT1		)
#endif // SQL80

#define IS_STRING(bType)	(	bType == SRVVARCHAR		|| \
								bType == SRVCHAR		|| \
								bType == SRVNVARCHAR	|| \
								bType == SRVNCHAR		|| \
								bType == SRVBIGVARCHAR	|| \
								bType == SRVBIGCHAR		)

#define IS_UNICODE(bType)	(	bType == SRVNVARCHAR	|| \
								bType == SRVNCHAR		|| \
								bType == SRVNTEXT		)

#define IS_BLOB(bType)		(	bType == SRVIMAGE		|| \
								bType == SRVTEXT		|| \
								bType == SRVNTEXT		)

#ifdef SQL80
#define IS_VARIANT(bType)	(	bType == SRVSSVARIANT	)
#endif // SQL80

#define IS_GUID(bType)		(	bType == SRVGUID		)

#define IS_DATE(bType)		(	bType == SRVDATETIME	|| \
								bType == SRVDATETIMN	|| \
								bType == SRVDATETIM4	)

#define XPDEF RETCODE __declspec(dllexport)

#ifdef _DEBUG
#define TRACE_PARAM(bType)	trace_param_type(bType)
#else
#define TRACE_PARAM(bType)
#endif

#define SYSNAME			128			// sysname
#define SYSNAME_CLEN	SYSNAME		// sysname character len
#define SYSNAME_BLEN	256			// sysname byte len

#define	MAX_COLUMN		SYSNAME		// max column name

#define MAX_INSTANCE_NAME	16
#define MAX_MSG			(DWORD)1024	// max message length in characters

#define MAX_CHAR		8000		// max number of characters for nchar or nvarchar
#define MAX_NCHAR		4000		// max number of characters for char or varchar

#ifdef UNICODE
#define MAX_TCHAR		MAX_NCHAR
#else
#define MAX_TCHAR		MAX_CHAR
#endif

// undocumented Unicode srv_pfield
//
#define	SRV_UNICODE_HOST		32
#define	SRV_UNICODE_APPLNAME	34

// ODS ANSI/Unicode mapping since ODS does not do that
//
#ifdef UNICODE
#define SRVTVARCHAR	SRVNVARCHAR
#define	SRVTCHAR	SRVNCHAR
#define	SRVTTEXT	SRVNTEXT
#else
#define SRVTVARCHAR	SRVVARCHAR
#define	SRVTCHAR	SRVCHAR
#define	SRVTTEXT	SRVTEXT
#endif

// XP return codes
//
#define	XP_NOERROR	0
#define	XP_SUCCESS	XP_NOERROR
#define	XP_ERROR	1

//----------------------------------------------------------------------------
// NAME: __GetXpVersion
//  
// PURPOSE: 
//  
// RETURNS: ULONG
//  
// NOTES:
//  
__declspec(dllexport) ULONG __GetXpVersion();

//----------------------------------------------------------------------------
// NAME: trace_param_type
//  
// PURPOSE: Trace parameter type to Debug Monitor
//  
// RETURNS: VOID
//  
// NOTES: DO ONLY USE THE MACRO TRACE_PARAM
//  
VOID trace_param_type(BYTE bType);

//----------------------------------------------------------------------------
// NAME: srv_describeW
//  
// PURPOSE: Unicode dependend version of srv_describe, use _srv_describe
//  
// RETURNS: int
//  
// NOTES: depended on UNICODE pre-compiler switch
//  
int srv_describeW(SRV_PROC* srvproc,int colnumber, WCHAR* column_name, int namelen, long int desttype,
				   long int destlen, long int srctype, long int srclen ,void* srcdata);

//----------------------------------------------------------------------------
// NAME: srv_sendmsgW
//  
// PURPOSE: Unicode depended version of srv_sendmsg, use _srv_sendmsg
//  
// RETURNS: int
//  
// NOTES: depended on UNICODE pre-compiler switch
//  
int srv_sendmsgW(SRV_PROC* srvproc, int msgtype, DBINT msgnum, DBTINYINT msgclass, 
				  BYTE state, WCHAR* rpcname, int rpcnamelen, USHORT linenum, WCHAR* message, int msglen);

// mappings
//
#ifdef UNICODE
#define srvsendmsg	srv_sendmsgW
#define srvdescribe	srv_describeW
#else
#define srvsendmsg	srv_sendmsg
#define srvdescribe	srv_describe
#endif // UNICODE


// helper functions
//
//----------------------------------------------------------------------------
// NAME: 
//  
// PURPOSE: 
//  
// RETURNS: 
//  
// NOTES: 
//  
HGLOBAL XPAlloc(UINT uFlags, DWORD dwBytes);

//----------------------------------------------------------------------------
// NAME: 
//  
// PURPOSE: 
//  
// RETURNS: 
//  
// NOTES: 
//  
HGLOBAL XPFree(HGLOBAL hMem);

//----------------------------------------------------------------------------
// NAME: Ansi2Unicode
//  
// PURPOSE: inplace conversion
//  
// RETURNS: BOOL
//  
// NOTES: 
//  
BOOL Ansi2Unicode(LPSTR lpszmbcsSource);

//----------------------------------------------------------------------------
// NAME: Unicode2Ansi
//  
// PURPOSE: Unicode2Ansi inplace conversion
//  
// RETURNS: BOOL
//  
// NOTES: 
//  
BOOL Unicode2Ansi(LPWSTR lpszwSource);

/*
RETCODE GetTransactionPointer(SRV_PROC* pSrvProc, ITransaction** ppTransaction)
{

	int nRetVal = XP_ERROR;
	int nODSRetVal = 0;

	// undocumented ODS call to obtain the transaction pointer for the current SPID
	// we need this to be able to obtain the cookie to pass to the child MTS 
	// component so that it can import and join the transaction

	// int SRVAPI srv_getdtcxact( SRV_PROC * srvproc, void** ppv);
	//
	// Obtains a copy of the OLE transaction object associated with the current
	// session. In the context of an XP, this API returns a pointer (ppv) to the 
	// DTC transaction associated with the calling session, if any. If there
	// is not an active DTC transaction, the returned pointer will be NULL.
	// If there is a DTC transaction associated with the session the returned 
	// pointer points to the ITransaction object, (with AddRef() called).

	nODSRetVal = srv_getdtcxact(pSrvProc, (void**)ppTransaction);

	if (SUCCEED == nODSRetVal)
		nRetVal = XP_NOERROR;
	return nRetVal;
}
*/

VARENUM MapSQLTypeToVariantType(BYTE SqlType);

//----------------------------------------------------------------------------
// NAME: CXpBase
//
// PURPOSE: XP base class
//
//----------------------------------------------------------------------------
class CXpBase
{
private:

protected:
	SRV_PROC* m_srvproc;

public:
	// constructors & destructors
	CXpBase(SRV_PROC* srvproc) : m_srvproc(srvproc) {};
	virtual ~CXpBase()	{if (m_srvproc) m_srvproc = NULL;};

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	VOID CXpBase::SendMsg(LPTSTR lpszMsg, ...)
	{
		if (NULL == lpszMsg)
			return;

		size_t	cLen = _tcslen(lpszMsg);
		size_t	bLen = cLen * sizeof(TCHAR);

		LPTSTR lpMsg = new TCHAR[cLen + 2];
		if (NULL == lpMsg)
		{
			return;
		}

		if (srv_bzero(lpMsg, (long) bLen) == FAIL)
		{
			goto cleanup;
		}

		_tcscpy(lpMsg, lpszMsg);

#ifdef _DEBUG
		OutputDebugString(lpMsg);
		OutputDebugString(TEXT("\n"));
#endif

		srvsendmsg(m_srvproc, SRV_MSG_INFO, 0, 0, (DBTINYINT)0, NULL, 0, 0, (LPTSTR)lpMsg, (int) bLen);

cleanup:

		if (lpMsg)
		{
			delete [] lpMsg;
			lpMsg = NULL;
		}

	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	VOID CXpBase::SendMsg(UINT uID, ...)
	{
		va_list	args;

		TCHAR szMsg[MAX_MSG + 1];
		TCHAR szBuf[MAX_MSG + 1];

		int pString = LoadString(
			g_hResDLL,				// handle of module containing string resource
			uID,					// resource identifier
			szBuf,					// address of buffer for resource
			MAX_MSG);				// size of buffer

		if (pString == 0)
		{
#ifdef _DEBUG
			DWORD dwErr = GetLastError();
#endif
			return;
		}

		va_start(args, uID);

		_vsntprintf(szMsg, (sizeof(szMsg) / sizeof(szMsg[0])), szBuf, args);

		va_end(args);

#ifdef _DEBUG
		OutputDebugString(szMsg);
		OutputDebugString(TEXT("\n"));
#endif

		srvsendmsg(m_srvproc, SRV_MSG_INFO, 0, 0, (DBTINYINT)0, NULL, 0, 0, 
			(LPTSTR)szMsg, SRV_NULLTERM);
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	VOID CXpBase::SendError(LPTSTR lpError, ...)
	{

		va_list	args;
		va_start(args, lpError);

		TCHAR szMsg[MAX_MSG + 1];

		if (NULL != lpError)
		{
			_vsntprintf(szMsg, (sizeof(szMsg) / sizeof(szMsg[0])), lpError, args);
		}

		va_end(args);

#ifdef _DEBUG
		OutputDebugString(szMsg);
		OutputDebugString(TEXT("\n"));
#endif

		srvsendmsg(m_srvproc, SRV_MSG_ERROR, 0, SRV_ENO_OS_ERR, (DBTINYINT)0, NULL, 0, 0, 
			(LPTSTR)szMsg, SRV_NULLTERM);
	}


	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	VOID CXpBase::SendError(UINT uID, ...)
	{
		va_list	args;

		TCHAR szMsg[MAX_MSG + 1];
		TCHAR szBuf[MAX_MSG + 1];

		int pString = LoadString(
			g_hResDLL,				// handle of module containing string resource
			uID,					// resource identifier
			szBuf,					// address of buffer for resource
			MAX_MSG);				// size of buffer

		if (pString == 0)
		{
#ifdef _DEBUG
			DWORD dwErr = GetLastError();
#endif
			return;
		}

		va_start(args, uID);

		_vsntprintf(szMsg, (sizeof(szMsg) / sizeof(szMsg[0])), szBuf, args);

		va_end(args);

#ifdef _DEBUG
		OutputDebugString(szMsg);
		OutputDebugString(TEXT("\n"));
#endif

		srvsendmsg(m_srvproc, SRV_MSG_ERROR, 0, SRV_ENO_OS_ERR, (DBTINYINT)0, NULL, 0, 0, 
			(LPTSTR)szMsg, SRV_NULLTERM);
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	VOID CXpBase::SendError(HRESULT hr)
	{
		DWORD	rc;
		LPTSTR	lpszErr	= NULL;

		if (! hr)
		{
			hr = GetLastError();
		}

		if (hr)
		{
			rc = FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_ARGUMENT_ARRAY,
				NULL,
				hr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpszErr,
				0,
				NULL );

#ifdef _DEBUG
			OutputDebugString(lpszErr);
			OutputDebugString(TEXT("\n"));
#endif

			srvsendmsg(m_srvproc, SRV_MSG_ERROR, hr, SRV_ENO_OS_ERR, (DBTINYINT)0, NULL, 0, 0, 
				(LPTSTR)lpszErr, SRV_NULLTERM);

			if (lpszErr)
			{
				LocalFree((HLOCAL)lpszErr);
			}
		}


	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	DWORD CXpBase::SendError(DWORD dwErr)
	{
		DWORD	rc;
		LPTSTR	lpszErr	= NULL;

		if (! dwErr)
		{
			dwErr = GetLastError();
		}

		if (dwErr)
		{
			rc = FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_ARGUMENT_ARRAY,
				NULL,
				dwErr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpszErr,
				0,
				NULL );

#ifdef _DEBUG
			OutputDebugString(lpszErr);
			OutputDebugString(_T("\n"));
#endif

			srvsendmsg(m_srvproc, SRV_MSG_ERROR, dwErr, SRV_ENO_OS_ERR, (DBTINYINT)0, NULL, 0, 0, 
				(LPTSTR)lpszErr, SRV_NULLTERM);

			if (lpszErr)
			{
				LocalFree((HLOCAL)lpszErr);
			}
		}

		return dwErr;
	}

}; // CXpBase

//----------------------------------------------------------------------------
// NAME: CXpProperty
//
// PURPOSE: XP properties class
//
//----------------------------------------------------------------------------
class CXpProperty : public CXpBase
{
private:

protected:

public:
	// constructors & destructors
	CXpProperty(SRV_PROC* srvproc) : CXpBase(srvproc){};
	// virtual ~CXp();

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpProperty::Username(LPTSTR lpUsername)
	{
		// parameter safety checks
		//
		assert(NULL != lpUsername);

		if (NULL == lpUsername)
		{
			return (FALSE);
		}

		// if trusted, username is blank
		//
		if (this->IsTrusted() )
		{
			_tcscpy(lpUsername, _T(""));
			return (TRUE);
		}

		int nLen = 0;

#ifdef _UNICODE
		_tcscpy(lpUsername, (LPTSTR)srv_pfield(m_srvproc, SRV_UNICODE_USER, &nLen));
#else // _UNICODE
		_tcscpy(lpUsername, srv_pfield(m_srvproc, SRV_USER, &nLen));
#endif // _UNICODE

		if (nLen)
		{
			lpUsername[nLen / sizeof(TCHAR)] = 0;
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	CString CXpProperty::Username()
	{
		CString username(_T(""));

		// if trusted, username is blank
		//
		if (this->IsTrusted() )
		{
			return username;
		}

		username.GetBufferSetLength(SYSNAME);

		int nLen = 0;

#ifdef _UNICODE
		username = (LPWSTR)srv_pfield(m_srvproc, SRV_UNICODE_USER, &nLen);
#else // _UNICODE
		username = (LPSTR) srv_pfield(m_srvproc, SRV_USER, &nLen));
#endif // _UNICODE

		if (nLen)
		{
			return username;
		}
		else
		{
			return CString(_T(""));
		}
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpProperty::Password(LPTSTR lpPassword)
	{
		// parameter safety checks
		//
		assert(NULL != lpPassword);

		if (NULL == lpPassword)
		{
			return (FALSE);
		}

		// if trusted, username is blank
		//
		if (this->IsTrusted() )
		{
			_tcscpy(lpPassword, _T(""));
			return (TRUE);
		}

		int nLen = 0;

#ifdef _UNICODE
		_tcscpy(lpPassword, (LPTSTR)srv_pfield(m_srvproc, SRV_UNICODE_PWD, &nLen));
#else // _UNICODE
		_tcscpy(lpPassword, srv_pfield(m_srvproc, SRV_USER, &nLen));
#endif // _UNICODE

		if (nLen)
		{
			lpPassword[nLen / sizeof(TCHAR)] = 0;
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	CString CXpProperty::Password()
	{
		CString password(_T(""));

		// if trusted, username is blank
		//
		if (this->IsTrusted() )
		{
			return password;
		}

		int nLen = 0;

#ifdef _UNICODE
		password = (LPWSTR)srv_pfield(m_srvproc, SRV_UNICODE_PWD, &nLen);
#else // _UNICODE
		password = (LPSTR) srv_pfield(m_srvproc, SRV_USER, &nLen);
#endif // _UNICODE

		if (nLen)
		{
			return password;
		}
		else
		{
			return CString(_T(""));
		}
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpProperty::IsSysAdmin()
	{
		if (strcmp(srv_pfield(m_srvproc, SRV_SAXP, (int *)NULL), "TRUE") == 0) 
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	//----------------------------------------------------------------------------
	// NAME: CXpProperty::IsTrusted
	//  
	// PURPOSE: Is connecttion established using integrated security
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpProperty::IsTrusted()
	{
		if (strcmp(srv_pfield(m_srvproc, SRV_LSECURE, (int *)NULL), "TRUE") == 0) 
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	//----------------------------------------------------------------------------
	// NAME: CXpProperty::Impersonate()
	//  
	// PURPOSE: impersonate connection
	//  
	// RETURNS: VOID
	//  
	// NOTES:
	//  
	BOOL CXpProperty::Impersonate()
	{
		return srv_impersonate_client(m_srvproc);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpProperty::Revert()
	//  
	// PURPOSE: Revert security context back  
	//
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpProperty::Revert()
	{
		return srv_revert_to_self(m_srvproc);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpProperty::Instance()
	//  
	// PURPOSE: return Instance name (SQL Server 2000 only)
	//
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpProperty::Instance(LPTSTR lpInstance)
	{
#ifdef SQL70
		return (FALSE);
#endif // SQL70

#ifdef SQL80
		// parameter safety checks
		//
		assert(NULL != lpInstance);

		if (NULL == lpInstance)
		{
			return (FALSE);
		}

		// instance name is always WCHAR*
		//
		WCHAR szInstance[64 + 1];
		int nLen = 0;

		wcscpy(szInstance, (LPTSTR)srv_pfieldex(m_srvproc, SRV_INSTANCENAME, &nLen));

		if (nLen)
		{
#ifdef _UNICODE
			wcsncpy(lpInstance, szInstance, nLen / sizeof(WCHAR));
			lpInstance[nLen / sizeof(WCHAR)] = 0;
#else // _UNICODE
			// convert to ANSI first
#endif // _UNICODE
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
#endif // SQL80
	}

	//----------------------------------------------------------------------------
	// NAME: CXpProperty::Instance()
	//  
	// PURPOSE: return Instance name (SQL Server 2000 only)
	//
	// RETURNS: CString
	//  
	// NOTES:
	//  
	CString CXpProperty::Instance()
	{
#ifdef SQL70
		return CString(_T(""));
#endif // SQL70

#ifdef SQL80

		CString instance(_T(""));
		instance.GetBufferSetLength(MAX_INSTANCE_NAME + 1);

		int nLen = 0;
		instance = (LPTSTR) srv_pfieldex(m_srvproc, SRV_INSTANCENAME, &nLen);

		if (nLen)
		{
			return instance;
		}
		else
		{
			return CString(_T(""));
		}
#endif // SQL80
	}

	//----------------------------------------------------------------------------
	// NAME: CXpProperty::Servername()
	//  
	// PURPOSE: return server name
	//
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpProperty::Servername(LPTSTR lpServername)
	{
		// parameter safety checks
		//
		assert(NULL != lpServername);

		if (NULL == lpServername)
		{
			return (FALSE);
		}

		// ToDo Is always returnining ANSI
		// needs to get fixed
		//
		CHAR szServername[MAX_COMPUTERNAME_LENGTH + 1];
		int nLen = 0;
		strcpy(szServername, (LPSTR)srv_pfield(m_srvproc, SRV_RMTSERVER, &nLen) );

		if (nLen)
		{

#ifdef _UNICODE
			MultiByteToWideChar(CP_ACP, 0, szServername, -1, lpServername, nLen * sizeof(WCHAR) );
#else  // ANSI
			strncpy(lpServername, szServername, (nLen / sizeof(CHAR)));
#endif

			return (TRUE);
		}
		else
		{
			return (FALSE);
		}

	}

	//----------------------------------------------------------------------------
	// NAME: CXpProperty::Servername()
	//  
	// PURPOSE: return server name
	//
	// RETURNS: CString
	//  
	// NOTES:
	//  
	CString CXpProperty::Servername()
	{
		CString servername(_T(""));
		servername.GetBufferSetLength(MAX_COMPUTERNAME_LENGTH + 1);

		int nLen = 0;
		servername = (LPSTR) srv_pfield(m_srvproc, SRV_RMTSERVER, &nLen);

		if (nLen)
		{
			return servername;
		}
		else
		{
			return CString(_T(""));
		}
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpProperty::ServerVersion(LPDWORD dwMajor, LPDWORD dwMinor, LPDWORD dwBuild)
	{
		return (TRUE);
	}


	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpProperty::Codepage(ULONG* ulCodePage)
	{
		CHAR*	szServerCodePage;

		szServerCodePage = srv_pfield(m_srvproc, SRV_SPROC_CODEPAGE, NULL);    
		if (NULL != szServerCodePage)
		{
			*ulCodePage = atol(szServerCodePage);
		}
		else 
		{
			return (FALSE);
		}

		return (TRUE);
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpProperty::LCID(ULONG* ulLCID)
	{
#ifdef SQL70
		return (FALSE);
#endif // SQL70

#ifdef SQL80
		ulLCID = (ULONG*) srv_pfieldex(m_srvproc, SRV_MSGLCID, NULL);
		return (TRUE);
#endif // SQL80
	}

}; // CXpProperty

//----------------------------------------------------------------------------
// NAME: CXpParameter
//
// PURPOSE: XP Parameter class
//
//----------------------------------------------------------------------------
class CXpParameter : public CXpBase
{
private:

protected:

public:
	// constructors & destructors
	CXpParameter(SRV_PROC* srvproc) : CXpBase(srvproc) {};
	// virtual ~CXpParameter() : ~CXpBase() {};

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::NumParams
	//  
	// PURPOSE: return number of parameters
	//  
	// RETURNS: INT
	//  
	// NOTES:
	//  
	INT CXpParameter::NumParams()
	{
		return (srv_rpcparams(m_srvproc));
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::IsType
	//  
	// PURPOSE: Is parameter of type bParamType
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsType(INT nParam, BYTE bParamType)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			return (FALSE);
		}

		TRACE_PARAM(bType);

		if (bType == bParamType)
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	BYTE CXpParameter::GetType(INT nParam)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			return (0);
		}

		TRACE_PARAM(bType);

		return (bType);
	}
	//----------------------------------------------------------------------------
	// NAME: CXpParameter::IsInteger
	//  
	// PURPOSE: Is parameter of type integer (tinyint, smallint, int(eger), bigint)
	//  
	// RETURNS: VOID
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsInteger(INT nParam)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			return (FALSE);
		}

		TRACE_PARAM(bType);

		if (IS_INT(bType))
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::
	//  
	// PURPOSE: 
	//  
	// RETURNS: VOID
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsString(INT nParam)
	{
		RETCODE	rc;
		BYTE	bType;
		ULONG	cbMaxLen;
		ULONG	cbActualLen;
		BOOL	fNull;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			return (FALSE);
		}

		TRACE_PARAM(bType);

		if (IS_STRING(bType))
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::IsUnicode
	//  
	// PURPOSE: 
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsUnicode(INT nParam)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			return (FALSE);
		}

		TRACE_PARAM(bType);

		if (IS_UNICODE(bType))
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::IsANSI
	//  
	// PURPOSE: 
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsANSI(INT nParam)
	{
		return (! ( this->IsUnicode(nParam) ) );
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::IsVariant
	//  
	// PURPOSE: 
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsVariant(INT nParam)
	{
#ifdef SQL70
		return (FALSE);
#endif // SQL70

#ifdef SQL80
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		assert(rc == SUCCEED);

		if (FAIL == rc)
		{
			return (FALSE);
		}

		TRACE_PARAM(bType);

		// Is SQL Server VARIANT
		//
		if (IS_VARIANT(bType))
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
#endif // SQL80
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::IsGuid
	//  
	// PURPOSE: 
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsGuid(INT nParam)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		assert(rc == SUCCEED);

		if (FAIL == rc)
		{
			return (FALSE);
		}

		TRACE_PARAM(bType);

		// Is SQL Server GUID (UniqueIdentifier)
		//
		if (IS_GUID(bType))
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::IsNULL
	//  
	// PURPOSE: 
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsNULL(INT nParam)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		assert(rc == SUCCEED);

		return (fNull);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::IsBlob
	//  
	// PURPOSE: 
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsBlob(INT nParam)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		assert(rc == SUCCEED);

		if (FAIL == rc)
		{
			return (FALSE);
		}

		TRACE_PARAM(bType);

		// is this a blob
		//
		if (IS_BLOB(bType))
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParamName
	//  
	// PURPOSE: return parameter name if named
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParamName(INT nParam, LPTSTR lpParamName)
	{
#ifdef _UNICODE
		RETCODE	rc;

		UINT	iServerCodePage;
		CHAR*	szServerCodePage;

		// assert(wcslen(lpParamName) >= SYSNAME);

		szServerCodePage = srv_pfield(m_srvproc, SRV_SPROC_CODEPAGE, NULL);    
		if (NULL != szServerCodePage)
		{
			iServerCodePage = atol(szServerCodePage);
		}
		else 
		{
			return (FALSE);
		}

		CHAR	szParamName[SYSNAME];
		INT		nByteLen;

		strcpy(szParamName, srv_paramname(m_srvproc, nParam, &nByteLen));
		assert(nByteLen < (SYSNAME * sizeof(CHAR))); // buffer too small

		WCHAR wszParamName[SYSNAME];

		rc = MultiByteToWideChar(
			iServerCodePage,			// code page
			0,							// character-type options
			szParamName,				// address of string to map
			-1,							// number of bytes in string (-1 NULL terminated)
			wszParamName,				// address of wide-character buffer
			SYSNAME);					// size of buffer

		wcscpy(lpParamName, wszParamName);

		if (0 == rc)
			return (FALSE);
		else
			return (TRUE);
#else // _UNICODE
		// assert(strlen(lpParamName) >= SYSNAME);

		//	CHAR	szParamName[SYSNAME];
		INT		nByteLen;

		strcpy(lpParamName, srv_paramname(m_srvproc, nParam, &nByteLen));
		assert(nByteLen < SYSNAME); // buffer too small

		return (nByteLen < SYSNAME);

#endif // _UNICODE
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParamName
	//  
	// PURPOSE: return parameter name if named
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParamName(INT nParam, _bstr_t& bstrParamName)
	{
		RETCODE	rc;

		UINT	iServerCodePage;
		CHAR*	szServerCodePage;

		szServerCodePage = srv_pfield(m_srvproc, SRV_SPROC_CODEPAGE, NULL);    
		if (NULL != szServerCodePage)
		{
			iServerCodePage = atol(szServerCodePage);
		}
		else 
		{
			return (FALSE);
		}

		CHAR	szParamName[SYSNAME + 1];
		INT		nByteLen;

		strcpy(szParamName, srv_paramname(m_srvproc, nParam, &nByteLen));
		assert(nByteLen < (SYSNAME * sizeof(CHAR))); // buffer too small

		WCHAR wszParamName[SYSNAME];

		rc = MultiByteToWideChar(
			iServerCodePage,			// code page
			0,							// character-type options
			szParamName,				// address of string to map
			-1,							// number of bytes in string (-1 NULL terminated)
			wszParamName,				// address of wide-character buffer
			SYSNAME);					// size of buffer in WCHAR characters

		bstrParamName = wszParamName;

		if (0 == rc)
			return (FALSE);
		else
			return (TRUE);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParam
	//  
	// PURPOSE: return LONG parameter by ordinal
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParam(INT nParam, LONG* pLong)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, (BYTE*)pLong, &fNull);

#ifdef _DEBUG
		assert(cbActualLen <= cbMaxLen);
		assert(IS_INT(bType));
#endif

		TRACE_PARAM(bType);

		return (rc == SUCCEED);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParam
	//  
	// PURPOSE: return BOOL parameter by ordinal
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParam(INT nParam, BOOL* pBool)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, (BYTE*)pBool, &fNull);

#ifdef _DEBUG
		assert(cbActualLen <= cbMaxLen);
		assert(IS_INT(bType));
#endif

		TRACE_PARAM(bType);

		return (rc == SUCCEED);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParamName
	//  
	// PURPOSE: return CHAR* parameter by ordinal
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParam(INT nParam, CHAR* pString)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		size_t		cbMaxLen	= 0;
		size_t		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		assert(NULL != pString);

		if (NULL == pString)
		{
			return (XP_ERROR);
		}

		cbMaxLen = (strlen(pString) * sizeof(CHAR));

		rc = srv_paraminfo(m_srvproc, nParam, &bType, (ULONG*) &cbMaxLen, (ULONG*) &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			*pString = 0x00;
			return (XP_ERROR);
		}

#ifdef _DEBUG
		assert(cbActualLen <= cbMaxLen);
		assert(IS_STRING(bType));
#endif

		TRACE_PARAM(bType);

		if (IS_UNICODE(bType))
		{

		}
		else
		{
			rc = srv_paraminfo(m_srvproc, nParam, &bType, (ULONG*) &cbMaxLen, (ULONG*) &cbActualLen, (BYTE*) pString, &fNull);
			*(pString + cbActualLen) = 0x00;
		}

		return (rc == SUCCEED);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParam
	//  
	// PURPOSE: return WCHAR* parameter by ordinal
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParam(INT nParam, WCHAR* pString)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		size_t		cbMaxLen	= 0;
		size_t		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		assert(NULL != pString);

		if (NULL == pString)
		{
			return (XP_ERROR);
		}

		cbMaxLen = (wcslen(pString) * sizeof(WCHAR));

		rc = srv_paraminfo(m_srvproc, nParam, &bType, (ULONG*) &cbMaxLen, (ULONG*) &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			*pString = 0x00;
			return (XP_ERROR);
		}

#ifdef _DEBUG
		assert(cbActualLen <= cbMaxLen);
		assert(IS_STRING(bType));
#endif

		TRACE_PARAM(bType);

		if (IS_UNICODE(bType))
		{
			rc = srv_paraminfo(m_srvproc, nParam, &bType, (ULONG*) &cbMaxLen, (ULONG*) &cbActualLen, (BYTE*) pString, &fNull);
			*(pString + (cbActualLen / sizeof(WCHAR))) = 0x00;
		}
		else
		{
			rc = srv_paraminfo(m_srvproc, nParam, &bType, (ULONG*) &cbMaxLen, (ULONG*) &cbActualLen, (BYTE*) pString, &fNull);
			*(pString + (cbActualLen / sizeof(CHAR))) = 0x0;
			Ansi2Unicode( (LPSTR) pString);
		}

		return (rc == SUCCEED);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParam
	//  
	// PURPOSE: return _bstr_t parameter by ordinal
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParam(INT nParam, _bstr_t& pString)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		// get param info, without data
		//
		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			pString = _T("");
			return (XP_ERROR);
		}

#ifdef _DEBUG
		assert(cbActualLen <= cbMaxLen);
		assert(IS_STRING(bType));
#endif

		TRACE_PARAM(bType);

		if (IS_UNICODE(bType))
		{
			LPWSTR lpwStr = new WCHAR[(cbActualLen / sizeof(WCHAR)) + 1];
			if (NULL == lpwStr)
			{
				pString = _T("");
				// error
			}
			else
			{
				rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, (BYTE*)lpwStr, &fNull);
				*(lpwStr + (cbActualLen / sizeof(WCHAR))) = 0x00;
				pString = lpwStr;
			}
			delete [] lpwStr;
			lpwStr = NULL;
		}
		else
		{
			LPSTR lpStr = new CHAR[((cbActualLen / sizeof(CHAR)) + 1) * 2];
			if (NULL == lpStr)
			{
				pString = _T("");
				// error
			}
			else
			{
				rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, (BYTE*)lpStr, &fNull);
				*(lpStr + (cbActualLen / sizeof(CHAR))) = 0x0;
				Ansi2Unicode( (LPSTR) lpStr);
				pString = (LPWSTR) lpStr;
			}
			delete [] lpStr;
			lpStr = NULL;
		}

		return (rc == SUCCEED);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParam
	//  
	// PURPOSE: return _variant_t parameter by ordinal
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParam(INT nParam, _variant_t& pVariant)
	{
		RETCODE		rc			= FAIL;
		BYTE		bType		= 0;
		ULONG		cbMaxLen	= 0;
		ULONG		cbActualLen	= 0;
		BOOL		fNull		= FALSE;

		// get param info, without data
		//
		rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			return (FALSE);
		}

#ifdef _DEBUG
		assert(cbActualLen <= cbMaxLen);
		assert(IS_STRING(bType));
#endif

		TRACE_PARAM(bType);

		VARTYPE vartype = MapSQLTypeToVariantType(bType);

		pVariant.ChangeType(vartype);

		if (IS_UNICODE(bType))
		{
			LPWSTR lpwStr = new WCHAR[(cbActualLen / sizeof(WCHAR)) + 1];
			if (NULL == lpwStr)
			{
				// pString = _T("");
				// error
			}
			else
			{
				rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, (BYTE*)lpwStr, &fNull);
				*(lpwStr + (cbActualLen / sizeof(WCHAR))) = 0x00;
				// pString = lpwStr;
			}
			delete [] lpwStr;
			lpwStr = NULL;
		}
		else
		{
			LPSTR lpStr = new CHAR[((cbActualLen / sizeof(CHAR)) + 1) * 2];
			if (NULL == lpStr)
			{
				// pString = _T("");
				// error
			}
			else
			{
				rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, (BYTE*)lpStr, &fNull);
				*(lpStr + (cbActualLen / sizeof(CHAR))) = 0x0;
				Ansi2Unicode( (LPSTR) lpStr);
				// pString = (LPWSTR) lpStr;
			}
			delete [] lpStr;
			lpStr = NULL;
		}

		return (rc == SUCCEED);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParam
	//  
	// PURPOSE: return LONG* parameter by name
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParam(LPCSTR lpszParam, LONG* pLong)
	{
		return (TRUE);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParam
	//  
	// PURPOSE: return CHAR* parameter by name
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParam(LPCSTR lpszParam, CHAR* pString)
	{
		return (TRUE);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::GetParam
	//  
	// PURPOSE: return WCHAR* parameter by name
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::GetParam(LPCSTR lpszParam, WCHAR* pString)
	{
		return (TRUE);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::SetParam
	//  
	// PURPOSE: set output parameter LONG
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::SetParam(INT nParam, LONG lLong)
	{
#ifdef _DEBUG
		// In debug mode, make sure this really is an output parameter
		// and it is an integer column
		//
		assert(IsOutputParam(nParam));
		assert(IsInteger(nParam));
#endif

		RETCODE rc = srv_paramsetoutput(m_srvproc, nParam, (BYTE*)&lLong, sizeof(LONG), FALSE);
		return (SUCCEED == rc);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::SetParam
	//  
	// PURPOSE: set output parameter CHAR*
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::SetParam(INT nParam, const CHAR* pString)
	{
		return (TRUE);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::SetParam
	//  
	// PURPOSE: set output parameter WCHAR*
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::SetParam(INT nParam, const WCHAR* pString)
	{
		return (TRUE);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::SetParam
	//  
	// PURPOSE: set output parameter GUID
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::SetParam(INT nParam, const GUID* pGUID)
	{
#ifdef _DEBUG
		// In debug mode, make sure this really is an output parameter
		// and it is an integer column
		//
		assert(IsOutputParam(nParam));
		assert(IsGuid(nParam));
#endif

		RETCODE rc = srv_paramsetoutput(m_srvproc, nParam, (BYTE*)pGUID, sizeof(GUID), FALSE);
		return (SUCCEED == rc);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::IsOutputParam
	//  
	// PURPOSE: return if parameter is an output parameter
	//  
	// RETURNS: BOOL
	//  
	// NOTES:
	//  
	BOOL CXpParameter::IsOutputParam(INT nParam)
	{
		return (srv_paramstatus(m_srvproc, nParam) & SRV_PARAMRETURN);

		/*
		if (srv_paramstatus(m_srvproc, nParam) == 1)
		{
		return (TRUE);
		}
		else
		{
		return (FALSE);
		}
		*/
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::ByteLen
	//  
	// PURPOSE: Return byte len of parameter
	//  
	// RETURNS: VOID
	//  
	// NOTES:
	//  
	ULONG CXpParameter::ByteLen(INT nParam)
	{
		BYTE	bType;
		ULONG	cbMaxLen = 0;
		ULONG	cbActualLen = 0;
		BYTE	bData = 0;
		BOOL	fNull;

		RETCODE rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			return (0);
		}

		return (cbActualLen);
	}

	//----------------------------------------------------------------------------
	// NAME: CXpParameter::CharLen
	//  
	// PURPOSE: Return character len of parameter (TCHAR)
	//  
	// RETURNS: VOID
	//  
	// NOTES:
	//  
	ULONG CXpParameter::CharLen(INT nParam)
	{
		BYTE	bType;
		ULONG	cbMaxLen = 0;
		ULONG	cbActualLen = 0;
		BYTE	bData = 0;
		BOOL	fNull;

		RETCODE rc = srv_paraminfo(m_srvproc, nParam, &bType, &cbMaxLen, &cbActualLen, NULL, &fNull);
		if (FAIL == rc)
		{
			return (0);
		}

		return (cbActualLen);
	}

}; // CXpParameter

//----------------------------------------------------------------------------
// NAME: CXpResultset
//
// PURPOSE: XP resultset class
//
//----------------------------------------------------------------------------
class CXpResultset : public CXpBase
{
private:
	INT		m_nColumns;
	LONG	m_nRows;

	struct XPCOLUMN
	{
		TCHAR			szColumn[SYSNAME];
		BYTE			bType;
		BYTE*			pData;
	};
	
	// pointer to array of columns
	XPCOLUMN*	m_pColumns;	

protected:

public:
	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	CXpResultset::CXpResultset(SRV_PROC* srvproc, INT nColumns) 
		:	CXpBase(srvproc),
		m_nColumns(nColumns),
		m_nRows(0),
		m_pColumns(NULL)
	{
		// create column structure
		//
		m_pColumns = new XPCOLUMN[m_nColumns];
		if (NULL == m_pColumns)
		{
			// throw();
		}

		// init column structure
		//
		for (INT i = 0; i < m_nColumns; i++)
		{
			m_pColumns->bType = 0;
			m_pColumns->pData = NULL;
			m_pColumns->szColumn[0] = 0x00;
		}
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	CXpResultset::~CXpResultset() 
	{
		SAFE_DELETE(m_pColumns);
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpResultset::Describe(INT nColumn, LPTSTR pszColumnName, 
		BYTE bSrcType, LONG nSrcByteLen, 
		BYTE bDstType, LONG nDstByteLen, PBYTE pData)
	{
		assert(_tcslen(pszColumnName) <= MAX_COLUMN);

		RETCODE rc = srvdescribe(
			m_srvproc, 
			nColumn, 
			pszColumnName, 
			SRV_NULLTERM, 
			bSrcType, 
			nSrcByteLen,
			bDstType,
			nDstByteLen,
			(BYTE*)pData);

		// wrong column order
		//
		assert(nColumn == rc);

		return (SUCCEED == rc);
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpResultset::SendRow()
	{
		RETCODE rc = srv_sendrow(m_srvproc);
		if (SUCCEED == rc)
		{
			++m_nRows;
		}
		return (SUCCEED == rc);
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpResultset::SendDone()
	{
		// send rowcount and done
		//
		RETCODE rc = srv_senddone(m_srvproc, (SRV_DONE_COUNT | SRV_DONE_MORE), 0, m_nRows);

		// reset m_Rows to zero, since we communicated the number of rows
		m_nRows = 0;

		return (SUCCEED == rc);
	}

	//----------------------------------------------------------------------------
	// NAME: 
	//  
	// PURPOSE: 
	//  
	// RETURNS: 
	//  
	// NOTES: 
	//  
	BOOL CXpResultset::Reset()
	{
		SAFE_DELETE(m_pColumns);
		m_nColumns = 0;
		m_nRows = 0;

		return (TRUE);
	}

}; // CXpResultset


typedef struct xp_param_ex
{
	int			iOrdinal;
	BYTE		bType;
	ULONG		cbMaxLen;
	ULONG		cbActualLen;
	BOOL		fNull;
	BOOL		fOutput;
	TCHAR		szName[SYSNAME + 1];
} XP_PARAM_EX;

// internal functions
RETCODE getparam_ex(SRV_PROC* srvproc, INT ordinal, XP_PARAM_EX* param);
RETCODE getparam_data_ex(SRV_PROC* srvproc, INT ordinal, XP_PARAM_EX* param, PBYTE data);

RETCODE setparam(SRV_PROC* srvproc, INT ordinal, const LONG value);
RETCODE setparam(SRV_PROC* srvproc, INT ordinal, const GUID* pValue);
RETCODE setparam(SRV_PROC* srvproc, INT ordinal, const CHAR* pValue);
#endif // _XP_HELPER_H