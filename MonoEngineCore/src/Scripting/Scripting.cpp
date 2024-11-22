#include "mepch.h"
#include "Scripting.h"

#include "Mono/MonoObjectOfType.h"

void Scripting::SetCachedPtrOnScriptingWrapper(MonoObjectPtr wrapper, Object* cachedPtr)
{
	GetObjectMemoryLayout(wrapper)->cachedPtr = cachedPtr;
}

MonoObjectPtr Scripting::ConnectMonoWrapperToObject(MonoObjectPtr object, Object* ptr)
{
	if (ptr->GetCachedMonoObject() != MONO_NULL)
		return ptr->GetCachedMonoObject();

	MonoObjectOfType<Object> wrapper(object);
	wrapper.SetCachedPtr(ptr);
	ptr->SetCachedMonoObject(object);
	return object;
}
