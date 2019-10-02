#include "CommandSuite.hpp"
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Engine/CommandRegistry.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Lua.hpp>
#include <ML/Engine/Python.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Window/Window.hpp>
#include <ML/Window/WindowEvents.hpp>

ML_PLUGIN_API ml::Plugin * ML_Plugin_Main()
{
	return new ml::CommandSuite {};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CommandSuite::CommandSuite()
		: Plugin {}
	{
		ML_EventSystem.addListener(EnterEvent::ID, this);
		ML_EventSystem.addListener(ExitEvent::ID,	this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void CommandSuite::onEvent(const Event & value)
	{
		switch (*value)
		{
		case EnterEvent::ID: 
			if (auto ev = value.as<EnterEvent>()) 
			{ 
				initialize_commands();

				for (auto *& cmd : m_commands) 
				{ 
					cmd->install(ML_CommandRegistry);
				}
			}
			break;

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void CommandSuite::initialize_commands()
	{
		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl {
			"cat",
			"Display the contents of a file",
			"cat [FILE]",
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

		m_commands.push_back(new CommandImpl { 
			"cd",
			"Set the current working directory",
			"cd [DIR]...",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				const String path = ([&]()
				{
					if (args.size() == 1) return String { "/" };
					SStream ss;
					for (size_t i = 1; i < args.size(); i++)
						ss << args[i];
					return (String)ss.str();
				})();
				return path && ML_FS.setPath(path);
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl {
			"clear",
			"Clear the terminal screen",
			"clear",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				return true; // See EditorTerminal.cpp
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { 
			"cwd",
			"Display the current working directory",
			"cwd",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				cout << ML_FS.getPath() << endl;
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { 
			"exit",
			"Close the application",
			"exit",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				ML_EventSystem.fireEvent(WindowKillEvent());
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { 
			"help",
			"Display information about commands.",
			"help [CMD]",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				switch (args.size())
				{
				case 1:	for (const auto & cmd : ML_CommandRegistry)
					cout << cmd->getName() << endl;
					return true;

				case 2: if (auto cmd = ML_CommandRegistry.find_by_name(args[1]))
					cout << (*cmd) << endl;
					return true;
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { 
			"history",
			"Display or manipulate history list",
			"history",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				return true; // See EditorTerminal.cpp
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl {
			"lua",
			"Execute lua string",
			"lua [STR]...",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				if (const String code = ([&]()
				{
					if (args.size() == 1) return String();
					SStream ss;
					for (size_t i = 1; i < args.size(); i++)
						ss << args[i] << " ";
					return (String)ss.str();
				})())
				{
					return (bool)ML_Lua.DoString(code);
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { 
			"ls",
			"List directory contents",
			"ls [DIR]...",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				if (const String path = ([&]()
				{
					if (args.size() == 1) return String { "." };
					SStream ss;
					for (size_t i = 1; i < args.size(); i++)
						ss << args[i];
					return (String)ss.str();
				})())
				{
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
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { 
			"os",
			"Execute operating system commands",
			"os [CMD] [FILE] [ARGS] [PATH] [FLAGS]",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				switch (args.size())
				{
				case 2: return (bool)OS::execute(args[1]);
				case 3: return (bool)OS::execute(args[1], args[2]);
				case 4: return (bool)OS::execute(args[1], args[2], args[3]);
				case 5: return (bool)OS::execute(args[1], args[2], args[3], args[4]);
				case 6: return (bool)OS::execute(args[1], args[2], args[3], args[4], alg::to_i32(args[5]));
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { 
			"pause",
			"Pause the console subsystem",
			"pause",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				Debug::pause(0);
				return true;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl {
			"py",
			"Execute python code.",
			"py [CODE]...",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				if (args.size() == 2)
				{
					if (args[1] == "--restart")
					{
						ML_Py.restart();
					}
				}
				else if (const String code = ([&]()
				{
					if (args.size() == 1) return String();
					SStream ss;
					for (size_t i = 1; i < args.size(); i++)
						ss << args[i] << " ";
					return (String)ss.str();
				})())
				{
					return (bool)ML_Py.DoString(code);
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl {
			"sys",
			"Run a system command",
			"sys [CMD]...",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				if (const String code = ([&]()
				{
					if (args.size() == 1) return String();
					SStream ss;
					for (size_t i = 1; i < args.size(); i++)
						ss << args[i] << " ";
					return (String)ss.str();
				})())
				{
					Debug::system(code.c_str());
					return true;
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl { 
			"pause",
			"Pause the console subsystem",
			"pause",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				Debug::pause(0);
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */

		m_commands.push_back(new CommandImpl {
			"execute",
			"Execute a script",
			"execute [SCR]...",
			new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
			{
				if (args.size() == 2)
				{
					if (Script * scr { ML_Content.get<Script>(args[1]) })
					{
						scr->execute();
						return true;
					}
				}
				return false;
			})
		});

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}