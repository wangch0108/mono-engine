#include "mepch.h"
#include "PathNameUtility.h"

static void AppendPathNameImpl(const std::string& pathName, const std::string& append, char separator, std::string& res)
{
	res.reserve(pathName.size() + append.size() + 1);
	if (!pathName.empty() && !append.empty())
	{
		if (pathName[pathName.size() - 1] == separator)
		{
			if (append[0] == separator)
			{
				res += pathName;
				res.append(append.begin() + 1, append.end());
				return;
			}
			else
			{
				res += pathName;
				res += append;
				return;
			}
		}
		else
		{
			if (append[0] == separator)
			{
				res += pathName;
				res += append;
				return;
			}
			else
			{
				res += pathName;
				res += separator;
				res += append;
				return;
			}
		}
	}
	else if (pathName.empty())
		res = append;
	else
		res = pathName;
}

std::string AppendPathName(const std::string& pathName, const std::string& append)
{
	std::string res;
	AppendPathNameImpl(pathName, append, kPathNameSeparator, res);
	return res;
}

static std::string DeleteLastPathNameComponentImpl(const std::string& pathName, const char* separators)
{
	auto pos = pathName.find_last_not_of(separators);
	if (pos == std::string::npos)
		return std::string();

	pos = pathName.find_last_of(separators, pos);
	if (pos == std::string::npos)
		return std::string();

	pos = pathName.find_last_not_of(separators, pos);
	if (pos == std::string::npos)
		return std::string();

	return pathName.substr(0, pos + 1);
}

std::string DeleteLastPathNameComponent(const std::string& pathName)
{
	char separators[] = { kPathNameSeparator, '\0' };
	return DeleteLastPathNameComponentImpl(pathName, separators);
}

void ConvertSeparatorsToPlatform(std::string& pathName)
{
	if (kPlatformPathNameSeparator == kPathNameSeparator)
		return;

	auto it = pathName.begin(), itEnd = pathName.end();
	while (it != itEnd)
	{
		if (*it == kPathNameSeparator)
			*it = kPlatformPathNameSeparator;
		++it;
	}
}
