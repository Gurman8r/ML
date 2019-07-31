#ifndef _ML_C_STRING_HPP_
#define _ML_C_STRING_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/Array.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using C_String		= typename const char *;
	using CW_String		= typename const wchar_t *;
	using CU16_String	= typename const char16_t *;
	using CU32_String	= typename const char32_t *;

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
	struct X_String final
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename char;
		using self_type			= typename X_String;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			size_t N
		> constexpr X_String(const value_type(& value)[N])
			: self_type { &value[0], (N - 1) }
		{
		}

		constexpr X_String(const_pointer begin, const_pointer end)
			: self_type { begin, static_value<size_t>{ end - begin } }
		{
		}

		constexpr X_String(const_pointer value)
			: self_type { value, alg::strlen(value) }
		{
		}

		constexpr X_String(const_pointer value, size_t size)
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
		constexpr auto hash()	const -> hash_t			{ return Hash(size(), begin()); }
		constexpr auto size()	const -> size_t			{ return m_size; }

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr operator const_pointer() const 
		{ 
			return this->c_str(); 
		}

		constexpr const_reference operator[](size_t i) const
		{ 
			return this->data()[i];
		}
			
		constexpr const_pointer operator()(size_t i) const 
		{ 
			return (this->data() + i);
		}

		constexpr self_type operator()(size_t begin, size_t end) const
		{
			return self_type { this->data() + begin, this->data() + end };
		}

		constexpr self_type pad(size_t begin_off, size_t end_off) const
		{
			return (*this)(begin_off, this->size() - end_off);
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		const_pointer	m_data;
		size_t			m_size;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const X_String & value)
	{
		for (const auto & elem : value)
		{
			out << elem;
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const X_String & lhs, const X_String & rhs)
	{
		return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator!=(const X_String & lhs, const X_String & rhs)
	{
		return !(lhs == rhs);
	}

	constexpr bool operator<(const X_String & lhs, const X_String & rhs)
	{
		return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator>(const X_String & lhs, const X_String & rhs)
	{
		return !(lhs > rhs);
	}

	constexpr bool operator<=(const X_String & lhs, const X_String & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	constexpr bool operator>=(const X_String & lhs, const X_String & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_C_STRING_HPP_