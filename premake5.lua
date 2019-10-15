-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Workspace
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
workspace "ML"
	startproject ("Launcher")
	configurations { "Debug", "Release" }
	platforms { "x86", "x64" }
	filter ("platforms:*32") architecture("x86")
	filter ("platforms:*86") architecture("x86")
	filter ("platforms:*64") architecture("x64")
	
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Paths
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
filter ("system:not Windows")
	sln_dir = "%{wks.location}/"
	bin_dir = "%{sln_dir}bin/%{cfg.buildcfg}/%{cfg.platform}/"
	lib_dir = "%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/"
	obj_dir = "%{sln_dir}obj/%{cfg.buildcfg}/%{cfg.platform}/"
	tmp_dir = "%{sln_dir}tmp/%{cfg.buildcfg}/%{cfg.platform}/"
	inc_dir = "%{sln_dir}include/ML/%{prj.name}/"
	src_dir = "%{sln_dir}src/ML/%{prj.name}/"
	prj_dir = "%{sln_dir}proj/ML/%{prj.name}/"
	ext_dir = "%{sln_dir}thirdparty/"
	
filter ("system:Windows")
	sln_dir = "%{wks.location}\\"
	bin_dir = "%{sln_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	lib_dir = "%{sln_dir}lib\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	obj_dir = "%{sln_dir}obj\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	tmp_dir = "%{sln_dir}obj\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	inc_dir = "%{sln_dir}include\\ML\\%{prj.name}\\"
	src_dir = "%{sln_dir}src\\ML\\%{prj.name}\\"
	prj_dir = "%{sln_dir}proj\\ML\\%{prj.name}\\"
	ext_dir = "%{sln_dir}thirdparty\\"
	
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Core
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
group "MemeLib"
project "Core"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	location		("%{prj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include" }
	defines 		{ "ML_CORE_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	vpaths 			{ ["Headers"] = { "**.h", "**.hpp" }, ["Sources"] = { "**.c", "**.cpp" } }
	files 			{ "%{inc_dir}**.h", "%{inc_dir}**.hpp", "%{src_dir}**.c", "%{src_dir}**.cpp" }
	filter ("configurations:Debug") symbols ("On")
	filter ("configurations:Release") optimize ("Speed")
	filter ("system:Windows")
		postbuildcommands 
		{
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}"
		}
		

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --	
-- Audio
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
group "MemeLib"
project "Audio"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include" }
	defines 		{ "ML_AUDIO_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Core" }
	vpaths 			{ ["Headers"] = { "**.h", "**.hpp" }, ["Sources"] = { "**.c", "**.cpp" } }
	files 			{ "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}",
		"OpenAL32", "flac", "ogg", "vorbis", "vorbisenc", "vorbisfile",
	}
	filter ("configurations:Debug") symbols ("On")
	filter ("configurations:Release") optimize ("Speed")
	filter ("system:Windows")
		postbuildcommands 
		{	
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}",
			"xcopy /y %{ext_dir}bin\\OpenAL32.dll %{bin_dir}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Network
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
group "MemeLib"
project "Network"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include" }
	defines 		{ "ML_NETWORK_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Core" }
	vpaths 			{ ["Headers"] = { "**.h", "**.hpp" }, ["Sources"] = { "**.c", "**.cpp" } }
	files 			{ "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{ 
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}", 
		"RakNet", "ws2_32",
	}
	filter ("configurations:Debug") symbols ("On")
	filter ("configurations:Release") optimize ("Speed")
	filter ("system:Windows")
		postbuildcommands
		{
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}"
		}
		

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --	
-- Window
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
group "MemeLib"
project "Window"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include" }
	defines 		{ "ML_WINDOW_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Core" }
	vpaths 			{ ["Headers"] = { "**.h", "**.hpp" }, ["Sources"] = { "**.c", "**.cpp" } }
	files 			{ "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links 
	{
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}", 
		"opengl32", "glfw3",
	}
	filter ("configurations:Debug") symbols ("On") linkoptions ("/NODEFAULTLIB:MSVCRT.lib")
	filter ("configurations:Release") optimize ("Speed")
	filter ("system:Windows")
		postbuildcommands
		{
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Graphics
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
group "MemeLib"
project "Graphics"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include" }
	defines 		{ "ML_GRAPHICS_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Core", "Window" }
	vpaths 			{ ["Headers"] = { "**.h", "**.hpp" }, ["Sources"] = { "**.c", "**.cpp" } }
	files 			{ "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.platform}",
		"glew32s", "opengl32", "assimp", "IrrXML", "zlibstatic",
	}
	filter ("configurations:Debug") symbols ("On")
	filter ("configurations:Release") optimize ("Speed")
	filter ("system:Windows")
		linkoptions ("/NODEFAULTLIB:LIBCMT.lib /NODEFAULTLIB:LIBCMTD.lib")
		postbuildcommands 
		{
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}",
			"xcopy /y %{ext_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\assimp.dll %{bin_dir}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Engine
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
group "MemeLib"
project "Engine"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include" }
	defines 		{ "ML_ENGINE_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Audio", "Core", "Graphics", "Network", "Window" }
	vpaths 			{ ["Headers"] = { "**.h", "**.hpp" }, ["Sources"] = { "**.c", "**.cpp" } }
	files 			{ "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.platform}",
		"lua",
	}
	filter ("configurations:Debug") symbols ("On") links { "python39_d" }
	filter ("configurations:Release") optimize ("Speed") links { "python39" }
	filter ("system:Windows")
		postbuildcommands
		{
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}",
			"if %{cfg.buildcfg} == Debug ( xcopy /y %{ext_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\python39_d.dll %{bin_dir} )",
			"if %{cfg.buildcfg} == Release ( xcopy /y %{ext_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\python39.dll %{bin_dir} )"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Editor
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
group "MemeLib"
project "Editor"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include" }
	defines 		{ "ML_EDITOR_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Audio", "Core", "Engine", "Graphics", "Network", "Window" }
	files 			{ "%{inc_dir}**.hpp", "%{src_dir}**.cpp" }
	vpaths 			{ ["Headers"] = { "**.h", "**.hpp" }, ["Sources"] = { "**.c", "**.cpp" } }
	files 
	{ 
		"%{inc_dir}**.hpp", 
		"%{src_dir}**.cpp",
		"%{sln_dir}src/ML/%{prj.name}/**.cpp",
		"%{sln_dir}thirdparty/include/imgui/**.h",
		"%{sln_dir}thirdparty/include/imgui/**.cpp",
		"%{sln_dir}thirdparty/include/ImGuiColorTextEdit/**.h",
		"%{sln_dir}thirdparty/include/ImGuiColorTextEdit/**.cpp",
	}
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Engine_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.platform}",
	}
	filter ("configurations:Debug") symbols ("On")
	filter ("configurations:Release") optimize ("Speed")
	filter ("system:Windows")
		postbuildcommands
		{
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Launcher
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
group "MemeLib"
project "Launcher"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	debugdir 		("%{bin_dir}")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include" }
	defines 		{ "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Audio", "Core", "Editor", "Engine", "Graphics", "Network", "Window" }
	files 			{ "%{inc_dir}**.hpp", "%{src_dir}**.cpp", "%{sln_dir}ML.ini", "%{sln_dir}assets/**.**", }
	vpaths
	{
		["Docs"] 		= { "**.txt", "**.md" },
		["Headers"] 	= { "**.h", "**.hpp" },
		["Fonts"] 		= { "**.ttf", "**.otf" },
		["Materials"]	= { "**.mat", "**.mtl" },
		["Meshes"] 		= { "**.obj", "**.fbx" },
		["Scripts"] 	= { "**.py", "**.lua" },
		["Shaders"] 	= { "**.glsl", "**.hlsl", "**.shader" },
		["Sources"] 	= { "**.c", "**.cpp" },
		["Styles"] 		= { "**.style" },
		["Textures"] 	= { "**.png", "**.jpg", "**.tiff" }, 
	}
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Editor_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Engine_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.platform}",
		"pdcurses",
	}
	filter ("configurations:Debug") kind("ConsoleApp") symbols("On")
	filter ("configurations:Release") kind("WindowedApp") optimize("On")
	filter ("system:Windows")
		postbuildcommands 
		{	
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.exe %{bin_dir}",
			"xcopy /y %{ext_dir}bin\\%{cfg.buildcfg}\\pdcurses.dll %{bin_dir}"
		}


-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Plugins		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
group "Plugins"
dofile "./plugins/Noobs/Noobs.lua"
dofile "./plugins/CommandSuite/CommandSuite.lua"
dofile "./plugins/TestPlugin/TestPlugin.lua"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --