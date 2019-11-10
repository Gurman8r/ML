-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --	
-- Audio
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Audio"
	targetname 		"ML_%{prj.name}"
	location		"%{prj_dir}ML/%{prj.name}/"
	targetdir		"%{bin_lib}"
	objdir			"%{bin_obj}"
	kind			"SharedLib"
	language		"C++"
	cppdialect 		"C++17"
	staticruntime	"Off"
	systemversion	"latest"
	dependson 
	{
		"Core",
	}
	defines
	{
		"ML_AUDIO_EXPORTS", 
		"_CRT_SECURE_NO_WARNINGS",
	}
	includedirs
	{
		"%{sln_dir}include", 
		"%{ext_dir}",
		"%{ext_dir}openal-soft/include",
	}
	files 
	{
		"%{inc_dir}**.h",
		"%{inc_dir}**.hpp",
		"%{inc_dir}**.inl",  
		"%{src_dir}**.c",
		"%{src_dir}**.cpp" 
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Core",
		"OpenAL32", "flac", "ogg", "vorbis", "vorbisenc", "vorbisfile",
	}
	
	filter { "configurations:Debug" }
		symbols "On"
	
	filter { "configurations:Release" } 
		optimize "Speed"
	
	filter { "system:Windows" }
		postbuildcommands 
		{	
			"%{ml_copy} %{bin_lib}ML_%{prj.name}.dll %{bin_out}",
			"%{ml_copy} %{ext_bin}OpenAL32.dll %{bin_out}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --