#pragma once
#include "MonoApi.h"

template<class T>
inline T& ExtractMonoObjectData(MonoObjectPtr object)
{
	auto pointer = (char*)object.GetNativePtr();
	return *reinterpret_cast<T*>(pointer + (size_t)MONO_OBJECT_HEADSIZE); // Unbox
}