#ifndef _ML_X_STRING_HPP_
#define _ML_X_STRING_HPP_

#include <ML/Core/String.hpp>
#include <ML/Core/Hash.hpp>

// https://github.com/Manu343726/ml/blob/master/include/ml/detail/XString.hpp

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

	class XString
	{
	public:
		template<size_t N>
		constexpr XString(const char(&str)[N]) :
			XString { &str[0], N - 1 }
		{
		}

		constexpr XString(CString begin, size_t length) :
			_str { begin },
			_length { length }
		{}

		constexpr XString(CString begin, CString end) :
			XString { begin, static_cast<size_t>(end - begin) }
		{
		}

		constexpr XString(CString begin) :
			XString { begin, length(begin) }
		{
		}

		static constexpr size_t length(CString str)
		{
			return *str ? 1 + length(str + 1) : 0;
		}

		constexpr size_t length() const
		{
			return _length;
		}

		constexpr size_t size() const
		{
			return length();
		}

		constexpr hash_t hash() const
		{
			return hash::fnv::do_hash(length(), begin());
		}

		ml::String cppstring() const
		{
			return { begin(), end() };
		}

		ml::String str() const
		{
			return cppstring();
		}

		operator ml::String() const
		{
			return str();
		}

		constexpr CString begin() const
		{
			return _str;
		}

		constexpr CString end() const
		{
			return _str + _length;
		}

		constexpr char operator[](size_t i) const
		{
			return _str[i];
		}

		constexpr CString operator()(size_t i) const
		{
			return _str + i;
		}

		constexpr XString operator()(size_t begin, size_t end) const
		{
			return { _str + begin, _str + end };
		}

		constexpr XString pad(size_t begin_offset, size_t end_offset) const
		{
			return operator()(begin_offset, size() - end_offset);
		}

		friend std::ostream& operator<<(std::ostream& os, const XString& str)
		{
			for (const char c : str)
			{
				os << c;
			}

			return os;
		}

	private:
		CString _str;
		size_t _length;
	};

	constexpr bool operator==(const XString& lhs, const XString& rhs)
	{
		return detail::equal_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator!=(const XString& lhs, const XString& rhs)
	{
		return !(lhs == rhs);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_X_STRING_HPP_