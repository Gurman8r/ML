-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Launcher
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Launcher"
	targetname 		"ML_%{prj.name}"
	location		"%{prj_dir}ML/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	debugdir 		"%{bin_out}"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson 
	{
		"Audio", "Core", "Editor", "Engine", "Graphics", "Network", "Window"
	}
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}
	includedirs
	{
		"%{sln_dir}include", 
		"%{ext_dir}",
	}
	files 
	{
		"%{inc_dir}**.h", 
		"%{inc_dir}**.hpp", 
		"%{inc_dir}**.inl", 
		"%{src_dir}**.c", 
		"%{src_dir}**.cpp", 
		"%{sln_dir}ML.ini", 
		"%{sln_dir}assets/**.**", 
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio", "ML_Core", "ML_Editor", "ML_Engine", "ML_Graphics", "ML_Network", "ML_Window",
	}
	
	filter { "configurations:Debug" }
		symbols "On"
		kind("ConsoleApp")
		defines { "ML_SUBSYSTEM_CONSOLE" }
	
	filter { "configurations:Release" } 
		optimize "Speed"
		kind("WindowedApp")
		defines { "ML_SUBSYSTEM_WINDOW" }
	
	filter { "system:Windows" }
		defines { "NOMINMAX" }
		postbuildcommands 
		{	
			"%{ml_copy} %{bin_lib}ML_%{prj.name}.exe %{bin_out}",
		}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --