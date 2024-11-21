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
