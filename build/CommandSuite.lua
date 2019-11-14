-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Plugins"
project "CommandSuite"
	targetname 		"%{prj.name}"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	location		"%{prj_dir}plugins/%{prj.name}/"
	kind			"SharedLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson {
		"Launcher"
	}
	defines {
		"_CRT_SECURE_NO_WARNINGS", "NOMINMAX",
	}
	includedirs {
		"%{sln_dir}include", "%{ext_dir}", "%{sln_dir}plugins/%{prj.name}"
	}
	files {
		"%{sln_dir}build/%{prj.name}.lua", "%{sln_dir}src/plugins/%{prj.name}/**.**",
	}
	libdirs {
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links {
		"MemeLib"
	}
	
	filter "configurations:Debug"
		symbols "On"
	
	filter "configurations:Release"
		optimize "Speed"
	
	filter { "system:Windows" }
		postbuildcommands { "%{ml_copy} %{bin_lib}%{prj.name}.dll %{bin_out}" }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --