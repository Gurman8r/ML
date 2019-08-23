#ifndef _ML_STRING_VIEW_HPP_
#define _ML_STRING_VIEW_HPP_

#include <ML/Core/Alg.hpp>
#include <ML/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct StringView final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using pointer			= typename char *;
		using reference			= typename char &;
		using const_pointer		= typename const char *;
		using const_reference	= typename const char &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr const size_t npos { static_cast<size_t>(-1) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			size_t N
		> constexpr StringView(const char(& value)[N]) noexcept
			: StringView { &value[0], (N - 1) }
		{
		}

		constexpr StringView(const_pointer begin, const_pointer end) noexcept
			: StringView { begin, (size_t)(end - begin) }
		{
		}

		constexpr StringView(const_pointer data) noexcept
			: StringView { data, alg::strlen(data) }
		{
		}

		constexpr StringView(const_pointer data, size_t size) noexcept
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

		constexpr size_t find_first_of(size_t off, char value) const
		{
			return ((off < size())
				? (((*this)[off] == value) ? (off) : find_first_of(off + 1, value))
				: npos
			);
		}

		constexpr size_t find_last_of(size_t off, char value) const
		{
			return ((off < size())
				? (((*this)[off] == value) ? (off) : find_last_of(off - 1, value))
				: npos
			);
		}

		constexpr StringView substr(size_t off, size_t count) const
		{
			return (((off + count) < size())
				? StringView { begin() + off, count }
				: StringView { (*this) }
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const_pointer	m_data;
		const size_t	m_size;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const StringView & value)
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
		return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
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
}

#endif // !_ML_STRING_VIEW_HPP_