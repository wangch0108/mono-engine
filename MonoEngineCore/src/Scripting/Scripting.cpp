#include "mepch.h"
#include "Scripting.h"

#include "Mono/MonoManager.h"
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

MonoObjectPtr Scripting::MonoWrapperFor(Object* o)
{
	if (!o)
		return MONO_NULL;

	MonoObjectPtr cachedInstance = o->GetCachedMonoObject();
	if (cachedInstance != MONO_NULL)
		return cachedInstance;

	MonoClassPtr klass = GetCommonScriptingClasses().monoBehaviour; // o->GetType(); RTTI needed
	return mono_engine_object_new(klass, o);
}
