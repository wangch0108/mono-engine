#include "mepch.h"
#include "LocalFileSystemWindows.h"
#include "PlatformDependent/Win/WinUnicode.h"
#include "PathNameUtility.h"

std::string LocalFileSystemWindows::GetApplicationPath() const
{
	if (_cachedApplicationPath.empty())
	{
		std::vector<wchar_t> pathBuffer;
		DWORD dataRetrieved = 0;
		auto appModule = GetModuleHandleW(nullptr);
		do
		{
			pathBuffer.resize(pathBuffer.size() + 260);
			dataRetrieved = GetModuleFileNameW(appModule, pathBuffer.data(), (DWORD)pathBuffer.size());
		} while (dataRetrieved > pathBuffer.size());

		pathBuffer.resize(dataRetrieved);
		ConvertWideStringToUTF8(pathBuffer.data(), _cachedApplicationPath);
	}

	return _cachedApplicationPath;
}

std::string LocalFileSystemWindows::GetApplicationFolder() const
{
	return DeleteLastPathNameComponent(GetApplicationPath());
}

std::string LocalFileSystemWindows::GetApplicationContentsFolder() const
{
	return AppendPathName(GetApplicationFolder(), "Data");
}