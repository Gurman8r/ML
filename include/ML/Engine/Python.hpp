#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Export.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/StringUtility.hpp>

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

#define ML_Py ::ml::Py::getInstance()

namespace ml
{
	struct ML_ENGINE_API Py final : public I_Singleton<Py>
	{
		inline bool Initialize(const String & name, const String & path)
		{
			if (!m_init && name && ML_FS.dirExists(path))
			{
				Py_SetProgramName(alg::widen(name).c_str());
				Py_SetPythonHome(alg::widen(path).c_str());
				Py_Initialize();
				return (m_init = true);
			}
			return false;
		}

		inline bool Finalize()
		{
			if (m_init)
			{
				Py_Finalize();
				return !(m_init = false);
			}
			return false;
		}

		inline int32_t SimpleString(const String & value)
		{
			if (m_init)
			{
				return PyRun_SimpleString(value.c_str());
			}
			return 0;
		}

	private:
		friend struct I_Singleton<Py>;
		bool m_init { false };
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PYTHON_HPP_