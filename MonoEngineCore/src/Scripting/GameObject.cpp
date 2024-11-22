#include "mepch.h"
#include "GameObject.h"

#include "Scripting.h"

GameObject::GameObject(const std::string& name) : Object(name)
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

	AwakeFromLoad();
}

void GameObject::AddComponent(const std::string& compName)
{
}

void GameObject::AwakeFromLoad()
{
}