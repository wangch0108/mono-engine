#pragma once
#include "MonoScriptCache.h"
#include "Scripting/BaseObject.h"
#include "Scripting/GameObject.h"

struct MonoScriptCache;
class MonoBehaviour : public Object
{
public:
	virtual void AwakeFromLoad();
	virtual void Deactivate();

	virtual void AddToManager();
	virtual void RemoveFromManager();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FixedUpdate();
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

public:
	bool enabled = true;

private:
	GameObject* _gameObject = nullptr;
	MonoScriptCache* _scriptCache = nullptr;

	bool _didAwake = false;
	bool _didStart = false;
	bool _didDestroy = false;
};
