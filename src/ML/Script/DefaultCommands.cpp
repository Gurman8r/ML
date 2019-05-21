#include <ML/Script/DefaultCommands.hpp>
#include <ML/Script/Interpreter.hpp>
#include <ML/Script/Script.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/OS.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	void DefaultCommands::install(Interpreter & interpreter)
	{
		interpreter.install({ "cat",	DefaultCommands::cmd_cat	});
		interpreter.install({ "cd",		DefaultCommands::cmd_cd		});
		interpreter.install({ "cwd",	DefaultCommands::cmd_cwd	});
		interpreter.install({ "exec",	DefaultCommands::cmd_exec	});
		interpreter.install({ "exists",	DefaultCommands::cmd_exists	});
		interpreter.install({ "get",	DefaultCommands::cmd_get	});
		interpreter.install({ "getcwd",	DefaultCommands::cmd_getcwd	});
		interpreter.install({ "help",	DefaultCommands::cmd_help	});
		interpreter.install({ "log",	DefaultCommands::cmd_log	});
		interpreter.install({ "ls",		DefaultCommands::cmd_ls		});
		interpreter.install({ "pause",	DefaultCommands::cmd_pause	});
		interpreter.install({ "os",		DefaultCommands::cmd_os		});
		interpreter.install({ "read",	DefaultCommands::cmd_read	});
		interpreter.install({ "run",	DefaultCommands::cmd_run	});
		interpreter.install({ "set",	DefaultCommands::cmd_set	});
		interpreter.install({ "system",	DefaultCommands::cmd_system	});
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
		Args temp { "getcwd" };
		return cmd_getcwd(temp).print();
	}

	Var DefaultCommands::cmd_exec(Args & args)
	{
		if (const String name = args.pop())
		{
			return ML_Interpreter.execFile(name);
		}
		return Var().errorValue("");
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
		bool flag_rebuild = args.find_and_erase("-r");
		
		if (const String name = args.pop())
		{
			if (Script * scr = 0)//ML_Resources.scripts.get(name))
			{
				auto build_fun = ((flag_rebuild)
					? (&Script::rebuild)
					: (&Script::build)
				);

				args.pop_front();

				if ((scr->*build_fun)(args))
				{
					if (scr->run())
					{
						return scr->retv();
					}
				}

				return Var().stringValue(name);
			}
			return Var().errorValue("Script not found: {0}", name);
		}
		return Var().errorValue("No script specified");
	}

	Var DefaultCommands::cmd_set(Args & args)
	{
		int32_t index;
		if (StringUtility::MakeInt(args.pop(), index) && !args.empty())
		{
			if (const String name = args.pop())
			{
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
					return Var().errorValue("");
				}
				return Var().errorValue("");
			}
			return Var().errorValue("");
		}
		return Var().errorValue("");
	}

	Var DefaultCommands::cmd_system(Args & args)
	{
		return Var().intValue(Debug::system(args.pop_front().ToString().c_str()));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}