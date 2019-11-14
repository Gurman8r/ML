-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Common"
	kind		"Utility"
	targetname 	"%{prj.name}"
	location	"%{prj_dir}ML/%{prj.name}/"
	targetdir	"%{bin_lib}"
	objdir		"%{bin_obj}"
	files {
		"%{sln_dir}assets/**.**", 
		"%{sln_dir}build/**.lua",
		"%{sln_dir}tools/**.**",
		"%{sln_dir}ML.ini", 
		"%{sln_dir}premake5.lua",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --