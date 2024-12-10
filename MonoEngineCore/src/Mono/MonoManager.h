#pragma once
#include "MonoScriptCache.h"

struct CommonScriptingClasses
{
	MonoClassPtr mainClass;
	MonoClassPtr monoBehaviour;
};

class MonoManager
{
public:
	enum AssemblyType
	{
		kEngineAssembly = 0,
		kScriptAssembly,
	};
	const char* ScriptAssemblyName = "Assembly-CSharp.dll";

	bool ReloadAssembly();
	MonoAssembly* GetAssembly(const char* name);
	MonoAssembly* GetScriptAssembly();
	MonoAssembly* GetEngineAssembly();
	MonoImagePtr GetImage(const char* dllName);
	MonoImagePtr GetScriptImage();
	MonoImagePtr GetEngineImage();

	MonoClassPtr GetMonoClass(const char* className, const char* theNameSpace = nullptr);
	MonoClassPtr GetMonoClassWithoutCorlib(const char* className, const char* theNameSpace);
	MonoClassPtr GetScriptClass(const char* className, const char* theNameSpace);
	MonoClassPtr GetEngineClass(const char* className, const char* theNameSpace);

	void RegisterMonoScriptCache(MonoScriptCache* msc);
	void UnregisterMonoScriptCache(const MonoScriptCache::IdentifierHashType hash);
	MonoScriptCache* GetMonoScriptCache(const MonoScriptCache::IdentifierHashType identifierHash);
	const CommonScriptingClasses& GetCommonScriptingClasses() const { return _commonScriptingClasses; }

	void FillCommonScriptingClasses();
	void DebugInitAssemblies();
	void ListTypesInScriptImage(); // Debug method
private:
	int GetAssemblyCount() { return (int) _assemblyNames.size(); }
	bool LoadAssembly(int assemblyIndex);
	void UnloadScriptAssemblies();
	void UnloadSystemAssemblies();
	MonoDomain* CreateAndSetChildDomain();

private:
	std::vector<std::string> _assemblyNames;
	std::vector<std::string> _assemblyPaths;
	std::vector<AssemblyType> _assemblyTypes;
	std::vector<MonoAssembly*> _assemblies;

	typedef std::map<MonoScriptCache::IdentifierHashType, MonoScriptCache*> MonoScriptCaches;
	MonoScriptCaches _caches;

	CommonScriptingClasses _commonScriptingClasses;
};

MonoManager& GetMonoManager();
inline const CommonScriptingClasses& GetCommonScriptingClasses() { return GetMonoManager().GetCommonScriptingClasses(); }

MonoMethodPtr FindStaticMonoMethod(const char* nameSpace, const char* className, const char* methodName);
MonoMethodPtr FindStaticMonoMethod(MonoImagePtr image, const char* nameSpace, const char* className, const char* methodName);

bool InitializeMonoFromMain(const std::vector<std::string>& monoPaths, const std::string& monoConfigPath, const std::string& dataPath);
void CleanupMono();
void CallScriptingMain(); // Test function

