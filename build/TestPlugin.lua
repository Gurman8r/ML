-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Plugins"
project "TestPlugin"
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
		"Example",
	}
	defines {
		"_CRT_SECURE_NO_WARNINGS",
	}
	includedirs {
		"%{sln_dir}include", "%{ext_dir}", "%{sln_dir}plugins/%{prj.name}"
	}
	files {
		"%{sln_dir}plugins/%{prj.name}/**.h", 
		"%{sln_dir}plugins/%{prj.name}/**.hpp",
		"%{sln_dir}plugins/%{prj.name}/**.inl",
		"%{sln_dir}plugins/%{prj.name}/**.c",
		"%{sln_dir}plugins/%{prj.name}/**.cpp",
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
		defines { "NOMINMAX" }
		postbuildcommands { "%{ml_copy} %{bin_lib}%{prj.name}.dll %{bin_out}" }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --