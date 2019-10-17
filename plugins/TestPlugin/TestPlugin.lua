-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Plugins"
project "TestPlugin"
	targetname 		("%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{sln_dir}proj/plugins/%{prj.name}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")
	includedirs 	{ "%{sln_dir}include", "%{dep_dir}include", "%{sln_dir}plugins/%{prj.name}" }
	defines 		{ "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Launcher" }
	files 
	{
		"%{inc_dir}**.h", "%{inc_dir}**.hpp", "%{inc_dir}**.inl",  
		"%{src_dir}**.c", "%{src_dir}**.cpp" 
	}
	vpaths 
	{ 
		["Headers"] = { "**.h", "**.hpp", "**.inl", },
		["Sources"] = { "**.c", "**.cpp" } 
	}
	libdirs
	{
		"%{sln_dir}lib/", "%{sln_dir}lib/%{cfg.buildcfg}/", "%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{dep_dir}lib/", "%{dep_dir}lib/%{cfg.buildcfg}/", "%{dep_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Editor_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Engine_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.platform}"
	}
	filter "configurations:Debug" symbols ("On")
	filter "configurations:Release" optimize ("Speed")
	filter ("system:Windows") postbuildcommands { "xcopy /y %{lib_dir}%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}" }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --