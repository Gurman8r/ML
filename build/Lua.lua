-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Vendor"
project "Lua"
	targetname 		"lua"
	location		"%{prj_dir}vendor/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	kind			"StaticLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"MAKE_LIB",
	}
	includedirs {
		"%{ext_dir}lua",
	}
	files {
		"%{ext_dir}lua/lapi.h", "%{ext_dir}lua/lauxlib.h", "%{ext_dir}lua/lcode.h", "%{ext_dir}lua/lctype.h",
		"%{ext_dir}lua/ldebug.h", "%{ext_dir}lua/ldo.h", "%{ext_dir}lua/lfunc.h", "%{ext_dir}lua/lgc.h",
		"%{ext_dir}lua/ljumptab.h", "%{ext_dir}lua/llex.h", "%{ext_dir}lua/llimits.h", "%{ext_dir}lua/lmem.h",
		"%{ext_dir}lua/lobject.h", "%{ext_dir}lua/lopcodes.h", "%{ext_dir}lua/lopnames.h", "%{ext_dir}lua/lparser.h",
		"%{ext_dir}lua/lprefix.h", "%{ext_dir}lua/lstate.h", "%{ext_dir}lua/lstring.h", "%{ext_dir}lua/ltable.h",
		"%{ext_dir}lua/ltests.h", "%{ext_dir}lua/ltm.h", "%{ext_dir}lua/luaconf.h", "%{ext_dir}lua/lua.h",
		"%{ext_dir}lua/lualib.h", "%{ext_dir}lua/lundump.h", "%{ext_dir}lua/lvm.h", "%{ext_dir}lua/lzio.h",
		"%{ext_dir}lua/lapi.c", "%{ext_dir}lua/lauxlib.c", "%{ext_dir}lua/lbaselib.c", "%{ext_dir}lua/lcode.c",
		"%{ext_dir}lua/lcorolib.c", "%{ext_dir}lua/lctype.c", "%{ext_dir}lua/ldblib.c", "%{ext_dir}lua/ldebug.c",
		"%{ext_dir}lua/ldo.c", "%{ext_dir}lua/ldump.c", "%{ext_dir}lua/lfunc.c", "%{ext_dir}lua/lgc.c",
		"%{ext_dir}lua/linit.c", "%{ext_dir}lua/liolib.c", "%{ext_dir}lua/llex.c", "%{ext_dir}lua/lmathlib.c",
		"%{ext_dir}lua/lmem.c", "%{ext_dir}lua/loadlib.c", "%{ext_dir}lua/lobject.c", "%{ext_dir}lua/lopcodes.c",
		"%{ext_dir}lua/loslib.c", "%{ext_dir}lua/lparser.c", "%{ext_dir}lua/lstate.c", "%{ext_dir}lua/lstring.c",
		"%{ext_dir}lua/lstrlib.c", "%{ext_dir}lua/ltable.c", "%{ext_dir}lua/ltablib.c", "%{ext_dir}lua/ltests.c",
		"%{ext_dir}lua/ltm.c", "%{ext_dir}lua/lua.c", "%{ext_dir}lua/lundump.c", "%{ext_dir}lua/lutf8lib.c",
		"%{ext_dir}lua/lvm.c", "%{ext_dir}lua/lzio.c",
	}
	excludes { 
		"onelua.c",
	}
	libdirs {
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	
	filter { "configurations:Debug" }
		symbols "On" 
		
	filter { "configurations:Release" }
		optimize "Speed" 
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --