-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --	
-- Window
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Window"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}")
	targetdir		("%{lib_dir}")
	objdir			("%{obj_dir}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{dep_dir}include" }
	defines 		{ "ML_WINDOW_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Core" }
	files 
	{
		"%{inc_dir}**.h", "%{inc_dir}**.hpp", "%{inc_dir}**.inl",  
		"%{src_dir}**.c", "%{src_dir}**.cpp" 
	}
	libdirs
	{
		"%{sln_dir}lib/", "%{sln_dir}lib/%{cfg.buildcfg}/", "%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{dep_dir}lib/", "%{dep_dir}lib/%{cfg.buildcfg}/", "%{dep_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links 
	{
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}", 
		"opengl32", "glfw3",
	}
	filter ("configurations:Debug") 
		symbols ("On") 
		linkoptions ("/NODEFAULTLIB:MSVCRT.lib")
	filter ("configurations:Release") 
		optimize ("Speed")
	filter ("system:windows")
		postbuildcommands
		{
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --