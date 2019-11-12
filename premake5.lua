-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Workspace
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "ML_%{_ACTION}"
	startproject "Launcher"
	configurations 
	{ 
		"Debug", "Release" 
	}
	platforms
	{ 
		"x86", "x64" 
	}
	filter { "platforms:*64" }
		architecture "x64"
	filter { "platforms:*32" }
		architecture "x86"
	filter { "platforms:*86" }
		architecture "x86"
		
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Global Paths
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

filter { "system:Unix", "system:Mac" }
	sln_dir = "%{wks.location}/"
	inc_dir = "%{sln_dir}include/ML/%{prj.name}/"
	src_dir = "%{sln_dir}src/ML/%{prj.name}/"
	prj_dir = "%{sln_dir}proj/%{_ACTION}/"
	bin_out = "%{sln_dir}bin/%{cfg.buildcfg}/%{cfg.platform}/"
	bin_lib = "%{sln_dir}bin-lib/%{cfg.buildcfg}/%{cfg.platform}/"
	bin_obj = "%{sln_dir}bin-obj/"
	ext_dir = "%{sln_dir}ext/"
	ext_bin = "%{sln_dir}ext-bin/"
	ext_lib = "%{sln_dir}ext-lib/"
	ml_copy	= "cp -rf"	
	ml_move = "mv -f"
	
filter { "system:Windows" }
	sln_dir = "%{wks.location}\\"
	inc_dir = "%{sln_dir}include\\ML\\%{prj.name}\\"
	src_dir = "%{sln_dir}src\\ML\\%{prj.name}\\"
	prj_dir = "%{sln_dir}proj\\%{_ACTION}\\"
	bin_out = "%{sln_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	bin_lib = "%{sln_dir}bin-lib\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	bin_obj = "%{sln_dir}bin-obj\\"
	ext_dir = "%{sln_dir}ext\\"
	ext_bin = "%{sln_dir}ext-bin\\"
	ext_lib = "%{sln_dir}ext-lib\\"
	ml_copy	= "xcopy /y"
	ml_move = "move /y"


-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Generate Project Files
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- Vendor
dofile "./build/GLFW.lua"
dofile "./build/Lua.lua"
dofile "./build/RakNet.lua"

-- MemeLib
dofile "./build/MemeLib.lua"

-- Examples
dofile "./build/Launcher.lua"
dofile "./build/Noobs.lua"
dofile "./build/CommandSuite.lua"
dofile "./build/TestPlugin.lua"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --