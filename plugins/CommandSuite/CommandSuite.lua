-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Plugins"
project "CommandSuite"
	targetname 		("%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}plugins/%{prj.name}")
	targetdir		("%{bin_lib}")
	objdir			("%{bin_obj}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}", "%{sln_dir}plugins/%{prj.name}" }
	defines 		{ "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Launcher" }
	files 
	{ 
		"%{sln_dir}plugins/%{prj.name}/**.h", 
		"%{sln_dir}plugins/%{prj.name}/**.hpp",
		"%{sln_dir}plugins/%{prj.name}/**.inl",
		"%{sln_dir}plugins/%{prj.name}/**.c",
		"%{sln_dir}plugins/%{prj.name}/**.cpp"
	}
	vpaths 
	{ 
		["Headers"] = { "**.h", "**.hpp", "**.inl", },
		["Sources"] = { "**.c", "**.cpp" } 
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
		"ML_Editor_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Engine_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.platform}"
	}
	filter "configurations:Debug" symbols ("On")
	filter "configurations:Release" optimize ("Speed")
	filter ("system:Windows") postbuildcommands { "xcopy /y %{bin_lib}%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_out}" }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --