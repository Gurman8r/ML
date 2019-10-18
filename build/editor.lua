-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Editor
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Editor"
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
	defines 		{ "ML_EDITOR_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Audio", "Core", "Engine", "Graphics", "Network", "Window" }
	files 
	{ 
		"%{inc_dir}**.h", "%{inc_dir}**.hpp", "%{inc_dir}**.inl", 
		"%{src_dir}**.c", "%{src_dir}**.cpp", 
		"%{sln_dir}src/ML/%{prj.name}/**.cpp",
		"%{dep_dir}include/imgui/**.h",
		"%{dep_dir}include/imgui/**.cpp",
		"%{dep_dir}include/ImGuiColorTextEdit/**.h",
		"%{dep_dir}include/ImGuiColorTextEdit/**.cpp",
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
		"ML_Engine_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Graphics_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Network_%{cfg.buildcfg}_%{cfg.platform}",
		"ML_Window_%{cfg.buildcfg}_%{cfg.platform}",
	}
	filter ("configurations:Debug") symbols ("On")
	filter ("configurations:Release") optimize ("Speed")
	filter ("system:windows")
		postbuildcommands
		{
			"xcopy /y %{lib_dir}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_dir}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --