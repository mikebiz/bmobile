
template<typename T>
inline PVOID Bmobile::System::CHeap<T>::operator new( size_t size )
{

	PVOID p = NULL;

	if( NULL == T::get_Heap() )
	{
		T::get_Cs()->Lock();
		if( NULL == T::get_Heap() )
		{
			T::set_Heap( HeapCreate(
						0 //HEAP_NO_SERIALIZE - can't use this flag with Low Fragmentation
						, 0
						, 0
						)
					);
			if( NULL == T::get_Heap() )
			{
				goto opNewExit;
			}
#ifndef UNDER_CE
			ULONG HeapInformationValue = 2; //set heap to low fragmentation
			HeapSetInformation(
					T::get_Heap()
					, HeapCompatibilityInformation
					, &HeapInformationValue
					, sizeof( HeapInformationValue )
					);
#endif
		}
		T::get_Cs()->Unlock();
	}

	p = HeapAlloc(
			T::get_Heap()
			, 0
			, size
			);

	if( p != NULL )
	{
		T::increment_NumAllocsInHeap();
//		OutputDebugStringf( TEXT( "--HeapAlloc %000000000000ld:" ), T::get_NumAllocsInHeap );

	}

opNewExit:

	return p;
}

template<typename T>
inline VOID Bmobile::System::CHeap<T>::operator delete( PVOID p )
{
	if( HeapFree(
			T::get_Heap()
			, 0
			, p
			)
		)
	{
		T::decrement_NumAllocsInHeap();
//		OutputDebugStringf( TEXT( "--HeapFree %000000000000ld:" ), T::get_NumAllocsInHeap );
}

	if( 0 == T::get_NumAllocsInHeap() )
	{
		T::get_Cs()->Lock();
		if( 0 == T::get_NumAllocsInHeap() )
		{
			if( HeapDestroy( T::get_Heap() ) )
			{
				T::delete_Heap();
			}
		}
		T::get_Cs()->Unlock();
	}

	return;
}

