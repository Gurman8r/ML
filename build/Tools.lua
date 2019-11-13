-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Common"
project "Tools"
	kind		"Utility"
	targetname 	"%{prj.name}"
	location	"%{prj_dir}common/%{prj.name}/"
	targetdir	"%{bin_lib}"
	objdir		"%{bin_obj}"
	files {
		"%{sln_dir}build/**.lua",
		"%{sln_dir}scripts/**.**",
		"%{sln_dir}premake5.lua",
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --