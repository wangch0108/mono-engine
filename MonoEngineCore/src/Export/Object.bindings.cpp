#include "mepch.h"
#include "Object.bindings.h"
#include "Mono/MonoIncludes.h"
#include "Mono/MonoObjectOfType.h"

static MonoString* Object_GetPropName(MonoObject* object)
{
	MonoObjectOfType<Object> self(object);
	return mono_string_new_wrapper( self->GetName() );
}

static void Object_SetPropName(MonoObject* object, MonoString* name)
{
	MonoObjectOfType<Object> self(object);
	const auto cppname = std::string(mono_string_to_utf8(name));
	self->SetName(cppname);
}

static const char* s_GameObject_IcallNames[] =
{
	"MonoEngine.Object::get_name",
	"MonoEngine.Object::set_name",
	nullptr
};

static const void* s_GameObject_IcallFuncs[] =
{
	(const void*)&Object_GetPropName,
	(const void*)&Object_SetPropName,
	nullptr,
};

void ExportObjectBindings()
{
	for (int i = 0; s_GameObject_IcallNames[i] != nullptr; ++i)
		mono_add_internal_call(s_GameObject_IcallNames[i], s_GameObject_IcallFuncs[i]);
}
