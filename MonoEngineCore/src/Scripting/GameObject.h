#pragma once
#include "Mono/MonoBehaviour.h"

class GameObject
{
public:
	std::string Name;

	void AddComponent(const std::string& compName);
	static GameObject* Find(const std::string& name);
private:
	std::vector<MonoBehaviour> _components;
};
