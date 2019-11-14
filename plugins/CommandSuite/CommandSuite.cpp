#include "CommandSuite.hpp"
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Window/Window.hpp>
#include <ML/Window/WindowEvents.hpp>

ML_PLUGIN_API ml::ptr_t<ml::Plugin> ML_Plugin_Main() { return new ml::CommandSuite {}; }

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CommandSuite::CommandSuite() : Plugin {}
	{
		ML_EventSystem.addListener<LoadEvent>(this);
		ML_EventSystem.addListener<MainMenuBarEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void CommandSuite::onEvent(const Event & value)
	{
		switch (*value)
		{
		case LoadEvent::ID: 
			if (auto ev = value.as<LoadEvent>()) 
			{ 
				/* * * * * * * * * * * * * * * * * * * * */

				List<CommandImpl *> cmd;

				/* * * * * * * * * * * * * * * * * * * * */

				cmd.push_back(new CommandImpl {
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

				cmd.push_back(new CommandImpl { 
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

				cmd.push_back(new CommandImpl {
					"clear",
					"Clear the terminal screen",
					"clear",
					new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
					{
						return true; // See EditorTerminal.cpp
					})
				});

				/* * * * * * * * * * * * * * * * * * * * */

				cmd.push_back(new CommandImpl { 
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

				cmd.push_back(new CommandImpl { 
					"exit",
					"Close the application",
					"exit",
					new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
					{
						ML_EventSystem.fireEvent<WindowKillEvent>();
						return true;
					})
				});

				/* * * * * * * * * * * * * * * * * * * * */

				cmd.push_back(new CommandImpl { 
					"help",
					"Display information about commands.",
					"help [CMD]",
					new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
					{
						switch (args.size())
						{
						case 1:	for (const auto & cmd : ML_Engine.commands())
							cout << cmd->getName() << endl;
							return true;

						case 2: if (auto cmd = ML_Engine.commands().find_by_name(args[1]))
							cout << (*cmd) << endl;
							return true;
						}
						return false;
					})
				});

				/* * * * * * * * * * * * * * * * * * * * */

				cmd.push_back(new CommandImpl { 
					"history",
					"Display or manipulate history list",
					"history",
					new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
					{
						return true; // See EditorTerminal.cpp
					})
				});

				/* * * * * * * * * * * * * * * * * * * * */

				cmd.push_back(new CommandImpl {
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
							return (bool)Script { Script::Language::Lua, code }.execute();
						}
						return false;
					})
				});

				/* * * * * * * * * * * * * * * * * * * * */

				cmd.push_back(new CommandImpl { 
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

				cmd.push_back(new CommandImpl { 
					"os",
					"Execute operating system commands",
					"os [CMD] [FILE] [ARGS] [PATH] [FLAGS]",
					new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
					{
						switch (args.size())
						{
						case 2: return (bool)Debug::execute(args[1]);
						case 3: return (bool)Debug::execute(args[1], args[2]);
						case 4: return (bool)Debug::execute(args[1], args[2], args[3]);
						case 5: return (bool)Debug::execute(args[1], args[2], args[3], args[4]);
						case 6: return (bool)Debug::execute(args[1], args[2], args[3], args[4], util::to_i32(args[5]));
						}
						return false;
					})
				});

				/* * * * * * * * * * * * * * * * * * * * */

				cmd.push_back(new CommandImpl { 
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

				cmd.push_back(new CommandImpl {
					"py",
					"Execute python code.",
					"py [CODE]...",
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
							return (bool)Script{ Script::Language::Python, code }.execute();
						}
						return false;
					})
				});

				/* * * * * * * * * * * * * * * * * * * * */

				cmd.push_back(new CommandImpl { 
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

				cmd.push_back(new CommandImpl {
					"execute",
					"Execute a script",
					"execute [SCR]...",
					new FunctionExecutor([](const CommandDescriptor & cmd, const List<String> & args)
					{
						if (args.size() == 2)
						{
							if (ptr_t<Script> scr { ML_Engine.content().get<Script>(args[1]) })
							{
								scr->execute();
								return true;
							}
						}
						return false;
					})
				});

				/* * * * * * * * * * * * * * * * * * * * */

				// Install All
				for (auto *& elem : cmd) 
				{ 
					if (elem && !elem->install(ML_Engine.commands()))
					{
						delete elem;
					}
				}

				/* * * * * * * * * * * * * * * * * * * * */
			}
			break;

		case MainMenuBarEvent::ID:
			if (auto ev = value.as<MainMenuBarEvent>())
			{
				switch (ev->submenu)
				{
				case MainMenuBarEvent::Plugins:
					ImGui::PushID(ML_ADDRESSOF(this));
					if (ImGui::BeginMenu(nameof<>::filter_namespace(get_type_info().name()).c_str()))
					{
						for (auto & cmd : ML_Engine.commands())
						{
							ImGui::MenuItem(cmd->getName().c_str(), "");
							ImGuiExt::Tooltip(util::to_string(*cmd));
						}
						ImGui::EndMenu();
					}
					ImGui::PopID();
					break;
				}
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}