#ifndef _ML_PLUGIN_API_HPP_
#define _ML_PLUGIN_API_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/StandardLib.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PLUGIN_API	ML_API_EXPORT
#define ML_Plugin_Main	ML_Plugin_Main

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_DLL_STR(name) name "_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".dll"
#define ML_LIB_STR(name) name "_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct	SharedLibrary;
	struct	Plugin;
	using	PluginMap = typename Map<SharedLibrary *, Plugin *>;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PLUGIN_API_HPP_