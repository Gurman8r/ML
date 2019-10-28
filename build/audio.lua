-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --	
-- Audio
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

group "MemeLib"
project "Audio"
	targetname 		("ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")
	location		("%{prj_dir}")
	targetdir		("%{bin_lib}")
	objdir			("%{bin_obj}")
	kind			("SharedLib")
	language		("C++")
	cppdialect 		("C++17")
	staticruntime	("Off")
	systemversion	("latest")
	includedirs 	{ "%{sln_dir}include", "%{ext_dir}" }
	defines 		{ "ML_AUDIO_EXPORTS", "_CRT_SECURE_NO_WARNINGS" }
	dependson 		{ "Core" }
	files 
	{
		"%{inc_dir}**.h", "%{inc_dir}**.hpp", "%{inc_dir}**.inl",  
		"%{src_dir}**.c", "%{src_dir}**.cpp" 
	}
	libdirs
	{
		"%{bin_lib}", "%{bin_lib}%{cfg.buildcfg}/", "%{bin_lib}%{cfg.buildcfg}/%{cfg.platform}/",
		"%{ext_lib}", "%{ext_lib}%{cfg.buildcfg}/", "%{ext_lib}%{cfg.buildcfg}/%{cfg.platform}/",
	}
	links
	{
		"ML_Core_%{cfg.buildcfg}_%{cfg.platform}",
		"OpenAL32", "flac", "ogg", "vorbis", "vorbisenc", "vorbisfile",
	}
	filter ("configurations:Debug") 
		symbols ("On")
	filter ("configurations:Release") 
		optimize ("Speed")
	filter ("system:windows")
		postbuildcommands 
		{	
			"xcopy /y %{bin_lib}ML_%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}.dll %{bin_out}",
			"xcopy /y %{ext_bin}OpenAL32.dll %{bin_out}"
		}
		
		
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --