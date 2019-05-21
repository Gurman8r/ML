#include <ML/Script/Interpreter.hpp>	
#include <ML/Script/ScriptMacros.hpp>
#include <ML/Script/DefaultCommands.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Interpreter::Interpreter()
		: m_commands()
	{
		this->addCommand({ "cat",		DefaultCommands::cmd_cat	});
		this->addCommand({ "cd",		DefaultCommands::cmd_cd		});
		this->addCommand({ "cwd",		DefaultCommands::cmd_cwd	});
		this->addCommand({ "exec",		DefaultCommands::cmd_exec	});
		this->addCommand({ "exists",	DefaultCommands::cmd_exists	});
		this->addCommand({ "get",		DefaultCommands::cmd_get	});
		this->addCommand({ "getcwd",	DefaultCommands::cmd_getcwd	});
		this->addCommand({ "help",		DefaultCommands::cmd_help	});
		this->addCommand({ "log",		DefaultCommands::cmd_log	});
		this->addCommand({ "ls",		DefaultCommands::cmd_ls		});
		this->addCommand({ "pause",		DefaultCommands::cmd_pause	});
		this->addCommand({ "os",		DefaultCommands::cmd_os		});
		this->addCommand({ "read",		DefaultCommands::cmd_read	});
		this->addCommand({ "run",		DefaultCommands::cmd_run	});
		this->addCommand({ "set",		DefaultCommands::cmd_set	});
		this->addCommand({ "system",	DefaultCommands::cmd_system	});
	}

	Interpreter::~Interpreter()
	{
		m_commands.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Command * Interpreter::addCommand(const Command & value)
	{
		return ((value.name() && (m_commands.find(value.name()) == m_commands.end()))
			? (&m_commands.insert({ value.name(), value }).first->second)
			: (NULL)
		);
	}

	Command * Interpreter::getCommand(const String & value)
	{
		CommandMap::iterator it;
		return (((it = m_commands.find(value)) != m_commands.begin())
			? (&it->second)
			: (NULL)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Var	Interpreter::execCommand(const String & value)
	{
		if (!value.empty())
		{
			Args args(value, " ");

			CommandMap::iterator it;
			if ((it = m_commands.find(args.front())) != m_commands.end())
			{
				return it->second(args);
			}

			if (value.find(';') != String::npos)
			{
				return execString(value);
			}
		}
		return Var().errorValue("Interpreter : Unknown Command ", value);
	}

	Var	Interpreter::execFile(const String & value)
	{
		if (!value.empty())
		{
			File file;
			if (file.loadFromFile(value))
			{
				return execTokens(ML_Lexer.genTokenList(file.data()));
			}
			return Var().errorValue("Interpreter : File not found {0}", value);
		}
		return Var().errorValue("Interpreter : File cannot be empty", value);
	}

	Var	Interpreter::execString(const String & value)
	{
		return ((!value.empty())
			? (execTokens(ML_Lexer.genTokenList(value)))
			: (Var().errorValue("Interpreter : String cannot be empty"))
		);
	}

	Var	Interpreter::execTokens(const TokenList & value)
	{
		return ((!value.empty())
			? (execTree(ML_Lexer.genTokenTree(value)))
			: (Var().errorValue("Interpreter : Token List cannot be empty"))
		);
	}

	Var Interpreter::execTree(const TokenTree & value)
	{
		return ((!value.empty())
			? (execBlock(Parser::genFromTree(value)))
			: (Var().errorValue("Interpreter : Token Tree cannot be empty"))
		);
	}

	Var	Interpreter::execBlock(AST_Block * value)
	{
		if (value)
		{
			if (value->run())
			{
				const Var v = (*value->getRet());

				delete value;

				return ((ML_Runtime.setVar(0, ML_SCR_NAME_RETV, v))
					? (v)
					: (Var().errorValue("Interpreter : Failed setting return"))
				);
			}
			else
			{
				return Var().errorValue("Interpreter : Failed running Block");
			}
		}
		else
		{
			return Var().errorValue("Interpreter : Root cannot be null");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}