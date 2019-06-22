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
		: public I_Singleton<Interpreter>
	{	
		friend I_Singleton<Interpreter>;

	public:
		using CommandMap  = typename HashMap<String, Command>;
		using CommandPair = typename Pair<String, Command>;

	private:
		Interpreter();
		~Interpreter();

	public:
		static Command * addCommand(const Command & value);
		static Command * getCommand(const String & value);

		template <
			class T,
			class ... Args
		> inline static Var execCommand(const String & fmt, const T & arg0, Args && ... args)
		{
			return execCommand(fmt.format(arg0, (args)...));
		}

		static Var	execCommand(const String & value);
		static Var	execFile(const String & value);
		static Var	execString(const String & value);
		static Var	execTokens(const TokenList & value);
		static Var	execTree(const TokenTree & value);
		static Var	execBlock(AST_Block * value);

		inline static const CommandMap & commands() { return m_commands; }

	private:
		static CommandMap m_commands;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_INTERPRETER_HPP_