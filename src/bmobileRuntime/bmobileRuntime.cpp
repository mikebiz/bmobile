// bmobileRuntime.cpp : Implementation of WinMain


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "bmobileRuntime_i.h"


using namespace ATL;

#include <stdio.h>

class CbmobileRuntimeModule : public ATL::CAtlServiceModuleT< CbmobileRuntimeModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_bmobileRuntimeLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BMOBILERUNTIME, "{2d4395a6-8f9b-4c80-8b83-3f3408587f3c}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication,
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY
		// and an appropriate non-null Security Descriptor.

		return S_OK;
	}
};

CbmobileRuntimeModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

