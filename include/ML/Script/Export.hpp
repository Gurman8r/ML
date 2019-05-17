#ifndef _ML_SCRIPT_EXPORT_HPP_
#define _ML_SCRIPT_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

// MemeLib Script

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_SCRIPT_EXPORTS)
#	define ML_SCRIPT_API ML_API_EXPORT
# else
#	define ML_SCRIPT_API ML_API_IMPORT
# endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_SCRIPT_EXPORT_HPP_