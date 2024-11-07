#pragma once

constexpr char kPathNameSeparator = '/';

std::string AppendPathName(const std::string& pathName, const std::string& append);
std::string DeleteLastPathNameComponent(const std::string& pathName);