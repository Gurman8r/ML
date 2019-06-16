#ifndef _ML_STYLE_LOADER_HPP_
#define _ML_STYLE_LOADER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Core/IReadable.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API StyleLoader final
		: public IObject
		, public IReadable
	{
	public:
		StyleLoader();
		explicit StyleLoader(const String & filename);
		StyleLoader(StyleLoader && copy);
		~StyleLoader();

	public:
		bool loadFromFile(const String & filename) override;

		inline operator bool() const { return m_good; }

	private:
		bool m_good;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STYLE_LOADER_HPP_