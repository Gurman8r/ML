-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Plugins"
project "TestPlugin"
	targetname 		("%{prj.name}")
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
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Audio", "ML_Core", "ML_Editor", "ML_Engine", "ML_Graphics", "ML_Network", "ML_Window"
	}
	filter "configurations:Debug" symbols ("On")
	filter "configurations:Release" optimize ("Speed")
	filter { "system:Windows" } postbuildcommands { Copy("%{prj.name}.dll", "%{bin_lib}", "%{bin_out}") }
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --