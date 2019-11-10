-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Network
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Network"
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
		"RakNet",
	}
	defines
	{
		"ML_NETWORK_EXPORTS", 
		"_CRT_SECURE_NO_WARNINGS",
		"_WINSOCK_DEPRECATED_NO_WARNINGS",
	}
	includedirs
	{
		"%{sln_dir}include", 
		"%{ext_dir}",
	}
	files 
	{
		"%{inc_dir}**.h", 
		"%{inc_dir}**.hpp", 
		"%{inc_dir}**.inl",  
		"%{src_dir}**.c", 
		"%{src_dir}**.cpp",
		"%{ext_dir}RakNet/**.h",
		"%{ext_dir}RakNet/**.cpp",
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{ 
		"ML_Core",
	}
	
	filter { "configurations:Debug" }
		symbols "On"
	
	filter { "configurations:Release" } 
		optimize "Speed"
	
	filter { "system:Windows" }
		links
		{
			"ws2_32",
		}
		linkoptions
		{
			"/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib",
		}
		postbuildcommands
		{
			"%{ml_copy} %{bin_lib}ML_%{prj.name}.dll %{bin_out}"
		}
		

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --