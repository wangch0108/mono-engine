#include "mepch.h"
#include "EditorMonoLoader.h"
#include "Mono/MonoLoader.h"
#include "Utility/PathNameUtility.h"
#include "Utility/VirtualFileSystem.h"

static const std::string kMonoLibraryName = "mono-2.0-sgen.dll";

bool LoadMonoForEditor(const std::string& dataFolder)
{
	std::string dllPath, monoBaseDir;
	GetMonoPaths(dllPath, monoBaseDir);
	const std::string& managedPath = AppendPathName(dataFolder, "Managed");

	std::vector<std::string> monoPaths;
	monoPaths.push_back(AppendPathName(monoBaseDir, "lib"));
	return LoadAndInitializeMono(monoPaths, AppendPathName(monoBaseDir, "etc"), managedPath, dllPath);
}

void GetMonoPaths(std::string& dllPath, std::string& distributionBasePath)
{
	auto dataFolder = GetFileSystem().GetApplicationContentsFolder();
	distributionBasePath = AppendPathName(dataFolder, "Mono");
	dllPath = AppendPathName(distributionBasePath, "/EmbedRuntime/" + kMonoLibraryName);
}