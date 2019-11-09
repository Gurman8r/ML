-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Launcher
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Launcher"
	targetname 		("ML_%{prj.name}")
	location		("%{prj_dir}ML/%{prj.name}/")
	targetdir		("%{bin_lib}")
	objdir			("%{bin_obj}")
	debugdir 		("%{bin_out}")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}" }
	defines 		{ "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Audio", "Core", "Editor", "Engine", "Graphics", "Network", "Window" }
	files 
	{
		"%{inc_dir}**.h", 
		"%{inc_dir}**.hpp", 
		"%{inc_dir}**.inl",  
		"%{src_dir}**.c", 
		"%{src_dir}**.cpp",
		"%{sln_dir}assets/**.**", 
		"%{sln_dir}ML.ini",
	}
	excludes { "assets/Lib/**.**" }
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio", "ML_Core", "ML_Editor", "ML_Engine", "ML_Graphics", "ML_Network", "ML_Window", 
		"pdcurses",
	}
	filter { "configurations:Debug" } 
		symbols ("On")
		kind("ConsoleApp")
	filter { "configurations:Release" } 
		optimize ("Speed")
		kind("WindowedApp")
	filter { "system:Windows" }
		postbuildcommands 
		{	
			Copy("ML_%{prj.name}.exe", "%{bin_lib}", "%{bin_out}"),
			Copy("pdcurses.dll", "%{ext_bin}%{cfg.buildcfg}\\", "%{bin_out}")
		}


-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --