#pragma once

struct MonoString;
struct MonoObject;
class MonoObjectPtr;

struct MonoArguments
{
	enum Constants
	{
		MAX_ARGS = 12
	};

	enum ArgType
	{
		ARGTYPE_BOOLEAN,
		ARGTYPE_INT,
		ARGTYPE_FLOAT,
		ARGTYPE_DOUBLE,
		ARGTYPE_STRING,
		ARGTYPE_OBJECT,
		ARGTYPE_STRUCT,
		ARGTYPE_ARRAY,
		ARGTYPE_ENUM,
		ARGTYPE_INT64,
		ARGTYPE_INTPTR,
		ARGTYPE_UINT64,
		ARGTYPE_OUT_OBJECT,
	};

	struct PrimitiveValue
	{
		union
		{
			signed char boolValue; // TODO: byte also ok?
			int intValue;
			float floatValue;
			double doubleValue;
			int64_t int64Value;
			uint64_t uint64Value;
			void* intPtrValue;
		};
	};

	PrimitiveValue PrimitiveStorage[MAX_ARGS] = {};

	struct Argument
	{
		union
		{
			const void* pointerToValue;
			MonoObject* monoObject;
		};

		Argument& operator=(Argument other)
		{
			pointerToValue = other.pointerToValue;
			return *this;
		}

		Argument()
		{
			pointerToValue = nullptr;
		}
	};

	Argument Arguments[MAX_ARGS] = {};

	MonoArguments();
	MonoArguments(const MonoArguments& other);
	MonoArguments& operator=(const MonoArguments& other);

	int ArgumentTypes[MAX_ARGS] = {};
	int Count;

	void AddBoolean(bool value);
	void AddInt(int value);
	void AddFloat(float value);
	void AddString(const char* str);
	void AddString(const std::string& str) { AddString(str.c_str()); }
	void AddMonoString(MonoString* str);
	void AddMonoObject(MonoObject* object);
	void AddOutObject(MonoObjectPtr* outObject);
	void AddIntPtr(void* value);
	void AddStruct(const void* pointerToStruct);
	void AddEnum(int value);

	bool GetBooleanAt(int index) const;
	void** InMonoFormat();

private:
	void IncreaseCount();
	void CopyFromOther(const MonoArguments& other);
};