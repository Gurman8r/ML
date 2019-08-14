#ifndef _COMMAND_SUITE_HPP_
#define _COMMAND_SUITE_HPP_

#include <ML/Editor/EditorPlugin.hpp>
#include <ML/Engine/CommandImpl.hpp>

extern "C"
{
	ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem);
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API CommandSuite final : public EditorPlugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit CommandSuite(EventSystem & eventSystem);

		~CommandSuite() {}

		virtual void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: List<CommandImpl *> m_commands;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_COMMAND_SUITE_HPP_