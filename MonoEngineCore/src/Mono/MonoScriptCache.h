#pragma once

#include "MonoTypes.h"
#include "Thread/AtomicRefCounter.h"

struct MonoScriptCache
{
	enum { kUpdate = 0, kLateUpdate, kFixedUpdate, kAwake, kInternalAwake, kStart, kOnEnable, kOnDisable, kOnDestroy, kMethodCount };

	typedef intptr_t IdentifierHashType;

	AtomicRefCounter refCounter;
	MonoClassPtr klass;
	std::vector<MonoMethodPtr> methods;
	IdentifierHashType IdentifierHash;

	MonoScriptCache();
	~MonoScriptCache();

	void Release() const;
	void Retain() const;
	int RefCount() const;
};

MonoScriptCache* CreateMonoScriptCache(MonoClassPtr klass);
MonoScriptCache* FindOrCreateMonoScriptCache(MonoClassPtr klass);