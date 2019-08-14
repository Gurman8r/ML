
#include "CommandSuite.hpp"
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/CommandRegistry.hpp>
#include <ML/Window/Window.hpp>
#include <ML/Window/WindowEvents.hpp>

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
		eventSystem.addListener(ExitEvent::ID,	this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void CommandSuite::onEvent(const Event & value)
	{
		switch (*value)
		{
			// Enter
			/* * * * * * * * * * * * * * * * * * * * */
		case EnterEvent::ID: 
			if (auto ev = value.as<EnterEvent>()) 
			{ 
				setup_commands();

				for (auto *& cmd : m_commands) 
				{ 
					cmd->install(ML_CommandRegistry);
				}
			}
			break;

			// Exit
			/* * * * * * * * * * * * * * * * * * * * */
		case ExitEvent::ID: 
			if (auto ev = value.as<ExitEvent>()) 
			{ 
				for (auto *& cmd : m_commands)
				{
					cmd->uninstall(ML_CommandRegistry);

					delete cmd;
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

		static EventSystem * evSys { nullptr };
		if (!evSys) evSys = &eventSystem();

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "cat",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				if (args.size() == 2)
				{
					String temp;
					if (ML_FS.getFileContents(args[1], temp))
					{
						cout << temp << endl;
						return true;
					}
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "cd",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				SStream ss;
				for (size_t i = 1; i < args.size(); i++)
					ss << args[i];
				return !ss.str().empty() && ML_FS.setPath(ss.str());
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "clear",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				// See EditorTerminal.cpp
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "cwd",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				cout << ML_FS.getPath() << endl;
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "exit",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				evSys->fireEvent(WindowKillEvent());
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "help",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				for (const auto & cmd : ML_CommandRegistry)
				{
					cout << cmd->getName() << endl;
				}
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "history",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				// See EditorTerminal.cpp
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "ls",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				const String path = ([&]()
				{
					if (args.size() == 1) return String { "." };
					SStream ss;
					for (size_t i = 1; i < args.size(); i++)
						ss << args[i] << "";
					return (String)ss.str();
				})();
				
				SStream dir;
				if (ML_FS.getDirContents(path, dir))
				{
					String line;
					while (std::getline(dir, line))
					{
						cout << line << endl;
					}
					return true;
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "open",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				SStream ss;
				for (size_t i = 1; i < args.size(); i++)
					ss << args[i] << (i < args.size() - 1 ? " " : "");
				return (bool)OS::execute("open", ss.str());
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "os",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				switch (args.size())
				{
				case 2: return (bool)OS::execute(args[1]);
				case 3: return (bool)OS::execute(args[1], args[2]);
				case 4: return (bool)OS::execute(args[1], args[2], args[3]);
				case 5: return (bool)OS::execute(args[1], args[2], args[3], args[4]);
				case 6: return (bool)OS::execute(args[1], args[2], args[3], args[4], alg::to_int(args[5]));
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "pause",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				Debug::pause(0);
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { "ping",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				Debug::log("pong!");
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}