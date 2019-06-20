#ifndef _ML_STYLE_LOADER_HPP_
#define _ML_STYLE_LOADER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/INewable.hpp>
#include <ML/Core/IReadable.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ImGui_StyleLoader final
		: public INewable
		, public IReadable
	{
	public:
		ImGui_StyleLoader();
		explicit ImGui_StyleLoader(const String & filename);
		ImGui_StyleLoader(ImGui_StyleLoader && copy);
		~ImGui_StyleLoader();

	public:
		bool loadFromFile(const String & filename) override;

		inline operator bool() const { return m_good; }

	private:
		bool m_good;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STYLE_LOADER_HPP_