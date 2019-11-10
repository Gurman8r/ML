-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Graphics
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Graphics"
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
		"Core", "Window",
	}
	defines
	{
		"ML_GRAPHICS_EXPORTS", 
		"_CRT_SECURE_NO_WARNINGS",
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
		"%{src_dir}**.cpp" 
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Core", "ML_Window",
		"opengl32", "glew32s", "assimp", "IrrXML", "zlibstatic",
	}
	
	filter { "configurations:Debug" }
		symbols "On"
	
	filter { "configurations:Release" } 
		optimize "Speed"
	
	filter { "system:Windows" }
		linkoptions
		{
			"/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib",
		}
		postbuildcommands 
		{
			"%{ml_copy} %{bin_lib}ML_%{prj.name}.dll %{bin_out}",
			"%{ml_copy} %{ext_bin}%{cfg.buildcfg}\\%{cfg.platform}\\assimp.dll %{bin_out}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --