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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend struct Singleton<Py>;

		Py() = default;
		
		bool	m_init { false };
		String	m_name {};
		String	m_home {};

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool	init(const String & name, const String & home);
		bool	restart();
		bool	dispose() override;
		int32_t doString(const String & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PYTHON_HPP_