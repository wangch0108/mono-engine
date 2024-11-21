#pragma once
#include "MonoArguments.h"
#include "MonoTypes.h"

class MonoInvocation
{
public:
	MonoInvocation();
	MonoInvocation(MonoMethodPtr method);
	MonoInvocation(MonoObjectPtr targetObject, MonoMethodPtr method);

	// TODO other type case...
	void AddInt(int value) { _arguments.AddInt(value); }

	template<typename T> T Invoke(MonoExceptionPtr* exception);
	template<typename T> T Invoke() { MonoExceptionPtr ex = MONO_NULL;  return Invoke<T>(&ex); }

	MonoObjectPtr Invoke(MonoExceptionPtr* exception);
	MonoObjectPtr Invoke() { MonoExceptionPtr ex = MONO_NULL; return Invoke(&ex); }

private:
	void SetDefaults();
	template<typename T> T InvokeHandlingReturnValue(MonoExceptionPtr* exception);

	MonoMethodPtr _method;
	MonoObjectPtr _targetObject;
	MonoArguments _arguments;
};

template <> bool MonoInvocation::Invoke<bool>(MonoExceptionPtr* exception);
template <> int MonoInvocation::Invoke<int>(MonoExceptionPtr* exception);
template <> float MonoInvocation::Invoke<float>(MonoExceptionPtr* exception);
template <> void MonoInvocation::Invoke<void>(MonoExceptionPtr* exception);
template <> MonoObjectPtr MonoInvocation::Invoke<MonoObjectPtr>(MonoExceptionPtr* exception);
template <> MonoStringPtr MonoInvocation::Invoke<MonoStringPtr>(MonoExceptionPtr* exception);

template <typename T>
T MonoInvocation::Invoke(MonoExceptionPtr* exception)
{
	static_assert(sizeof(T) == 0, "Not supported yet!");
	return T{};
}
