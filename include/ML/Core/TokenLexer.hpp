#ifndef _ML_TOKEN_LEXER_HPP_
#define _ML_TOKEN_LEXER_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Token.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API TokenLexer final
		: public I_Newable
	{
	public:
		TokenLexer();
		~TokenLexer();

	private:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TOKEN_LEXER_HPP_