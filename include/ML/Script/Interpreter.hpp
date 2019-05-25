#ifndef _ML_INTERPRETER_HPP_
#define _ML_INTERPRETER_HPP_

#include <ML/Core/EventListener.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Script/Lexer.hpp>
#include <ML/Script/Parser.hpp>
#include <ML/Script/Runtime.hpp>
#include <ML/Script/Command.hpp>
#include <ML/Script/ScriptEvents.hpp>

#define ML_Interpreter ml::Interpreter::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Interpreter final
		: public IObject
		, public ISingleton<Interpreter>
	{	
		friend ISingleton<Interpreter>;

	public:
		using CommandMap  = typename HashMap<String, Command>;
		using CommandPair = typename Pair<String, Command>;

	private:
		Interpreter();
		~Interpreter();

	public:
		Command * addCommand(const Command & value);
		Command * getCommand(const String & value);

		template <
			class T,
			class ... Args
		> inline Var execCommand(const String & fmt, const T & arg0, Args && ... args)
		{
			return execCommand(fmt.format(arg0, (args)...));
		}

		Var	execCommand(const String & value);
		Var	execFile(const String & value);
		Var	execString(const String & value);
		Var	execTokens(const TokenList & value);
		Var execTree(const TokenTree & value);
		Var	execBlock(AST_Block * value);

		inline const CommandMap & commands() const { return m_commands; }

	private:
		CommandMap m_commands;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_INTERPRETER_HPP_