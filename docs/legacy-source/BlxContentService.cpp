// BlxContentService.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "BlxContentService.h"

#include "ServerContext.h"
#include "ServerNetwork.h"

#include <stdio.h>

class CBlxContentServiceModule : public CAtlServiceModuleT< CBlxContentServiceModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_BlxContentServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BLXCONTENTSERVICE, "{F3B0C24F-E96D-4B3D-ACFB-07688AA167F8}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for 
		// your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.

		return S_OK;
	}
	HRESULT Run( int nShowCmd );
	void OnStop();

private:
	CServerNetwork*				m_pServerNetwork;

	BCS_SERVER_NETWORK_CONTEXT	m_ServerNetworkContext;
};

CBlxContentServiceModule _AtlModule;

HRESULT CBlxContentServiceModule::Run( int nShowCmd = SW_HIDE )
{
	HRESULT hr = S_OK;
	LONG res = 0;
	CRegKey regKeyOps;

	ULONG ulIpAddress = 0;
	WCHAR szIpAddress[ 255 ] = { L'\0' };

	DWORD dwPort = 0;
	DWORD dwMTU = 0;

	ULONG ulDNSTarget = 0;
	WCHAR szDNSTarget[ 255 ] = { L'\0' };

	DWORD dwReceiveNwBuffer = 0;
	DWORD dwReceiveTpMin = 0;
	DWORD dwReceiveTpMax = 0;

	DWORD dwSendNwBuffer = 0;
	DWORD dwSendTpMin = 0;
	DWORD dwSendTpMax = 0;

	ULONG ulManagementQName = 0;
	WCHAR szManagementQName[ 255 ] = { L'\0' };
	DWORD dwManagementQBuffer = 0;
	DWORD dwManagementQTpMin = 0;
	DWORD dwManagementQTpMax = 0;

	ULONG ulDbConnString = 0;
	WCHAR szDbConnString[ 255 ] = { L'\0' };

	if( regKeyOps.Open( HKEY_LOCAL_MACHINE, TEXT( "SOFTWARE\\Blxware, LLC\\BlxContentServer" ), KEY_READ ) == ERROR_SUCCESS )
	{

		res = regKeyOps.QueryStringValue( 
			TEXT( "IpAddress" )
			, NULL
			, &ulIpAddress
			);
		res = regKeyOps.QueryStringValue( 
			TEXT( "IpAddress" )
			, szIpAddress
			, &ulIpAddress
			);

		res = regKeyOps.QueryDWORDValue( 
			TEXT( "Port" )
			, dwPort
			);

		res = regKeyOps.QueryDWORDValue( 
			TEXT( "MTU" )
			, dwMTU
			);

		res = regKeyOps.QueryStringValue( 
			TEXT( "DNSTarget" )
			, NULL
			, &ulDNSTarget
			);
		res = regKeyOps.QueryStringValue( 
			TEXT( "DNSTarget" )
			, szDNSTarget
			, &ulDNSTarget
			);

		res = regKeyOps.QueryDWORDValue( 
			TEXT( "ReceiveNwBuffer" )
			, dwReceiveNwBuffer
			);

		res = regKeyOps.QueryDWORDValue( 
			TEXT( "ReceiveTpMin" )
			, dwReceiveTpMin
			);

		res = regKeyOps.QueryDWORDValue( 
			TEXT( "ReceiveTpMax" )
			, dwReceiveTpMax
			);

		res = regKeyOps.QueryDWORDValue( 
			TEXT( "SendNwBuffer" )
			, dwSendNwBuffer
			);

		res = regKeyOps.QueryDWORDValue( 
			TEXT( "SendTpMin" )
			, dwSendTpMin
			);

		res = regKeyOps.QueryDWORDValue( 
			TEXT( "SendTpMax" )
			, dwSendTpMax
			);

		res = regKeyOps.QueryStringValue( 
			TEXT( "ManagementQName" )
			, NULL
			, &ulManagementQName
			);
		res = regKeyOps.QueryStringValue( 
			TEXT( "ManagementQName" )
			, szManagementQName
			, &ulManagementQName
			);
		res = regKeyOps.QueryDWORDValue( 
			TEXT( "ManagementQBuffer" )
			, dwManagementQBuffer
			);
		res = regKeyOps.QueryDWORDValue( 
			TEXT( "ManagementQTpMin" )
			, dwManagementQTpMin
			);
		res = regKeyOps.QueryDWORDValue( 
			TEXT( "ManagementQTpMax" )
			, dwManagementQTpMax
			);

		res = regKeyOps.QueryStringValue( 
			TEXT( "DbConnString" )
			, NULL
			, &ulDbConnString
			);
		res = regKeyOps.QueryStringValue( 
			TEXT( "DbConnString" )
			, szDbConnString
			, &ulDbConnString
			);

	}
	regKeyOps.Close();

	m_ServerNetworkContext.m_ServerContext.m_szIpAddress = szIpAddress;

	wcscpy_s(
		m_ServerNetworkContext.m_szDNSTarget
		, _countof( m_ServerNetworkContext.m_szDNSTarget )
		, szDNSTarget
		);

	m_ServerNetworkContext.m_uiReceiveNwBuffer = dwReceiveNwBuffer;
	m_ServerNetworkContext.m_uiReceiveTpMin = dwReceiveTpMin;
	m_ServerNetworkContext.m_uiReceiveTpMax = dwReceiveTpMax;

	m_ServerNetworkContext.m_uiSendNwBuffer = dwSendNwBuffer;
	m_ServerNetworkContext.m_uiSendTpMin = dwSendTpMin;
	m_ServerNetworkContext.m_uiSendTpMax = dwSendTpMax;

	m_ServerNetworkContext.m_usMTU = dwMTU;
	m_ServerNetworkContext.m_usPort = dwPort;

	wcscpy_s(
		m_ServerNetworkContext.m_szManagementQName
		, _countof( m_ServerNetworkContext.m_szManagementQName )
		, szManagementQName
		);
	m_ServerNetworkContext.m_uiManagementQBuffer = dwManagementQBuffer;
	m_ServerNetworkContext.m_uiManagementQTpMin = dwManagementQTpMin;
	m_ServerNetworkContext.m_uiManagementQTpMax = dwManagementQTpMax;

	wcscpy_s(
		m_ServerNetworkContext.m_szDbConnString
		, _countof( m_ServerNetworkContext.m_szDbConnString )
		, szDbConnString
		);

	m_pServerNetwork = new CServerNetwork( &m_ServerNetworkContext );
	CServerNetwork::StartServiceThread( m_pServerNetwork );

	hr = __super::Run( nShowCmd );

	return hr;
}

void CBlxContentServiceModule::OnStop()
{
	CServerNetwork::EndServiceThread( m_pServerNetwork );
	delete m_pServerNetwork;
	return;
}

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

