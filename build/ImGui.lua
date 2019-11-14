-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Vendor"
project "ImGui"
	targetname 		"%{prj.name}"
	location		"%{prj_dir}vendor/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	kind			"StaticLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson {
		"glfw3", 
	}
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"GLEW_STATIC",
		"IMGUI_IMPL_OPENGL_LOADER_GLEW",
	}
	includedirs {
		"%{ext_dir}",
		"%{ext_dir}imgui",
		"%{ext_dir}imgui/examples",
		"%{ext_dir}glfw/include",
	}
	files {
		"%{sln_dir}build/%{prj.name}.lua",
		"%{ext_dir}imgui/*.h",
		"%{ext_dir}imgui/*.cpp",
		"%{ext_dir}imgui/examples/imgui_impl_glfw.h",
		"%{ext_dir}imgui/examples/imgui_impl_glfw.cpp",
		"%{ext_dir}imgui/examples/imgui_impl_opengl3.h",
		"%{ext_dir}imgui/examples/imgui_impl_opengl3.cpp",
		"%{ext_dir}ImGuiColorTextEdit/*.h",
		"%{ext_dir}ImGuiColorTextEdit/*.cpp",
		"%{ext_dir}ImGuizmo/*.h",
		"%{ext_dir}ImGuizmo/*.cpp",
	}
	libdirs {
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links {
		"opengl32", "glfw3", "glew32s"
	}
	
	filter { "configurations:Debug" }
		symbols "On" 
	
	filter { "system:Windows", "configurations:Debug" }
		linkoptions { 
			"/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib"
		}
		
	filter { "configurations:Release" }
		optimize "Speed"
	
	filter { "system:Windows", "configurations:Release" }
		linkoptions { "/NODEFAULTLIB:LIBCMT.lib" }

	filter { "system:Windows" }
		defines { "IMGUI_API=__declspec(dllexport)" }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --