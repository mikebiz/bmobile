#pragma once

template<typename T>
CPriorityQueue<T>
	::CPriorityQueue(  UINT32 MinBufferSize, UINT32 MaxBufferSize  )
	: m_uiMinBufferSize( MinBufferSize )
	, m_uiMaxBufferSize( MaxBufferSize )
	, m_uiCurrentBufferSize( 0 )
{
//	InitializeSRWLock( &m_SRWLock );
	InitializeCriticalSectionAndSpinCount( &m_SRWLock, 4);
	m_hEvent = 
		CreateEvent(
			NULL
			, TRUE
			, FALSE
			, NULL
			);
}

template<typename T>
BOOL CPriorityQueue<T>
	::Empty() 
{	// test if queue is empty
	BOOL fRc = FALSE;
//	AcquireSRWLockExclusive( &m_SRWLock );
	EnterCriticalSection( &m_SRWLock );
	fRc = m_pq.empty();
	m_uiCurrentBufferSize = 0;
	LeaveCriticalSection( &m_SRWLock );
//	ReleaseSRWLockExclusive( &m_SRWLock );
	return fRc;
}

template<typename T>
size_t CPriorityQueue<T>
	::Size() 
{	// return length of queue
	size_t uiRet = 0;
//	AcquireSRWLockShared( &m_SRWLock );
	EnterCriticalSection( &m_SRWLock );
	uiRet = m_pq.size();
//	ReleaseSRWLockShared( &m_SRWLock );
	LeaveCriticalSection( &m_SRWLock );
	return uiRet;
}

//template<typename T>
//T& const CPriorityQueue<T>
//	::Top() const
//{	// return mutable highest-priority element (retained)
////	AcquireSRWLockShared( &m_SRWLock );
//	return( m_pq.top() );
////	ReleaseSRWLockShared( &m_SRWLock );
////	return _m;
//}

template<typename T>
T& CPriorityQueue<T>
	::Top()
{	// return mutable highest-priority element (retained)
	T _p;
	
//				AcquireSRWLockShared( &m_SRWLock );
	DWORD dwEvent =
		WaitForSingleObject( 
			m_hEvent
			, 100
			);

	switch( dwEvent )
	{
	case WAIT_FAILED:
		_p = NULL;
		goto Top_Exit;
		break;
	case WAIT_TIMEOUT:
		_p = NULL;
		goto Top_Exit;
		break;
	}
	EnterCriticalSection( &m_SRWLock );

	//try
	//{
	//	for( ; ; )
	//	{
	//		if( !m_pq.empty() )
	//		{
	//			if( !IsBelowThreshold() )
	//			{
					_p = m_pq.top();
					m_pq.pop();
					m_uiCurrentBufferSize--;
	//				break;
	//			}
	//		}
	//	}
	//}
	//catch( ... )
	//{
	//	_p = NULL;
	//}

//				ReleaseSRWLockShared( &m_SRWLock );
	if( IsBelowThreshold() )
	{
		ResetEvent( m_hEvent );
	}
	else
	{
		SetEvent( m_hEvent );
	}

	LeaveCriticalSection( &m_SRWLock );
//	WakeAllConditionVariable( &m_cvReadyToWrite );
Top_Exit:
	return _p;
}

template<typename T>
BOOL CPriorityQueue<T>
	::Push(const T& Pred) 
{	// insert value in priority order
//	AcquireSRWLockExclusive( &m_SRWLock );
	EnterCriticalSection( &m_SRWLock );

	try
	{
		if( IsFull() )
		{
			T _r = m_pq.top();
			if( _r < Pred )
			{
				//drop what you're pushing
	//			ReleaseSRWLockExclusive( &m_SRWLock );
				LeaveCriticalSection( &m_SRWLock );
				return FALSE;
			}
			else
			{
				if( 0 != m_uiCurrentBufferSize )
				{
					WSASetEvent( _r->hEvent );
					m_pq.pop();
					m_uiCurrentBufferSize--;
				}
			}
		}
	}
	catch( ... ) 
	{
		return FALSE;
	}

	if( !IsFull() )
	{
		m_uiCurrentBufferSize++;
		m_pq.push( Pred );
	}
	if( IsBelowThreshold() )
	{
		ResetEvent( m_hEvent );
	}
	else
	{
		SetEvent( m_hEvent );
	}
//	ReleaseSRWLockExclusive( &m_SRWLock );
	LeaveCriticalSection( &m_SRWLock );


	return TRUE;
}

template<typename T>
VOID CPriorityQueue<T>
	::Pop()
{	// erase highest-priority element
//	AcquireSRWLockShared( &m_SRWLock );
	EnterCriticalSection( &m_SRWLock );
	m_pq.pop();
//	ReleaseSRWLockShared( &m_SRWLock );
	LeaveCriticalSection( &m_SRWLock );
	return;
}

template<typename T>
BOOL CPriorityQueue<T>
	::IsFull() 
{	// insert value in priority order
	if( m_uiCurrentBufferSize <= m_uiMaxBufferSize )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

template<typename T>
BOOL CPriorityQueue<T>
	::IsBelowThreshold() 
{	// insert value in priority order
	if( m_uiCurrentBufferSize < m_uiMinBufferSize )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
