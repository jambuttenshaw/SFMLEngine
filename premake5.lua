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


include "SFMLEngine"
include "Sandbox"
