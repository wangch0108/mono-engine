#pragma once

/*
 * Unity 提供这种加载动态库和获取动态库中符号地址的方式，那我也保留这样的实现，也许这样方便同时支持il2cpp
 * 当然对我来说还是dll+lib的方式更加方便快捷一点
 */

enum SymbolRequirement
{
	kSymbolRequired = 0,
	kSymbolOptional = 1,
};

void* GetLoadedLibrary(const std::string& absolutePath);
void* LoadDynamicLibrary(const std::string& absolutePath, bool exposeSymbolsGlobally = false);

void UnloadDynamicLibrary(void* libraryReference);
void UnloadDynamicLibrary(const std::string& absolutePath);

void* LookupSymbol(void* libraryReference, const char* symbolName, SymbolRequirement symbolRequirement = kSymbolRequired);