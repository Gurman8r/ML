#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Export.hpp>
#include <ML/Core/FileSystem.hpp>

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
	struct ML_ENGINE_API Py final
	{
		Py() = delete;

		static inline int32_t RunString(const String & value)
		{
			Py_Initialize();
			int32_t retval = PyRun_SimpleString(value.c_str());
			Py_Finalize();
			return retval;
		}

		static inline int32_t RunFile(const String & filename)
		{
			if (const String file { ML_FS.getFileContents(filename) })
			{
				return RunString(file.c_str());
			}
			return EXIT_FAILURE;
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PYTHON_HPP_