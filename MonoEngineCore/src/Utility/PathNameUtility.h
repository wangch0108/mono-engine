#pragma once

constexpr char kPathNameSeparator = '/';

#if PLATFORM_WIN
constexpr char kPlatformPathNameSeparator = '\\';
#else
constexpr char kPlatformPathNameSeparator = '/';
#endif

std::string AppendPathName(const std::string& pathName, const std::string& append);
std::string DeleteLastPathNameComponent(const std::string& pathName);

void ConvertSeparatorsToPlatform(std::string& pathName);