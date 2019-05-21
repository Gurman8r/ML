#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/PluginAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_SYSTEM_WINDOWS)
#	pragma comment(lib, ML_LIB_STR("Audio")		)
#	pragma comment(lib, ML_LIB_STR("Core")		)
#	pragma comment(lib, ML_LIB_STR("Editor")	)
#	pragma comment(lib, ML_LIB_STR("Engine")	)
#	pragma comment(lib, ML_LIB_STR("Graphics")	)
#	pragma comment(lib, ML_LIB_STR("Network")	)
#	pragma comment(lib, ML_LIB_STR("Physics")	)
#	pragma comment(lib, ML_LIB_STR("Script")	)
#	pragma comment(lib, ML_LIB_STR("Window")	)
# endif

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void * ML_Plugin_Test(void * data);
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_