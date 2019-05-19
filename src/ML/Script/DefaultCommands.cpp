#include <ML/Script/DefaultCommands.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/OS.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	void DefaultCommands::install()
	{
		ML_Interpreter.install({ "cat",		DefaultCommands::cmd_cat	});
		ML_Interpreter.install({ "cd",		DefaultCommands::cmd_cd		});
		ML_Interpreter.install({ "cwd",		DefaultCommands::cmd_cwd	});
		ML_Interpreter.install({ "exec",	DefaultCommands::cmd_exec	});
		ML_Interpreter.install({ "exists",	DefaultCommands::cmd_exists	});
		ML_Interpreter.install({ "exit",	DefaultCommands::cmd_exit	});
		ML_Interpreter.install({ "get",		DefaultCommands::cmd_get	});
		ML_Interpreter.install({ "getcwd",	DefaultCommands::cmd_getcwd	});
		ML_Interpreter.install({ "help",	DefaultCommands::cmd_help	});
		ML_Interpreter.install({ "load",	DefaultCommands::cmd_load	});
		ML_Interpreter.install({ "log",		DefaultCommands::cmd_log	});
		ML_Interpreter.install({ "ls",		DefaultCommands::cmd_ls		});
		ML_Interpreter.install({ "pause",	DefaultCommands::cmd_pause	});
		ML_Interpreter.install({ "os",		DefaultCommands::cmd_os		});
		ML_Interpreter.install({ "read",	DefaultCommands::cmd_read	});
		ML_Interpreter.install({ "set",		DefaultCommands::cmd_set	});
		ML_Interpreter.install({ "system",	DefaultCommands::cmd_system	});
		ML_Interpreter.install({ "target",	DefaultCommands::cmd_target	});
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Var DefaultCommands::cmd_cat(Args & args)
	{
		String buf;
		if (ML_FS.getFileContents(args.pop(), buf))
		{
			cout << buf << endl;

			return Var().boolValue(true);
		}
		return Var().boolValue(false);
	}

	Var DefaultCommands::cmd_cd(Args & args)
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

	Var DefaultCommands::cmd_cwd(Args & args)
	{
		return ML_Interpreter.execCommand("getcwd").print();
	}

	Var DefaultCommands::cmd_exec(Args & args)
	{
		return ML_Interpreter.execFile(args.pop());
	}

	Var DefaultCommands::cmd_exists(Args & args)
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

	Var DefaultCommands::cmd_exit(Args & args)
	{
		return Var().errorValue("EXIT WIP");
		//if (Application * app = ML_Engine.app())
		//{
		//	app->close();
		//}
		//return Var().voidValue();
	}

	Var DefaultCommands::cmd_get(Args & args)
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

	Var DefaultCommands::cmd_getcwd(Args & args)
	{
		return Var().stringValue(ML_FS.getWorkingDir());
	}

	Var DefaultCommands::cmd_help(Args & args)
	{
		SStream ss;
		for (auto & pair : ML_Interpreter.commands())
		{
			cout << pair.first << endl;
		}
		return Var().boolValue(true);
	}

	Var DefaultCommands::cmd_load(Args & args)
	{
		return Var().errorValue("LOAD WIP");
		//if (const String type = args.pop())
		//{
		//	if (const String name = args.pop())
		//	{
		//		if (const String file = args.pop())
		//		{
		//			const ManifestItem item = 
		//			{
		//				{ "type", type },
		//				{ "name", name },
		//				{ "file", file },
		//			};
		//			if (ML_Resources.parseItem(item))
		//			{
		//				return Var().boolValue(Debug::log(
		//					"Success loading {0}: {1} \'{2}\'",
		//					type, name, file
		//				));
		//			}
		//			else
		//			{
		//				return Var().errorValue(
		//					"Failed loading {0}: {1} \'{2}\'",
		//					type, name, file
		//				);
		//			}
		//		}
		//		return Var().errorValue("Invalid File");
		//	}
		//	return Var().errorValue("Invalid Name");
		//}
		//return Var().errorValue("Invalid Type");
	}

	Var DefaultCommands::cmd_log(Args & args)
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

	Var DefaultCommands::cmd_ls(Args & args)
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

	Var DefaultCommands::cmd_pause(Args & args)
	{
		return Var().intValue(Debug::pause(EXIT_SUCCESS));
	}

	Var DefaultCommands::cmd_os(Args & args)
	{
		switch (args.pop_front().size())
		{
		case 0: return Var().errorValue("");
		case 1: return Var().boolValue(OS::execute(args[0]));
		case 2: return Var().boolValue(OS::execute(args[0], args[1]));
		case 3: return Var().boolValue(OS::execute(args[0], args[1], args[2]));
		default: return Var().errorValue("");
		}
	}

	Var DefaultCommands::cmd_read(Args & args)
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

	Var DefaultCommands::cmd_run(Args & args)
	{
		return Var().errorValue("RUN WIP");
		//bool flag_rebuild = args.find_and_erase("-r");
		//
		//const String name = args.pop();
		//if (Script * scr = ML_Resources.scripts.get(name))
		//{
		//	auto build_fun = ((flag_rebuild)
		//		? (&Script::rebuild)
		//		: (&Script::build)
		//	);
		//
		//	args.pop_front();
		//
		//	if ((scr->*build_fun)(args))
		//	{
		//		if (scr->run())
		//		{
		//			return scr->retv();
		//		}
		//	}
		//	return Var().stringValue(name);
		//}
		//return Var().errorValue("Script not found: {0}", name);
	}

	Var DefaultCommands::cmd_set(Args & args)
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

	Var DefaultCommands::cmd_system(Args & args)
	{
		return Var().intValue(Debug::system(args.pop_front().ToString().c_str()));
	}

	Var DefaultCommands::cmd_target(Args & args)
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