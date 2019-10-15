-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

project "TestPlugin"
	targetname 		("%{prj.name}_%{prj_ext}")
	location		("%{sln_dir}plugins/%{prj.name}")
	kind			("SharedLib")
	language		("C++")
	targetdir		(lib_dir)
	objdir			(obj_dir)
	cppdialect 		("C++17")
	staticruntime 	("Off")
	systemversion 	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}include", "%{prj.location}" }
	defines 		{ "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Launcher" }
	files 			{ "%{prj.location}/**.hpp", "%{prj.location}/**.cpp" }
	vpaths 			{ ["Header Files"] = { "**.h", "**.hpp" }, ["Source Files"] = { "**.c", "**.cpp"} }
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
		"%{wks.name}_Audio_%{prj_ext}",
		"%{wks.name}_Core_%{prj_ext}",
		"%{wks.name}_Editor_%{prj_ext}",
		"%{wks.name}_Engine_%{prj_ext}",
		"%{wks.name}_Graphics_%{prj_ext}",
		"%{wks.name}_Network_%{prj_ext}",
		"%{wks.name}_Window_%{prj_ext}"
	}
	filter "system:Windows"
		postbuildcommands {	"xcopy /y %{lib_dir}%{prj.name}_%{prj_ext}.dll %{bin_dir}" }
	filter "configurations:Debug"
		symbols "On"
	filter "configurations:Release"
		optimize "Speed"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --