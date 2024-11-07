#pragma once
#include "VirtualFileSystem.h"

class LocalFileSystemWindows : public FileSystemHandler
{
public:
	~LocalFileSystemWindows() = default;

	std::string GetApplicationPath() const override;
	std::string GetApplicationFolder() const override;
	std::string GetApplicationContentsFolder() const override;

private:
	mutable std::string _cachedApplicationPath;
};