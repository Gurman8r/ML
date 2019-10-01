#ifndef _ML_STRING_VIEW_HPP_
#define _ML_STRING_VIEW_HPP_

#include <ML/Core/Alg.hpp>
#include <ML/Core/Hash.hpp>

namespace ml
{
	struct StringView final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename char;
		using self_type			= typename StringView;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;
		using cppstring			= typename std::basic_string<value_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto npos { static_cast<size_t>(-1) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t N> constexpr StringView(const value_type(&arr)[N]) noexcept
			: self_type { &arr[0], (N - 1) }
		{
		}

		constexpr StringView(const_iterator first, const_iterator last) noexcept
			: self_type { first, (size_t)(last - first) }
		{
		}

		constexpr StringView(const_pointer data) noexcept
			: self_type { data, alg::strlen(data) }
		{
		}

		constexpr StringView(const_pointer data, size_t size) noexcept
			: m_data { data }
			, m_size { size }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto back()	const -> const_reference	{ return (*(end() - 1)); }
		constexpr auto begin()	const -> const_iterator		{ return m_data; }
		constexpr auto cbegin() const -> const_iterator		{ return m_data; }
		constexpr auto cend()	const -> const_iterator		{ return m_data + m_size; }
		constexpr auto c_str()	const -> const_pointer		{ return m_data; }
		constexpr auto data()	const -> const_pointer		{ return m_data; }
		constexpr auto empty()	const -> bool				{ return (m_size == 0); }
		constexpr auto end()	const -> const_iterator		{ return m_data + m_size; }
		constexpr auto front()	const -> const_reference	{ return (*begin()); }
		constexpr auto hash()	const -> hash_t				{ return Hash { m_data, m_size }; }
		constexpr auto size()	const -> size_t				{ return m_size; }
		inline	  auto str()	const -> cppstring			{ return { m_data, m_size }; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr const_reference operator[](size_t i) const
		{
			return m_data[i];
		}

		constexpr operator const const_pointer &() const
		{ 
			return m_data;
		}

		inline operator cppstring() const
		{
			return str();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr size_t find_first_of(value_type value) const
		{
			return find_first_of(0, value);
		}

		constexpr size_t find_first_of(size_t off, value_type value) const
		{
			return ((off < size())
				? (((*this)[off] == value) ? (off) : find_first_of(off + 1, value))
				: self_type::npos
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr size_t find_last_of(value_type value) const
		{
			return find_last_of(size() - 1, value);
		}

		constexpr size_t find_last_of(size_t off, value_type value) const
		{
			return ((off < size())
				? (((*this)[off] == value) ? (off) : find_last_of(off - 1, value))
				: self_type::npos
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type substr(size_t off) const
		{
			return substr(off, size() - off);
		}

		constexpr self_type substr(size_t off, size_t count) const
		{
			return self_type { begin() + off, count };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const const_pointer	m_data;
		const size_t m_size;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const StringView & value)
	{
		for (const auto & elem : value)
		{
			out << elem;
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const StringView & lhs, const StringView & rhs)
	{
		return ((lhs.begin() == rhs.begin()) && (lhs.end() == rhs.end())) ||
			alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator!=(const StringView & lhs, const StringView & rhs)
	{
		return !(lhs == rhs);
	}

	constexpr bool operator<(const StringView & lhs, const StringView & rhs)
	{
		return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator>(const StringView & lhs, const StringView & rhs)
	{
		return !(lhs > rhs);
	}

	constexpr bool operator<=(const StringView & lhs, const StringView & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	constexpr bool operator>=(const StringView & lhs, const StringView & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const StringView & lhs, C_String rhs)
	{
		return (lhs == StringView { rhs });
	}

	constexpr bool operator!=(const StringView & lhs, C_String rhs)
	{
		return (lhs != StringView { rhs });
	}

	constexpr bool operator<(const StringView & lhs, C_String rhs)
	{
		return (lhs < StringView { rhs });
	}

	constexpr bool operator>(const StringView & lhs, C_String rhs)
	{
		return (lhs < StringView { rhs });
	}

	constexpr bool operator<=(const StringView & lhs, C_String rhs)
	{
		return (lhs <= StringView { rhs });
	}

	constexpr bool operator>=(const StringView & lhs, C_String rhs)
	{
		return (lhs >= StringView { rhs });
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr bool operator==(const StringView & lhs, const std::basic_string<char> & rhs)
	{
		return (lhs == rhs.c_str());
	}

	constexpr bool operator!=(const StringView & lhs, const std::basic_string<char> & rhs)
	{
		return (lhs != rhs.c_str());
	}

	constexpr bool operator<(const StringView & lhs, const std::basic_string<char> & rhs)
	{
		return (lhs < rhs.c_str());
	}

	constexpr bool operator>(const StringView & lhs, const std::basic_string<char> & rhs)
	{
		return (lhs < rhs.c_str());
	}

	constexpr bool operator<=(const StringView & lhs, const std::basic_string<char> & rhs)
	{
		return (lhs <= rhs.c_str());
	}

	constexpr bool operator>=(const StringView & lhs, const std::basic_string<char> & rhs)
	{
		return (lhs >= rhs.c_str());
	}
}

template<> struct _STD hash<_ML StringView>
{
	inline _STD size_t operator()(const _ML StringView & value) const noexcept
	{
		return static_cast<_STD size_t>(value.hash());
	}
};

#endif // !_ML_STRING_VIEW_HPP_