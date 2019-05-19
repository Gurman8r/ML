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
	{
		DefaultCommands::install(*this);

		//ML_EventSystem.addListener(ScriptEvent::EV_Command, this);
	}

	Interpreter::~Interpreter()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Interpreter::onEvent(const IEvent * value)
	{
		switch (*value)
		{
		case ScriptEvent::EV_Command:
			if (auto ev = value->as<CommandEvent>())
			{
				if (execCommand(ev->cmd).isErrorType())
				{
					Debug::logError(ev->cmd);
				}
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Command * Interpreter::install(const Command & value)
	{
		return ((value.name() && (m_cmd.find(value.name()) == m_cmd.end()))
			? (&m_cmd.insert({ value.name(), value }).first->second)
			: (NULL)
		);
	}

	Command * Interpreter::getCommand(const String & value)
	{
		CommandMap::iterator it;
		return (((it = m_cmd.find(value)) != m_cmd.begin())
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
			if ((it = m_cmd.find(args.front())) != m_cmd.end())
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
			? (execBlock(ML_Parser.genFromTree(value)))
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