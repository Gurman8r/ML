#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_ENGINE_API Script final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Language { Unknown, Lua, Python };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script();
		Script(Language language);
		Script(Language language, String const & text);
		explicit Script(String const & filename);
		Script(Script const & copy);
		~Script();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(String const & filename);
		
		int32_t execute() const;

		Script & setLanguage(Language value);
		
		Script & setText(String const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const
		{
			return ((m_lang != Language::Unknown) && m_text);
		}

		inline auto language()	const -> Language const &	{ return m_lang; }
		inline auto path()		const -> String const &		{ return m_path; }
		inline auto text()		const -> String const &		{ return m_text; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Language	m_lang;
		String		m_path;
		String		m_text;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_HPP_