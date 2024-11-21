#include "mepch.h"
#include "GameObject.bindings.h"
#include "Mono/MonoIncludes.h"

static MonoObject* GameObject_Find(MonoString* str)
{
	return nullptr;
}

static const char* s_GameObject_IcallNames[] =
{
	"MonoEngine.GameObject::Find",
	nullptr
};

static const void* s_GameObject_IcallFuncs[] =
{
	(const void*)&GameObject_Find,
	nullptr,
};

void ExportGameObjectBindings()
{
	for (int i = 0; s_GameObject_IcallNames[i] != nullptr; ++i)
		mono_add_internal_call(s_GameObject_IcallNames[i], s_GameObject_IcallFuncs[i]);
}
