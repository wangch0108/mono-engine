#include "mepch.h"
#include "MonoTypes.h"
#include "MonoApi.h"

bool MonoMethodPtr::IsInstance() const
{
	return mono_method_is_instance(_nativePtr);
}

MonoStringPtr MonoObjectPtr::ToMonoString() const
{
	return MonoStringPtr((MonoString*)_nativePtr);
}

MonoStringPtr MonoObjectPtr::ConvertToString() const
{
	return mono_object_to_string(_nativePtr, nullptr);
}

void MonoGCHandle::Release()
{
	Acquire(MONO_NULL, GCHANDLE_INVALID);
	if (HasTarget())
		mono_gchandle_free(_handle);
}

void MonoGCHandle::AcquireWeak(MonoObjectPtr obj)
{
	Acquire(obj, GCHANDLE_WEAK);
}

void MonoGCHandle::AcquireStrong(MonoObjectPtr obj)
{
	Acquire(obj, GCHANDLE_STRONG);
}

void MonoGCHandle::Acquire(MonoObjectPtr obj, MonoGCHandleWeakness weakness)
{
	_weakness = weakness;
	if (obj == MONO_NULL)
	{
		_handle = INVALID_GC_HANDLE;
	}
	else
	{
		_handle = _weakness == GCHANDLE_WEAK ?
			mono_gchandle_new_weakref(obj.GetNativePtr(), 1) :
			mono_gchandle_new(obj.GetNativePtr(), 1);
	}
}

MonoObjectPtr MonoGCHandle::ResolveGCHandle(uint32_t handle)
{
	return mono_gchandle_get_target(handle);
}
