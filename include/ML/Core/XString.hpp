#ifndef _ML_X_STRING_HPP_
#define _ML_X_STRING_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/cstring.hpp

#include <ML/Core/Algorithm.hpp>
#include <ML/Core/Hash.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Compile-time string
	class XString final
	{
	public:
		template<size_t N>
		constexpr XString(const char(&str)[N]) 
			: XString { &str[0], (N - 1) }
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
		static constexpr size_t length(CString value)
		{
			return (*value) ? 1 + length(value + 1) : 0;
		}

		constexpr size_t	length()	const { return m_length; }
		constexpr size_t	size()		const { return length(); }
		constexpr hash_t	hash()		const { return hash::fnv1a<char>(length(), begin()); }
		constexpr CString	begin()		const { return m_str; }
		constexpr CString	end()		const { return m_str + m_length; }

		CString c_str()		const { return begin(); }
		String	str()		const { return { begin(), end() }; }
		operator CString()	const { return c_str(); }
		operator String()	const { return str(); }

		constexpr char operator[](size_t i) const 
		{ 
			return m_str[i]; 
		}
		
		constexpr CString operator()(size_t i) const 
		{ 
			return (m_str + i); 
		}

		constexpr XString operator()(size_t begin, size_t end) const
		{
			return { m_str + begin, m_str + end };
		}

		constexpr XString pad(size_t begin_offset, size_t end_offset) const
		{
			return XString::operator()(begin_offset, size() - end_offset);
		}

		friend std::ostream & operator<<(std::ostream & out, const XString& str)
		{
			for (const char c : str)
			{
				out << c;
			}
			return out;
		}

	private:
		CString m_str;
		size_t	m_length;
	};

	constexpr bool operator==(const XString & lhs, const XString & rhs)
	{
		return algorithm::equal_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	constexpr bool operator!=(const XString & lhs, const XString & rhs)
	{
		return !(lhs == rhs);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_X_STRING_HPP_