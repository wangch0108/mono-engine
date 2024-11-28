#include "mepch.h"
#include "GameObjectUtility.h"

#include "CoreMacros.h"
#include "GameObject.h"
#include "Scripting.h"
#include "Mono/MonoBehaviour.h"
#include "Mono/MonoManager.h"
#include "Mono/MonoUtility.h"

static bool IsSubclassOfMonoBehaivour(MonoClassPtr klass)
{
	return mono_class_is_subclass_of(klass, GetCommonScriptingClasses().monoBehaviour, true);
}

MonoObjectPtr MonoAddComponentWithType(GameObject& go, MonoObject* systemTypeInstance)
{
	auto monoType = ExtractMonoObjectData<MonoType*>(systemTypeInstance);
	MonoClassPtr klass = mono_class_from_mono_type(monoType);
	if (klass == MONO_NULL)
		return MONO_NULL;

	if (klass == GetCommonScriptingClasses().monoBehaviour)
	{
		std::cerr << "AddComponent with MonoBehaviour is not allowed.\n";
		return MONO_NULL;
	}

	if (!IsSubclassOfMonoBehaivour(klass))
	{
		std::cerr << "Native component have not supported.\n";
		return MONO_NULL;
	}

	// Collect required components(No similar logic)

	// Create new MonoBehaviour
	auto comp = static_cast<MonoBehaviour*>(NewObject<MonoBehaviour>());  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
	go.AddComponentInternal(comp);

	comp->SetupMonoClass(klass);
	comp->AwakeFromLoad();

	if (comp)
		return Scripting::MonoWrapperFor(comp);

	return MONO_NULL;
}
