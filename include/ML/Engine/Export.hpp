#ifndef _ML_ENGINE_EXPORT_HPP_
#define _ML_ENGINE_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

// MemeLib Engine

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#if defined(ML_STATIC)
#	define ML_ENGINE_API
#elif defined(ML_ENGINE_EXPORTS)
#	define ML_ENGINE_API ML_API_EXPORT
#else
#	define ML_ENGINE_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ENGINE_EXPORT_HPP_