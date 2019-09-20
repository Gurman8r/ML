#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Readable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_ENGINE_API Script final
		: public I_Newable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Language { Lua, Python };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script();
		Script(Language language);
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

		inline auto filename()	const -> const String & { return m_filename; }
		inline auto language() const -> const Language & { return m_language; }
		inline auto text() const -> const String & { return m_text; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Language m_language;
		String m_filename;
		String m_text;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_HPP_