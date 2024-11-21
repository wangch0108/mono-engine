#pragma once

__forceinline int AtomicIncrement(int volatile* i)
{
#if PLATFORM_WIN
	return _InterlockedIncrement((long volatile*)i);
#else
#error "Current unsupport yet!"
#endif
}

__forceinline int AtomicDecrement(int volatile* i)
{
#if PLATFORM_WIN
	return _InterlockedDecrement((long volatile*)i);
#else
#error "Current unsupport yet!"
#endif
}