#include "mepch.h"
#include "Behaviour.h"

void Behaviour::AwakeFromLoad()
{
	UpdateEnabledState(IsActive());
}

void Behaviour::Deactivate()
{
	UpdateEnabledState(false);
}

void Behaviour::SetEnabled(bool value)
{
	if ((bool)_enabled == value)
		return;
	_enabled = value;
	UpdateEnabledState(IsActive());
}

void Behaviour::UpdateEnabledState(bool active)
{
	bool shouldBeAdded = active && _enabled;
	if (shouldBeAdded == _isAdded)
		return;
	if (shouldBeAdded)
	{
		_isAdded = true;
		AddToManager();
	}
	else
	{
		_isAdded = false;
		RemoveFromManager();
	}
}

/******************** BEHAVIOUR MANAGER **********************/

BaseBehaviourManager::~BaseBehaviourManager()
{
}

void BaseBehaviourManager::AddBehaviour(BehaviourListNode& node, int queue)
{
	auto& listPair = _lists[queue];
	if (listPair.first == nullptr)
	{
		listPair.first = new BehaviourList();
		listPair.second = new BehaviourList();
	}

	listPair.second->push_back(node);
}

void BaseBehaviourManager::RemoveBehaviour(BehaviourListNode& node)
{
	node.RemoveFromList();
}

void BaseBehaviourManager::FlushLists()
{
	for (auto& _list : _lists)
	{
		auto& listPair = _list.second;
		listPair.first->append(*listPair.second); // Flush second level buffer into the first level
		assert(listPair.second->empty());
	}
}

template <typename T>
void BaseBehaviourManager::CommonUpdate()
{
	FlushLists();

	for (auto& _list : _lists)
	{
		auto& listPair = _list.second;
		SafeIterator iterator(*listPair.first);
		while (iterator.Next())
		{
			Behaviour& behaviour = **iterator;
			T::UpdateBehaviour(behaviour);
		}
	}
}

class BehaviourManager : BaseBehaviourManager
{
public:
	virtual void Update() override
	{
		CommonUpdate<BehaviourManager>();
	}

	static inline void UpdateBehaviour(Behaviour& behaviour)
	{
		behaviour.Update();
	}
};

class FixedBehaviourManager : BaseBehaviourManager
{
public:
	virtual void Update() override
	{
		CommonUpdate<FixedBehaviourManager>();
	}

	static inline void UpdateBehaviour(Behaviour& behaviour)
	{
		behaviour.FixedUpdate();
	}
};

class UpdateManager : BaseBehaviourManager
{
public:
	virtual void Update() override
	{
		CommonUpdate<UpdateManager>();
	}

	static inline void UpdateBehaviour(Behaviour& behaviour)
	{
		behaviour.Update();
	}
};

class LateBehaviourManager : BaseBehaviourManager
{
public:
	virtual void Update() override
	{
		CommonUpdate<LateBehaviourManager>();
	}

	static inline void UpdateBehaviour(Behaviour& behaviour)
	{
		behaviour.LateUpdate();
	}
};

#define GET_BEHAVIOUR_MANAGER(x) \
	x* s_Instance##x; \
	BaseBehaviourManager& Get##x() { return reinterpret_cast<BaseBehaviourManager&>(*s_Instance##x); } \
	void CreateInstance##x() { s_Instance##x = new x; } \
	void ReleaseInstance##x() { delete s_Instance##x; }

GET_BEHAVIOUR_MANAGER(BehaviourManager)
GET_BEHAVIOUR_MANAGER(FixedBehaviourManager)
GET_BEHAVIOUR_MANAGER(UpdateManager)
GET_BEHAVIOUR_MANAGER(LateBehaviourManager)

void Behaviour::InitializeClass()
{
	CreateInstanceBehaviourManager();
	CreateInstanceFixedBehaviourManager();
	CreateInstanceUpdateManager();
	CreateInstanceLateBehaviourManager();
}

void Behaviour::CleanupClass()
{
	ReleaseInstanceBehaviourManager();
	ReleaseInstanceFixedBehaviourManager();
	ReleaseInstanceUpdateManager();
	ReleaseInstanceLateBehaviourManager();
}
