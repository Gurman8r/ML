-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Core
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Core"
	targetname 		("ML_%{prj.name}")
	targetdir		("%{bin_lib}")
	objdir			("%{bin_obj}")
	location		("%{prj_dir}ML/%{prj.name}/")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}" }
	defines 		{ "ML_CORE_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	files 
	{
		"%{inc_dir}**.h", 
		"%{inc_dir}**.hpp", 
		"%{inc_dir}**.inl",  
		"%{src_dir}**.c", 
		"%{src_dir}**.cpp",
	}
	filter { "configurations:Debug" } 
		symbols ("On")
	filter { "configurations:Release" } 
		optimize ("Speed")
	filter { "system:Windows" }
		postbuildcommands { Copy("ML_%{prj.name}.dll", "%{bin_lib}", "%{bin_out}") }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --