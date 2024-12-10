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

MonoBehaviour::MonoBehaviour()
	:	Super(),
		_updateNode(this),
		_fixedUpdateNode(this),
		_lateUpdateNode(this)
{
	_didAwake = _didStart = _didDestroy = false;
}

void MonoBehaviour::AwakeFromLoad()
{
	MonoObjectPtr instance = GetInstance();
	if (instance == MONO_NULL)
	{
		std::cerr << "Support scripting instance only.";
		return;
	}

	if (GetEnabled() && IsActive())
	{
		Super::AwakeFromLoad();
		return;
	}

	if (!_didAwake)
		CallAwake();

	Super::AwakeFromLoad();
}

void MonoBehaviour::Deactivate()
{
	Super::Deactivate();
}

#define RETURN_IS_DESTROYED_OR_DISABLED if (!IsInstanceValid(instance) || !GetEnabled()) return;

void MonoBehaviour::AddToManager()
{
	MonoObjectPtr instance = GetInstance();
	if (instance == MONO_NULL)
		return;

	AddBehaviourCallbacksToManagers();

	if (!_didAwake)
	{
		CallAwake();
		RETURN_IS_DESTROYED_OR_DISABLED
	}

	if (!GetMethod(MonoScriptCache::kOnEnable).IsNull())
		CallMethodIfAvaliable(MonoScriptCache::kOnEnable);
}

void MonoBehaviour::RemoveFromManager()
{
	_updateNode.RemoveFromList();
	_fixedUpdateNode.RemoveFromList();
	_lateUpdateNode.RemoveFromList();

	MonoObjectPtr instance = GetInstance();
	if (IsInstanceValid(instance) && !GetMethod(MonoScriptCache::kOnDisable).IsNull() && _didAwake)
		CallMethod(GetMethod(MonoScriptCache::kOnDisable));
}

void MonoBehaviour::Update()
{
	CallUpdateMethod(MonoScriptCache::kUpdate);
}

void MonoBehaviour::LateUpdate()
{
	CallUpdateMethod(MonoScriptCache::kLateUpdate);
}

void MonoBehaviour::FixedUpdate()
{
	CallUpdateMethod(MonoScriptCache::kFixedUpdate);
}

void MonoBehaviour::Start()
{
	assert(IsActive());

	if (_didStart)
		return;

	_didStart = true;

	if (!GetMethod(MonoScriptCache::kStart).IsNull())
		CallMethodIfAvaliable(MonoScriptCache::kStart);
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
	MonoMethodPtr method = GetMethod(methodIndex);
	if (method.IsNull())
		return;

	assert(GetInstance() != MONO_NULL);
	assert(_didAwake);

	if (!IsActive())
		return;

	MonoInvocation invocation(GetInstance(), method);
	invocation.Invoke();
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
	assert(IsActive() && GetEnabled());
	MonoObjectPtr instance = GetInstance();
	if (instance == MONO_NULL)
		return;

	// Ensure Start has been called
	Start();

	if (!IsInstanceValid(instance))
		return;

	CallMethodIfAvaliable(methodIndex);
}

void MonoBehaviour::AddBehaviourCallbacksToManagers()
{
	if (!GetMethod(MonoScriptCache::kFixedUpdate).IsNull())
		GetFixedBehaviourManager().AddBehaviour(_fixedUpdateNode, executionOrder);
	if (!GetMethod(MonoScriptCache::kUpdate).IsNull())
		GetBehaviourManager().AddBehaviour(_updateNode, executionOrder);
	if (!GetMethod(MonoScriptCache::kLateUpdate).IsNull())
		GetLateBehaviourManager().AddBehaviour(_lateUpdateNode, executionOrder);
}
