#include "mepch.h"
#include "GameObject.h"

#include "Scripting.h"
#include "Mono/MonoBehaviour.h"

GameObject::GameObject(const std::string& name) : _name(name)
{
	Activate();
}

GameObject::~GameObject()
{
}

void GameObject::Activate()
{
	if (_monoHandle.HasTarget())
		_monoHandle.Release();

	_isActive = true;
	AwakeFromLoad();
}

void GameObject::AddComponentInternal(MonoBehaviour* component)
{
	_components.push_back(component);
	component->SetGameObjectInternal(this);
}

const char* GameObject::GetName() const
{
	return _name.c_str();
}

void GameObject::SetName(const char* string)
{
	_name = string;
}

void GameObject::AwakeFromLoad()
{
}
