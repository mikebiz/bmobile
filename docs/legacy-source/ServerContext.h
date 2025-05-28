#pragma once

#include <ws2tcpip.h>

class CServerNetwork;

typedef struct _BCS_SERVER_CONTEXT
{
	UINT32				m_uiThreadId;
	CString				m_szIpAddress;

} BCS_SERVER_CONTEXT, *PBCS_SERVER_CONTEXT;

typedef struct _BCS_SERVER_NETWORK_CONTEXT
{
	BCS_SERVER_CONTEXT	m_ServerContext;
	USHORT				m_usPort;
	USHORT				m_usMTU;
	WCHAR				m_szDNSTarget[ 255 ];
	UINT32				m_uiReceiveTpMax;
	UINT32				m_uiReceiveTpMin;
	UINT32				m_uiReceiveNwBuffer;
	UINT32				m_uiSendTpMax;
	UINT32				m_uiSendTpMin;
	UINT32				m_uiSendNwBuffer;
	WCHAR				m_szManagementQName[ 255 ];
	UINT32				m_uiManagementQTpMax;
	UINT32				m_uiManagementQTpMin;
	UINT32				m_uiManagementQBuffer;
	WCHAR				m_szDbConnString[ 255 ];

} BCS_SERVER_NETWORK_CONTEXT, *PBCS_SERVER_NETWORK_CONTEXT;

typedef struct _SERVICE_THREAD_SOCKET_CONTEXT
{
	WSADATA					m_ServerVideoData;
	SOCKET					m_ServerVideoSocket;
	SOCKADDR_IN				m_ServerVideoSocketAddr;
	INT32					m_iServerVideoLastError;
	WSAEVENT				m_hServerVideoEvent;
	addrinfo*				m_aiServerVideoRessrc;
} SERVICE_THREAD_SOCKET_CONTEXT, *PSERVICE_THREAD_SOCKET_CONTEXT;


typedef struct _SOCKET_THREAD_CONTEXT
{
	UINT32				m_uiThreadId;
	CServerNetwork*		m_pServerNetwork;
} SOCKET_THREAD_CONTEXT, *PSOCKET_THREAD_CONTEXT;
