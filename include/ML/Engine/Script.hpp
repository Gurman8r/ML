#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_ENGINE_API Script final : public I_Newable, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Language { Lua, Python, Unknown };

		static constexpr C_String Language_names[] = {
			"Lua", "Python", "Unknown"
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script();
		Script(Language language);
		Script(Language language, const String & text);
		explicit Script(const String & filename);
		Script(const Script & copy);
		~Script();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(const String & filename) override;
		int32_t execute() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script & setLanguage(Language value);
		Script & setText(const String & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const
		{
			return ((m_lang != Language::Unknown) && m_text);
		}

		inline auto language()	const -> const Language &	{ return m_lang; }
		inline auto path()		const -> const String &		{ return m_path; }
		inline auto text()		const -> const String &		{ return m_text; }

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