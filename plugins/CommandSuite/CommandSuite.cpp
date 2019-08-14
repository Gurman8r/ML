#include "CommandSuite.hpp"
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp>
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
		: Plugin { eventSystem }
	{
		eventSystem.addListener(EnterEvent::ID, this);
		eventSystem.addListener(StartEvent::ID, this);
		eventSystem.addListener(ExitEvent::ID,	this);
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
				setup_commands();

				for (auto *& cmd : m_commands) 
				{ 
					if (cmd)
					{
						cmd->install(ML_CommandRegistry);
					}
				}
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
				for (auto *& cmd : m_commands)
				{
					if (cmd)
					{
						cmd->uninstall(ML_CommandRegistry);

						delete cmd;
					}
				}
				m_commands.clear();
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void CommandSuite::setup_commands()
	{
		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "ping",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				Debug::log("pong!");
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "concat",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				if (args.size() != 2) return false;
				String temp;
				if (ML_FS.getFileContents(args[1], temp))
				{
					cout << temp << endl;
					return true;
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}