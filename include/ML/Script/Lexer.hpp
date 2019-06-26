#ifndef _ML_LEXER_HPP_
#define _ML_LEXER_HPP_

#include <ML/Script/TokenList.hpp>
#include <ML/Script/Args.hpp>

#define ML_Lexer ::ml::Lexer::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Lexer final
		: public I_Singleton<Lexer>
	{
		friend struct I_Singleton<Lexer>;

	public:
		using const_iterator = typename List<char>::const_iterator;

	private:
		Lexer() {}
		~Lexer() {}

	public:
		static Token		genToken(const String & value);
		static TokenList	genArgsArray(const Arguments & value);
		static TokenList	genTokenList(const String & value);
		static TokenList	genTokenList(const List<char> & value);
		static TokenTree	genTokenTree(const TokenList & value);

	public:
		static bool scanName(const List<char> & value, const_iterator & it, String & text);
		static bool scanNumber(const List<char> & value, const_iterator & it, String & text);
		static bool scanString(const List<char> & value, const_iterator & it, String & text);
		static bool scanSymbol(const List<char> & value, const_iterator & it, String & text);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LEXER_HPP_