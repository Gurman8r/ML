-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Examples"
project "Launcher"
	targetname 		"%{prj.name}"
	location		"%{prj_dir}examples/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	debugdir 		"%{bin_out}"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson 
	{
		"MemeLib",
	}
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}
	includedirs
	{
		"%{sln_dir}include", 
		"%{ext_dir}",
	}
	files 
	{
		"%{sln_dir}examples/%{prj.name}/**.h", 
		"%{sln_dir}examples/%{prj.name}/**.hpp",
		"%{sln_dir}examples/%{prj.name}/**.inl",
		"%{sln_dir}examples/%{prj.name}/**.c",
		"%{sln_dir}examples/%{prj.name}/**.cpp",
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"MemeLib"
	}
	
	filter { "configurations:Debug" }
		symbols "On"
		kind "ConsoleApp"
	
	filter { "configurations:Release" } 
		optimize "Speed"
		kind "WindowedApp"
	
	filter { "system:Windows" }
		defines { "NOMINMAX" }
		postbuildcommands 
		{	
			"%{ml_copy} %{bin_lib}%{prj.name}.exe %{bin_out}",
		}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --