#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Plugin.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "Audio_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Core_"		ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Editor_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Engine_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Graphics_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Network_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Physics_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Window_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void * ML_Plugin_Test(void * data);
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_