#ifndef _ML_SCRIPTABLE_OBJECT_HPP_
#define _ML_SCRIPTABLE_OBJECT_HPP_

#include <ML/Engine/Script.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_ENGINE_API ScriptableObject final
		: public I_Newable
		, public I_Disposable
		, public I_NonCopyable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ScriptableObject();
		~ScriptableObject();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool dispose() override;

		virtual bool loadFromFile(const String & filename) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto script()		-> Script &			{ return m_script; }
		inline auto script() const	-> const Script &	{ return m_script; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Script m_script;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPTABLE_OBJECT_HPP_