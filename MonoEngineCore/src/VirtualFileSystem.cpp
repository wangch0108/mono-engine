#include "mepch.h"
#include "VirtualFileSystem.h"

#if PLATFORM_WIN
#include "LocalFileSystemWindows.h"
#define OS_FILE_SYSTEM_HANDLER LocalFileSystemWindows
#else
#error Please implement a Virtual File System handler for this platform.
#endif

static FileSystem GFileSystem;

FileSystem::FileSystem()
{
	_localFileSystemHandler = new OS_FILE_SYSTEM_HANDLER();
}

FileSystem::~FileSystem()
{
	delete _localFileSystemHandler;
}

std::string FileSystem::GetApplicationPath() const
{
	return _localFileSystemHandler->GetApplicationPath();
}

std::string FileSystem::GetApplicationFolder() const
{
	return _localFileSystemHandler->GetApplicationFolder();
}

std::string FileSystem::GetApplicationContentsFolder() const
{
	return _localFileSystemHandler->GetApplicationContentsFolder();
}

FileSystem& GetFileSystem()
{
	return GFileSystem;
}