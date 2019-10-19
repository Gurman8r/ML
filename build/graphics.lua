-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Graphics
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Graphics"
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
	defines 		{ "ML_GRAPHICS_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Core", "Window" }
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
		"ML_Window_%{cfg.buildcfg}_%{cfg.platform}",
		"glew32s", "opengl32", "assimp", "IrrXML", "zlibstatic",
	}
	filter ("configurations:Debug") 
		symbols ("On")
	filter ("configurations:Release") 
		optimize ("Speed")
	filter ("system:windows")
		linkoptions ("/NODEFAULTLIB:LIBCMT.lib /NODEFAULTLIB:LIBCMTD.lib")
		postbuildcommands 
		{
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}",
			"xcopy /y %{dep_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\assimp.dll %{bin_dir}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --