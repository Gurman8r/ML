#ifndef _ML_PYTHON_HPP_
#define _ML_PYTHON_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Export.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/StringUtility.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Py ::ml::Py::getInstance()

namespace ml
{
	class ML_ENGINE_API Py final : public Singleton<Py>, Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend struct Singleton<Py>;

		Py() {}
		~Py() {}
		
		bool	m_init { false };
		String	m_name {};
		String	m_home {};

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool	init(String const & name, String const & home);
		bool	restart();
		bool	dispose() override;
		int32_t doString(String const & value) const;
		int32_t doFile(String const & filename) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PYTHON_HPP_