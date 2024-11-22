#include "mepch.h"
#include "BaseObject.h"
#include "Mono/MonoObjectOfType.h"

MonoObjectPtr Object::GetCachedMonoObject()
{
	return _monoHandle.Resolve();
}

void Object::SetCachedMonoObject(MonoObjectPtr object)
{
	if (object)
	{
		_monoHandle.AcquireStrong(object);
		return;
	}

	if (!_monoHandle.HasTarget())
	{
		std::cerr << "Don't do this";
		return;
	}

	MonoObjectOfType<Object> instance(_monoHandle.Resolve());
	instance.SetCachedPtr(nullptr);
	_monoHandle.Release();
}

Object::~Object()
{
	if (_monoHandle.HasTarget())
		delete_object_internal(this);
}

void DestroySingleObject(Object* o)
{
	delete_object_internal(o);
}

void delete_object_internal(Object* o)
{
	o->_monoHandle.Release();
}
