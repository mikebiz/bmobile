
template<typename T>
PVOID CBlxHeap<T>::operator new( size_t size )
{
	PVOID p = NULL;
	if( NULL == T::get_Heap() )
	{
		T::set_Heap( HeapCreate(
					0
					, 0
					, 0
					)
				);
		if( NULL == T::get_Heap() )
		{
			goto opNewExit;
		}
		//ULONG HeapInformationValue = 2;
		//HeapSetInformation(
		//		T::get_Heap()
		//		, HeapCompatibilityInformation
		//		, &HeapInformationValue
		//		, sizeof( HeapInformationValue )
		//		);
	}

	p = HeapAlloc(
			T::get_Heap()
			, 0
			, size
			);

	if( p != NULL )
	{
		T::increment_NumAllocsInHeap();
	}

opNewExit:
	return p;
}

template<typename T>
VOID CBlxHeap<T>::operator delete( PVOID p )
{
	if( HeapFree(
			T::get_Heap()
			, 0
			, p
			)
		)
	{
		T::decrement_NumAllocsInHeap();
	}

	if( 0 == T::get_NumAllocsInHeap() )
	{
		if( HeapDestroy( T::get_Heap() ) )
		{
			T::delete_Heap();
		}
	}
	return;
}

