-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "Vendor"
project "RakNet"
	targetname 		"RakNet"
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
		"_WINSOCK_DEPRECATED_NO_WARNINGS",
	}
	includedirs {
		"%{ext_dir}RakNet/Source",
	}
	files {
		"%{ext_dir}RakNet/Source/**.h",
		"%{ext_dir}RakNet/Source/**.cpp",
	}
	libdirs {
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	
	filter { "configurations:Debug" }
		symbols "On"
	
	filter { "configurations:Release" } 
		optimize "Speed"
	
	filter { "system:Windows" }
		defines { "NOMINMAX" }
		links {
			"ws2_32",
		}
		linkoptions {
			"/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib",
		}
		

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --