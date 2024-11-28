#include "mepch.h"
#include "MonoApi.h"

#include <cassert>

#include "Scripting/Scripting.h"

MonoMethodPtr mono_produce_method_from_backend(MonoMethod* method)
{
	if (method == nullptr)
		return nullptr;

	MonoMethodSignature* sig = mono_method_signature(method);
	if (!sig)
	{
		// TODO: Loader error
		return nullptr;
	}
	return MonoMethodPtr(method);
}

void mono_class_get_methods_ex(MonoClassPtr klass, std::vector<MonoMethodPtr>& result)
{
	void* iterator = nullptr;
	MonoMethodPtr methodPtr;
	while (MonoMethod* method = mono_class_get_methods(klass, &iterator))
	{
		if (!(methodPtr = mono_produce_method_from_backend(method)).IsNull())
			result.emplace_back(methodPtr);
	}
}

bool mono_method_is_instance(MonoMethod* method)
{
	MonoMethodSignature* sig = mono_method_signature(method);
	return sig == nullptr ? false : mono_signature_is_instance(sig);
}

int mono_method_get_argument_count(MonoMethodPtr method)
{
	MonoMethodSignature* sig = mono_method_signature(method.GetNativeMethod());
	assert(sig);
	return mono_signature_get_param_count(sig);
}

MonoClass* mono_class_from_name_ex(MonoImage* image, const char* name_space, const char* name)
{
	// Namespace cannot be NULL; doing so will cause an error in this version
	name_space = name_space == nullptr ? "" : name_space;
	return mono_class_from_name(image, name_space, name);
}

MonoObjectPtr mono_engine_object_new(MonoClassPtr klass, Object* nativeInstance)
{
	MonoObjectPtr managedInstance = mono_object_new(mono_domain_get(), klass);
	if (managedInstance != MONO_NULL)
		Scripting::ConnectMonoWrapperToObject(managedInstance, nativeInstance);

	return managedInstance;
}
