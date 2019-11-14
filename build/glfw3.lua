-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Vendor"
project "glfw3"
	targetname 		"%{prj.name}"
	location		"%{prj_dir}vendor/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	kind			"StaticLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"_GLFW_WIN32=1",
	}
	includedirs {
		"%{ext_dir}glfw/include",
		"%{ext_dir}glfw/include",
		"%{ext_dir}glfw/src",
	}
	files {
		"%{sln_dir}build/%{prj.name}.lua",
		"%{ext_dir}glfw/include/glfw/**.h",
		"%{ext_dir}glfw/src/context.c", 
		"%{ext_dir}glfw/src/egl_context.c",
		"%{ext_dir}glfw/src/egl_context.h",
		"%{ext_dir}glfw/src/glfw_config.h",  
		"%{ext_dir}glfw/src/init.c", 
		"%{ext_dir}glfw/src/input.c", 
		"%{ext_dir}glfw/src/internal.h", 
		"%{ext_dir}glfw/src/mappings.h", 
		"%{ext_dir}glfw/src/monitor.c", 
		"%{ext_dir}glfw/src/osmesa_context.c",
		"%{ext_dir}glfw/src/osmesa_context.h",
		"%{ext_dir}glfw/src/vulkan.c",
		"%{ext_dir}glfw/src/wgl_context.c",
		"%{ext_dir}glfw/src/wgl_context.h",
		"%{ext_dir}glfw/src/window.c",
	}
	libdirs {
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links {
		"opengl32",
	}
	
	filter { "configurations:Debug" }
		symbols "On" 
	
	filter { "system:Windows", "configurations:Debug" }
		linkoptions { "/NODEFAULTLIB:MSVCRT.lib" }
		
	filter { "configurations:Release" }
		optimize "Speed"
		
	filter { "system:Windows" }
		files {
			"%{ext_dir}glfw/src/win32_init.c",
			"%{ext_dir}glfw/src/win32_joystick.c",
			"%{ext_dir}glfw/src/win32_joystick.h",
			"%{ext_dir}glfw/src/win32_monitor.c",
			"%{ext_dir}glfw/src/win32_platform.h",
			"%{ext_dir}glfw/src/win32_thread.c",
			"%{ext_dir}glfw/src/win32_time.c",
			"%{ext_dir}glfw/src/win32_window.c",
		}
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --