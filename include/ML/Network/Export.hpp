#ifndef _ML_NETWORK_EXPORT_HPP_
#define _ML_NETWORK_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

// MemeLib Network

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_NETWORK_EXPORTS)
#	define ML_NETWORK_API ML_API_EXPORT
# else
#	define ML_NETWORK_API ML_API_IMPORT
# endif

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_SYSTEM_WINDOWS)
#	pragma comment(lib, "RakNet.lib")
#	pragma comment(lib, "ws2_32.lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_NETWORK_EXPORT_HPP_