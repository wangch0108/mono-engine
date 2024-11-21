#include "mepch.h"
#include "MonoArguments.h"
#include "MonoIncludes.h"

#include <cassert>

MonoArguments::MonoArguments()
	: Count(0)
{
}

MonoArguments::MonoArguments(const MonoArguments& other)
{
	CopyFromOther(other);
}

MonoArguments& MonoArguments::operator=(const MonoArguments& other)
{
	if (this != &other)
	{
		CopyFromOther(other);
	}

	return *this;
}

void MonoArguments::AddBoolean(bool value)
{
	PrimitiveStorage[Count].boolValue = value ? 1 : 0;
	Arguments[Count].pointerToValue = &PrimitiveStorage[Count].boolValue;
	ArgumentTypes[Count] = ARGTYPE_BOOLEAN;
	IncreaseCount();
}

void MonoArguments::AddInt(int value)
{
	PrimitiveStorage[Count].intValue = value;
	Arguments[Count].pointerToValue = &PrimitiveStorage[Count].intValue;
	ArgumentTypes[Count] = ARGTYPE_INT;
	IncreaseCount();
}

void MonoArguments::AddFloat(float value)
{
	PrimitiveStorage[Count].floatValue = value;
	Arguments[Count].pointerToValue = &PrimitiveStorage[Count].floatValue;
	ArgumentTypes[Count] = ARGTYPE_FLOAT;
	IncreaseCount();
}

void MonoArguments::AddString(const char* str)
{
	MonoString* mono = mono_string_new_wrapper(str);
	Arguments[Count].monoObject = (MonoObject*)mono;
	ArgumentTypes[Count] = ARGTYPE_STRING;
	IncreaseCount();
}

void MonoArguments::AddMonoString(MonoString* str)
{
	Arguments[Count].monoObject = (MonoObject*)str;
	ArgumentTypes[Count] = ARGTYPE_STRING;
	IncreaseCount();
}

void MonoArguments::AddMonoObject(MonoObject* object)
{
	Arguments[Count].monoObject = object;
	ArgumentTypes[Count] = ARGTYPE_STRING;
	IncreaseCount();
}

void MonoArguments::AddOutObject(MonoObjectPtr* outObject)
{
	Arguments[Count].pointerToValue = outObject;
	ArgumentTypes[Count] = ARGTYPE_OUT_OBJECT;
	IncreaseCount();
}

void MonoArguments::AddIntPtr(void* value)
{
	PrimitiveStorage[Count].intPtrValue = value;
	Arguments[Count].pointerToValue = &PrimitiveStorage[Count].intPtrValue;
	ArgumentTypes[Count] = ARGTYPE_INTPTR;
	IncreaseCount();
}

void MonoArguments::AddStruct(const void* pointerToStruct)
{
	Arguments[Count].pointerToValue = pointerToStruct;
	ArgumentTypes[Count] = ARGTYPE_STRUCT;
	IncreaseCount();
}

void MonoArguments::AddEnum(int value)
{
	AddInt(value);
	ArgumentTypes[Count - 1] = ARGTYPE_ENUM;
}

bool MonoArguments::GetBooleanAt(int index) const
{
	return PrimitiveStorage[index].boolValue == 1;
}

void** MonoArguments::InMonoFormat()
{
	return (void**) & Arguments[0];
}

void MonoArguments::IncreaseCount()
{
	Count++;
	assert(Count <= MAX_ARGS);
}

void MonoArguments::CopyFromOther(const MonoArguments& other)
{
	Count = 0;
	int count = other.Count;

	for (int i = 0; i < Count; i++)
	{
		switch (int argumentType = other.ArgumentTypes[i])
		{
		case ARGTYPE_BOOLEAN:
			AddBoolean(other.GetBooleanAt(i));
			break;

		default:
			std::cerr << "Not Supported yet!";
		}
	}
}
