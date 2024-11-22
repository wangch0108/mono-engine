#pragma once

#include "Mono/MonoTypes.h"
#include "Scripting/Scripting.h"

template<class T>
struct MonoObjectOfType
{
	MonoObjectOfType(MonoObject* object)
	{
		this->_object = object;
	}

	T& GetReference() const
	{
		T* ptr = GetPtr();
		if (ptr != nullptr)
			return *ptr;

		return *(T*)nullptr;
	}

	inline T* GetPtr() const
	{
		if (_object == MONO_NULL)
			return nullptr;

		void* cachedPtr = Scripting::GetCachedPtrFromScriptingWrapper(_object);
		if (cachedPtr != nullptr)
			return (T*)cachedPtr;

		return nullptr;
	}

	inline void SetCachedPtr(Object* cachedPtr)
	{
		Scripting::SetCachedPtrOnScriptingWrapper(_object, cachedPtr);
	}

	inline T& operator*() const
	{
		return GetReference();
	}

	inline T* operator->() const
	{
		return &GetReference();
	}

private:
	MonoObjectPtr _object;
};
