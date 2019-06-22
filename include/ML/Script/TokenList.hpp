#ifndef _ML_TOKEN_LIST_HPP_
#define _ML_TOKEN_LIST_HPP_

#include <ML/Script/Token.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class TokenList;

	using TokenTree = List<TokenList>;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API TokenList final
		: public I_Newable
		, public I_Comparable<TokenList>
	{
	public:
		using iterator				= List<Token>::iterator;
		using const_iterator		= List<Token>::const_iterator;
		using reverse_iterator		= List<Token>::reverse_iterator;
		using const_reverse_iterator= List<Token>::const_reverse_iterator;
		using difference_type		= List<Token>::difference_type;

	public:
		TokenList();
		TokenList(const Token & value);
		TokenList(const List<Token> & values);
		TokenList(const TokenList & copy);
		~TokenList();

	public:
		Token	at(size_t index) const;
		Token	back() const;
		Token	front() const;
		String	str() const;
		Token	pop();

	public:
		bool	empty() const;

		size_t	size() const;
		bool	size(const size_t value) const;

		bool	back(const Token & value) const;
		bool	back(const String & value) const;
		bool	back(char value) const;
		bool	front(const Token & value) const;
		bool	front(const String & value) const;
		bool	front(char value) const;

		bool	inRange(size_t index) const;
		bool	inRange(const_iterator it) const;
		bool	isWrap(const Token & value) const;
		bool	isWrap(const Token & lhs, const Token & rhs) const;

	public:
		bool	match_type(size_t index, char c) const;
		bool	match_type(const const_iterator & it, char c) const;
		
		bool	match_type_str(const String & str) const;
		bool	match_type_str(size_t index, const String & str) const;
		bool	match_type_str(const const_iterator & it, const String & str) const;

		bool	match_type_list(size_t index, const List<char> & pattern) const;
		bool	match_type_list(const const_iterator & it, const List<char> & pattern) const;
		
		bool	match_data(const List<CString> & data) const;
		bool	match_data(size_t index, const List<CString> & data) const;
		bool	match_data(const const_iterator & it, const List<CString> & data) const;

	public:
		TokenList	after(size_t index) const;
		TokenList	between(const Token & lhs, const Token & rhs) const;
		TokenList	unwrapped() const;

	public:
		TokenList & assign(const List<Token> & value);
		TokenList &	clear();
		TokenList &	erase(size_t index, size_t count = 1);
		TokenList &	erase(const_iterator it, size_t count = 1);
		TokenList &	erase(const_iterator first, const_iterator last);
		TokenList &	insert(size_t index, char value);
		TokenList &	insert(size_t index, const Token & value);
		TokenList &	insert(iterator it, const Token & value);
		TokenList &	pop_back();
		TokenList &	pop_front();
		TokenList &	push_back(char value);
		TokenList &	push_back(const Token & value);
		TokenList &	push_back(const List<Token> & value);
		TokenList &	push_back(const TokenList & value);
		TokenList &	push_front(char value);
		TokenList &	push_front(const Token & value);
		TokenList &	push_front(const List<Token> & value);
		TokenList &	push_front(const TokenList & value);
		TokenList &	unwrap();
		TokenList &	wrap(const Token & value);
		TokenList &	wrap(const Token & lhs, const Token & rhs);

	public:
		const_iterator	find(const Token & value, size_t begin = 0) const;
		const_iterator	find_first(const Token & value) const;
		const_iterator	find_first_not_of(const Token & value, size_t begin = 0) const;
		const_iterator	find_last(const Token & value) const;
		const_iterator	find_last_not_of(const Token & value) const;

	public:
		inline iterator					begin()				{ return m_values.begin();	}
		inline iterator					end()				{ return m_values.end();	}
		inline const_iterator			begin()		const	{ return m_values.begin();	}
		inline const_iterator			end()		const	{ return m_values.end();	}
		inline const_iterator			cbegin()	const	{ return m_values.cbegin(); }
		inline const_iterator			cend()		const	{ return m_values.cend();	}
		inline reverse_iterator			rbegin()			{ return m_values.rbegin(); }
		inline reverse_iterator			rend()				{ return m_values.rend();	}
		inline const_reverse_iterator	crbegin()	const	{ return m_values.crbegin();}
		inline const_reverse_iterator	crend()		const	{ return m_values.crend();	}

	public:
		inline const Token & operator[](size_t index) const
		{
			static Token dv;
			return inRange(index)
				? m_values[index]
				: dv;
		}
		inline Token & operator[](size_t index)
		{
			static Token dv;
			return inRange(index)
				? m_values[index]
				: dv;
		}

	public:
		bool equals(const TokenList & value) const override;
		bool lessThan(const TokenList & value) const override;

	private:
		List<Token> m_values;
	};

	/* * * * * * * * * * * * * * * * * * * * */
	
	inline ML_SERIALIZE(ostream & out, const TokenList & value)
	{
		TokenList::const_iterator it;
		for (it = value.begin(); it != value.end(); it++)
		{
			if ((*it) == '\n')
			{
				out << '\n';
				continue;
			}
			out << (*it) << (it != value.end() - 1 ? ", " : "");
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TOKEN_LIST_HPP_