local function set_work_directory (dir)
    local work_dir = path.getabsolute(dir)
    os.chdir(work_dir)
    print("Work directory: " .. work_dir)
end

set_work_directory("../")
workspace "MonoEngine"
	architecture "x86_64"
	startproject "MonoEngineCore"
    configurations
	{
		"Debug",
	 	"Release",
	}

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

set_work_directory("MonoEngineCore")
project "MonoEngineCore"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}/")

	pchheader "mepch.h"
	pchsource "src/mepch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

	includedirs
    {
        "src"
    }

    links
    {
        "MonoEngine",
        "Assembly-CSharp"
    }

    postbuildcommands
    {
        "{COPYDIR} %{prj.location}Data %{cfg.targetdir}/Data"
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        optimize "Full"
        symbols "Default"

set_work_directory("../MonoEngine")
project "MonoEngine"	
    kind "SharedLib"
    language "C#"
    dotnetframework "4.8.1"

    targetdir ("../MonoEngineCore/Data/Managed")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}/")

    files
    {
        "**.cs"
    }

    filter "configurations:Debug"
    optimize "Off"
    symbols "On"

    filter "configurations:Release"
    optimize "On"
    symbols "Default"

set_work_directory("../Assembly-CSharp")
project "Assembly-CSharp"	
    kind "SharedLib"
    language "C#"
    dotnetframework "4.8.1"

    targetdir ("../MonoEngineCore/Data/Managed")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}/")

    files
    {
        "**.cs"
    }

    links
    {
        "MonoEngine"
    }

    filter "configurations:Debug"
    optimize "Off"
    symbols "On"

    filter "configurations:Release"
    optimize "On"
    symbols "Default"

group "Test"
set_work_directory("../MonoApiTest")
project "MonoApiTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}/")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
        "include"
    }

    links
    {
        "lib/mono-2.0-sgen.lib",
        "MonoScriptTest"
    }

    postbuildcommands
    {
        "{COPYDIR} %{prj.location}bin %{cfg.targetdir}"
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        optimize "Full"
        symbols "Default"

set_work_directory("../MonoScriptTest")
project "MonoScriptTest"	
    kind "SharedLib"
	language "C#"
	dotnetframework "4.8.1"

    targetdir ("../MonoApiTest/assembly")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}/")

    files
    {
        "**.cs"
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"
group ""