#include "mepch.h"
#include "GameObjectManager.h"

static GameObjectManager GManager;

void GameObjectManager::RegisterGameObject(std::shared_ptr<GameObject>& object)
{
	if (object == nullptr || !*object)
		return;

	auto name = object->GetName();

	if (_objects.find(name) != _objects.end())
	{
		std::cerr << "Duplicate game object names are not supported!"; // TODO: Improve this
		return;
	}

	_objects[name] = object;
}

void GameObjectManager::UnregisterGameObject(const char* name)
{
	if (_objects.find(name) == _objects.end())
	{
		std::cerr << "GameObject(" << name << ") not found!";
		return;
	}

	_objects.erase(name);
}

std::shared_ptr<GameObject> GameObjectManager::FindGameObject(const char* name)
{
	if (_objects.find(name) == _objects.end())
		return nullptr;

	return _objects[name];
}

GameObjectManager& GetGameObjectManager()
{
	return GManager;
}
