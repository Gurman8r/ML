-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Engine
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Engine"
	targetname 		"ML_%{prj.name}"
	location		"%{prj_dir}ML/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	kind			"SharedLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson 
	{
		"Audio", "Core", "Graphics", "Network", "Window",
		"Lua",
	}
	defines
	{
		"ML_ENGINE_EXPORTS", 
		"_CRT_SECURE_NO_WARNINGS",
	}
	includedirs
	{
		"%{sln_dir}include", 
		"%{ext_dir}",
		"%{ext_dir}cpython/include",
	}
	files 
	{
		"%{inc_dir}**.h", 
		"%{inc_dir}**.hpp", 
		"%{inc_dir}**.inl",  
		"%{src_dir}**.c", 
		"%{src_dir}**.cpp",
		"%{ext_dir}lua/**.h", 
		"%{ext_dir}lua/**.hpp", 
		"%{ext_dir}lua/**.c",
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio", "ML_Core", "ML_Graphics", "ML_Network", "ML_Window",
		"pdcurses",
	}
	
	filter { "configurations:Debug" }
		symbols "On" 
		links
		{
			"python39_d"
		}
		
	filter { "configurations:Release" } 
		optimize "Speed" 
		links
		{
			"python39"
		}
		
	filter { "system:Windows" }
		linkoptions { "/NODEFAULTLIB:LIBCMT.lib" }
		postbuildcommands
		{
			"%{ml_copy} %{bin_lib}ML_%{prj.name}.dll %{bin_out}",
			"%{ml_copy} %{ext_bin}%{cfg.buildcfg}\\pdcurses.dll %{bin_out}",
			"if %{cfg.buildcfg} == Debug ( %{ml_copy} %{ext_bin}%{cfg.buildcfg}\\%{cfg.platform}\\python39_d.dll %{bin_out} )",
			"if %{cfg.buildcfg} == Release ( %{ml_copy} %{ext_bin}%{cfg.buildcfg}\\%{cfg.platform}\\python39.dll %{bin_out} )"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --