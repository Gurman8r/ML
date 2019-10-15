-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "CommandSuite"
	targetname 		("%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{sln_dir}plugins/%{prj.name}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include", "%{prj.location}" }
	defines 		{ "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Launcher" }
	vpaths 			{ ["Headers"] = { "**.h", "**.hpp", "**.hxx" }, ["Sources"] = { "**.c", "**.cpp", "**.cxx" } }
	files 			{ "%{prj.location}/**.hpp", "%{prj.location}/**.cpp" }
	libdirs
	{
		"%{sln_dir}lib/",
		"%{sln_dir}lib/%{cfg.buildcfg}/",
		"%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{sln_dir}thirdparty/lib/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/",
		"%{sln_dir}thirdparty/lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"%{wks.name}_Audio_%{cfg.buildcfg}_%{cfg.platform}",
		"%{wks.name}_Core_%{cfg.buildcfg}_%{cfg.platform}",
		"%{wks.name}_Editor_%{cfg.buildcfg}_%{cfg.platform}",
		"%{wks.name}_Engine_%{cfg.buildcfg}_%{cfg.platform}",
		"%{wks.name}_Graphics_%{cfg.buildcfg}_%{cfg.platform}",
		"%{wks.name}_Network_%{cfg.buildcfg}_%{cfg.platform}",
		"%{wks.name}_Window_%{cfg.buildcfg}_%{cfg.platform}"
	}
	filter "configurations:Debug" symbols ("On")
	filter "configurations:Release" optimize ("Speed")
	filter ("system:Windows") postbuildcommands { "xcopy /y %{lib_dir}%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}" }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --