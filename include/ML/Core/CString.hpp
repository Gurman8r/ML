#ifndef _ML_C_STRING_HPP_
#define _ML_C_STRING_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Alg.hpp>
#include <ML/Core/Hash.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using CString		= typename const char *;
	using CWString		= typename const wchar_t *;
	using CU16String	= typename const char16_t *;
	using CU32String	= typename const char32_t *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		template <
			class Ch
		> static constexpr size_t strlen(const Ch * value)
		{
			return ((*value) ? (1 + alg::strlen(value + 1)) : 0);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Compile-Time String
	struct ct_string //final
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename char;
		using self_type			= typename ct_string;
		using hash_type			= typename size_t;
		using size_type			= typename size_t;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			size_type N
		> constexpr ct_string(const value_type(& value)[N])
			: self_type { &value[0], (N - 1) }
		{
		}

		constexpr ct_string(const_pointer begin, const_pointer end)
			: self_type { begin, type_t<size_type>{ end - begin }() }
		{
		}

		constexpr ct_string(const_pointer value)
			: self_type { value, alg::strlen(value) }
		{
		}

		constexpr ct_string(const_pointer value, size_type size)
			: m_data { value }
			, m_size { size }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr auto begin()	const -> const_iterator	{ return data(); }
		constexpr auto cbegin() const -> const_iterator	{ return begin(); }
		constexpr auto cend()	const -> const_iterator	{ return end(); }
		constexpr auto c_str()	const -> const_pointer	{ return begin(); }
		constexpr auto data()	const -> const_pointer	{ return m_data; }
		constexpr auto end()	const -> const_iterator	{ return begin() + size(); }
		constexpr auto hash()	const -> size_t			{ return Hash()(size(), begin()); }
		constexpr auto size()	const -> size_t			{ return m_size; }

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr operator const_pointer() const 
		{ 
			return this->c_str(); 
		}

		constexpr const_reference operator[](size_type i) const
		{ 
			return this->data()[i];
		}
			
		constexpr const_pointer operator()(size_type i) const 
		{ 
			return (this->data() + i);
		}

		constexpr self_type operator()(size_type begin, size_type end) const
		{
			return self_type { this->data() + begin, this->data() + end };
		}

		constexpr self_type pad(size_type begin_off, size_type end_off) const
		{
			return (*this)(begin_off, this->size() - end_off);
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		const_pointer	m_data;
		size_type		m_size;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(ostream & out, const ct_string & value)
	{
		for (const auto & elem : value)
		{
			out << elem;
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const ct_string & lhs, const ct_string & rhs)
	{
		return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator!=(const ct_string & lhs, const ct_string & rhs)
	{
		return !(lhs == rhs);
	}

	constexpr bool operator<(const ct_string & lhs, const ct_string & rhs)
	{
		return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator>(const ct_string & lhs, const ct_string & rhs)
	{
		return !(lhs > rhs);
	}

	constexpr bool operator<=(const ct_string & lhs, const ct_string & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	constexpr bool operator>=(const ct_string & lhs, const ct_string & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_C_STRING_HPP_