-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Vendor"
project "Lua"
	targetname 		"lua"
	location		"%{prj_dir}ext/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	kind			"StaticLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"MAKE_LIB",
	}
	includedirs
	{
		"%{ext_dir}lua",
	}
	files 
	{
		"%{ext_dir}lua/*.h", 
		"%{ext_dir}lua/*.c",
	}
	libdirs
	{
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	
	filter { "configurations:Debug" }
		symbols "On" 
		
	filter { "configurations:Release" } 
		optimize "Speed" 
		
	filter { "system:Windows" }
		defines { "NOMINMAX" }
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --