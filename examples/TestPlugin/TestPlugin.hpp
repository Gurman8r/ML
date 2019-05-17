#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeEngine/Plugin.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "MemeAudio_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "MemeCore_"		ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "MemeEditor_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "MemeEngine_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "MemeNet_"		ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "MemePhysics_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "MemeScript_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "MemeWindow_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void * ML_Plugin_Test(void * data);
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_