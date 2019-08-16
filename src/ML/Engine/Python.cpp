#include <ML/Engine/Python.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	PyObject * Py::Run_SimpleString(const String & value)
	{
		Py_Initialize();
		PyRun_SimpleString(value.c_str());
		Py_Finalize();
		return nullptr;
	}

	PyObject * Py::Run_SimpleFile(const String & filename)
	{
		if (const String file { ML_FS.getFileContents(filename) })
		{
			return Run_SimpleString(file.c_str());
		}
		return nullptr;
	}
}