#include "mepch.h"
#include "Application.h"
#include "EditorMonoLoader.h"
#include "Mono/MonoManager.h"
#include "Utility/VirtualFileSystem.h"

static Application GApplication;

void Application::Init()
{
	std::cout << "Application contents path: " << GetFileSystem().GetApplicationContentsFolder() << '\n';
	LoadMonoForEditor();

	/*
	 * Scripting test flow:
	 * 1. Call("CSharpInit"); // C Invoke C#
	 * 2. var ob = GameObject.Find("xx"); // C# Invoke C
	 * 3. ob.AddComponent("xx_comp"); // C# Invoke C
	 *	3.1. Create MonoBehaviour, SetClass // C Logic
	 *	3.2. AwakeFromLoad CallMethod(e.g. Awake/OnEnable/Start) // C Invoke C#
	 *	3.3. Register behaviour to PlayerLoop // C Logic
	 * 4. Call Update/LateUpdate // C Invoke C#
	 */

	CallScriptingMain();
}

Application& GetApplication()
{
	return GApplication;
}
