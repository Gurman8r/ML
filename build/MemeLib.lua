-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "ML"
project "MemeLib"
	targetname 		"%{prj.name}"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	location		"%{prj_dir}ML/%{prj.name}/"
	kind			"SharedLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson {
		"glfw3", "ImGui", "lua", "RakNet",
	}
	defines {
		"ML_AUDIO_EXPORTS", 
		"ML_CORE_EXPORTS", 
		"ML_EDITOR_EXPORTS", 
		"ML_ENGINE_EXPORTS", 
		"ML_GRAPHICS_EXPORTS", 
		"ML_NETWORK_EXPORTS", 
		"ML_WINDOW_EXPORTS", 
		"ML_INI_FILENAME=\"../../../ML.ini\"",
		"ML_IMPL_NEW=std::malloc",
		"ML_IMPL_DELETE=std::free",
		"ML_IMPL_PLATFORM_GLFW",
		"ML_IMPL_RENDERER_OPENGL",
		"ML_IMPL_OPENGL_LOADER_GLEW",
		"GLEW_STATIC",
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
	}
	includedirs {
		"%{sln_dir}build/%{prj.name}.lua",
		"%{sln_dir}include",
		"%{ext_dir}",
		"%{ext_dir}openal-soft/al",
		"%{ext_dir}openal-soft/alc",
		"%{ext_dir}openal-soft/include",
		"%{ext_dir}imgui",
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
		"%{sln_dir}build/%{prj.name}.lua",
		"%{sln_dir}build/%{prj.name}.lua",
		"%{sln_dir}assets/**.**", 
		"%{sln_dir}include/ML/**.hpp",
		"%{sln_dir}src/ML/**.cpp",
		"%{sln_dir}tools/**.**",
		"%{sln_dir}ML.ini", 
		"%{sln_dir}premake5.lua",
		"%{sln_dir}README.md",
		"%{sln_dir}TODO.txt",
	}
	libdirs {
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links {
		"lua", "RakNet", "ImGui", "glfw3",
		"opengl32", "glew32s", 
		"assimp", "IrrXML", "zlibstatic", "freetype", 
		"OpenAL32", "flac", "ogg", "vorbis", "vorbisenc", "vorbisfile",
		"pdcurses", 
	}
	
	filter { "system:Windows", "configurations:Debug" }
		symbols "On"
		links { "python39_d" }
		postbuildcommands {	
			"%{ml_copy} %{ext_bin}%{cfg.buildcfg}\\%{cfg.platform}\\python39_d.dll %{bin_out}",
		}
		linkoptions { 
			"/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib"
		}
	
	filter { "system:Windows", "configurations:Release" }
		optimize "Speed"
		links { "python39" }
		postbuildcommands {	
			"%{ml_copy} %{ext_bin}%{cfg.buildcfg}\\%{cfg.platform}\\python39.dll %{bin_out}",
		}
		linkoptions {
			"/NODEFAULTLIB:LIBCMT.lib"
		}
	
	filter { "system:Windows" }
		includedirs { "%{ext_dir}cpython/PC" }
		links { "ws2_32" }
		postbuildcommands {	
			"%{ml_copy} %{bin_lib}%{prj.name}.dll %{bin_out}",
			"%{ml_copy} %{ext_bin}OpenAL32.dll %{bin_out}",
			"%{ml_copy} %{ext_bin}%{cfg.buildcfg}\\pdcurses.dll %{bin_out}",
			"%{ml_copy} %{ext_bin}%{cfg.buildcfg}\\%{cfg.platform}\\assimp.dll %{bin_out}",
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --