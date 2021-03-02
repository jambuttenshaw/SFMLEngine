workspace "SFMLEngine"
    architecture "x86_64"
		startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["SFML"] = "%{wks.location}/SFMLEngine/vendor/SFML/include"
IncludeDir["spdlog"] = "%{wks.location}/SFMLEngine/vendor/spdlog/include"
IncludeDir["glew"] = "%{wks.location}/SFMLEngine/vendor/glew/include"
IncludeDir["tracy"] = "%{wks.location}/SFMLEngine/vendor/tracy"
IncludeDir["json"] = "%{wks.location}/SFMLEngine/vendor/json/single_include"


LibraryDir = {}
LibraryDir["SFML"] = "%{wks.location}/SFMLEngine/vendor/SFML/lib"
LibraryDir["glew"] = "%{wks.location}/SFMLEngine/vendor/glew/lib/Release/x64"

project "SFMLEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	location("%{wks.location}/SFMLEngine")

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"SFMLEngine/src/**.h",
		"SFMLEngine/src/**.cpp",
		"SFMLEngine/vendor/tracy/TracyClient.cpp"
	}

	defines
	{
		"SFML_STATIC",
		"GLEW_STATIC",
		"TRACY_ENABLE"
	}

	includedirs
	{
		"%{wks.location}/SFMLEngine/src",
		"%{IncludeDir.SFML}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glew}",
		"%{IncludeDir.tracy}",
		"%{IncludeDir.json}"
	}
	
	libdirs
	{
		"%{LibraryDir.SFML}",
		"%{LibraryDir.glew}"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		defines 
		{
			"SFMLENGINE_ENABLE_ASSERTS",
			"SFMLENGINE_DEBUG",
			"_CRTDBG_MAP_ALLOC"
		}
		
		links
		{
			"winmm.lib",
			"opengl32.lib",
			"freetype.lib",
			"glew32s.lib",
			"sfml-graphics-s-d.lib",
			"sfml-window-s-d.lib",
			"sfml-system-s-d.lib"

		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		
		links
		{
			"winmm.lib",
			"opengl32.lib",
			"freetype.lib",
			"glew32s.lib",
			"sfml-graphics-s.lib",
			"sfml-window-s.lib",
			"sfml-system-s.lib"
		}





project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	location("%{wks.location}/Sandbox")
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"Sandbox/src/**.h",
		"Sandbox/src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/SFMLEngine/src",
		"%{wks.location}/Sandbox/assets/scripts",
		"%{IncludeDir.SFML}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glew}",
		"%{IncludeDir.tracy}",
		"%{IncludeDir.json}"
	}

	links
	{
		"SFMLEngine"
	}

	libdirs
	{
		"%{LibraryDir.SFML}",
		"%{LibraryDir.glew}"
	}
	
	defines
	{
		"SFML_STATIC"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		defines 
		{
			"SFMLENGINE_ENABLE_ASSERTS",
			"SFMLENGINE_DEBUG",
			"_CRTDBG_MAP_ALLOC"
		}
		
		links
		{
			"winmm.lib",
			"opengl32.lib",
			"freetype.lib",
			"glew32s.lib",
			"sfml-graphics-s-d.lib",
			"sfml-window-s-d.lib",
			"sfml-system-s-d.lib"

		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		
		links
		{
			"winmm.lib",
			"opengl32.lib",
			"freetype.lib",
			"glew32s.lib",
			"sfml-graphics-s.lib",
			"sfml-window-s.lib",
			"sfml-system-s.lib"
		}

