#include <ML/Script/Lexer.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Token Lexer::genToken(const String & value)
	{
		if (value.empty())
		{
			return { Token::Empty, value };
		}
		else if (StringUtility::IsInt(value))
		{
			return { Token::Int, value };
		}
		else if (StringUtility::IsDecimal(value))
		{
			return { Token::Float, value };
		}
		else
		{
			return { Token::Str, value };
		}
	}

	TokenList Lexer::genArgsArray(const Args & value)
	{
		TokenList out;
		out.push_back('[');
		if (!value.empty())
		{
			TokenList temp = genTokenList(value.str());

			for (size_t i = 0, imax = temp.size(); i < imax; i++)
			{
				if (temp[i].type == Token::Name)
				{
					temp[i] = { Token::Str, temp[i].data };
				}

				out.push_back(temp[i]);

				if (i < imax - 1)
				{
					out.push_back(',');
				}
			}
		}
		out.push_back(']');
		return out;
	}


	TokenList Lexer::genTokenList(const String & value)
	{
		return genTokenList(List<char>(value.begin(), value.end()));
	}

	TokenList Lexer::genTokenList(const List<char> & value)
	{
		TokenList out;

		List<char>::const_iterator it;
		for (it = value.begin(); it != value.end(); it++)
		{
			String text;

			// End of line
			if ((*it) == '\n')
			{
				out.push_back({ '\n' });
			}
			// String
			else if (scanString(value, it, text))
			{
				out.push_back({ Token::Str, text });
			}
			// Name
			else if (scanName(value, it, text))
			{
				out.push_back({ Token::Name, text });
			}
			// Number
			else if (scanNumber(value, it, text))
			{
				// Integer
				if (StringUtility::IsInt(text))
				{
					out.push_back({ Token::Int, text });
				}
				// Float
				else if (StringUtility::IsDecimal(text))
				{
					out.push_back({ Token::Float, text });
				}
				// Error
				else
				{
					out.push_back({ Token::Error, text });
				}
			}
			// Symbols
			else if (scanSymbol(value, it, text))
			{
				Token::SymbolMap::const_iterator it;
				if ((it = Token::Symbols.find(text)) != Token::Symbols.end())
				{
					out.push_back({ it->second, text });
				}
				else
				{
					out.push_back({ Token::Error, text });
				}
			}
		}

		return out;
	}

	TokenTree Lexer::genTokenTree(const TokenList & value)
	{
		TokenTree tree	= { TokenList() };
		
		for (TokenList::const_iterator it = value.begin(); it != value.end(); it++)
		{
			switch (it->type)
			{
			case '\n': // New Line
				continue;

			case '#': // Comment
				while ((it != value.end()) && ((*it) != '\n')) 
					it++;
				break;

			case '{': // Begin Block
				tree.back().push_back(*it);

			case ';': // End Statement
				if (!tree.back().empty())
					tree.push_back(TokenList());
				break;

			default: // Other
				tree.back().push_back(*it);
				break;
			}
		}

		return tree;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Lexer::scanName(const List<char> & value, const_iterator & it, String & text)
	{
		if (std::isalpha(*it) || (*it) == '_')
		{
			String out;
			while (it != value.end())
			{
				if (std::isalnum(*it) || (*it) == '_')
				{
					out += (*it);
					it++;
				}
				else
				{
					break;
				}
			}
			it--;
			text = out;
			return true;
		}
		text = String();
		return false;
	}

	bool Lexer::scanNumber(const List<char> & value, const_iterator & it, String & text)
	{
		if (std::isdigit(*it))
		{
			String out;
			while (it != value.end())
			{
				if (std::isdigit(*it) || (*it) == '.')
				{
					out += (*it);
					it++;
				}
				else
				{
					break;
				}
			}
			it--;
			text = out;
			return true;
		}
		text = String();
		return false;
	}

	bool Lexer::scanString(const List<char> & value, const_iterator & it, String & text)
	{
		if ((*it) == '\"')
		{
			String out;
			while (++it != value.end())
			{
				if ((*it) == '\"')
				{
					break;
				}
				else if (std::isprint(*it))
				{
					out += (*it);
				}
			}
			text = out;
			return true;
		}
		text = String();
		return false;
	}

	bool Lexer::scanSymbol(const List<char> & value, const_iterator & it, String & text)
	{
		auto issymbol = [](const char c)
		{
			return
				(c >= '!'  && c <= '/') ||
				(c >= ':'  && c <= '@') ||
				(c >= '['  && c <= '`') ||
				(c >= '{'  && c <= '~');
		};

		if (issymbol(*it))
		{
			text = String(1, (*it));
			return true;
		}
		text = String();
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}