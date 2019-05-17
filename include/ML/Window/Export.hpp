#ifndef _ML_WINDOW_EXPORT_HPP_
#define _ML_WINDOW_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

// ML Window Exports

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_WINDOW_EXPORTS)
#	define ML_WINDOW_API ML_API_EXPORT
# else
#	define ML_WINDOW_API ML_API_IMPORT
# endif

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_SYSTEM_WINDOWS)
#	pragma comment(lib, "glfw3.lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_WINDOW_EXPORT_HPP_