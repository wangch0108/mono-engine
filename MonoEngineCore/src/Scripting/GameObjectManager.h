#pragma once
#include "GameObject.h"

#include <unordered_map>

class GameObjectManager
{
public:
	void RegisterGameObject(std::shared_ptr<GameObject>& object);
	void UnregisterGameObject(const char* name);
	std::shared_ptr<GameObject> FindGameObject(const char* name);

private:
	std::unordered_map < std::string /* TODO ConstantString(same pointer for mono string) */, std::shared_ptr<GameObject> > _objects; // TODO TreeNode
};

GameObjectManager& GetGameObjectManager();