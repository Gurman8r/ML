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

dofile "./build/ext/GLFW.lua"
dofile "./build/ext/Lua.lua"
dofile "./build/ext/RakNet.lua"

--dofile "./build/ML/Audio.lua"
--dofile "./build/ML/Core.lua"
--dofile "./build/ML/Editor.lua"
--dofile "./build/ML/Engine.lua"
--dofile "./build/ML/Graphics.lua"
--dofile "./build/ML/Launcher.lua"
--dofile "./build/ML/Network.lua"
--dofile "./build/ML/Window.lua"
dofile "./build/ML/MemeLib.lua"

dofile "./examples/Launcher/Launcher.lua"
dofile "./examples/Noobs/Noobs.lua"
dofile "./examples/CommandSuite/CommandSuite.lua"
dofile "./examples/TestPlugin/TestPlugin.lua"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --