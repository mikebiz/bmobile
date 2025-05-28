#pragma once

#ifdef _MANAGED
#include <vcclr.h>
using namespace System;
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#if _MANAGED

class Utilities
{
public:
	static System::Guid FromGUID( GUID& guid );
	static GUID ToGUID( System::Guid& guid );
	
private:
	Utilities(){};
	~Utilities(){};
};

#endif
