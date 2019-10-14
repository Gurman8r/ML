-- Memelib
workspace "MemeLib"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release"
	}

-- Global Directories
sln_dir = "%{wks.location}/"
bin_dir = sln_dir .. "bin/%{cfg.buildcfg}/%{cfg.architecture}"
lib_dir = sln_dir .. "lib/%{cfg.buildcfg}/%{cfg.architecture}"
tmp_dir = sln_dir .. "obj/%{cfg.buildcfg}/%{cfg.architecture}"

-- Core
project "Core"
	-- General
	location sln_dir .. "proj/ML/%{prj.name}"
	kind "SharedLib"
	language "C++"
	targetdir (lib_dir)
	objdir (tmp_dir)

	-- Files
	files
	{
		sln_dir .. "include/ML/%{prj.name}/**.hpp",
		sln_dir .. "src/ML/%{prj.name}/**.cpp"
	}

	-- Additional Include Directories
	includedirs
	{
		sln_dir .. "include",
		sln_dir .. "thirdparty/include"
	}

	-- Preprocessor Definitions
	defines
	{
		"ML_CORE_EXPORTS",
		"_CRT_SECURE_NO_WARNINGS"
	}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "On"

	-- Windows
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"