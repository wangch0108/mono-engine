#include "mepch.h"
#include "LoadDylib.h"
#include "WinUnicode.h"

typedef std::map<std::string, void*> LibraryMap;
static LibraryMap GLoaded;

void* GetLoadedLibrary(const std::string& absolutePath)
{
	void* library = nullptr;
	if (GLoaded.find(absolutePath) != GLoaded.end())
	{
		library = GLoaded[absolutePath];
	}
	else
	{
#if PLATFORM_WIN
		std::wstring widePath;
		ConvertUTF8ToWideString(absolutePath.c_str(), widePath);
		HINSTANCE module = GetModuleHandleW(widePath.c_str());
		library = module;
#endif

		if (library)
			GLoaded[absolutePath] = library;
	}

	return library;
}

void* LoadDynamicLibrary(const std::string& absolutePath, bool exposeSymbolsGlobally)
{
	void* library = nullptr;
	if (GLoaded.find(absolutePath) != GLoaded.end())
	{
		library = GLoaded[absolutePath];
	}
	else
	{
#if PLATFORM_WIN
		std::wstring widePath;
		ConvertUTF8ToWideString(absolutePath.c_str(), widePath);
		HINSTANCE module = LoadLibraryW(widePath.c_str());
		library = module;
#endif

		if (library)
			GLoaded[absolutePath] = library;
	}

	return library;
}

void UnloadDynamicLibrary(void* libraryReference)
{
	for (auto it = GLoaded.begin(); it != GLoaded.end(); ++it)
	{
		if (it->second == libraryReference)
		{
#if PLATFORM_WIN
			FreeLibrary((HMODULE)it->second);
#endif
		}

		GLoaded.erase(it);
		break;
	}
}

void UnloadDynamicLibrary(const std::string& absolutePath)
{
	if (GLoaded.count(absolutePath) && GLoaded[absolutePath])
	{
#if PLATFORM_WIN
		FreeLibrary((HMODULE)GLoaded[absolutePath]);
#endif
	}
	GLoaded.erase(absolutePath);
}

void* LookupSymbol(void* libraryReference, const char* symbolName, SymbolRequirement symbolRequirement)
{
#if PLATFORM_WIN
	if (!libraryReference)
		return nullptr;
	auto sym = (void*) GetProcAddress((HMODULE)libraryReference, symbolName);
	if (symbolRequirement == kSymbolRequired && !sym)
		std::cerr << "Could not load symbol " << symbolName << '\n';
#else
#error "Now unsupported yet!"
#endif

	return sym;
}
