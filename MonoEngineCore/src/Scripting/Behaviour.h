#pragma once
#include "BaseObject.h"
#include "Utility/LinkedList.h"

class Behaviour : public Object
{
	REGISTER_CLASS(Behaviour)

public:
	Behaviour() : _enabled(true), _isAdded(false) {}

	virtual void AwakeFromLoad();
	virtual void Deactivate();

	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FixedUpdate() {}

	virtual bool IsActive() const = 0;
	virtual bool GetEnabled() const { return _enabled != 0; }
	virtual void SetEnabled(bool value);

	virtual void AddToManager() = 0;
	virtual void RemoveFromManager() = 0;

	static void InitializeClass();
	static void CleanupClass();

private:
	void UpdateEnabledState(bool active);

	bool _enabled;
	bool _isAdded;
};

typedef ListNode<Behaviour> BehaviourListNode;

class BaseBehaviourManager
{
public:
	virtual ~BaseBehaviourManager();

	virtual void Update() = 0;

	void AddBehaviour(BehaviourListNode& node, int queue);
	void RemoveBehaviour(BehaviourListNode& node);



protected:
	template<typename T> void CommonUpdate();
	void FlushLists();

	typedef List<BehaviourListNode> BehaviourList;
	typedef std::map<int, std::pair<BehaviourList*, BehaviourList*>> Lists;
	Lists _lists;
};

BaseBehaviourManager& GetBehaviourManager();
BaseBehaviourManager& GetFixedBehaviourManager();
BaseBehaviourManager& GetUpdateManager();
BaseBehaviourManager& GetLateBehaviourManager();

