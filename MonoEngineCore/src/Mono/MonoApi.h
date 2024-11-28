#pragma once

#include "MonoIncludes.h"

class Object;
MonoMethodPtr mono_produce_method_from_backend(MonoMethod* method);
void mono_class_get_methods_ex(MonoClassPtr klass, std::vector<MonoMethodPtr>& result);
bool mono_method_is_instance(MonoMethod* method);
int mono_method_get_argument_count(MonoMethodPtr method);
MonoClass* mono_class_from_name_ex(MonoImage* image, const char* name_space, const char* name);
MonoObjectPtr mono_engine_object_new(MonoClassPtr klass, Object* nativeInstance);

enum { kManagedObjectOffset = sizeof(void*) * 2 }; // vtable and thread synchronisation
#define MONO_OBJECT_HEADSIZE kManagedObjectOffset