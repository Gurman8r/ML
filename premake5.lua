-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Workspace
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "ML"
	startproject ("Launcher")
	configurations { "Debug", "Release" }
	platforms { "x86", "x64" }
	filter ("platforms:*32") architecture("x86")
	filter ("platforms:*86") architecture("x86")
	filter ("platforms:*64") architecture("x64")
	
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Paths
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

filter ("system:not windows")
	sln_dir = "%{wks.location}/"
	bin_dir = "%{sln_dir}bin/%{cfg.buildcfg}/%{cfg.platform}/"
	lib_dir = "%{sln_dir}lib/%{cfg.buildcfg}/%{cfg.platform}/"
	obj_dir = "%{sln_dir}obj/"
	inc_dir = "%{sln_dir}include/ML/%{prj.name}/"
	src_dir = "%{sln_dir}src/ML/%{prj.name}/"
	prj_dir = "%{sln_dir}proj/ML/%{prj.name}/"
	dep_dir = "%{sln_dir}thirdparty/"
	
filter ("system:windows")
	sln_dir = "%{wks.location}\\"
	bin_dir = "%{sln_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	lib_dir = "%{sln_dir}lib\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	obj_dir = "%{sln_dir}obj\\"
	inc_dir = "%{sln_dir}include\\ML\\%{prj.name}\\"
	src_dir = "%{sln_dir}src\\ML\\%{prj.name}\\"
	prj_dir = "%{sln_dir}proj\\ML\\%{prj.name}\\"
	dep_dir = "%{sln_dir}thirdparty\\"


-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- MemeLib
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
	
dofile "./build/core.lua"
dofile "./build/audio.lua"
dofile "./build/network.lua"
dofile "./build/window.lua"
dofile "./build/graphics.lua"
dofile "./build/engine.lua"
dofile "./build/editor.lua"
dofile "./build/launcher.lua"

	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Plugins		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

dofile "./plugins/Noobs/Noobs.lua"
dofile "./plugins/CommandSuite/CommandSuite.lua"
dofile "./plugins/TestPlugin/TestPlugin.lua"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --