#pragma once

class FileSystemHandler
{
public:
	virtual ~FileSystemHandler() = default;
	virtual std::string GetApplicationPath() const = 0;
	virtual std::string GetApplicationFolder() const = 0;
	virtual std::string GetApplicationContentsFolder() const = 0;
};

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	std::string GetApplicationPath() const;
	std::string GetApplicationFolder() const;
	std::string GetApplicationContentsFolder() const;

private:
	FileSystemHandler* _localFileSystemHandler;
};

FileSystem& GetFileSystem();