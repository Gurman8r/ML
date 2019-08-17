#include <ML/Engine/Python.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	int32_t Py::Run_SandboxedString(const String & value)
	{
		Py_Initialize();
		int32_t retval = PyRun_SimpleString(value.c_str());
		Py_Finalize();
		return retval;
	}

	int32_t Py::Run_SandboxedFile(const String & filename)
	{
		if (const String file { ML_FS.getFileContents(filename) })
		{
			return Run_SandboxedString(file.c_str());
		}
		return EXIT_FAILURE;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}