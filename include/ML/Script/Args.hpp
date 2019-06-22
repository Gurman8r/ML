#ifndef _ML_ARGS_HPP_
#define _ML_ARGS_HPP_

#include <ML/Script/Export.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Args final
		: public I_Newable
		, public I_Comparable<Args>
	{
	public:
		using list_type				= typename List<String>;
		using iterator				= typename list_type::iterator;
		using const_iterator		= typename list_type::const_iterator;
		using reverse_iterator		= typename list_type::reverse_iterator;
		using const_reverse_iterator= typename list_type::const_reverse_iterator;
		using difference_type		= typename list_type::difference_type;

	public:
		Args();
		Args(int32_t argc, char ** argv);
		Args(const String & value);
		Args(const List<String> & values);
		Args(const Initializer<String>& values);
		Args(const String & value, const String & delim);
		Args(const Args & copy);
		~Args();

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
		Args & assign(const List<String> & value);
		Args & clear();
		Args & copy(const Args & other);
		Args & copy(const Args & other, size_t index);
		Args & copy(const Args & other, size_t index, size_t count);
		Args & copy(const_iterator first, const_iterator last);
		Args & erase(size_t index, size_t count = 1);
		Args & erase(const_iterator it, size_t count = 1);
		Args & erase(const_iterator first, const_iterator last);
		Args & insert(size_t index, char value);
		Args & insert(size_t index, CString value);
		Args & insert(size_t index, const String & value);
		Args & pop_back();
		Args & pop_front();
		Args & push_back(char value);
		Args & push_back(CString value);
		Args & push_back(const String & value);
		Args & push_back(const List<String> & value);
		Args & push_back(const Args & value);
		Args & push_front(char value);
		Args & push_front(CString value);
		Args & push_front(const String & value);
		Args & push_front(const List<String> & value);
		Args & push_front(const Args & value);
		Args & remove(const String & value);
		Args & removeAll(const String & value);
		Args & resize(size_t size);
		Args & reverse();

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
		
		inline Args & operator=(const Args & copy)
		{
			return (*this).copy(copy);
		}
		
		inline Args & operator=(const List<String> & value)
		{
			return (*this) = Args(value);
		}

	public:
		bool equals(const Args & value) const override;
		bool lessThan(const Args & value) const override;

	public:
		inline SStream sstr() const
		{
			SStream ss;
			for (const auto & e : (*this))
				ss << e;
			return ss;
		}
		inline String	str()	const { return sstr().str(); }
		inline CString	c_str() const { return str().c_str(); }

	private:
		List<String> m_values;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ARGS_HPP_