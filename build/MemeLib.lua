-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "MemeLib"
	targetname 		"%{prj.name}"
	location		"%{prj_dir}memelib/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	kind			"SharedLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson {
		"GLFW", "Lua", "RakNet",
	}
	defines {
		"ML_AUDIO_EXPORTS", 
		"ML_CORE_EXPORTS", 
		"ML_EDITOR_EXPORTS", 
		"ML_ENGINE_EXPORTS", 
		"ML_GRAPHICS_EXPORTS", 
		"ML_NETWORK_EXPORTS", 
		"ML_WINDOW_EXPORTS", 
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_USER_CONFIG=<ML/Editor/ImGuiConfig.hpp>",
		"IMGUI_IMPL_OPENGL_LOADER_GLEW",
		"_WINSOCK_DEPRECATED_NO_WARNINGS",
		"_GLFW_USE_CONFIG_H",
		"GLEW_STATIC",
	}
	includedirs {
		"%{sln_dir}include",
		"%{ext_dir}",
		"%{ext_dir}openal-soft/al",
		"%{ext_dir}openal-soft/alc",
		"%{ext_dir}openal-soft/include",
		"%{ext_dir}imgui",
		"%{ext_dir}imgui/examples",
		"%{ext_dir}pybind11/include",
		"%{ext_dir}cpython/Include",
		"%{ext_dir}cpython/Include/internal",
		"%{ext_dir}assimp/include",
		"%{ext_dir}freetype2/include",
		"%{ext_dir}freetype2/include/freetype",
		"%{ext_dir}glfw/include",
		"%{ext_dir}RakNet/Source",
	}
	files {
		"%{sln_dir}include/ML/**.hpp",
		"%{sln_dir}include/ML/**.inl",
		"%{sln_dir}src/ML/**.cpp",
		"%{ext_dir}imgui/*.h",
		"%{ext_dir}imgui/*.cpp",
		"%{ext_dir}imgui/examples/imgui_impl_glfw.h",
		"%{ext_dir}imgui/examples/imgui_impl_glfw.cpp",
		"%{ext_dir}imgui/examples/imgui_impl_opengl3.h",
		"%{ext_dir}imgui/examples/imgui_impl_opengl3.cpp",
		"%{ext_dir}ImGuiColorTextEdit/*.h",
		"%{ext_dir}ImGuiColorTextEdit/*.cpp",
	}
	libdirs {
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links {
		"opengl32",
		"glew32s", 
		"glfw",
		"OpenAL32", 
		"flac", 
		"ogg",
		"vorbis", 
		"vorbisenc", 
		"vorbisfile",
		"pdcurses", 
		"lua",
		"freetype", 
		"assimp", 
		"IrrXML", 
		"zlibstatic",
		"RakNet",
	}
	
	filter { "configurations:Debug" }
		symbols "On"
		links { "python39_d", }
		filter { "system:Windows", "configurations:Debug", }
	
	filter { "configurations:Release" } 
		optimize "Speed"
		links { "python39", }
		filter { "system:Windows", "configurations:Release", }
	
	filter { "system:Windows" }
		defines { "NOMINMAX", }
		includedirs { "%{ext_dir}cpython/PC", }
		linkoptions { "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib", }
		links { "ws2_32", }
		postbuildcommands {	
			"%{ml_copy} %{bin_lib}%{prj.name}.dll %{bin_out}",
			"%{ml_copy} %{ext_lib}OpenAL32.dll %{bin_out}",
			"%{ml_copy} %{ext_lib}%{cfg.buildcfg}\\pdcurses.dll %{bin_out}",
			"if %{cfg.buildcfg} == Debug ( %{ml_copy} %{ext_lib}%{cfg.buildcfg}\\%{cfg.platform}\\python39_d.dll %{bin_out} )",
			"if %{cfg.buildcfg} == Release ( %{ml_copy} %{ext_lib}%{cfg.buildcfg}\\%{cfg.platform}\\python39.dll %{bin_out} )",
			"%{ml_copy} %{ext_lib}%{cfg.buildcfg}\\%{cfg.platform}\\assimp.dll %{bin_out}",
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --