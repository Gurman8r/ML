#ifndef _ML_STYLE_LOADER_HPP_
#define _ML_STYLE_LOADER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/NonNewable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API ImGuiStyleLoader final : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ImGuiStyleLoader();
		
		explicit ImGuiStyleLoader(const String & filename);
		
		ImGuiStyleLoader(ImGuiStyleLoader && copy);
		
		~ImGuiStyleLoader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(const String & filename);

		inline operator bool() const { return m_good; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: bool m_good;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STYLE_LOADER_HPP_