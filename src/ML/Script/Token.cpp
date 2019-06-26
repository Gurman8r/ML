#include <ML/Script/Token.hpp>
#include <ML/Script/Operator.hpp>
#include <ML/Script/Var.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const Token::SymbolMap Token::Symbols({
		{ "\0"	,	Token::Error	},
		{ " "	,	Token::Empty	},
		{ "\n"	,	Token::Endl		},
		{ "^"	,	Token::Pow		},
		{ "*"	,	Token::Mul		},
		{ "/"	,	Token::Div		},
		{ "+"	,	Token::Plus		},
		{ "-"	,	Token::Minus	},
		{ "%"	,	Token::Mod		},
		{ "&"	,	Token::And		},
		{ "|"	,	Token::Or		},
		{ "="	,	Token::Equal	},
		{ "<"	,	Token::Less		},
		{ ">"	,	Token::Greater	},
		{ "!"	,	Token::Not		},
		{ ";"	,	Token::SColon	},
		{ ","	,	Token::Comma	},
		{ ":"	,	Token::Colon	},
		{ "#"	,	Token::Hash		},
		{ "\\"	,	Token::Term		},
		{ "."	,	Token::Dot		},
		{ "?"	,	Token::QMark	},
		{ "$"	,	Token::Dollar	},
		{ "@"	,	Token::At		},
		{ "("	,	Token::LParen	}, 
		{ ")"	,	Token::RParen	},
		{ "{"	,	Token::LBrace	}, 
		{ "}"	,	Token::RBrace	},
		{ "["	,	Token::LBrack	}, 
		{ "]"	,	Token::RBrack	},
		{ "\""	,	Token::DQuote	},
		{ "\'"	,	Token::SQuote	},

	});

	/* * * * * * * * * * * * * * * * * * * * */

	Token::Token()
		: type(Token::Error)
		, data(String(1, type))
	{
	}

	Token::Token(char type)
		: type(type)
		, data(String())
	{
	}

	Token::Token(char type, const String & data)
		: type(type)
		, data(data)
	{
	}

	Token::Token(const Token & copy)
		: type(copy.type)
		, data(copy.data)
	{
	}

	Token::Token(const Var & v)
		: type(Token::Error)
		, data(v.textValue())
	{
		switch (v.getTypeID())
		{
		case Var::Bool:
		case Var::Func:
		case Var::Pointer:	
			type = 'n'; 
			break;
		
		case Var::Float:
			type = 'f'; 
			break;
		
		case Var::Integer:	
			type = 'i'; 
			break;
		
		case Var::Str:
		default:
			type = 's'; 
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Token & Token::operator=(const Token & value)
	{
		type = value.type;
		data = value.data;
		return (*this);
	}

	Token & Token::operator=(const char & value)
	{
		type = value;
		data = String(1, type);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Token::isOperand() const
	{
		return
			type == 'n' ||
			type == 'f' ||
			type == 'i' ||
			type == 's';
	}

	bool Token::isOperator() const
	{
		return
			type == '^' ||
			type == '*' ||
			type == '/' ||
			type == '+' ||
			type == '-' ||
			type == '=' ||
			type == '<' ||
			type == '>' ||
			type == '!' ||
			type == '%' ||
			type == '|' ||
			type == '&';
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	bool Token::equals(const Token & value) const
	{
		return (type == value.type) && (data == value.data);
	}

	bool Token::equals(const char & value) const
	{
		return (char)type == value;
	}

	bool Token::equals(const String & value) const
	{
		return data == value;
	}
	
	
	bool Token::lessThan(const Token & value) const
	{
		if (Operator lhs = Operator(data))
		{
			if (Operator rhs = Operator(value.data))
			{
				return lhs < rhs;
			}
		}
		return false;
	}

	bool Token::lessThan(const char & value) const
	{
		return (char)type < value;
	}

	bool Token::lessThan(const String & value) const
	{
		return data < value;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	ML_SERIALIZE(Ostream & out, const Token & value)
	{
		return out << FMT()
			<< FG::White << "[ "
			<< FG::Green << value.type
			<< FG::White << " "
			<< FG::Yellow << value.data
			<< FG::White << " ]"
			<< FMT();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}