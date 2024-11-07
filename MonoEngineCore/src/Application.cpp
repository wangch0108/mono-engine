#include "mepch.h"
#include "Application.h"
#include "VirtualFileSystem.h"
#include "EditorMonoLoader.h"

static Application GApplication;

void Application::Init()
{
	std::cout << "Application contents path: " << GetFileSystem().GetApplicationContentsFolder() << '\n';
	LoadMonoForEditor();
}

Application& GetApplication()
{
	return GApplication;
}
