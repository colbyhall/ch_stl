workspace "ch_stl_test"
	architecture "x64"
	startproject "ch_test"

	configurations
    {
        "Debug",
        "Release"
    }

	targetdir ("../bin")
	objdir ("../bin")
	debugdir ("../bin")
	characterset ("ascii")

include ".."

project "ch_test"
	kind "ConsoleApp"
    language "C++"

	dependson 
	{
		"ch_stl"
	}

	files 
	{
		"*.h",
		"*.cpp"
	}

	links
	{
		"../bin/ch_stl.lib",
		
	}

	includedirs
	{
		"../src"
	}