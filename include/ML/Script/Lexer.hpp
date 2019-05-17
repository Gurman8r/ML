#ifndef _ML_LEXER_HPP_
#define _ML_LEXER_HPP_

#include <ML/Script/TokenList.hpp>
#include <ML/Script/Args.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Lexer ml::Lexer::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Lexer final
		: public ITrackable
		, public ISingleton<Lexer>
	{
		friend class ISingleton<Lexer>;

	public:
		using const_iterator = typename List<char>::const_iterator;

	private:
		Lexer();
		~Lexer();

	public:
		Token		genToken(const String & value) const;
		TokenList	genArgsArray(const Args & value) const;
		TokenList	genTokenList(const String & value) const;
		TokenList	genTokenList(const List<char> & value) const;
		TokenTree	genTokenTree(const TokenList & value) const;

	public:
		bool scanName(const List<char> & value, const_iterator & it, String & text) const;
		bool scanNumber(const List<char> & value, const_iterator & it, String & text) const;
		bool scanString(const List<char> & value, const_iterator & it, String & text) const;
		bool scanSymbol(const List<char> & value, const_iterator & it, String & text) const;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LEXER_HPP_