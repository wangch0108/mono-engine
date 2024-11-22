#pragma once
#include "BaseObject.h"
#include "Mono/MonoBehaviour.h"
#include "Mono/MonoTypes.h"

class GameObject : public Object
{
public:
	GameObject(const std::string& name);
	~GameObject();
	void Activate();
	void AddComponent(const std::string& compName);

	operator bool() const { return _monoHandle.HasTarget(); }
private:
	void AwakeFromLoad();

	std::vector<MonoBehaviour> _components;
};
