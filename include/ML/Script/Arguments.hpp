#ifndef _ML_ARGUMENTS_HPP_
#define _ML_ARGUMENTS_HPP_

#include <ML/Script/Export.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Comparable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Arguments final
		: public I_Newable
		, public I_Comparable<Arguments>
	{
	public:
		using list_type				= typename List<String>;
		using iterator				= typename list_type::iterator;
		using const_iterator		= typename list_type::const_iterator;
		using reverse_iterator		= typename list_type::reverse_iterator;
		using const_reverse_iterator= typename list_type::const_reverse_iterator;
		using difference_type		= typename list_type::difference_type;

	public:
		Arguments();
		Arguments(int32_t argc, char ** argv);
		Arguments(const String & value);
		Arguments(const List<String> & values);
		Arguments(const Initializer<String>& values);
		Arguments(const String & value, const String & delim);
		Arguments(const Arguments & copy);
		~Arguments();

		String pop();

		String	at(size_t index) const;
		String	back() const;
		String	front() const;

		size_t	count(const String & value) const;
		size_t	count(const_iterator first, const_iterator last, const String & value) const;
		size_t	indexOf(const String & value) const;
		size_t	size() const;

		bool	contains(const String & value) const;
		bool	empty() const;
		bool	inRange(size_t index) const;
		bool	inRange(const_iterator it) const;

	public:
		Arguments & assign(const List<String> & value);
		Arguments & clear();
		Arguments & copy(const Arguments & other);
		Arguments & copy(const Arguments & other, size_t index);
		Arguments & copy(const Arguments & other, size_t index, size_t count);
		Arguments & copy(const_iterator first, const_iterator last);
		Arguments & erase(size_t index, size_t count = 1);
		Arguments & erase(const_iterator it, size_t count = 1);
		Arguments & erase(const_iterator first, const_iterator last);
		Arguments & insert(size_t index, char value);
		Arguments & insert(size_t index, C_String value);
		Arguments & insert(size_t index, const String & value);
		Arguments & pop_back();
		Arguments & pop_front();
		Arguments & push_back(char value);
		Arguments & push_back(C_String value);
		Arguments & push_back(const String & value);
		Arguments & push_back(const List<String> & value);
		Arguments & push_back(const Arguments & value);
		Arguments & push_front(char value);
		Arguments & push_front(C_String value);
		Arguments & push_front(const String & value);
		Arguments & push_front(const List<String> & value);
		Arguments & push_front(const Arguments & value);
		Arguments & remove(const String & value);
		Arguments & removeAll(const String & value);
		Arguments & resize(size_t size);
		Arguments & reverse();

	public:
		const_iterator	find(const String & value, size_t begin = 0) const;
		const_iterator	find_first(const String & value) const;
		const_iterator	find_first_not_of(const String & value, size_t begin = 0) const;
		const_iterator	find_last(const String & value) const;
		const_iterator	find_last_not_of(const String & value) const;

		bool find_and_erase(const String & value);

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
		inline String operator[](size_t index) const
		{
			if (inRange(index))
			{
				return m_values.at(index);
			}
			return String();
		};
		
		inline Arguments & operator=(const Arguments & copy)
		{
			return (*this).copy(copy);
		}
		
		inline Arguments & operator=(const List<String> & value)
		{
			return (*this) = Arguments(value);
		}

	public:
		bool equals(const Arguments & value) const override;
		bool lessThan(const Arguments & value) const override;

	public:
		inline SStream sstr() const
		{
			SStream ss;
			for (const auto & e : (*this))
				ss << e;
			return ss;
		}
		inline String	str()	const { return sstr().str(); }
		inline C_String	c_str() const { return str().c_str(); }

	private:
		List<String> m_values;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ARGUMENTS_HPP_