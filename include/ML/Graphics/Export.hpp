#ifndef _ML_GRAPHICS_EXPORT_HPP_
#define _ML_GRAPHICS_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

// Meme Graphics

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_GRAPHICS_EXPORTS)
#	define ML_GRAPHICS_API ML_API_EXPORT
# else
#	define ML_GRAPHICS_API ML_API_IMPORT
# endif

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_SYSTEM_WINDOWS)
#	pragma comment(lib, "glew32s.lib")
#	pragma comment(lib, "opengl32.lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_WINDOW_EXPORT_HPP_