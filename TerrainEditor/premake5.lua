project "TerrainEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/SFMLEngine/src",
		"assets/scripts",
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