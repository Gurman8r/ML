-- Workspace
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
workspace "ML"
	configurations { "Debug", "Release" }
	
	platforms { "x86", "x64" }
	
	startproject ("Launcher")
		
-- Globals
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
filter "system:Windows"
	sln_dir = "%{wks.location}\\"
	bin_dir = "%{sln_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	lib_dir = "%{sln_dir}lib\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	obj_dir = "%{sln_dir}obj\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	inc_dir = "%{sln_dir}include\\%{wks.name}\\%{prj.name}\\"
	src_dir = "%{sln_dir}src\\%{wks.name}\\%{prj.name}\\"
	prj_dir = "%{sln_dir}proj\\%{wks.name}\\"
	prj_ext = "%{cfg.buildcfg}_%{cfg.platform}"
	ext_dir = "%{sln_dir}thirdparty\\"

-- Core
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
project "Core"
	targetname 		("ML_%{prj.name}_%{prj_ext}")
	location		("%{prj_dir}%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")

	-- Additional Include Directories
	includedirs { "%{sln_dir}include", "%{ext_dir}include" }

	-- Preprocessor Definitions
	defines { "ML_CORE_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }

	-- Source Files
	files { "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	
	-- Project Filters
	vpaths { ["Header Files"] = { "**.h", "**.hpp" }, ["Source Files"] = { "**.c", "**.cpp"} }
	
	-- Post Build Commands
	filter "system:Windows"
		postbuildcommands {	"xcopy /y %{lib_dir}%{wks.name}_%{prj.name}_%{prj_ext}.dll %{bin_dir}" }

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "Speed"
	
-- Audio
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
project "Audio"
	targetname 		("ML_%{prj.name}_%{prj_ext}")
	location		("%{prj_dir}%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")

	-- Additional Include Directories
	includedirs { "%{sln_dir}include", "%{ext_dir}include" }

	-- Preprocessor Definitions
	defines { "ML_AUDIO_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	
	-- Project Dependencies
	dependson { "Core" }

	-- Source Files
	files { "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	
	-- Project Filters
	vpaths { ["Header Files"] = { "**.h", "**.hpp" }, ["Source Files"] = { "**.c", "**.cpp"} }

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}

	-- Linker Input
	links
	{
		"%{wks.name}_Core_%{prj_ext}",
		"flac",
		"ogg",
		"OpenAL32",
		"vorbis",
		"vorbisenc",
		"vorbisfile"
	}
	
	-- Post Build Commands
	filter "system:Windows"
		postbuildcommands 
		{	
			"xcopy /y %{lib_dir}%{wks.name}_%{prj.name}_%{prj_ext}.dll %{bin_dir}",
			"xcopy /y %{ext_dir}bin\\OpenAL32.dll %{bin_dir}"
		}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "Speed"
		

-- Network
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
project "Network"
	targetname 		("ML_%{prj.name}_%{prj_ext}")
	location		("%{prj_dir}%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")

	-- Additional Include Directories
	includedirs { "%{sln_dir}include", "%{ext_dir}include" }

	-- Preprocessor Definitions
	defines { "ML_NETWORK_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	
	-- Project Dependencies
	dependson { "Core" }

	-- Source Files
	files { "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	
	-- Project Filters
	vpaths { ["Header Files"] = { "**.h", "**.hpp" }, ["Source Files"] = { "**.c", "**.cpp"} }

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}

	-- Linker Input
	links { "%{wks.name}_Core_%{prj_ext}", "RakNet", "ws2_32" }
	
	-- Post Build Commands
	filter "system:Windows"
		postbuildcommands {	"xcopy /y %{lib_dir}%{wks.name}_%{prj.name}_%{prj_ext}.dll %{bin_dir}" }

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "Speed"
		
-- Window		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
project "Window"
	targetname 		("ML_%{prj.name}_%{prj_ext}")
	location		("%{prj_dir}%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")

	-- Additional Include Directories
	includedirs { "%{sln_dir}include", "%{ext_dir}include" }

	-- Preprocessor Definitions
	defines { "ML_WINDOW_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	
	-- Project Dependencies
	dependson { "Core" }

	-- Source Files
	files { "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	
	-- Project Filters
	vpaths { ["Header Files"] = { "**.h", "**.hpp" }, ["Source Files"] = { "**.c", "**.cpp"} }

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}

	-- Linker Input
	links { "%{wks.name}_Core_%{prj_ext}", "opengl32", "glfw3" }
	
	-- Post Build Commands
	filter "system:Windows"
		postbuildcommands {	"xcopy /y %{lib_dir}%{wks.name}_%{prj.name}_%{prj_ext}.dll %{bin_dir}" }

	-- Debug
	filter "configurations:Debug"
		symbols ("On")
		linkoptions ("/NODEFAULTLIB:MSVCRT.lib")

	-- Release
	filter "configurations:Release"
		optimize ("On")

-- Graphics	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
project "Graphics"
	targetname 		("ML_%{prj.name}_%{prj_ext}")
	location		("%{prj_dir}%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")

	-- Additional Include Directories
	includedirs { "%{sln_dir}include", "%{ext_dir}include" }

	-- Preprocessor Definitions
	defines { "ML_GRAPHICS_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	
	-- Project Dependencies
	dependson { "Core", "Window" }

	-- Source Files
	files { "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	
	-- Project Filters
	vpaths { ["Header Files"] = { "**.h", "**.hpp" }, ["Source Files"] = { "**.c", "**.cpp"} }

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}

	-- Linker Input
	links
	{
		"%{wks.name}_Core_%{prj_ext}",
		"%{wks.name}_Window_%{prj_ext}",
		"glew32s",
		"opengl32",
		"assimp",
		"IrrXML",
		"zlibstatic"
	}
	
	-- Post Build Commands
	filter "system:Windows"
		postbuildcommands 
		{	
			"xcopy /y %{lib_dir}%{wks.name}_%{prj.name}_%{prj_ext}.dll %{bin_dir}",
			"xcopy /y %{ext_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\assimp.dll %{bin_dir}"
		}

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "Speed"
		
	-- Windows
	filter "system:Windows"
		linkoptions "/NODEFAULTLIB:LIBCMT.lib /NODEFAULTLIB:LIBCMTD.lib"

-- Engine		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
project "Engine"
	targetname 		("ML_%{prj.name}_%{prj_ext}")
	location		("%{prj_dir}%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")

	-- Additional Include Directories
	includedirs { "%{sln_dir}include", "%{ext_dir}include" }

	-- Preprocessor Definitions
	defines { "ML_ENGINE_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	
	-- Project Dependencies
	dependson { "Audio", "Core", "Graphics", "Network", "Window" }

	-- Source Files
	files { "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	
	-- Project Filters
	vpaths { ["Header Files"] = { "**.h", "**.hpp" }, ["Source Files"] = { "**.c", "**.cpp"} }

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}

	-- Linker Input
	links
	{
		"%{wks.name}_Audio_%{prj_ext}",
		"%{wks.name}_Core_%{prj_ext}",
		"%{wks.name}_Graphics_%{prj_ext}",
		"%{wks.name}_Network_%{prj_ext}",
		"%{wks.name}_Window_%{prj_ext}",
		"lua.lib"
	}
	
	-- Post Build Commands
	filter "system:Windows"
		postbuildcommands
		{
			"xcopy /y %{lib_dir}%{wks.name}_%{prj.name}_%{prj_ext}.dll %{bin_dir}",
			"if %{cfg.buildcfg} == Debug ( xcopy /y %{ext_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\python39_d.dll %{bin_dir} )",
			"if %{cfg.buildcfg} == Release ( xcopy /y %{ext_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\python39.dll %{bin_dir} )"
		}

	-- Debug
	filter "configurations:Debug"
		symbols "On"
		links { "python39_d.lib" }

	-- Release
	filter "configurations:Release"
		optimize "Speed"
		links { "python39.lib" }
		
-- Editor
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
project "Editor"
	targetname 		("ML_%{prj.name}_%{prj_ext}")
	location		("%{prj_dir}%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")

	-- Additional Include Directories
	includedirs { "%{sln_dir}include", "%{ext_dir}include" }

	-- Preprocessor Definitions
	defines { "ML_EDITOR_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	
	-- Project Dependencies
	dependson { "Audio", "Core", "Engine", "Graphics", "Network", "Window" }

	-- Source Files
	files { "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	
	-- Project Filters
	vpaths { ["Header Files"] = { "**.h", "**.hpp" }, ["Source Files"] = { "**.c", "**.cpp"} }

	-- Source Files
	files 
	{ 
		"%{inc_dir}**.hpp", 
		"%{src_dir}**.cpp",
		"%{sln_dir}src/%{wks.name}/%{prj.name}/**.cpp",
		"%{sln_dir}thirdparty/include/imgui/**.h",
		"%{sln_dir}thirdparty/include/imgui/**.cpp",
		"%{sln_dir}thirdparty/include/ImGuiColorTextEdit/TextEditor.h",
		"%{sln_dir}thirdparty/include/ImGuiColorTextEdit/TextEditor.cpp",
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
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}

	-- Linker Input
	links
	{
		"%{wks.name}_Audio_%{prj_ext}",
		"%{wks.name}_Core_%{prj_ext}",
		"%{wks.name}_Engine_%{prj_ext}",
		"%{wks.name}_Graphics_%{prj_ext}",
		"%{wks.name}_Network_%{prj_ext}",
		"%{wks.name}_Window_%{prj_ext}",
	}
	
	-- Post Build Commands
	filter "system:Windows"
		postbuildcommands {	"xcopy /y %{lib_dir}%{wks.name}_%{prj.name}_%{prj_ext}.dll %{bin_dir}" }

	-- Debug
	filter "configurations:Debug"
		symbols "On"

	-- Release
	filter "configurations:Release"
		optimize "Speed"
		
-- Launcher		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
project "Launcher"
	targetname 		("ML_%{prj.name}_%{prj_ext}")
	location		("%{prj_dir}%{prj.name}")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")
	debugdir 		(bin_dir)

	-- Additional Include Directories
	includedirs { "%{sln_dir}include", "%{ext_dir}include" }

	-- Preprocessor Definitions
	defines { "_CRT_SECURE_NO_WARNINGS" }
	
	-- Project Dependencies
	dependson { "Audio", "Core", "Editor", "Engine", "Graphics", "Network", "Window" }

	-- Source Files
	files { "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	
	-- Project Filters
	vpaths { ["Header Files"] = { "**.h", "**.hpp" }, ["Source Files"] = { "**.c", "**.cpp"} }

	-- Additional Library Directories
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}

	-- Linker Input
	links
	{
		"%{wks.name}_Audio_%{prj_ext}",
		"%{wks.name}_Core_%{prj_ext}",
		"%{wks.name}_Editor_%{prj_ext}",
		"%{wks.name}_Engine_%{prj_ext}",
		"%{wks.name}_Graphics_%{prj_ext}",
		"%{wks.name}_Network_%{prj_ext}",
		"%{wks.name}_Window_%{prj_ext}",
		"pdcurses"
	}

	-- Windows
	filter "system:Windows"
		postbuildcommands 
		{	
			"xcopy /y %{lib_dir}%{wks.name}_%{prj.name}_%{prj_ext}.exe %{bin_dir}",
			"xcopy /y %{ext_dir}bin\\%{cfg.buildcfg}\\pdcurses.dll %{bin_dir}"
		}

	-- Debug
	filter "configurations:Debug"
		symbols ("On")
		kind	("ConsoleApp")

	-- Release
	filter "configurations:Release"
		optimize("On")
		kind	("WindowedApp")

-- Plugins		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

dofile "./plugins/Noobs/Noobs.lua"
dofile "./plugins/CommandSuite/CommandSuite.lua"
dofile "./plugins/TestPlugin/TestPlugin.lua"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --