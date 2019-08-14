#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Readable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Currently empty, leftover from old scripting lib.
	struct ML_ENGINE_API Script final
		: public I_Disposable
		, public I_Newable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Script();
		explicit Script(const String & filename);
		Script(Script && copy);
		~Script() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const 
		{ 
			return m_filename; 
		}

		inline auto filename()	const -> const String & { return m_filename; }
		inline auto params()	const -> const String & { return m_params; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String m_filename;
		String m_params;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_HPP_