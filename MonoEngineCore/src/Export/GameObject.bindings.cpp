#include "mepch.h"
#include "GameObject.bindings.h"
#include "Mono/MonoIncludes.h"
#include "Mono/MonoObjectOfType.h"
#include "Scripting/GameObject.h"
#include "Scripting/GameObjectManager.h"
#include "Scripting/GameObjectUtility.h"
#include "Scripting/Scripting.h"

static void GameObject_Internal_CreateGameObject(MonoObject* self, MonoString* name)
{
	const auto str = std::string(mono_string_to_utf8(name));
	auto ob = std::make_shared<GameObject>(str);
	Scripting::ConnectMonoWrapperToObject(self, ob.get());
	GetGameObjectManager().RegisterGameObject(ob);
}

static MonoObject* GameObject_Find(MonoString* str)
{
	const auto name = mono_string_to_utf8(str);
	auto object = GetGameObjectManager().FindGameObject(name);
	if (!object)
		return nullptr;

	return object->GetCachedMonoObject().GetNativePtr();
}

static MonoObject* GameObject_Internal_AddComponentWithType(MonoObject* self_, MonoObject* componentType_)
{
	MonoObjectOfType<GameObject> self(self_);
	return MonoAddComponentWithType(*self, componentType_);
}

static const char* s_GameObject_IcallNames[] =
{
	"MonoEngine.GameObject::Internal_CreateGameObject",
	"MonoEngine.GameObject::Find",
	"MonoEngine.GameObject::Internal_AddComponentWithType",
	nullptr
};

static const void* s_GameObject_IcallFuncs[] =
{
	(const void*)&GameObject_Internal_CreateGameObject,
	(const void*)&GameObject_Find,
	(const void*)& GameObject_Internal_AddComponentWithType,
	nullptr,
};

void ExportGameObjectBindings()
{
	for (int i = 0; s_GameObject_IcallNames[i] != nullptr; ++i)
		mono_add_internal_call(s_GameObject_IcallNames[i], s_GameObject_IcallFuncs[i]);
}
