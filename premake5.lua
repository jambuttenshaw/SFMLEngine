workspace "SFMLEngine"
    architecture "x86_64"

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


LibraryDir = {}
LibraryDir["SFML"] = "%{wks.location}/SFMLEngine/vendor/SFML/lib"
LibraryDir["glew"] = "%{wks.location}/SFMLEngine/vendor/glew/lib/Release/x64"

project "SFMLEngine"
    kind "ConsoleApp"
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
		"src",
		"%{IncludeDir.SFML}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glew}",
		"%{IncludeDir.tracy}"
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
		defines "SFMLENGINE_ENABLE_ASSERTS"
		
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
