#pragma once

/*
 * Unity Application
 * Data/Mono(MonoBleedingEdge)/bin : dynamic lib
 * Data/Mono(MonoBleedingEdge)/lib : static lib
 * Data/Mono(MonoBleedingEdge)/lib/mono : .NET API
 */

bool LoadMono(const std::string& libraryPath);
void UnloadMono();
bool LoadAndInitializeMono(const std::vector<std::string>& monoPaths, const std::string& monoConfigPath,
	const std::string& dataPath, const std::string& monoDll);

const char* GetMonoModulePath();