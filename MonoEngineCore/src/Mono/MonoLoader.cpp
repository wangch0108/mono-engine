#include "mepch.h"
#include "MonoLoader.h"
#include "MonoManager.h"
#include "Utility/LoadDylib.h"

#define DO_API(r, n, p) typedef r (*fp##n##Type) p;
#include "MonoFunctions.h"

#define DO_API(r, n, p) fp##n##Type n = nullptr;
#include "MonoFunctions.h"

static std::string GMonoModulePath;
static void* GMonoModule;

bool LoadMono(const std::string& libraryPath)
{
	GMonoModule = LoadDynamicLibrary(libraryPath);
	if (!GMonoModule)
	{
		std::cerr << "Unable to load mono library from: " << libraryPath << '\n';
		return false;
	}

	GMonoModulePath = libraryPath;
	bool funcsOK = true;
#define DO_API(r, n, p)	n = (fp##n##Type) LookupSymbol(GMonoModule, #n); if ( !n ) { funcsOK = false; std::cerr << "mono: function " #n " not found\n"; }
#define DO_API_OPTIONAL(r, n, p) n = (fp##n##Type) LookupSymbol(GMonoModule, #n, kSymbolOptional);
#include "MonoFunctions.h"

	if (!funcsOK)
	{
		std::cerr << "mono: function lookup failed!\n";
		UnloadDynamicLibrary(GMonoModule);
		GMonoModule = nullptr;
		return false;
	}

	return true;
}

bool LoadAndInitializeMono(const std::vector<std::string>& monoPaths, const std::string& monoConfigPath,
	const std::string& dataPath, const std::string& monoDll)
{
	if (!LoadMono(monoDll))
		return false;

	if (!InitializeMonoFromMain(monoPaths, monoConfigPath, dataPath))
	{
		std::cerr << "mono: initialization failed!\n";
		return false;
	}

	return true;
}