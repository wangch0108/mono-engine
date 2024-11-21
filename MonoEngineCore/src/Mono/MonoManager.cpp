#include "mepch.h"
#include "MonoManager.h"
#include "MonoIncludes.h"
#include "MonoInvocation.h"
#include "MonoApi.h"
#include "Utility/PathNameUtility.h"
#include "Export/MonoExport.h"

#include <cassert>
#include <sstream>
#include <fstream>

#if PLATFORM_WIN
#include <csignal>
#endif

// Assembly path reference unity project
static auto EngineTargetAssemblyPath = "Data/Managed/MonoEngine.dll";
static auto ScriptTargetAssemblyPath = "Data/Managed/Assembly-CSharp.dll";
// static const char* ScriptTargetAssemblyPath = "Library/ScriptAssemblies/Assembly-CSharp.dll";

static const char* ValidMonoLogLevelStr[] = { "error", "critical", "warning", "message", "info", "debug", nullptr };
typedef enum
{
	G_LOG_LEVEL_ERROR, G_LOG_LEVEL_CRITICAL, G_LOG_LEVEL_WARNING,
	G_LOG_LEVEL_MESSAGE, G_LOG_LEVEL_INFO, G_LOG_LEVEL_DEBUG, G_LOG_OFF
} MonoLogLevelFlags;

static const char* ValidMonoLogAreaStr[] = { "asm", "type", "dll", "gc", "cfg", "aot", "security", "all", nullptr };
typedef enum
{
	MONO_TRACE_NONE = 0,
	MONO_TRACE_ASSEMBLY = 1 << 0,
	MONO_TRACE_TYPE = 1 << 1,
	MONO_TRACE_DLLIMPORT = 1 << 2,
	MONO_TRACE_GC = 1 << 3,
	MONO_TRACE_CONFIG = 1 << 4,
	MONO_TRACE_AOT = 1 << 5,
	MONO_TRACE_SECURITY = 1 << 6,
	MONO_TRACE_ALL = ~(1 << 7)
} MonoLogAreaFlags;

static MonoLogLevelFlags GMonoLogLevel = G_LOG_LEVEL_WARNING;

// Call like mono_trace_set_mask_string("asm, dll"). see https://github.com/corngood/mono/blob/master/mono/utils/mono-logger.c#L209
static auto GMonoLogArea = (MonoLogAreaFlags)(MONO_TRACE_ASSEMBLY | MONO_TRACE_DLLIMPORT);

static MonoManager GMonoManager;

// ------------------------------ Setup functions begin ------------------------------ //

static void MonoLogHandler(const char* log_domain, const char* log_level, const char* message, int fatal, void* user_data)
{
	if (fatal)
		printf("[FATAL ERROR]\n");
	printf("[%s] [%s] %s\n", log_domain ? log_domain : "mono", log_level, message);
}

static void MonoPrintSetup()
{
	if (GMonoLogLevel == G_LOG_OFF || GMonoLogArea == MONO_TRACE_NONE)
		return;

	mono_trace_set_log_handler(MonoLogHandler, nullptr);
	mono_trace_set_level_string(ValidMonoLogLevelStr[GMonoLogLevel]);

	std::stringstream ss;
	int index = 0;
	do
	{
		if (GMonoLogArea & 1 << index)
		{
			ss << ValidMonoLogAreaStr[index];
			ss << ',';
		}

		index++;
	} while (ValidMonoLogAreaStr[index]);

	auto mask = ss.str();
	if (!mask.empty() && mask.back() == ',')
		mask.pop_back();

	mono_trace_set_mask_string(mask.c_str());
}

static void DECLSPEC_NORETURN HandleAbort(int signal);

static void SetupSignalHandlers()
{
#if PLATFORM_WIN
	_set_abort_behavior(0, _WRITE_ABORT_MSG);
	(void)signal(SIGABRT, HandleAbort);
	mono_set_signal_chaining(1); // This causes mono to forward the signal to us, if the signal is not mono related.
#endif
}

static void SetupMonoPaths(const std::vector<std::string>& monoPaths, const std::string& monoConfigPath)
{
	std::string assemblyDir = monoPaths[0];
	std::string configDir = monoConfigPath;
	ConvertSeparatorsToPlatform(assemblyDir);
	ConvertSeparatorsToPlatform(configDir);
	mono_set_dirs(assemblyDir.c_str(), configDir.c_str());
}

// ------------------------------ Setup functions end ------------------------------ //

bool MonoManager::ReloadAssembly()
{
	if (!CreateAndSetChildDomain())
		return false;

	for (int i = 0; i < GetAssemblyCount(); i++)
	{
		if (!LoadAssembly(i))
			return false;
	}

	return true;
}

MonoAssembly* MonoManager::GetAssembly(const char* name)
{
	auto assemblyIndex = -1;
	for (int i = 0; i < GetAssemblyCount(); i++)
	{
		if (_assemblyNames[i] == name)
		{
			assemblyIndex = i;
			break;
		}
	}

	if (assemblyIndex == -1)
		return nullptr;

	return _assemblies[assemblyIndex];
}

MonoAssembly* MonoManager::GetScriptAssembly()
{
	return _assemblies[kScriptAssembly];
}

MonoImagePtr MonoManager::GetImage(const char* dllName)
{
	auto ass = GetAssembly(dllName);
	return mono_assembly_get_image(ass);
}

MonoImagePtr MonoManager::GetScriptImage()
{
	return mono_assembly_get_image(GetScriptAssembly());
}

MonoClassPtr MonoManager::GetMonoClass(const char* className, const char* theNameSpace)
{
	MonoClassPtr klass = mono_class_from_name_ex(mono_get_corlib(), theNameSpace, className);
	MonoImagePtr curImage = nullptr;

	for (auto it = _assemblies.begin(), end = _assemblies.end(); it != end && klass == nullptr; ++it)
	{
		auto ass = *it;
		if (!ass) continue;
		curImage = mono_assembly_get_image(ass);
		if (!curImage)
			continue;

		klass = mono_class_from_name_ex(curImage, theNameSpace, className);
	}

	return klass;
}

MonoClassPtr MonoManager::GetMonoClassWithoutCorlib(const char* className, const char* theNameSpace)
{
	MonoClassPtr klass = nullptr;
	MonoImagePtr curImage = nullptr;

	for (auto it = _assemblies.begin(), end = _assemblies.end(); it != end && klass == nullptr; ++it)
	{
		auto ass = *it;
		if (!ass) continue;
		curImage = mono_assembly_get_image(ass);
		if (!curImage)
			continue;

		klass = mono_class_from_name_ex(curImage, theNameSpace, className);
	}

	return klass;
}

MonoClassPtr MonoManager::GetScriptClass(const char* className, const char* theNameSpace)
{
	auto scriptAssembly = GetMonoManager().GetScriptAssembly();
	auto scriptImage = mono_assembly_get_image(scriptAssembly);
	if (!scriptImage)
		return nullptr;

	return mono_class_from_name_ex(scriptImage, theNameSpace, className);
}

void MonoManager::RegisterMonoScriptCache(MonoScriptCache* msc)
{
	if (GetMonoScriptCache(msc->IdentifierHash))
		return;

	_caches[msc->IdentifierHash] = msc;
}

void MonoManager::UnregisterMonoScriptCache(const MonoScriptCache::IdentifierHashType hash)
{
	MonoScriptCaches::const_iterator element = _caches.find(hash);
	auto cache = element->second;
	if (cache->RefCount() > 1)
		return;

	// The last ref is MonoManager
	_caches.erase(hash);
	cache->Release();
}

MonoScriptCache* MonoManager::GetMonoScriptCache(const MonoScriptCache::IdentifierHashType identifierHash)
{
	auto element = _caches.find(identifierHash);
	if (element != _caches.end())
		return element->second;

	return nullptr;
}

static char* ReadStringFromFile(const std::string& filepath, uint32_t* outSize)
{
	std::ifstream in(filepath, std::ios::binary | std::ios::ate);
	if (!in)
		return nullptr;

	auto end = in.tellg();
	in.seekg(0, std::ios::beg);
	uint32_t size = (uint32_t)(end - in.tellg());
	if (size == 0)
		return nullptr;

	auto buffer = new char[size];
	in.read(buffer, size);

	*outSize = size;
	return buffer;
}

bool MonoManager::LoadAssembly(int assemblyIndex)
{
	assert(_assemblyPaths.size() > assemblyIndex);
	auto& assemblyPath = _assemblyPaths[assemblyIndex];

	uint32_t dataSize = 0;
	auto data = ReadStringFromFile(assemblyPath, &dataSize);

	int status = 0;
	MonoImagePtr image = mono_image_open_from_data_with_name(data, dataSize, true, &status, false, assemblyPath.c_str());
	if (status != 0 || image == nullptr)
	{
		std::cerr << "Failed to load assembly: " << assemblyPath << '\n';
		return false;
	}

	MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, false);
	if (status != 0 || assembly == nullptr)
	{
		std::cerr << "Failed to load assembly: " << assemblyPath << '\n';
		return false;
	}

	mono_image_close(image);
	_assemblies[assemblyIndex] = assembly;

	return true;
}

void MonoManager::UnloadScriptAssemblies()
{
	// Do this when unloading app domain
}

void MonoManager::UnloadSystemAssemblies()
{
	// Do this when unloading app domain
}

static void UnloadDomain()
{
	MonoDomain* domainToUnload = mono_domain_get();
	if (domainToUnload && domainToUnload != mono_get_root_domain())
	{
		if (!mono_domain_set(mono_get_root_domain(), false))
			std::cerr << "Exception setting domain\n";

		mono_domain_unload(domainToUnload);
		mono_gc_collect(mono_gc_max_generation()); // Is a nice point in time to have the GC run -> unity say
	}
}

MonoDomain* MonoManager::CreateAndSetChildDomain()
{
	UnloadDomain();
	UnloadSystemAssemblies();
	UnloadScriptAssemblies();

	if (MonoDomain* newDomain = mono_domain_create_appdomain("App Domain", nullptr))
	{
		if (!mono_domain_set(newDomain, false))
		{
			std::cerr << "Exception setting domain\n";
			return nullptr;
		}

		return mono_domain_get();
	}

	return nullptr;
}

void MonoManager::FillCommonScriptingClasses()
{
	_commonScriptingClasses.mainClass = GetScriptClass("Main", nullptr);
}

void MonoManager::DebugInitAssemblies()
{
	_assemblyNames = { "MonoEngine.dll", "Assembly-CSharp.dll" };
	_assemblyPaths = { EngineTargetAssemblyPath, ScriptTargetAssemblyPath };
	_assemblyTypes = { kEngineAssembly, kScriptAssembly };
	_assemblies = { nullptr, nullptr };
}

void MonoManager::ListTypesInScriptImage()
{
#if USE_MONO_METADATA
	auto ass = GetScriptAssembly();
	auto image = mono_assembly_get_image(ass);
	auto table = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int rows = mono_table_info_get_rows(table);

	for (int i = 0; i < rows; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(table, i, cols, MONO_TYPEDEF_SIZE);

		const char* namespaceName = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		const char* typeName = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

		std::cout << "Namespace: " << (namespaceName ? namespaceName : "<global>")
			<< ", Type: " << (typeName ? typeName : "<unknown>") << "\n";
	}
#endif
}

MonoManager& GetMonoManager()
{
	return GMonoManager;
}

MonoMethodPtr FindStaticMonoMethod(const char* nameSpace, const char* className, const char* methodName)
{
	MonoClassPtr klass = GetMonoManager().GetMonoClass(className, nameSpace);
	if (!klass)
		return nullptr;
	return mono_class_get_method_from_name(klass, methodName, -1);
}

MonoMethodPtr FindStaticMonoMethod(MonoImagePtr image, const char* nameSpace, const char* className,
	const char* methodName)
{
	auto klass = mono_class_from_name_ex(image, nameSpace, className);
	if (!klass)
		return nullptr;

	MonoMethodPtr method = mono_class_get_method_from_name(klass, methodName, -1);
	if (method.IsNull())
		return nullptr;

	return method;
}

bool InitializeMonoFromMain(const std::vector<std::string>& monoPaths, const std::string& monoConfigPath,
                            const std::string& dataPath)
{
	std::cout << "Initialize mono\n";
	std::cout << "Runtime version: " << mono_get_runtime_build_info() << '\n';

	MonoPrintSetup();
	SetupMonoPaths(monoPaths, monoConfigPath);
	SetupSignalHandlers();

	int opt = mono_parse_default_optimizations(nullptr);
	mono_set_defaults(0, opt);

	auto domain = mono_jit_init_version("Root Domain", "v4.0.30319");
	if (domain == nullptr)
		return false;

	mono_config_parse(nullptr);

	RegisterMonoFunctions();

	auto& manager = GetMonoManager();
	manager.DebugInitAssemblies();
	manager.ReloadAssembly(); // Script compilation pipeline
	manager.ListTypesInScriptImage();
	manager.FillCommonScriptingClasses();

	std::cout << "Mono initialize success!\n";

	return true;
}

void CallScriptingMain()
{
	auto method = FindStaticMonoMethod(GetMonoManager().GetScriptImage(), nullptr, "Main", "CSharpInit");
	if (method == nullptr)
	{
		std::cerr << "mono: C# function Main::CSharpInit not found!\n";
		return;
	}

	MonoInvocation invocation(method);
	auto result = invocation.Invoke<int>();
	std::cout << "Call CSharpInit method, result is: " << result << '\n';
}

#if PLATFORM_WIN

void DECLSPEC_NORETURN HandleAbort(int signal)
{
	// Unity handle bug reporter here
	std::cerr << "Program aborted with signal " << signal << " (SIGABRT)\n";
	_exit(EXIT_FAILURE);
}

#endif
