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
    characterset ("ASCII")

    files
    {
        "src/*.h",
        "src/*.cpp",
    }

    includedirs
    {
        "src/**",
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

    filter "configurations:Debug"
		defines 
		{
			"BUILD_DEBUG#1",
			"BUILD_RELEASE#0"
		}
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines 
		{
			"BUILD_RELEASE#1",
			"BUILD_DEBUG#0",
			"NDEBUG"
		}
		runtime "Release"
        optimize "On"
        
    filter "system:windows"
        cppdialect "C++17"
		systemversion "latest"
		architecture "x64"

		links
		{
			"opengl32",
			"user32",
			"kernel32",
			"shlwapi"
		}

		files 
		{
			"src/win32/**.h",
			"src/win32/**.cpp"
		}

project "ch_test"
	kind "ConsoleApp"
    language "C++"
    characterset ("ASCII")

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
		"src"
	}