#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Export.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/StringUtility.hpp>
#include <python/Python.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Py ::ml::Py::getInstance()

namespace ml
{
	class ML_ENGINE_API Py final : public Singleton<Py>, Disposable
	{
		friend struct Singleton<Py>;
		bool	m_init { false };
		String	m_name {};
		String	m_home {};

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool init(const String & name, const String & home)
		{
			if (!m_init && name && ML_FS.dirExists(home))
			{
				Py_SetProgramName(util::widen(m_name = name).c_str());
				Py_SetPythonHome(util::widen(m_home = home).c_str());
				Py_Initialize();
				return (m_init = true);
			}
			return false;
		}

		inline bool restart()
		{
			if (dispose())
			{
				return init(m_name, m_home);
			}
			return false;
		}

		inline bool dispose() override
		{
			if (m_init)
			{
				Py_Finalize();
				return !(m_init = false);
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline int32_t DoString(const String & value)
		{
			return ((value && m_init) ? PyRun_SimpleString(value.c_str()) : 0);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PYTHON_HPP_