#include "mepch.h"
#include "Application.h"

#include "EditorMonoLoader.h"
#include "Mono/MonoLoader.h"
#include "Mono/MonoManager.h"
#include "Scripting/Behaviour.h"
#include "Utility/VirtualFileSystem.h"

#include <chrono>
#include <thread>

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
	Update();
}

void Application::Deinit()
{
	CleanupMono();
	UnloadMono();
}

void Application::Update()
{
	while (_isRunning)
	{
		// PlayerLoop ...
		GetFixedBehaviourManager().Update();
		GetBehaviourManager().Update();
		GetLateBehaviourManager().Update();

		// Simulate works
		std::this_thread::sleep_for(std::chrono::milliseconds(66)); // fps: 30
		std::cout << "Frame <" << ++_frameCount << "> finished.\n";
	}
}

Application& GetApplication()
{
	return GApplication;
}
