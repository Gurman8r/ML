#ifndef _ML_AUDIO_EXPORT_HPP_
#define _ML_AUDIO_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

// MemeLib Audio

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#if defined(ML_STATIC)
#	define ML_AUDIO_API
#elif defined(ML_AUDIO_EXPORTS)
#	define ML_AUDIO_API ML_API_EXPORT
#else
#	define ML_AUDIO_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_AUDIO_EXPORT_HPP_