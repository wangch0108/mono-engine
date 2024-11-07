#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <string>

namespace MonoApiTest
{
	MonoDomain* InitMono();
	char* ReadBytes(const std::string& filepath, uint32_t* outSize);
	MonoAssembly* LoadAssembly(const std::string& assemblyPath);
	void PrintAssemblyTypes(MonoAssembly* assembly);
	MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* name_space, const char* className);
	MonoObject* InstantiateClass(const char* name_space, const char* className);
	void TestCallMethod(MonoObject* obj);
	void TestField(MonoObject* obj);
}