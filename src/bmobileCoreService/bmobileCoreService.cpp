// bmobileCoreService.cpp : Implementation of WinMain


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "bmobileCoreService_i.h"


using namespace ATL;

#include <stdio.h>

class CbmobileCoreServiceModule : public ATL::CAtlServiceModuleT< CbmobileCoreServiceModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_bmobileCoreServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BMOBILECORESERVICE, "{c274b8ec-5a50-4862-af35-417565845916}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication,
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY
		// and an appropriate non-null Security Descriptor.

		return S_OK;
	}
};

CbmobileCoreServiceModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

