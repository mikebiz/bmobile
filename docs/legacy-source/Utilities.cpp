#include "Utilities.h"

#if _MANAGED

System::Guid Utilities::FromGUID( GUID& guid ) 
{
	return System::Guid( guid.Data1, guid.Data2, guid.Data3, 
                guid.Data4[ 0 ], guid.Data4[ 1 ], 
                guid.Data4[ 2 ], guid.Data4[ 3 ], 
                guid.Data4[ 4 ], guid.Data4[ 5 ], 
                guid.Data4[ 6 ], guid.Data4[ 7 ] );
}

GUID Utilities::ToGUID( System::Guid& guid ) {
   array<Byte>^ guidData = guid.ToByteArray();
   pin_ptr<Byte> data = &(guidData[ 0 ]);

   return *(_GUID *)data;
}

#endif