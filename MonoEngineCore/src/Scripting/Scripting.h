#pragma once
#include "BaseObject.h"
#include "Mono/MonoUtility.h"

struct ObjectMemoryLayout
{
	Object* cachedPtr;
};

namespace Scripting
{
	inline ObjectMemoryLayout* GetObjectMemoryLayout(MonoObjectPtr object)
	{
		return ExtractMonoObjectPointer<ObjectMemoryLayout>(object);
	}

	inline Object* GetCachedPtrFromScriptingWrapper(MonoObjectPtr object)
	{
		return GetObjectMemoryLayout(object)->cachedPtr;
	}

	void SetCachedPtrOnScriptingWrapper(MonoObjectPtr wrapper, Object* cachedPtr);
	MonoObjectPtr ConnectMonoWrapperToObject(MonoObjectPtr object, Object* ptr);
}
