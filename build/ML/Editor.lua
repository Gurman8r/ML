-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Editor
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Editor"
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
		"Audio", "Core", "Engine", "Graphics", "Network", "Window",
		"ImGui",
	}
	defines
	{
		"ML_EDITOR_EXPORTS", 
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_USER_CONFIG=<ML/Editor/ImGuiConfig.hpp>",
		"IMGUI_IMPL_OPENGL_LOADER_GLEW",
		"GLEW_STATIC",
	}
	includedirs
	{
		"%{sln_dir}include", 
		"%{ext_dir}",
		"%{ext_dir}imgui",
		"%{ext_dir}glfw/include",
	}
	files 
	{ 
		"%{inc_dir}**.h",
		"%{inc_dir}**.hpp",
		"%{inc_dir}**.inl", 
		"%{src_dir}**.c",
		"%{src_dir}**.cpp", 
		"%{ext_dir}imgui/*.h",
		"%{ext_dir}imgui/*.cpp",
		"%{ext_dir}imgui/examples/imgui_impl_glfw.h",
		"%{ext_dir}imgui/examples/imgui_impl_glfw.cpp",
		"%{ext_dir}imgui/examples/imgui_impl_opengl3.h",
		"%{ext_dir}imgui/examples/imgui_impl_opengl3.cpp",
		"%{ext_dir}ImGuiColorTextEdit/*.h",
		"%{ext_dir}ImGuiColorTextEdit/*.cpp",
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio", "ML_Core", "ML_Engine", "ML_Graphics", "ML_Network", "ML_Window",
		"opengl32", "glew32s", "glfw",
	}
	
	filter { "configurations:Debug" }
		symbols "On"
	
	filter { "configurations:Release" } 
		optimize "Speed"
	
	filter { "system:Windows" }
		defines { "NOMINMAX" }
		linkoptions { "/NODEFAULTLIB:LIBCMT.lib /NODEFAULTLIB:MSVCRT.lib" }
		postbuildcommands
		{
			"%{ml_copy} %{bin_lib}ML_%{prj.name}.dll %{bin_out}"
		}
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --