#pragma once

struct MonoScriptCache;
class MonoBehaviour
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

private:
	inline void CallMethodIfAvaliable(int methodIndex);
	void CallUpdateMethod(int methodIndex);

public:
	bool enabled = true;

private:
	MonoScriptCache* _scriptCache = nullptr;

	bool _didAwake = false;
	bool _didStart = false;
	bool _didDestroy = false;
};
