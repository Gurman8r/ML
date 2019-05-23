#ifndef _ML_X_STRING_HPP_
#define _ML_X_STRING_HPP_

#include <ML/Core/String.hpp>
#include <ML/Core/Hash.hpp>

// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/cstring.hpp

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		template<typename LhsIt, typename RhsIt>
		constexpr bool equal_range(LhsIt lb, LhsIt le, RhsIt rb, RhsIt re)
		{
			return ((lb != le && rb != re)
				? *lb == *rb && equal_range(lb + 1, le, rb + 1, re)
				: (lb == le && rb == re)
			);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class XString final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		template<size_t N>
		constexpr XString(const char(&str)[N])
			: XString { &str[0], N - 1 }
		{
		}

		constexpr XString(CString begin, size_t length)
			: m_str { begin }
			, m_length { length }
		{
		}

		constexpr XString(CString begin, CString end)
			: XString { begin, static_cast<size_t>(end - begin) }
		{
		}

		constexpr XString(CString begin)
			: XString { begin, length(begin) }
		{
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		static constexpr size_t length(CString str) { return *str ? 1 + length(str + 1) : 0; }

		constexpr size_t length() const { return m_length; }

		constexpr size_t size() const { return length(); }

		constexpr hash_t hash() const { return hash::fnv::do_hash(length(), begin()); }

		String cppstring() const { return { begin(), end() }; }

		String str() const { return cppstring(); }

		operator String() const { return str(); }

		operator CString() const { return m_str; }

		constexpr CString begin() const { return m_str; }

		constexpr CString end() const { return m_str + m_length; }

		constexpr char operator[](size_t i) const { return m_str[i]; }

		constexpr CString operator()(size_t i) const { return m_str + i; }

		constexpr XString operator()(size_t begin, size_t end) const { return { m_str + begin, m_str + end }; }

		constexpr XString pad(size_t begin_offset, size_t end_offset) const { return operator()(begin_offset, size() - end_offset); }

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		friend std::ostream & operator<<(std::ostream & out, const XString & str)
		{
			for (const char c : str)
				out << c;
			return out;
		}

		friend constexpr bool operator==(const XString & lhs, const XString & rhs)
		{
			return detail::equal_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		friend constexpr bool operator!=(const XString & lhs, const XString & rhs)
		{
			return !(lhs == rhs);
		}

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		CString m_str;
		size_t m_length;
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

namespace std
{
	template <>
	struct hash<ml::XString>
	{
		inline ml::hash_t operator()(const ml::XString & value) const noexcept
		{
			return value.hash();
		}
	};
}

#endif // !_ML_X_STRING_HPP_