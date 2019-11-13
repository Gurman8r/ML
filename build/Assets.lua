-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Common"
project "Assets"
	kind		"Utility"
	targetname 	"%{prj.name}"
	location	"%{prj_dir}common/%{prj.name}/"
	targetdir	"%{bin_lib}"
	objdir		"%{bin_obj}"
	files {
		"%{sln_dir}assets/**.**", 
		"%{sln_dir}ML.ini", 
	}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --