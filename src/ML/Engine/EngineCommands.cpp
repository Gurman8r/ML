#include <ML/Engine/EngineCommands.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Resources.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/OS.hpp>
#include <ML/Engine/Application.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	void EngineCommands::install()
	{
		ML_Interpreter.install({ "cat",		EngineCommands::cmd_cat		});
		ML_Interpreter.install({ "cd",		EngineCommands::cmd_cd		});
		ML_Interpreter.install({ "config",	EngineCommands::cmd_config	});
		ML_Interpreter.install({ "cwd",		EngineCommands::cmd_cwd		});
		ML_Interpreter.install({ "exec",	EngineCommands::cmd_exec	});
		ML_Interpreter.install({ "exists",	EngineCommands::cmd_exists	});
		ML_Interpreter.install({ "exit",	EngineCommands::cmd_exit	});
		ML_Interpreter.install({ "get",		EngineCommands::cmd_get		});
		ML_Interpreter.install({ "getcwd",	EngineCommands::cmd_getcwd	});
		ML_Interpreter.install({ "help",	EngineCommands::cmd_help	});
		ML_Interpreter.install({ "load",	EngineCommands::cmd_load	});
		ML_Interpreter.install({ "log",		EngineCommands::cmd_log		});
		ML_Interpreter.install({ "ls",		EngineCommands::cmd_ls		});
		ML_Interpreter.install({ "pause",	EngineCommands::cmd_pause	});
		ML_Interpreter.install({ "os",		EngineCommands::cmd_os		});
		ML_Interpreter.install({ "read",	EngineCommands::cmd_read	});
		ML_Interpreter.install({ "set",		EngineCommands::cmd_set		});
		ML_Interpreter.install({ "system",	EngineCommands::cmd_system	});
		ML_Interpreter.install({ "target",	EngineCommands::cmd_target	});
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Var EngineCommands::cmd_cat(Args & args)
	{
		String buf;
		if (ML_FS.getFileContents(args.pop(), buf))
		{
			cout << buf << endl;

			return Var().boolValue(true);
		}
		return Var().boolValue(false);
	}

	Var EngineCommands::cmd_cd(Args & args)
	{
		const String path = args.pop();
		if (path == "/")
		{
			return Var().boolValue(ML_FS.setWorkingDir(
				ML_FS.getPathTo("")));
		}
		else
		{
			return Var().boolValue(ML_FS.setWorkingDir(path));
		}
	}

	Var EngineCommands::cmd_config(Args & args)
	{
		if (!args.pop_front().empty())
		{
			if (args.front().size() == 1)
			{
				const char type = args.front().front();
				if (!args.pop_front().empty())
				{
					const String name = args.front();
					if (!args.pop_front().empty())
					{
						switch (type)
						{
						case 'b':
						{
							bool value;
							if (StringUtility::MakeBool(args.front(), value))
							{
								if (name == "flag_itop")
								{
									ML_Parser.showItoP(value);
									return Var().boolValue(ML_Parser.showItoP());
								}
								else if (name == "flag_toks")
								{
									ML_Parser.showToks(value);
									return Var().boolValue(ML_Parser.showToks());
								}
								else if (name == "flag_tree")
								{
									ML_Parser.showTree(value);
									return Var().boolValue(ML_Parser.showTree());
								}
								else
								{
									return Var().errorValue("Unknown Bool {0}", name);
								}
							}
						}
						break;
						case 'f':
						{
							float value;
							if (StringUtility::MakeFloat(args.front(), value))
							{
								if (true)
								{
								}
								else
								{
									return Var().errorValue("Unknown Float {0}", name);
								}
							}
						}
						break;
						case 's':
						{
							String value;
							if (!(value = args.ToString()).empty())
							{
								if (name == "title")
								{
									//return Var().stringValue(SETTINGS.title = value);
								}
								else
								{
									return Var().errorValue("Unknown String {0}", name);
								}
							}
						}
						break;
						}
					}
				}
			}

		}
		return Var().voidValue();
	}

	Var EngineCommands::cmd_cwd(Args & args)
	{
		return ML_Interpreter.execCommand("getcwd").print();
	}

	Var EngineCommands::cmd_exec(Args & args)
	{
		return ML_Interpreter.execFile(args.pop());
	}

	Var EngineCommands::cmd_exists(Args & args)
	{
		const String str = args.pop();

		if (StringUtility::IsInt(str) && !args.empty())
		{
			return Var().boolValue(
				ML_Runtime.getVar(
					StringUtility::ToInt(str),
					args.pop())
			);
		}
		else if (str == "." || str == "..")
		{
			return Var().boolValue(true);
		}
		else
		{
			return Var().boolValue(ML_FS.fileExists(str));
		}
	}

	Var EngineCommands::cmd_exit(Args & args)
	{
		if (Application * app = ML_Engine.app())
		{
			app->close();
		}
		return Var().voidValue();
	}

	Var EngineCommands::cmd_get(Args & args)
	{
		const String scope = args.pop();

		if (StringUtility::IsInt(scope) && !args.empty())
		{
			if (Var * v = ML_Runtime.getVar(
				StringUtility::ToInt(scope), args.pop()))
			{
				return (*v);
			}
		}
		return Var().voidValue();
	}

	Var EngineCommands::cmd_getcwd(Args & args)
	{
		return Var().stringValue(ML_FS.getWorkingDir());
	}

	Var EngineCommands::cmd_help(Args & args)
	{
		SStream ss;
		for (auto & pair : ML_Interpreter.commands())
		{
			cout << pair.first << endl;
		}
		return Var().boolValue(true);
	}

	Var EngineCommands::cmd_load(Args & args)
	{
		if (const String type = args.pop())
		{
			if (const String name = args.pop())
			{
				if (const String file = args.pop())
				{
					const ManifestItem item = 
					{
						{ "type", type },
						{ "name", name },
						{ "file", file },
					};
					if (ML_Resources.parseItem(item))
					{
						return Var().boolValue(Debug::log(
							"Success loading {0}: {1} \'{2}\'",
							type, name, file
						));
					}
					else
					{
						return Var().errorValue(
							"Failed loading {0}: {1} \'{2}\'",
							type, name, file
						);
					}
				}
				return Var().errorValue("Invalid File");
			}
			return Var().errorValue("Invalid Name");
		}
		return Var().errorValue("Invalid Type");
	}

	Var EngineCommands::cmd_log(Args & args)
	{
		const String name = args.pop();
		if (name.empty())
		{
			return Var().boolValue(false);
		}
		else if (name == "msg")
		{
			return Var().intValue(Debug::log(args.pop_front().ToString()));
		}
		else if (name == "wrn")
		{
			return Var().intValue(Debug::logWarning(args.pop_front().ToString()));
		}
		else if (name == "err")
		{
			return Var().intValue(Debug::logError(args.pop_front().ToString()));
		}
		else
		{
			return Var().boolValue(false);
		}
	}

	Var EngineCommands::cmd_ls(Args & args)
	{
		const String name = args.pop_front().empty() ? "." : args.ToString();
		SStream ss;
		if (ML_FS.getDirContents(name, ss))
		{
			String line;
			while (std::getline(ss, line))
			{
				cout << line << endl;
			}
			return Var().boolValue(true);
		}
		return Var().boolValue(false);
	}

	Var EngineCommands::cmd_pause(Args & args)
	{
		return Var().intValue(Debug::pause(EXIT_SUCCESS));
	}

	Var EngineCommands::cmd_os(Args & args)
	{
		switch (args.pop_front().size())
		{
		case 0: return Var().errorValue("");
		case 1: return Var().boolValue(ML_OS.execute(args[0]));
		case 2: return Var().boolValue(ML_OS.execute(args[0], args[1]));
		case 3: return Var().boolValue(ML_OS.execute(args[0], args[1], args[2]));
		default: return Var().errorValue("");
		}
	}

	Var EngineCommands::cmd_read(Args & args)
	{
		const String name = args.pop();
		if (ML_FS.fileExists(name))
		{
			String buf;
			if (ML_FS.getFileContents(name, buf))
			{
				return Var().stringValue(buf);
			}
		}
		else
		{
			SStream ss;
			if (ML_FS.getDirContents(name, ss))
			{
				return Var().stringValue(ss.str());
			}
		}
		return Var().boolValue(false);
	}

	Var EngineCommands::cmd_set(Args & args)
	{
		int32_t index;
		if (StringUtility::MakeInt(args.pop(), index) && !args.empty())
		{
			const String name = args.pop();

			if (StringUtility::IsName(name) && !args.empty())
			{
				const Token value = ML_Lexer.genToken(args.pop());

				if (ML_Runtime.setVar(
					index,
					name,
					Var::makeSingle(value)
				))
				{
					return Var().boolValue(true);
				}
			}
		}
		return Var().boolValue(false);
	}

	Var EngineCommands::cmd_system(Args & args)
	{
		return Var().intValue(Debug::system(args.pop_front().ToString().c_str()));
	}

	Var EngineCommands::cmd_target(Args & args)
	{
		if (!args.pop_front().empty())
		{
			const String & opt = args.front();
			if (opt == "name")
			{
				//return Var().stringValue(SETTINGS.title);
			}
			else if (opt == "config")
			{
				return Var().stringValue(ML_CONFIGURATION);
			}
			else if (opt == "platform")
			{
				return Var().stringValue(ML_PLATFORM_TARGET);
			}
		}
		return Var().boolValue(true);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}