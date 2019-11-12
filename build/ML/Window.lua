-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --	
-- Window
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Window"
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
		"Core",
		"GLFW",
	}
	defines
	{
		"ML_WINDOW_EXPORTS", 
		"_CRT_SECURE_NO_WARNINGS",
		"_GLFW_USE_CONFIG_H",
	}
	includedirs
	{
		"%{sln_dir}include", 
		"%{ext_dir}",
		"%{ext_dir}glfw/include",
	}
	files 
	{
		"%{inc_dir}**.h", 
		"%{inc_dir}**.hpp", 
		"%{inc_dir}**.inl",
		"%{src_dir}**.c", 
		"%{src_dir}**.cpp",
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links 
	{
		"ML_Core", "opengl32", "glew32s", "glfw",
	}
	
	filter { "configurations:Debug" }
		symbols "On" 
		
	filter { "configurations:Release" }
		optimize "Speed"
		
	filter { "system:Windows" }
		defines { "NOMINMAX" }
		linkoptions { "/NODEFAULTLIB:MSVCRT.lib", }
		postbuildcommands
		{
			"%{ml_copy} %{bin_lib}ML_%{prj.name}.dll %{bin_out}"
		}
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --