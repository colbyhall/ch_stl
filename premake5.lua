workspace "ch_stl"
    architecture "x64"
    startproject "ch_test"

    configurations
    {
        "Debug",
        "Release"
    }

	targetdir ("bin")
	objdir ("bin")
	debugdir ("bin")

project "ch_stl"
    kind "StaticLib"
    language "C++"
    characterset ("Unicode")

    files
    {
        "src/*.h",
		"src/*.cpp",
		"include/*.h"
    }

    includedirs
    {
        "include",
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
		architecture "x64"

		files 
		{
			"src/win32/**.h",
			"src/win32/**.cpp"
		}

project "ch_test"
	kind "ConsoleApp"
    language "C++"
    characterset ("Unicode")

	dependson 
	{
		"ch_stl"
	}

	files 
	{
		"test/*.h",
		"test/*.cpp"
	}

	links
	{
		"bin/ch_stl.lib"
	}

	includedirs
	{
		"include"
	}