-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Engine
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Engine"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}ML/%{prj.name}/")
	targetdir		("%{bin_lib}")
	objdir			("%{bin_obj}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}", "%{ext_dir}cpython/Include" }
	defines 		{ "ML_ENGINE_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Audio", "Core", "Graphics", "Network", "Window" }
	files 
	{
		"%{inc_dir}**.h", "%{inc_dir}**.hpp", "%{inc_dir}**.inl",  
		"%{src_dir}**.c", "%{src_dir}**.cpp",
		"%{ext_dir}lua/**.h", "%{ext_dir}lua/**.hpp", "%{ext_dir}lua/**.c",
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.platform}",
	}
	filter ("configurations:Debug") 
		symbols ("On")
		links { "python39_d" }
	filter ("configurations:Release") 
		optimize ("Speed") 
		links { "python39" }
	filter ("system:windows")
		linkoptions ("/NODEFAULTLIB:LIBCMT.lib")
		postbuildcommands
		{
			"xcopy /y %{bin_lib}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_out}",
			"if %{cfg.buildcfg} == Debug ( xcopy /y %{ext_bin}%{cfg.buildcfg}\\%{cfg.platform}\\python39_d.dll %{bin_out} )",
			"if %{cfg.buildcfg} == Release ( xcopy /y %{ext_bin}%{cfg.buildcfg}\\%{cfg.platform}\\python39.dll %{bin_out} )"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --