-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "ML"
	architecture "x64"
	configurations
	{
		"Debug", "Release"
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

sln_dir = "%{wks.location}/"
bin_dir = (sln_dir .. "bin/%{cfg.buildcfg}/%{cfg.architecture}")
lib_dir = (sln_dir .. "lib/%{cfg.buildcfg}/%{cfg.architecture}")
obj_dir = (sln_dir .. "obj/%{cfg.buildcfg}/%{cfg.architecture}")

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "Core"
	-- General
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	location		(sln_dir .. "proj/%{wks.name}/%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("On")
	systemversion 	("latest")

	-- Preprocessor Definitions
	defines
	{
		"ML_CORE_EXPORTS", "_CRT_SECURE_NO_WARNINGS"
	}

	-- Additional Include Directories
	includedirs
	{
		(sln_dir .. "include"),
		(sln_dir .. "thirdparty/include")
	}

	-- Headers / Source Files
	files
	{
		(sln_dir .. "include/%{wks.name}/%{prj.name}/**.hpp"),
		(sln_dir .. "src/%{wks.name}/%{prj.name}/**.cpp")
	}
	
	-- Filters
	vpaths {
		["Header Files"] = { "**.h", "**.hpp" },
		["Source Files"] = { "**.c", "**.cpp"}
	}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "On"
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "Window"
	-- General
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	location		(sln_dir .. "proj/%{wks.name}/%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("On")
	systemversion 	("latest")

	-- Preprocessor Definitions
	defines
	{
		"ML_WINDOW_EXPORTS", "_CRT_SECURE_NO_WARNINGS"
	}

	-- Additional Include Directories
	includedirs
	{
		(sln_dir .. "include"),
		(sln_dir .. "thirdparty/include")
	}

	-- Headers / Source Files
	files
	{
		(sln_dir .. "include/%{wks.name}/%{prj.name}/**.hpp"),
		(sln_dir .. "src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		(sln_dir .. "lib/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
		(sln_dir .. "thirdparty/lib/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
	}

	-- Links
	links
	{
		"ML_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"glfw"
	}
	
	-- Filters
	vpaths {
		["Header Files"] = { "**.h", "**.hpp" },
		["Source Files"] = { "**.c", "**.cpp"}
	}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "On"
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "Graphics"
	-- General
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	location		(sln_dir .. "proj/%{wks.name}/%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("On")
	systemversion 	("latest")

	-- Preprocessor Definitions
	defines
	{
		"ML_GRAPHICS_EXPORTS", "_CRT_SECURE_NO_WARNINGS"
	}

	-- Additional Include Directories
	includedirs
	{
		(sln_dir .. "include"),
		(sln_dir .. "thirdparty/include")
	}

	-- Headers / Source Files
	files
	{
		(sln_dir .. "include/%{wks.name}/%{prj.name}/**.hpp"),
		(sln_dir .. "src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		(sln_dir .. "lib/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
		(sln_dir .. "thirdparty/lib/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
	}

	-- Links
	links
	{
		"ML_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.architecture}",
		"glew32s",
		"opengl32",
		"assimp",
		"IrrXML",
		"zlibstatic"
	}
	
	-- Filters
	vpaths {
		["Header Files"] = { "**.h", "**.hpp" },
		["Source Files"] = { "**.c", "**.cpp"}
	}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "On"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "Network"
	-- General
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	location		(sln_dir .. "proj/%{wks.name}/%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("On")
	systemversion 	("latest")

	-- Preprocessor Definitions
	defines
	{
		"ML_NETWORK_EXPORTS", "_CRT_SECURE_NO_WARNINGS"
	}

	-- Additional Include Directories
	includedirs
	{
		(sln_dir .. "include"),
		(sln_dir .. "thirdparty/include")
	}

	-- Headers / Source Files
	files
	{
		(sln_dir .. "include/%{wks.name}/%{prj.name}/**.hpp"),
		(sln_dir .. "src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		(sln_dir .. "lib/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
		(sln_dir .. "thirdparty/lib/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
	}

	-- Links
	links
	{
		"ML_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"RakNet",
		"ws2_32"
	}
	
	-- Filters
	vpaths {
		["Header Files"] = { "**.h", "**.hpp" },
		["Source Files"] = { "**.c", "**.cpp"}
	}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "On"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "Audio"
	-- General
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	location		(sln_dir .. "proj/%{wks.name}/%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("On")
	systemversion 	("latest")

	-- Preprocessor Definitions
	defines
	{
		"ML_AUDIO_EXPORTS", "_CRT_SECURE_NO_WARNINGS"
	}

	-- Additional Include Directories
	includedirs
	{
		(sln_dir .. "include"),
		(sln_dir .. "thirdparty/include")
	}

	-- Headers / Source Files
	files
	{
		(sln_dir .. "include/%{wks.name}/%{prj.name}/**.hpp"),
		(sln_dir .. "src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		(sln_dir .. "lib/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
		(sln_dir .. "thirdparty/lib/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
	}

	-- Links
	links
	{
		"ML_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"flac",
		"ogg",
		"OpenAL32",
		"vorbis",
		"vorbisenc",
		"vorbisfile"
	}
	
	-- Filters
	vpaths {
		["Header Files"] = { "**.h", "**.hpp" },
		["Source Files"] = { "**.c", "**.cpp"}
	}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "On"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "Engine"
	-- General
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	location		(sln_dir .. "proj/%{wks.name}/%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("On")
	systemversion 	("latest")

	-- Preprocessor Definitions
	defines
	{
		"ML_ENGNIE_EXPORTS", "_CRT_SECURE_NO_WARNINGS"
	}

	-- Additional Include Directories
	includedirs
	{
		(sln_dir .. "include"),
		(sln_dir .. "thirdparty/include")
	}

	-- Headers / Source Files
	files
	{
		(sln_dir .. "include/%{wks.name}/%{prj.name}/**.hpp"),
		(sln_dir .. "src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		(sln_dir .. "lib/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
		(sln_dir .. "thirdparty/lib/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
	}

	-- Links
	links
	{
		"ML_Audio_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.architecture}",
		"python37.lib",
		"lua.lib"
	}
	
	-- Filters
	vpaths {
		["Header Files"] = { "**.h", "**.hpp" },
		["Source Files"] = { "**.c", "**.cpp"}
	}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "On"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "Editor"
	-- General
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	location		(sln_dir .. "proj/%{wks.name}/%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("On")
	systemversion 	("latest")

	-- Preprocessor Definitions
	defines
	{
		"ML_EDITOR_EXPORTS", "_CRT_SECURE_NO_WARNINGS"
	}

	-- Additional Include Directories
	includedirs
	{
		(sln_dir .. "include"),
		(sln_dir .. "thirdparty/include")
	}

	-- Headers / Source Files
	files
	{
		(sln_dir .. "include/%{wks.name}/%{prj.name}/**.hpp"),
		(sln_dir .. "src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		(sln_dir .. "lib/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
		(sln_dir .. "thirdparty/lib/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
	}

	-- Links
	links
	{
		"ML_Audio_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Engine_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.architecture}",
	}
	
	-- Filters
	vpaths {
		["Header Files"] = { "**.h", "**.hpp" },
		["Source Files"] = { "**.c", "**.cpp"}
	}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "On"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "Launcher"
	-- General
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	location		(sln_dir .. "proj/%{wks.name}/%{prj.name}")
	kind			("ConsoleApp")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("On")
	systemversion 	("latest")

	-- Preprocessor Definitions
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	-- Additional Include Directories
	includedirs
	{
		(sln_dir .. "include"),
		(sln_dir .. "thirdparty/include")
	}

	-- Headers / Source Files
	files
	{
		(sln_dir .. "include/%{wks.name}/%{prj.name}/**.hpp"),
		(sln_dir .. "src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		(sln_dir .. "lib/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/"),
		(sln_dir .. "lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
		(sln_dir .. "thirdparty/lib/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/"),
		(sln_dir .. "thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/"),
	}

	-- Links
	links
	{
		"ML_Audio_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Editor_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Engine_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.architecture}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.architecture}",
		"pdcurses"
	}

	--[[
	xcopy /y $(SolutionDir)thirdparty\bin\$(Configuration)\$(PlatformTarget)\assimp.dll $(OutDir)
	xcopy /y $(SolutionDir)thirdparty\bin\OpenAL32.dll $(OutDir)
	xcopy /y $(SolutionDir)thirdparty\bin\$(Configuration)\pdcurses.dll $(OutDir)
	if $(ConfigurationName) == Debug  ( xcopy /y $(SolutionDir)thirdparty\bin\$(Configuration)\$(PlatformTarget)\python39_d.dll $(OutDir) )
	if $(ConfigurationName) == Release ( xcopy /y $(SolutionDir)thirdparty\bin\$(Configuration)\$(PlatformTarget)\python39.dll $(OutDir) )
	]]--
	
	-- Filters
	vpaths {
		["Header Files"] = { "**.h", "**.hpp" },
		["Source Files"] = { "**.c", "**.cpp"}
	}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "On"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --