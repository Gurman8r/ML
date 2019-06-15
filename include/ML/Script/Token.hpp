#ifndef _ML_TOKEN_HPP_
#define _ML_TOKEN_HPP_

#include <ML/Script/Args.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Var;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Token final
		: public IObject
		, public IComparable<Token>
		, public IComparable<char>
		, public IComparable<String>
	{
	public:
		enum : char
		{
			Error	= '\0', // '\0' | Error
			Empty	= ' ',	// '  ' | Nothing / Space
			Endl	= '\n',	// '\n' | End of line
			Name	= 'n',	// 'n'	| Name
			Float	= 'f',	// 'f'	| Float / Decimal
			Int		= 'i',	// 'i'	| Integer
			Str		= 's',	// 's'	| String
			Pow		= '^',	// '^'	| Pow
			Mul		= '*',	// '*'	| Multiply
			Div		= '/',	// '/'	| Divide
			Plus	= '+',	// '+'	| Add
			Minus	= '-',	// '-'	| Subtract
			Equal	= '=',	// '='	| Equal
			Less	= '<',	// '<'	| Less Than
			Greater	= '>',	// '>'	| Greater Than
			Not		= '!',	// '!'	| Not
			Mod		= '%',	// '%'	| Mod
			Or		= '|',	// '|'	| Or
			And		= '&',	// '&'	| And
			Hash	= '#',	// '#'	| Hash
			Term	= '\\',	// '\\' | Terminate
			SColon	= ';',	// ';'	| Semicolon
			Comma	= ',',	// ','	| Comma
			Colon	= ':',	// ':'	| Colon
			Dot		= '.',	// '.'	| Dot
			QMark	= '?',	// '?'	| Question Mark
			Dollar	= '$',	// '$'	| Dollar
			At		= '@',	// '@'	| At
			LParen	= '(',	// '('	| Left Paren
			RParen	= ')',	// ')'	| Right Paren
			LBrace	= '{',	// '{'	| Left Brace
			RBrace	= '}',	// '}'	| Right Brace
			LBrack	= '[',	// '['	| Left Bracket
			RBrack	= ']',	// ']'	| Right Bracket
			DQuote	= '\"', // '\"' | Double Quote
			SQuote	= '\'', // '\'' | Single Quote
		};

	public:
		using SymbolMap = HashMap<String, char>;
		
		static const SymbolMap Symbols;

	public:
		Token();
		Token(char type);
		Token(char type, const String & data);
		Token(const Token & copy);
		Token(const Var & v);

	public:
		char	type;
		String	data;
		
	public:
		Token & operator=(const Token & value);
		Token & operator=(const char & value);

	public:
		bool isOperand() const;
		bool isOperator() const;

	public:
		bool equals(const Token & value) const override;
		bool equals(const char & value) const override;
		bool equals(const String & value) const override;

	public:
		bool lessThan(const Token & value) const override;
		bool lessThan(const char & value) const override;
		bool lessThan(const String & value) const override;

	public:
		void serialize(OStream & out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TOKEN_HPP_