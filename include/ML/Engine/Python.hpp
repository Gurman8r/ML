#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifdef ML_SYSTEM_WINDOWS
#	define MS_WINDOWS
#	if ML_DEBUG
#		pragma comment(lib, "python39_d.lib")
#	else
#		pragma comment(lib, "python39.lib")
#	endif
# endif

/* * * * * * * * * * * * * * * * * * * * */

#include <python/Python.h>
//#include <pybind11/pybind11.h>

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PYTHON_HPP_