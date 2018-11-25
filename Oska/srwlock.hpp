
#pragma once

#define _AMD64_ // For SRWLock...
#include <windef.h>
#include <WinBase.h>


template < bool L = false >
struct SRWLock {

};

template < >
struct SRWLock < true > {

	void lock ( ) noexcept { AcquireSRWLockExclusive ( &srwlock_handle ); }
	bool tryLock ( ) noexcept { return TryAcquireSRWLockExclusive ( &srwlock_handle ) != 0; }
	void unlock ( ) noexcept { ReleaseSRWLockExclusive ( &srwlock_handle ); } // Look at this...

	void lockRead ( ) noexcept { AcquireSRWLockShared ( &srwlock_handle ); }
	bool tryLockRead ( ) noexcept { return TryAcquireSRWLockShared ( &srwlock_handle ) != 0; }
	void unlockRead ( ) noexcept { ReleaseSRWLockShared ( &srwlock_handle ); }

	SRWLock ( ) noexcept : srwlock_handle ( SRWLOCK_INIT ) { }
	~SRWLock ( ) noexcept { tryLockRead ( ) ? unlockRead ( ) : unlock ( ); }

	SRWLock < true > & operator = ( const SRWLock < true > & rhs_ ) { return * this; }

private:

	SRWLOCK srwlock_handle;
};

template < >
struct SRWLock < false > {

	void lock ( ) const noexcept { }
	bool tryLock ( ) const noexcept { return true; }
	void unlock ( ) const noexcept { }

	void lockRead ( ) const noexcept { }
	bool tryLockRead ( ) const noexcept { return true; }
	void unlockRead ( ) const noexcept { }

	SRWLock < false > & operator = ( const SRWLock < false > & rhs_ ) { return * this; }
};
