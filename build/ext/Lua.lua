-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Vendor"
project "Lua"
	targetname 		"lua"
	location		"%{prj_dir}ext/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	kind			"SharedLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson 
	{
		"Audio", "Core", "Graphics", "Network", "Window",
		"Lua",
	}
	defines
	{
		"ML_ENGINE_EXPORTS", 
		"_CRT_SECURE_NO_WARNINGS",
		"MAKE_LIB",
	}
	includedirs
	{
		"%{sln_dir}include", 
		"%{ext_dir}",
		"%{ext_dir}pybind11/include",
		"%{ext_dir}cpython/Include",
		"%{ext_dir}cpython/Include/internal",
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