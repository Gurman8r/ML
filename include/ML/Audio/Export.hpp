#ifndef _ML_AUDIO_EXPORT_HPP_
#define _ML_AUDIO_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

// ML Audio Exports

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_AUDIO_EXPORTS)
#	define ML_AUDIO_API ML_API_EXPORT
# else
#	define ML_AUDIO_API ML_API_IMPORT
# endif

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_SYSTEM_WINDOWS)
#	pragma comment(lib, "OpenAL32.lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_AUDIO_EXPORT_HPP_