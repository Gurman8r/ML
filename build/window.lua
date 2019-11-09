-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --	
-- Window
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Window"
	targetname 		("ML_%{prj.name}")
	location		("%{prj_dir}ML/%{prj.name}/")
	targetdir		("%{bin_lib}")
	objdir			("%{bin_obj}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}", "%{ext_dir}glfw/include", "%{ext_dir}glfw/src" }
	defines 		{ "ML_WINDOW_EXPORTS", "_CRT_SECURE_NO_WARNINGS", "_GLFW_USE_CONFIG_H" }
	dependson 		{ "Core" }
	files 
	{
		"%{inc_dir}**.h", 
		"%{inc_dir}**.hpp", 
		"%{inc_dir}**.inl",  
		"%{src_dir}**.c", 
		"%{src_dir}**.cpp",
		"%{ext_dir}glfw/include/glfw/**.h",
		"%{ext_dir}glfw/src/context.c", 
		"%{ext_dir}glfw/src/glfw_config.h", 
		"%{ext_dir}glfw/src/init.c", 
		"%{ext_dir}glfw/src/input.c", 
		"%{ext_dir}glfw/src/internal.h", 
		"%{ext_dir}glfw/src/mappings.h", 
		"%{ext_dir}glfw/src/monitor.c", 
		"%{ext_dir}glfw/src/vulkan.c", 
		"%{ext_dir}glfw/src/glfw_window.c", 
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links 
	{
		"ML_Core", 
		"opengl32",
	}
	filter { "configurations:Debug" }
		symbols ("On") 
		linkoptions ("/NODEFAULTLIB:MSVCRT.lib")
	filter { "configurations:Release" } 
		optimize ("Speed")
	filter { "system:Windows" }
		files
		{
			"%{ext_dir}glfw/src/egl_context.c",
			"%{ext_dir}glfw/src/egl_context.h",
			"%{ext_dir}glfw/src/osmesa_context.c",
			"%{ext_dir}glfw/src/osmesa_context.h",
			"%{ext_dir}glfw/src/wgl_context.c",
			"%{ext_dir}glfw/src/wgl_context.h",
			"%{ext_dir}glfw/src/win32_init.c",
			"%{ext_dir}glfw/src/win32_joystick.c",
			"%{ext_dir}glfw/src/win32_joystick.h",
			"%{ext_dir}glfw/src/win32_monitor.c",
			"%{ext_dir}glfw/src/win32_platform.h",
			"%{ext_dir}glfw/src/win32_thread.c",
			"%{ext_dir}glfw/src/win32_time.c",
			"%{ext_dir}glfw/src/win32_window.c",
		}
		postbuildcommands { Copy("ML_%{prj.name}.dll", "%{bin_lib}", "%{bin_out}") }
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --