#pragma once
#include "BaseObject.h"
#include "Mono/MonoTypes.h"

class MonoBehaviour;
class GameObject : public Object
{
public:
	GameObject(const std::string& name);
	~GameObject();
	void Activate();
	bool IsActive() const { return _isActive; }

	void AddComponentInternal(MonoBehaviour* component);

	const char* GetName() const override;
	void SetName(const char*) override;

	operator bool() const { return _monoHandle.HasTarget(); }
private:
	std::string _name;
	std::vector<MonoBehaviour*> _components;
	bool _isActive = true;

	void AwakeFromLoad();
};
