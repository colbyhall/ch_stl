project "ch_stl"
    kind "StaticLib"
    language "C++"

    files
    {
        "src/*.h",
		"src/*.cpp",
		"src/gl/*.h"
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

	filter "configurations:Release"
		runtime "Release"
        optimize "On"
        
    filter "system:windows"
        cppdialect "C++17"
		systemversion "latest"

		files 
		{
			"src/win32/**.h",
			"src/win32/**.cpp",
		}