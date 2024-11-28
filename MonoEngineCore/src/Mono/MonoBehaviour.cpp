#include "mepch.h"
#include "MonoBehaviour.h"
#include "MonoInvocation.h"
#include "MonoScriptCache.h"
#include "MonoObjectOfType.h"

#include <cassert>

static bool IsInstanceValid(MonoObjectPtr target)
{
	if (target == MONO_NULL)
		return false;

	MonoObjectOfType<Object> wrapper(target);
	return wrapper.GetCachedPtr() != nullptr;
}

void MonoBehaviour::AwakeFromLoad()
{
	MonoObjectPtr instance = GetInstance();
	if (instance == MONO_NULL)
	{
		std::cerr << "Support scripting instance only.";
		return;
	}

	if (enabled && IsActive())
		AddToManager();

	if (!_didAwake)
		CallAwake();
}

void MonoBehaviour::Deactivate()
{
}

#define RETURN_IS_DESTROYED_OR_DISABLED if (!IsInstanceValid(instance) || !enabled) return;

void MonoBehaviour::AddToManager()
{
	MonoObjectPtr instance = GetInstance();
	if (instance == MONO_NULL)
		return;

	if (!_didAwake)
	{
		CallAwake();
		RETURN_IS_DESTROYED_OR_DISABLED
	}
}

void MonoBehaviour::RemoveFromManager()
{
}

void MonoBehaviour::Update()
{
}

void MonoBehaviour::LateUpdate()
{
}

void MonoBehaviour::FixedUpdate()
{
}

void MonoBehaviour::Start()
{
}

void MonoBehaviour::SetGameObjectInternal(GameObject* go)
{
	_gameObject = go;
}

void MonoBehaviour::SetupMonoClass(MonoClassPtr klass)
{
	_scriptCache = FindOrCreateMonoScriptCache(klass);
	_scriptCache->Retain();

	auto instance = mono_engine_object_new(klass, this);
	mono_runtime_object_init(instance);
}

void MonoBehaviour::CallAwake()
{
	_didAwake = true;

	auto internalAwakeMethod = GetMethod(MonoScriptCache::kInternalAwake);
	if (!internalAwakeMethod.IsNull())
		CallMethod(internalAwakeMethod);

	auto awakeMethod = GetMethod(MonoScriptCache::kAwake);
	if (!awakeMethod.IsNull())
	{
		auto instance = GetInstance();
		CallMethod(awakeMethod);
	}
}

void MonoBehaviour::CallMethodIfAvaliable(int methodIndex)
{
}

void MonoBehaviour::CallMethod(MonoMethodPtr method) const
{
	MonoObjectPtr instance = GetInstance();
	assert(instance != MONO_NULL);

	MonoInvocation invocation(instance, method);
	invocation.Invoke();
}

void MonoBehaviour::CallUpdateMethod(int methodIndex)
{
}
