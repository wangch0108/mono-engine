#include "mepch.h"
#include "MonoInvocation.h"
#include "MonoIncludes.h"
#include "MonoUtility.h"

MonoInvocation::MonoInvocation()
{
	SetDefaults();
}

MonoInvocation::MonoInvocation(MonoMethodPtr method)
{
	SetDefaults();
	_method = method;
}

MonoInvocation::MonoInvocation(MonoObjectPtr targetObject, MonoMethodPtr method)
{
	SetDefaults();
	_targetObject = targetObject;
	_method = method;
}

MonoObjectPtr MonoInvocation::Invoke(MonoExceptionPtr* exception)
{
	MonoObjectPtr returnValue = MONO_NULL;
	*exception = MONO_NULL;

	{
		// TODO -> Profiler
		returnValue = mono_runtime_invoke(_method, _targetObject, _arguments.InMonoFormat(), (MonoObject**) exception);
	}

	if (!*exception)
		return returnValue;

	// TODO Log csharp stack trace...

	return MONO_NULL;
}

void MonoInvocation::SetDefaults()
{
	_targetObject = MONO_NULL;
	_method = nullptr;
	_arguments.Count = 0;
}

namespace
{
	template<class T>
	struct ReturnValueTraits
	{
		static T Convert(MonoObjectPtr o)
		{
			return ExtractMonoObjectData<T>(o);
		}
	};

	template<>
	struct ReturnValueTraits<bool>
	{
		// Mono stores bool as char while in C++ it may be of different size.
		// These specializations ensure that we always convert a correct number of bytes to C++'s bool
		static bool Convert(MonoObjectPtr o)
		{
			return (bool)ExtractMonoObjectData<char>(o);
		}
	};
}

template<typename T>
inline T MonoInvocation::InvokeHandlingReturnValue(MonoExceptionPtr* exception)
{
	MonoObjectPtr o = Invoke(exception);
	if (*exception != MONO_NULL)
		return T();

	return ReturnValueTraits<T>::Convert(o);
}


template <> bool MonoInvocation::Invoke<bool>(MonoExceptionPtr* exception)
{
	return InvokeHandlingReturnValue<bool>(exception);
}

template <> int MonoInvocation::Invoke<int>(MonoExceptionPtr* exception)
{
	return InvokeHandlingReturnValue<int>(exception);
}

template <> float MonoInvocation::Invoke<float>(MonoExceptionPtr* exception)
{
	return InvokeHandlingReturnValue<float>(exception);
}

template <> void MonoInvocation::Invoke<void>(MonoExceptionPtr* exception)
{
	Invoke(exception);
}

template <> MonoObjectPtr MonoInvocation::Invoke<MonoObjectPtr>(MonoExceptionPtr* exception)
{
	return Invoke(exception);
}

template <> MonoStringPtr MonoInvocation::Invoke<MonoStringPtr>(MonoExceptionPtr* exception)
{
	return Invoke(exception).ToMonoString();
}