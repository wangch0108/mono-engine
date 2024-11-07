#include "mepch.h"
#include "MonoManager.h"

bool InitializeMonoFromMain(const std::vector<std::string>& monoPaths, const std::string& monoConfigPath,
	const std::string& dataPath)
{
	auto domain = mono_jit_init_version("Root Domain", "v4.0.30319");
	if (domain == nullptr)
		return false;

	std::cout << "Mono initialized!\n";
	std::cout << "Runtime version: " << mono_get_runtime_build_info() << '\n';
	return true;
}