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
	filter { "platforms:*32", "platforms:*86" }
		architecture "x86"
	
	filter { "platforms:*64" }
		architecture "x64"
		
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Paths
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

filter { "system:Unix", "system:Mac" }
	sln_dir  = "%{wks.location}/"
	inc_dir  = "%{sln_dir}include/ML/%{prj.name}/"
	src_dir  = "%{sln_dir}src/ML/%{prj.name}/"
	prj_dir  = "%{sln_dir}proj/%{_ACTION}/"
	bin_out  = "%{sln_dir}bin/%{cfg.buildcfg}/%{cfg.platform}/"
	bin_lib  = "%{sln_dir}bin-lib/%{cfg.buildcfg}/%{cfg.platform}/"
	bin_obj  = "%{sln_dir}bin-obj/"
	ext_dir  = "%{sln_dir}ext/"
	ext_bin  = "%{sln_dir}ext-bin/"
	ext_lib  = "%{sln_dir}ext-lib/"
	
filter { "system:Windows" }
	sln_dir  = "%{wks.location}\\"
	inc_dir  = "%{sln_dir}include\\ML\\%{prj.name}\\"
	src_dir  = "%{sln_dir}src\\ML\\%{prj.name}\\"
	prj_dir  = "%{sln_dir}proj\\%{_ACTION}\\"
	bin_out  = "%{sln_dir}bin\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	bin_lib  = "%{sln_dir}bin-lib\\%{cfg.buildcfg}\\%{cfg.platform}\\"
	bin_obj  = "%{sln_dir}bin-obj\\"
	ext_dir  = "%{sln_dir}ext\\"
	ext_bin  = "%{sln_dir}ext-bin\\"
	ext_lib  = "%{sln_dir}ext-lib\\"


-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Generate Files
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

dofile "./build/memelib/core.lua"
dofile "./build/memelib/audio.lua"
dofile "./build/memelib/network.lua"
dofile "./build/memelib/window.lua"
dofile "./build/memelib/graphics.lua"
dofile "./build/memelib/engine.lua"
dofile "./build/memelib/editor.lua"
dofile "./build/memelib/launcher.lua"

dofile "./build/plugins/Noobs.lua"
dofile "./build/plugins/CommandSuite.lua"
dofile "./build/plugins/TestPlugin.lua"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --