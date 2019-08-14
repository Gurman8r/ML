#include "CommandSuite.hpp"
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/CommandRegistry.hpp>

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem)
{
	return new ml::CommandSuite { eventSystem };
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CommandSuite::CommandSuite(EventSystem & eventSystem)
		: EditorPlugin { eventSystem }
	{
		eventSystem.addListener(EnterEvent::ID, this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void CommandSuite::onEvent(const Event & value)
	{
		switch (*value)
		{
			// Enter Event
			/* * * * * * * * * * * * * * * * * * * * */
		case EnterEvent::ID: 
			if (auto ev = value.as<EnterEvent>()) 
			{ 
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				m_commands.push_back(new CommandImpl {
					"test",
					"description",
					"usage",
					new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
					{
						if (args)
						{
							Debug::log(args.front());
						}
						return true;
					})
				});

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
				
				for (auto *& cmd : m_commands) { cmd->install(ML_CommandRegistry); }

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			}
			break;

			// Start Event
			/* * * * * * * * * * * * * * * * * * * * */
		case StartEvent::ID:
			if (auto ev = value.as<StartEvent>())
			{
				Debug::log("Hello from \'{0}\'", (*this));
			}
			break;

			// Exit Event
			/* * * * * * * * * * * * * * * * * * * * */
		case ExitEvent::ID: 
			if (auto ev = value.as<ExitEvent>()) 
			{ 
				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

				for (auto *& cmd : m_commands)
				{
					if (cmd)
					{
						cmd->uninstall(ML_CommandRegistry);
						delete cmd;
					}
				}
				m_commands.clear();

				/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}