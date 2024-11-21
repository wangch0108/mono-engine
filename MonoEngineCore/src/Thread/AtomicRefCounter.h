#pragma once
#include "AtomicOps.h"

class AtomicRefCounter
{
public:
	AtomicRefCounter() : _counter(1) {}

	__forceinline void Retain()
	{
		AtomicIncrement(&_counter);
	}

	__forceinline bool Release()
	{
		int afterDecrement = AtomicDecrement(&_counter);
		return afterDecrement == 0;
	}

	_forceinline int Count() const
	{
		return _counter;
	}

private:
	volatile int _counter;
};