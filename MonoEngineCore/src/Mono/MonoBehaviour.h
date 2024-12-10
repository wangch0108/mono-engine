#pragma once
#include "MonoScriptCache.h"
#include "Scripting/BaseObject.h"
#include "Scripting/Behaviour.h"
#include "Scripting/GameObject.h"

struct MonoScriptCache;
class MonoBehaviour : public Behaviour
{
	REGISTER_CLASS(MonoBehaviour)

public:
	MonoBehaviour();

	virtual void AwakeFromLoad() override;
	virtual void Deactivate() override;

	virtual void AddToManager() override;
	virtual void RemoveFromManager() override;

	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void LateUpdate() override;

	inline void Start();

	void SetGameObjectInternal(GameObject* go);
	void SetupMonoClass(MonoClassPtr klass);

	inline bool IsActive() const
	{
		return _gameObject != nullptr && _gameObject->IsActive();
	}

	MonoObjectPtr GetInstance() const
	{
		return GetCachedMonoObject();
	}

	MonoMethodPtr GetMethod(int index) const { return _scriptCache->GetCachedMethod(index); }

private:
	inline void CallMethodIfAvaliable(int methodIndex);
	void CallMethod(MonoMethodPtr method) const;

	void CallAwake();
	void CallUpdateMethod(int methodIndex);
	void AddBehaviourCallbacksToManagers();

private:
	GameObject* _gameObject = nullptr;
	MonoScriptCache* _scriptCache = nullptr;

	BehaviourListNode _updateNode;
	BehaviourListNode _fixedUpdateNode;
	BehaviourListNode _lateUpdateNode;

	const int executionOrder = 0;
	bool _didAwake = false;
	bool _didStart = false;
	bool _didDestroy = false;
};
