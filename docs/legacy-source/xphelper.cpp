#include "xphelper.h"

ULONG __GetXpVersion()
{
	return (ODS_VERSION);
}

VOID trace_param_type(BYTE bType)
{
	CHAR*		pdbcDataType;
	int			cbDataType;
    
    pdbcDataType = srv_symbol(SRV_DATATYPE, (int)bType, &cbDataType);
	OutputDebugStringA(pdbcDataType);
	OutputDebugStringA("\n");
}

int srv_describeW(SRV_PROC* srvproc,int colnumber, WCHAR* column_name, int namelen, long int desttype,
				   long int destlen, long int srctype, long int srclen ,void* srcdata) 
{
	RETCODE	rc;
    UINT	serverCodePage;
    CHAR*	szServerCodePage;
	LPSTR	_column_name = NULL;
	size_t	_namelen;

	szServerCodePage = srv_pfield(srvproc, SRV_SPROC_CODEPAGE, NULL);    
    if (NULL != szServerCodePage)
	{
		serverCodePage = atol(szServerCodePage);
	}
	else 
	{
		srv_senddone(srvproc, (SRV_DONE_ERROR | SRV_DONE_MORE), 0, 0);
		return 1;
	}

	if (column_name)
	{
		_namelen	 = wcslen(column_name);
		_column_name = new CHAR[_namelen];
		if (NULL != _column_name)
		{
			rc = WideCharToMultiByte(
				serverCodePage,                 // code page
				0,                              // default
				column_name,					// wide-character string
				-1,                             // string is null terminated
				_column_name,					// address of buffer for new string
				(int) _namelen,					// size of buffer in bytes
				NULL, 
				NULL);
		}
	}

	rc = srv_describe(srvproc, colnumber, _column_name, (int) _namelen, desttype, 
		(long) destlen, srctype, (long) srclen, srcdata);

	SAFE_DELETE(_column_name);

	return (rc);
}

int srv_sendmsgW(SRV_PROC* srvproc, int msgtype, DBINT msgnum, DBTINYINT msgclass, 
				  BYTE state, WCHAR* rpcname, int rpcnamelen, USHORT linenum, WCHAR* message, int msglen)
{
	RETCODE	rc					= FAIL;
    UINT	serverCodePage		= 0;
    CHAR*	szServerCodePage	= NULL;
	LPSTR	_rpcname			= NULL;
	LPSTR	_message			= NULL;
	size_t	_rpcnamelen			= 0;
	size_t	_msglen				= 0;

	szServerCodePage = srv_pfield(srvproc, SRV_SPROC_CODEPAGE, NULL);    
    if (NULL != szServerCodePage)
	{
		serverCodePage = atol(szServerCodePage);
	}
	else 
	{
		srv_senddone(srvproc, (SRV_DONE_ERROR | SRV_DONE_MORE), 0, 0);
		return FAIL;
	}

	if (rpcname)
	{
		_rpcnamelen = wcslen(rpcname);
		_rpcname = new CHAR[_rpcnamelen];
		if (_rpcname)
		{
			rc = WideCharToMultiByte(
				serverCodePage,                 // code page
				0,                              // default
				rpcname,						// wide-character string
				-1,                             // string is null terminated
				_rpcname,						// address of buffer for new string
				(int) _rpcnamelen,				// size of buffer in bytes
				NULL, 
				NULL);
		}
	}

	if (message)
	{
		_msglen = wcslen(message);
		_message = new CHAR[_msglen];
		if (_message)
		{
			rc = WideCharToMultiByte(
				serverCodePage,                 // code page
				0,                              // default
				message,						// wide-character string
				-1,                             // string is null terminated
				_message,						// address of buffer for new string
				(int) _msglen,					// size of buffer in bytes
				NULL, 
				NULL);
		}
	}
	
	rc = srv_sendmsg(srvproc, msgtype, msgnum, msgclass, state, _rpcname, (int) _rpcnamelen, linenum, _message, (int) _msglen);
	
	SAFE_DELETE(_rpcname);
	SAFE_DELETE(_message);
	
	return (rc);
}

HGLOBAL XPAlloc(UINT uFlags, DWORD dwBytes)
{
	HGLOBAL	hrc=(HGLOBAL)malloc(dwBytes);

	if (hrc && (GMEM_ZEROINIT==(uFlags & GMEM_ZEROINIT)))
	{
		memset(hrc,0x00,dwBytes);
	}
	return(hrc);
}

HGLOBAL XPFree(HGLOBAL hMem)
{
	if (hMem)
	{
		free(hMem);
		hMem=NULL;
	}
	return(NULL);
}

BOOL Ansi2Unicode(LPSTR lpszmbcsSource)
{
	// effectively in-place conversion to Unicode

	LPWSTR	lpszwDest=NULL;
	size_t	icch;

	if (!lpszmbcsSource)
	{
		return(FALSE);
	}

	icch = strlen(lpszmbcsSource);

	if (icch == 0)
	{
		*lpszmbcsSource = 0x00;
		*(lpszmbcsSource + 1) = 0x00;
		return(TRUE);					// in effect the conversion succeeded
	}

	if (!(lpszwDest = (LPWSTR) XPAlloc(GPTR, (DWORD) (icch + 1) * sizeof(WCHAR))))
	{
		return(FALSE);
	}

	MultiByteToWideChar(CP_ACP, 0, lpszmbcsSource, -1, lpszwDest, (int) icch);
	wcscpy((LPWSTR)lpszmbcsSource, lpszwDest);

	if (lpszwDest)
	{
		XPFree((HGLOBAL)lpszwDest);
	}
	
	return (TRUE);
}

BOOL Unicode2Ansi(LPWSTR lpszwSource)
{
	// effectively in-place conversion to MBCS

	LPSTR	lpszmbcsDest=NULL;
	size_t	icch;

	if (!lpszwSource)
	{
		return(FALSE);
	}

	icch = wcslen(lpszwSource);

	if (icch == 0)
	{
		*lpszwSource=0x0000;
		return(1);				// in effect the conversion succeeded
	}

	if (!(lpszmbcsDest=(LPSTR)XPAlloc(GPTR, (DWORD) (icch+1)*sizeof(WCHAR))))
	{
		return(FALSE);
	}

	WideCharToMultiByte(CP_ACP, 0, lpszwSource, -1, lpszmbcsDest, (int) (icch * sizeof(WCHAR)), NULL, NULL);
	
	strcpy((LPSTR)lpszwSource, lpszmbcsDest);

	if (lpszmbcsDest)
	{
		XPFree((HGLOBAL)lpszmbcsDest);
	}
	
	return (TRUE);
}

VARENUM MapSQLTypeToVariantType(BYTE SqlType)
{
	switch (SqlType) 
	{
	case SRVNULL:
		return VT_NULL;
		break;

	case SRVINT1:
		return VT_I1;
		break;

	case SRVINT2:
		return VT_I2;
		break;

	case SRVINT4:
		return VT_I4;
		break;

#ifdef SQL80
	case SRVINT8:
		return VT_INT;
		break;
#endif

	case SRVINTN:
		return VT_INT;
		break;

	case SRVCHAR:
	case SRVBIGCHAR:
	case SRVVARCHAR:
	case SRVBIGVARCHAR:
	case SRVTEXT:
	case SRVNCHAR:
	case SRVNVARCHAR:
	case SRVNTEXT:
		return VT_BSTR;
		break;

	case SRVBINARY:
	case SRVBIGBINARY:
	case SRVVARBINARY:
	case SRVBIGVARBINARY:
	case SRVIMAGE:
		return VT_EMPTY;
		break;

	case SRVDATETIM4:
	case SRVDATETIME:
	case SRVDATETIMN:
		return VT_DATE;
		break;

	case SRVFLT4:
		return VT_R4;    
		break;

	case SRVFLT8:
		return VT_R8;
		break;

	case SRVFLTN:
		return VT_R8;
		break;

	case SRVMONEY4:
	case SRVMONEY:
	case SRVMONEYN:
		return VT_CY;
		break;

	case SRVDECIMAL:
	case SRVDECIMALN:
	case SRVNUMERIC:
	case SRVNUMERICN:
		return VT_DECIMAL;
		break;

	case SRVBIT:
	case SRVBITN:
		return VT_BOOL;
		break;

	case SRVGUID:
		return VT_BSTR;
		break;

#ifdef SQL80
	case SRVSSVARIANT:
		return VT_VARIANT;
		break;
#endif

	default:
		return VT_EMPTY;
		break;
    }
}


//----------------------------------------------------------------------------
// NAME: getparam_ex
//  
// PURPOSE: same as getparam but also retrieves the parameter name
//  
// RETURNS: 
//  
// NOTES:
//  
RETCODE getparam_ex(SRV_PROC* srvproc, INT ordinal, XP_PARAM_EX* param)
{
	RETCODE rc = FAIL;

	param->iOrdinal = ordinal;

	rc = srv_paraminfo(srvproc, ordinal, &param->bType, &param->cbMaxLen, &param->cbActualLen, NULL, &param->fNull);
	if (FAIL == rc)
	{
		return rc;
	}
	
	param->fOutput = (srv_paramstatus(srvproc, ordinal) & SRV_PARAMRETURN);

#ifdef _UNICODE

	UINT	iServerCodePage;
	CHAR*	szServerCodePage;

	szServerCodePage = srv_pfield(srvproc, SRV_SPROC_CODEPAGE, NULL);    
	if (NULL != szServerCodePage)
	{
        iServerCodePage = atol(szServerCodePage);
	}
	else
    {
		return rc;
	}

	CHAR	szParamName[SYSNAME + 1];
	INT		nByteLen;

	strncpy(szParamName, srv_paramname(srvproc, ordinal, &nByteLen), SYSNAME);

	rc = MultiByteToWideChar(
        iServerCodePage,			// code page
		0,							// character-type options
		szParamName,				// address of string to map
		-1,							// number of bytes in string (-1 NULL terminated)
		param->szName,				// address of wide-character buffer
		SYSNAME);					// size of buffer

#else // _UNICODE

	INT	nByteLen;
	strncpy(param->szName, srv_paramname(srvproc, ordinal, &nByteLen), SYSNAME);

#endif // _UNICODE
	rc = SUCCEED;

	return rc;
}


//----------------------------------------------------------------------------
// NAME: getparam_ex
//  
// PURPOSE: same as getparam but also retrieves the parameter name
//  
// RETURNS: 
//  
// NOTES:
//  
RETCODE getparam_data_ex(SRV_PROC* srvproc, INT ordinal, XP_PARAM_EX* param, PBYTE data)
{
	RETCODE rc = FAIL;

	param->iOrdinal = ordinal;
	if( NULL == data )
	{
		return FAIL;
	}

	rc = srv_paraminfo(srvproc, ordinal, &param->bType, &param->cbMaxLen, &param->cbActualLen, data, &param->fNull);
	if (FAIL == rc)
	{
		return rc;
	}
	
	return rc;
}

//----------------------------------------------------------------------------
// NAME: setparam
//  
// PURPOSE: set output parameter LONG
//  
// RETURNS: RETCODE
//  
// NOTES:
//  
RETCODE setparam(SRV_PROC* srvproc, INT ordinal, const LONG value)
{
	return srv_paramsetoutput(srvproc, ordinal, (BYTE*)&value, sizeof(LONG), FALSE);
}

//----------------------------------------------------------------------------
// NAME: setparam
//  
// PURPOSE: set output parameter GUID
//  
// RETURNS: RETCODE
//  
// NOTES:
RETCODE setparam(SRV_PROC* srvproc, INT ordinal, const GUID* pValue)
{
	return srv_paramsetoutput(srvproc, ordinal, (BYTE*)pValue, sizeof(GUID), FALSE);
}

//----------------------------------------------------------------------------
// NAME: setparam
//  
// PURPOSE: set output parameter ANSI STRING
//  
// RETURNS: RETCODE
//  
// NOTES:
//  
RETCODE setparam(SRV_PROC* srvproc, INT ordinal, const CHAR* pValue)
{
	size_t charlen = strlen(pValue);
	return srv_paramsetoutput(srvproc, ordinal, (BYTE*)pValue, (ULONG) charlen, (charlen == 0 ? TRUE : FALSE));
}

