#pragma once
#include "Mono/MonoTypes.h"

class Object
{
public:
	Object(const std::string& name) : _name(name) {}
	const char* GetName() const { return _name.c_str(); }
	void SetName(const std::string& name) { _name = name; }
	friend void delete_object_internal(Object* o);

	MonoObjectPtr GetCachedMonoObject();
	void SetCachedMonoObject(MonoObjectPtr object);
protected:

	virtual ~Object();
	std::string _name;
	MonoGCHandle _monoHandle;
};

void DestroySingleObject(Object* o);
void delete_object_internal(Object* o);
