#ifndef _ML_C_STRING_HPP_
#define _ML_C_STRING_HPP_

#include <ML/Core/Array.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using C_String	 = typename const char *;
	using C_WString	 = typename const wchar_t *;
	using C_String16 = typename const char16_t *;
	using C_String32 = typename const char32_t *;

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

	// Constexpr String
	struct CX_String final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename char;
		using self_type			= typename CX_String;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t npos { static_cast<size_t>(-1) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			size_t N
		> constexpr CX_String(const value_type(& value)[N]) noexcept
			: self_type { &value[0], (N - 1) }
		{
		}

		constexpr CX_String(const_pointer begin, const_pointer end) noexcept
			: self_type { begin, (size_t)(end - begin) }
		{
		}

		constexpr CX_String(const_pointer data) noexcept
			: self_type { data, alg::strlen(data) }
		{
		}

		constexpr CX_String(const_pointer data, size_t size) noexcept
			: m_data { data }
			, m_size { size }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto begin()	const -> const_iterator	{ return data(); }
		constexpr auto cbegin() const -> const_iterator	{ return begin(); }
		constexpr auto cend()	const -> const_iterator	{ return end(); }
		constexpr auto c_str()	const -> const_pointer	{ return begin(); }
		constexpr auto data()	const -> const_pointer	{ return m_data; }
		constexpr auto end()	const -> const_iterator	{ return begin() + size(); }
		constexpr auto hash()	const -> hash_t			{ return Hash { data(), size() }; }
		constexpr auto size()	const -> size_t			{ return m_size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator const_pointer() const
		{ 
			return m_data;
		}

		constexpr const_reference operator[](size_t i) const
		{ 
			return m_data[i];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr size_t find_first_of(size_t off, value_type value) const
		{
			return ((off < this->size())
				? (((*this)[off] == value) ? off : this->find_first_of(off + 1, value))
				: self_type::npos
			);
		}

		constexpr size_t find_last_of(size_t off, value_type value) const
		{
			return ((off < this->size())
				? (((*this)[off] == value) ? off : this->find_last_of(off - 1, value))
				: self_type::npos
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const_pointer	m_data;
		size_t			m_size;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const CX_String & value)
	{
		for (const auto & elem : value)
		{
			out << elem;
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const CX_String & lhs, const CX_String & rhs)
	{
		return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator!=(const CX_String & lhs, const CX_String & rhs)
	{
		return !(lhs == rhs);
	}

	constexpr bool operator<(const CX_String & lhs, const CX_String & rhs)
	{
		return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator>(const CX_String & lhs, const CX_String & rhs)
	{
		return !(lhs > rhs);
	}

	constexpr bool operator<=(const CX_String & lhs, const CX_String & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	constexpr bool operator>=(const CX_String & lhs, const CX_String & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const CX_String & lhs, C_String rhs)
	{
		return (lhs == CX_String { rhs });
	}

	constexpr bool operator!=(const CX_String & lhs, C_String rhs)
	{
		return (lhs != CX_String { rhs });
	}

	constexpr bool operator<(const CX_String & lhs, C_String rhs)
	{
		return (lhs < CX_String { rhs });
	}

	constexpr bool operator>(const CX_String & lhs, C_String rhs)
	{
		return (lhs < CX_String { rhs });
	}

	constexpr bool operator<=(const CX_String & lhs, C_String rhs)
	{
		return (lhs <= CX_String { rhs });
	}

	constexpr bool operator>=(const CX_String & lhs, C_String rhs)
	{
		return (lhs >= CX_String { rhs });
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_C_STRING_HPP_