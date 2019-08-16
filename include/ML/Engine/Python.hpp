#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Export.hpp>
#include <ML/Core/String.hpp>

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
#include <pybind11/embed.h>
#include <pybind11/stl.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct ML_ENGINE_API Py
	{
		static PyObject * Run_SandboxedString(const String & value);

		static PyObject * Run_SandboxedFile(const String & filename);
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PYTHON_HPP_