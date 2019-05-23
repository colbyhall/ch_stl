workspace "ch_stl"
    architecture "x64"
    startproject "ch_stl"
    
    configurations
    {
        "Debug",
        "Release"
    }

    targetdir ("bin")
    objdir ("bin")

project "ch_stl"
    kind "ConsoleApp"
    language "C++"

    characterset("ASCII")

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    files
    {
        "src/*.h",
        "src/*.cpp",
    }

    includedirs
    {
        "src/**",
        "libs/"
    }

    links
    {
        "opengl32",
        "user32",
        "kernel32",
	"shlwapi",
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

	files 
	{
            "src/win32/**.h",
	    "src/win32/**.cpp"
	}