#include "mepch.h"
#include "MonoScriptCache.h"
#include "MonoManager.h"
#include "MonoApi.h"

// enum { kUpdate = 0, kLateUpdate, kFixedUpdate, kAwake, kInternalAwake, kStart, kOnEnable, kOnDisable, kOnDestroy, kMethodCount };
const char* GMethodNames[] =
{
	"Update", "LateUpdate", "FixedUpdate", "Awake", "__internalAwake", "Start", "OnEnable", "OnDisable", "OnDestroy", nullptr
};

MonoScriptCache::MonoScriptCache()
{
	IdentifierHash = 0;
}

MonoScriptCache::~MonoScriptCache()
{
}

void MonoScriptCache::Release() const
{
	auto cache = const_cast<MonoScriptCache*>(this);
	if (cache->refCounter.Release())
	{
		delete cache;
	}
	else
		GetMonoManager().UnregisterMonoScriptCache(IdentifierHash);
}

void MonoScriptCache::Retain() const
{
	const_cast<AtomicRefCounter&>(refCounter).Retain();
	auto cache = const_cast<MonoScriptCache*>(this);
	GetMonoManager().RegisterMonoScriptCache(cache);
}

int MonoScriptCache::RefCount() const
{
	return refCounter.Count();
}

static MonoScriptCache::IdentifierHashType GenerateMonoScriptCacheHash(MonoClassPtr klass)
{
	void* hash = klass.Hash();
	return reinterpret_cast<MonoScriptCache::IdentifierHashType>(hash);
}

static void PopulateMethods(MonoScriptCache& cache, MonoClassPtr klass)
{
	typedef std::vector<MonoMethodPtr> MethodVector;
	typedef std::map<const char*, MonoMethodPtr> MethodMap;

	// Check all methods we support
	MethodVector allMethods;
	mono_class_get_methods_ex(klass, allMethods);

	MethodMap methodMap;
	for (MethodVector::const_reverse_iterator method = allMethods.rbegin(), end = allMethods.rend(); method != end; ++method)
	{
		if (method->IsInstance())
			methodMap[mono_method_get_name(method->GetNativeMethod())] = *method;
	}

	cache.methods.resize(MonoScriptCache::kMethodCount);
	for (int i = 0; i < MonoScriptCache::kMethodCount; i++)
	{
		if (GMethodNames[i] == nullptr)
			continue;

		MethodMap::const_iterator it = methodMap.find(GMethodNames[i]);
		MonoMethodPtr method = it == methodMap.end() ? nullptr : it->second;
		if (!method.IsNull())
		{
			if (mono_method_get_argument_count(method) != 0)
			{
				method = nullptr;
				auto className = mono_class_get_name(klass);
				std::cerr << "Script error (" << className << "): " << GMethodNames[i] << "() can not take parameters.";
			}
		}

		cache.methods[i] = method;
	}
}

MonoScriptCache* CreateMonoScriptCache(MonoClassPtr klass)
{
	auto cache = new MonoScriptCache();
	cache->klass = klass;
	cache->IdentifierHash = GenerateMonoScriptCacheHash(klass);
	PopulateMethods(*cache, klass);

	return cache;
}

MonoScriptCache* FindOrCreateMonoScriptCache(MonoClassPtr klass)
{
	if (auto msc = GetMonoManager().GetMonoScriptCache(GenerateMonoScriptCacheHash(klass)))
		return msc;

	return CreateMonoScriptCache(klass);
}
