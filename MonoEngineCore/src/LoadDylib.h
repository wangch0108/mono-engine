#pragma once

/*
 * Unity �ṩ���ּ��ض�̬��ͻ�ȡ��̬���з��ŵ�ַ�ķ�ʽ������Ҳ����������ʵ�֣�Ҳ����������ͬʱ֧��il2cpp
 * ��Ȼ������˵����dll+lib�ķ�ʽ���ӷ�����һ��
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