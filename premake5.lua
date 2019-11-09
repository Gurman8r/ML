-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Workspace
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "ML_%{_ACTION}"
	startproject ("Launcher")
	configurations { "Debug", "Release" }
	platforms { "x86", "x64" }
	filter ("platforms:*32") architecture("x86")
	filter ("platforms:*86") architecture("x86")
	filter ("platforms:*64") architecture("x64")
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Paths
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

filter { "system:not Windows" }
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
	cmd_copy = "cp -rf"
	
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
	cmd_copy = "xcopy /y"


-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --
-- Functions
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

function Copy(filename, srcPath, dstPath)
	return (cmd_copy .. " " .. srcPath .. filename .. " " .. dstPath)
end

-- print(Copy("myfile.dll", "/lib/etc/", "/bin/etc/"))
-- os.exit(1)


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

dofile "./plugins/Noobs.lua"
dofile "./plugins/CommandSuite.lua"
dofile "./plugins/TestPlugin.lua"
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --