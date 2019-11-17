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

		constexpr auto at(size_t i) const -> const_reference	{ return *(cbegin() + i); }
		constexpr auto back()		const -> const_reference	{ return (*(cend() - 1)); }
		constexpr auto begin()		const -> const_iterator		{ return m_data; }
		constexpr auto cbegin()		const -> const_iterator		{ return m_data; }
		constexpr auto cend()		const -> const_iterator		{ return m_data + m_size; }
		constexpr auto data()		const -> const_pointer		{ return m_data; }
		constexpr auto empty()		const -> bool				{ return (m_size == 0); }
		constexpr auto end()		const -> const_iterator		{ return m_data + m_size; }
		constexpr auto front()		const -> const_reference	{ return (*cbegin()); }
		constexpr auto hash()		const -> hash_t				{ return Hash{}(m_data, m_size); }
		constexpr auto size()		const -> size_t				{ return m_size; }
		inline	  auto str()		const -> std::string		{ return { m_data, m_size }; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const
		{
			return size();
		}

		constexpr const_reference operator[](size_t i) const 
		{ 
			return at(i); 
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr size_t find_first_of(value_type value) const
		{
			return find_first_of(value, 0);
		}

		constexpr size_t find_first_of(value_type value, size_t off) const
		{
			return ((off < size())
				? (at(off) != value)
					? find_first_of(value, off + 1)
					: off
				: self_type::npos
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr size_t find_last_of(value_type value) const
		{
			return find_last_of(value, size() - 1);
		}

		constexpr size_t find_last_of(value_type value, size_t off) const
		{
			return ((off < size())
				? (at(off) != value) 
					? find_last_of(value, off - 1)
					: off
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

	private: const_pointer m_data; size_t m_size;

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