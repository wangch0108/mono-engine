#pragma once
#include "MonoIncludes.h"

class MonoManager
{
public:
};

bool InitializeMonoFromMain(const std::vector<std::string>& monoPaths, const std::string& monoConfigPath, const std::string& dataPath);