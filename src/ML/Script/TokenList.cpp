#include <ML/Script/TokenList.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	TokenList::TokenList()
		: m_values()
	{
	}
	
	TokenList::TokenList(const Token & value)
		: m_values({ value })
	{
	}
	
	TokenList::TokenList(const List<Token> & values)
		: m_values(values)
	{
	}
	
	TokenList::TokenList(const TokenList & copy)
		: m_values(copy.m_values)
	{
	}
	
	TokenList::~TokenList()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Token	TokenList::at(size_t index) const
	{
		return (*this)[index];
	}

	Token	TokenList::back() const
	{
		if (!empty())
		{
			return m_values.back();
		}
		return Token();
	}

	Token	TokenList::front() const
	{
		if (!empty())
		{
			return m_values.front();
		}
		return Token();
	}
	
	String	TokenList::str() const
	{
		String out;
		for (const_iterator it = begin(); it != end(); it++)
		{
			out += it->data + (it != end() - 1 ? " " : "");
		}
		return out;
	}

	Token TokenList::pop()
	{
		return pop_front().front();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	bool	TokenList::empty() const
	{
		return m_values.empty();
	}
	
	size_t	TokenList::size() const
	{
		return m_values.size();
	}

	bool	TokenList::size(const size_t value) const
	{
		return size() == value;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool	TokenList::back(const Token & value) const
	{
		return back() == value;
	}
	
	bool	TokenList::back(const String & value) const
	{
		return back() == value;
	}
	
	bool	TokenList::back(char value) const
	{
		return back() == value;
	}


	bool	TokenList::front(const Token & value) const
	{
		return front() == value;
	}
	
	bool	TokenList::front(const String & value) const
	{
		return front() == value;
	}
	
	bool	TokenList::front(char value) const
	{
		return front() == value;
	}

	
	bool	TokenList::inRange(size_t index) const
	{
		return (index < size());
	}
	
	bool	TokenList::inRange(const_iterator it) const
	{
		return it < end();
	}
	
	bool	TokenList::isWrap(const Token & value) const
	{
		return isWrap(value, value);
	}
	
	bool	TokenList::isWrap(const Token & lhs, const Token & rhs) const
	{
		return size() >= 2 && front().type == lhs.type && back().type == rhs.type;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool	TokenList::match_type(size_t index, char type) const
	{
		if (inRange(index))
		{
			switch (type)
			{
			case 'A':	return	true;					// Any
			case 'E':	return  at(index).isOperand();	// Expression
			case 'O':	return  at(index).isOperator();	// Operator
			case 'S':	return !at(index).isOperand();	// Symbol
			default	:	return  at(index).type == type;	// Match Type
			}
		}
		return false;
	}
	
	bool	TokenList::match_type(const const_iterator & it, char c) const
	{
		return match_type((it - begin()), c);
	}
	

	bool	TokenList::match_type_str(const String & str) const
	{
		return match_type_str(begin(), str);
	}

	bool	TokenList::match_type_str(size_t index, const String & str) const
	{
		return match_type_str(begin() + index, str);
	}
	
	bool	TokenList::match_type_str(const const_iterator & it, const String & str) const
	{
		return match_type_list(it, List<char>(str.begin(), str.end()));
	}


	bool	TokenList::match_type_list(size_t index, const List<char> & pattern) const
	{
		if (!empty() && !pattern.empty())
		{
			if (index < size() && index < pattern.size())
			{
				if (pattern.size() <= size())
				{
					for (size_t i = index; i < pattern.size(); i++)
					{
						if (!match_type(i, pattern.at(i)))
						{
							return false;
						}
					}
					return true;
				}
			}
		}
		return false;
	}
	
	bool	TokenList::match_type_list(const const_iterator & it, const List<char> & pattern) const
	{
		return match_type_list((it - begin()), pattern);
	}
	

	bool	TokenList::match_data(const List<CString> & data) const
	{
		return match_data(begin(), data);
	}

	bool	TokenList::match_data(size_t index, const List<CString> & data) const
	{
		if (!empty() && !data.empty())
		{
			if (index < size() && index < data.size())
			{
				if (data.size() <= size())
				{
					for (size_t i = index; i < data.size(); i++)
					{
						if (at(i).data != data.at(i))
						{
							return false;
						}
					}
					return true;
				}
			}
		}
		return false;
	}
	
	bool	TokenList::match_data(const const_iterator & it, const List<CString> & data) const
	{
		return match_data((it - begin()), data);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	TokenList	TokenList::after(size_t index) const
	{
		if (inRange(index))
		{
			TokenList out;

			const_iterator it;
			for (it = begin() + index; it != end(); it++)
			{
				out.push_back(*it);
			}

			return out;
		}
		return TokenList();
	}
	
	TokenList	TokenList::between(const Token & lhs, const Token & rhs) const
	{
		const_iterator from = find_first(lhs);
		if (from != end())
		{
			const_iterator to = find_last(rhs);
			if (to != end())
			{
				TokenList out;

				for (const_iterator it = from; it != to; it++)
				{
					out.push_back(*it);
				}

				return out;
			}
		}

		return m_values;
	}
	
	TokenList	TokenList::unwrapped() const
	{
		return TokenList(*this).unwrap();
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	TokenList &	TokenList::assign(const List<Token> & value)
	{
		m_values = value;
		return (*this);
	}
	
	TokenList &	TokenList::clear()
	{
		m_values.clear();
		return (*this);
	}
	
	TokenList &	TokenList::erase(size_t index, size_t count)
	{
		return erase(begin() + index, count);
	}
	
	TokenList &	TokenList::erase(const_iterator it, size_t count)
	{
		return erase(it, it + count);
	}
	
	TokenList &	TokenList::erase(const_iterator first, const_iterator last)
	{
		if (!empty() && first >= begin() && last != end())
		{
			m_values.erase(first, last);
		}
		return (*this);
	}
	
	TokenList &	TokenList::insert(size_t index, char value)
	{
		return insert(index, Token(value));
	}
	
	TokenList &	TokenList::insert(size_t index, const Token& value)
	{
		return insert(begin() + index, value);
	}
	
	TokenList &	TokenList::insert(iterator it, const Token & value)
	{
		m_values.insert(it, value);
		return (*this);
	}
	
	TokenList &	TokenList::pop_back()
	{
		if (!empty())
		{
			m_values.erase(end() - 1);
		}

		return (*this);
	}
	
	TokenList &	TokenList::pop_front()
	{
		if (!empty())
		{
			m_values.erase(begin());
		}

		return (*this);
	}
	
	TokenList &	TokenList::push_back(char value)
	{
		return push_back(Token(value));
	}
	
	TokenList &	TokenList::push_back(const Token& value)
	{
		m_values.push_back(value);

		return (*this);
	}
	
	TokenList &	TokenList::push_back(const List<Token> & value)
	{
		for (auto it = value.begin(); it != value.end(); it++)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	
	TokenList &	TokenList::push_back(const TokenList & value)
	{
		return push_back(value.m_values);
	}
	
	TokenList &	TokenList::push_front(char value)
	{
		return push_front(Token(value));
	}
	
	TokenList &	TokenList::push_front(const Token& value)
	{
		return insert(0, value);
	}
	
	TokenList &	TokenList::push_front(const List<Token> & value)
	{
		for (auto it = value.end(); it != value.begin(); it--)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	
	TokenList &	TokenList::push_front(const TokenList & value)
	{
		return push_front(value.m_values);
	}
	
	TokenList &	TokenList::unwrap()
	{
		pop_front();
		return pop_back();
	}
	
	TokenList &	TokenList::wrap(const Token & value)
	{
		return wrap(value, value);
	}
	
	TokenList &	TokenList::wrap(const Token & lhs, const Token & rhs)
	{
		return push_front(lhs).push_back(rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	TokenList::const_iterator TokenList::find(const Token& value, size_t index) const
	{
		for (TokenList::const_iterator it = begin() + index; it != end(); it++)
		{
			if (*it == value)
			{
				return it;
			}
		}
		return end();
	}
	
	TokenList::const_iterator TokenList::find_first(const Token& value) const
	{
		for (TokenList::const_iterator it = begin(); it != end(); it++)
		{
			if ((*it) == value)
			{
				return it;
			}
		}
		return end();
	}
	
	TokenList::const_iterator TokenList::find_first_not_of(const Token& value, size_t index) const
	{
		for (TokenList::const_iterator it = begin(); it != end(); it++)
		{
			if ((*it) != value)
			{
				return it;
			}
		}
		return end();
	}
	
	TokenList::const_iterator TokenList::find_last(const Token& value) const
	{
		for (TokenList::const_iterator it = end(); it != begin(); it--)
		{
			if ((*it) == value)
			{
				return it;
			}
		}
		return end();
	}
	
	TokenList::const_iterator TokenList::find_last_not_of(const Token& value) const
	{
		for (auto it = end(); it != begin(); it--)
		{
			if ((*it) != value)
			{
				return it;
			}
		}
		return end();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void TokenList::serialize(std::ostream & out) const
	{
		TokenList::const_iterator it;
		for (it = begin(); it != end(); it++)
		{
			if ((*it) == '\n')
			{
				out << '\n';
				continue;
			}
			out << (*it) << (it != end() - 1 ? ", " : "");
		}
	}

	void TokenList::deserialize(std::istream & in)
	{
	}

	bool TokenList::equals(const TokenList & value) const
	{
		if (size() == value.size())
		{
			for (size_t i = 0, imax = size(); i < imax; i++)
			{
				if ((*this)[i] != value[i])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	bool TokenList::lessThan(const TokenList & value) const
	{
		if (size() != value.size())
		{
			for (size_t i = 0, imax = size(); i < imax; i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}
		return size() < value.size();
	}


	/* * * * * * * * * * * * * * * * * * * * */
}