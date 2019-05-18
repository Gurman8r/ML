#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Plugin.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "Audio"		"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Core"		"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Editor"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Engine"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Graphics"	"_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Network"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Physics"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Window"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void * ML_Plugin_Test(void * data);
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_