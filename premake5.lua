project "ch_stl"
    kind "StaticLib"
    language "C++"

    files
    {
        "*.h",
		"*.cpp",
		"thirdparty/*.h"
    }

	links
	{
		"opengl32"
	}

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

    filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		defines 
		{
			"CH_BUILD_DEBUG#1"
		}

	filter "configurations:Release"
		runtime "Release"
        optimize "On"
        
    filter "system:windows"
        cppdialect "C++17"
		systemversion "latest"

		files 
		{
			"win32/**.h",
			"win32/**.cpp",
		}