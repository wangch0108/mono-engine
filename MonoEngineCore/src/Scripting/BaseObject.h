#pragma once
#include "Mono/MonoTypes.h"
#include "ObjectDefines.h"

class Object
{
public:
	typedef Object ThisType;

	Object() = default;

	virtual const char* GetName() const { return ""; }
	virtual void SetName(const char*) {}

	friend void delete_object_internal(Object* o);

	MonoObjectPtr GetCachedMonoObject() const;
	void SetCachedMonoObject(MonoObjectPtr object);
protected:

	virtual ~Object();
	MonoGCHandle _monoHandle;
};

void DestroySingleObject(Object* o);
void delete_object_internal(Object* o);
