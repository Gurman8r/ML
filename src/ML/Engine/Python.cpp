#include <ML/Engine/Python.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	PyObject * Py::Run_SandboxedString(const String & value)
	{
		Py_Initialize();
		PyRun_SimpleString(value.c_str());
		Py_Finalize();
		return nullptr;
	}

	PyObject * Py::Run_SandboxedFile(const String & filename)
	{
		if (const String file { ML_FS.getFileContents(filename) })
		{
			return Run_SandboxedString(file.c_str());
		}
		return nullptr;
	}
}