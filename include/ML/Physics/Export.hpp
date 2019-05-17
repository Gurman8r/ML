#ifndef _ML_PHYSICS_EXPORT_HPP_
#define _ML_PHYSICS_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

// MemeLib Physics

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_PHYSICS_EXPORTS)
#	define ML_PHYSICS_API ML_API_EXPORT
# else
#	define ML_PHYSICS_API ML_API_IMPORT
# endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PHYSICS_EXPORT_HPP_