-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "ML"
	configurations 	{ "Debug", "Release" }
	architecture 	("x86")
	startproject 	("Launcher")

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

sln_dir = "%{wks.location}/"
bin_dir = ("%{sln_dir}bin/%{cfg.buildcfg}/%{cfg.architecture}")
lib_dir = ("%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.architecture}")
obj_dir = ("%{sln_dir}obj/%{cfg.buildcfg}/%{cfg.architecture}")

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "Core"
	-- General
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	location		("%{sln_dir}proj/%{wks.name}/%{prj.name}")
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
		("%{sln_dir}include"),
		("%{sln_dir}thirdparty/include")
	}

	-- Source Files
	files
	{
		("%{sln_dir}include/%{wks.name}/%{prj.name}/**.hpp"),
		("%{sln_dir}src/%{wks.name}/%{prj.name}/**.cpp")
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
	location		("%{sln_dir}proj/%{wks.name}/%{prj.name}")
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
		("%{sln_dir}include"),
		("%{sln_dir}thirdparty/include")
	}

	-- Source Files
	files
	{
		("%{sln_dir}include/%{wks.name}/%{prj.name}/**.hpp"),
		("%{sln_dir}src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.architecture}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/",
	}
	
	-- Project Dependencies
	dependson { "Core" }

	-- Linker Input
	links
	{
		"%{wks.name}_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"flac",
		"ogg",
		"OpenAL32",
		"vorbis",
		"vorbisenc",
		"vorbisfile"
	}
	
	-- Project Filters
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
	location		("%{sln_dir}proj/%{wks.name}/%{prj.name}")
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
		("%{sln_dir}include"),
		("%{sln_dir}thirdparty/include")
	}

	-- Source Files
	files
	{
		("%{sln_dir}include/%{wks.name}/%{prj.name}/**.hpp"),
		("%{sln_dir}src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.architecture}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/",
	}
	
	-- Project Dependencies
	dependson { "Core" }

	-- Linker Input
	links
	{
		"%{wks.name}_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"RakNet",
		"ws2_32"
	}
	
	-- Project Filters
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
	location		("%{sln_dir}proj/%{wks.name}/%{prj.name}")
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
		("%{sln_dir}include"),
		("%{sln_dir}thirdparty/include")
	}

	-- Source Files
	files
	{
		("%{sln_dir}include/%{wks.name}/%{prj.name}/**.hpp"),
		("%{sln_dir}src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.architecture}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/",
	}
	
	-- Project Dependencies
	dependson { "Core" }

	-- Linker Input
	links
	{
		"%{wks.name}_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"glfw3"
	}
	
	-- Project Filters
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
	location		("%{sln_dir}proj/%{wks.name}/%{prj.name}")
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
		("%{sln_dir}include"),
		("%{sln_dir}thirdparty/include")
	}

	-- Source Files
	files
	{
		("%{sln_dir}include/%{wks.name}/%{prj.name}/**.hpp"),
		("%{sln_dir}src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.architecture}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/",
	}
	
	-- Project Dependencies
	dependson { "Core", "Window" }

	-- Linker Input
	links
	{
		"%{wks.name}_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Window_%{cfg.buildcfg}_%{cfg.architecture}",
		"glew32s",
		"opengl32",
		"assimp",
		"IrrXML",
		"zlibstatic"
	}
	
	-- Project Filters
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
	location		("%{sln_dir}proj/%{wks.name}/%{prj.name}")
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
		("%{sln_dir}include"),
		("%{sln_dir}thirdparty/include")
	}

	-- Source Files
	files
	{
		("%{sln_dir}include/%{wks.name}/%{prj.name}/**.hpp"),
		("%{sln_dir}src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.architecture}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/",
	}
	
	-- Project Dependencies
	dependson { "Audio", "Core", "Graphics", "Network", "Window" }

	-- Linker Input
	links
	{
		"%{wks.name}_Audio_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Graphics_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Network_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Window_%{cfg.buildcfg}_%{cfg.architecture}",
		"python37.lib",
		"lua.lib"
	}
	
	-- Project Filters
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
	location		("%{sln_dir}proj/%{wks.name}/%{prj.name}")
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
		("%{sln_dir}include"),
		("%{sln_dir}thirdparty/include")
	}

	-- Source Files
	files
	{
		("%{sln_dir}include/%{wks.name}/%{prj.name}/**.hpp"),
		("%{sln_dir}src/%{wks.name}/%{prj.name}/**.cpp"),
		("%{sln_dir}thirdparty/include/imgui/**.h"),
		("%{sln_dir}thirdparty/include/imgui/**.cpp"),
		("%{sln_dir}thirdparty/include/ImGuiColorTextEdit/TextEditor.h"),
		("%{sln_dir}thirdparty/include/ImGuiColorTextEdit/TextEditor.cpp"),
	}

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.architecture}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/",
	}
	
	-- Project Dependencies
	dependson { "Audio", "Core", "Engine", "Graphics", "Network", "Window" }

	-- Linker Input
	links
	{
		"%{wks.name}_Audio_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Engine_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Graphics_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Network_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Window_%{cfg.buildcfg}_%{cfg.architecture}",
	}
	
	-- Project Filters
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
	location		("%{sln_dir}proj/%{wks.name}/%{prj.name}")
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
		("%{sln_dir}include"),
		("%{sln_dir}thirdparty/include")
	}

	-- Source Files
	files
	{
		("%{sln_dir}include/%{wks.name}/%{prj.name}/**.hpp"),
		("%{sln_dir}src/%{wks.name}/%{prj.name}/**.cpp")
	}

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.architecture}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.architecture}/",
	}
	
	-- Project Dependencies
	dependson { "Audio", "Core", "Editor", "Engine", "Graphics", "Network", "Window" }

	-- Linker Input
	links
	{
		"%{wks.name}_Audio_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Core_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Editor_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Engine_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Graphics_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Network_%{cfg.buildcfg}_%{cfg.architecture}",
		"%{wks.name}_Window_%{cfg.buildcfg}_%{cfg.architecture}",
		"pdcurses"
	}

	-- Post Build Commands
	filter "system:Windows"
		postbuildcommands 
		{	
			"copy %{lib_dir}\\%{wks.name}_Audio_%{cfg.buildcfg}_%{cfg.architecture}.dll %{bin_dir}",
			"copy %{lib_dir}\\%{wks.name}_Core_%{cfg.buildcfg}_%{cfg.architecture}.dll %{bin_dir}",
			"copy %{lib_dir}\\%{wks.name}_Editor_%{cfg.buildcfg}_%{cfg.architecture}.dll %{bin_dir}",
			"copy %{lib_dir}\\%{wks.name}_Engine_%{cfg.buildcfg}_%{cfg.architecture}.dll %{bin_dir}",
			"copy %{lib_dir}\\%{wks.name}_Graphics_%{cfg.buildcfg}_%{cfg.architecture}.dll %{bin_dir}",
			"copy %{lib_dir}\\%{wks.name}_Launcher_%{cfg.buildcfg}_%{cfg.architecture}.exe %{bin_dir}",
			"copy %{lib_dir}\\%{wks.name}_Network_%{cfg.buildcfg}_%{cfg.architecture}.dll %{bin_dir}",
			"copy %{lib_dir}\\%{wks.name}_Window_%{cfg.buildcfg}_%{cfg.architecture}.dll %{bin_dir}",
			"copy ${sln_dir}thirdparty\\bin\\${cfg.buildcfg}\\${cfg.architecture}\\assimp.dll ${bin_dir}",
			"copy ${sln_dir}thirdparty\\bin\\OpenAL32.dll ${bin_dir}",
			"copy ${sln_dir}thirdparty\\bin\\${cfg.buildcfg}\\pdcurses.dll ${bin_dir}",
			"copy ${sln_dir}thirdparty\\bin\\${cfg.buildcfg}\\${cfg.architecture}\\python39.dll  ${bin_dir}"
		}
	
	-- Project Filters
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