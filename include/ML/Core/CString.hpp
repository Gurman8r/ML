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

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Ch
		> struct basic_c_string final
		{
			/* * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename Ch;
			using self_type			= typename basic_c_string<value_type>;
			using hash_type			= typename size_t;
			using size_type			= typename size_t;
			using pointer			= typename value_type *;
			using reference			= typename value_type &;
			using const_pointer		= typename const value_type *;
			using const_reference	= typename const value_type &;

			/* * * * * * * * * * * * * * * * * * * * */

			template <
				size_type N
			> constexpr basic_c_string(const value_type(& value)[N])
				: self_type { &value[0], (N - 1) }
			{
			}

			constexpr basic_c_string(const_pointer begin, const_pointer end)
				: self_type { begin, type_t<size_type>{ end - begin }() }
			{
			}

			constexpr basic_c_string(const_pointer value)
				: self_type { value, alg::strlen(value) }
			{
			}

			constexpr basic_c_string(const_pointer value, size_type size)
				: m_data { value }
				, m_size { size }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * */

			constexpr auto begin()	const -> const_pointer	{ return m_data; }
			constexpr auto cbegin() const -> const_pointer	{ return begin(); }
			constexpr auto cend()	const -> const_pointer	{ return end(); }
			constexpr auto c_str()	const -> const_pointer	{ return begin(); }
			constexpr auto end()	const -> const_pointer	{ return begin() + size(); }
			constexpr auto hash()	const -> hash_type		{ return ml::hash()(size(), begin()); }
			constexpr auto size()	const -> size_type		{ return m_size; }

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
			const size_type	m_size;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		using cstring	= typename basic_c_string<char>;
		using cwstring	= typename basic_c_string<wchar_t>;
		using c16string	= typename basic_c_string<char16_t>;
		using c32string	= typename basic_c_string<char32_t>;

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Ch
		> inline ML_SERIALIZE(ostream & out, const basic_c_string<Ch> & value)
		{
			for (const auto & elem : value)
			{
				out << elem;
			}
			return out;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Ch
		> constexpr bool operator==(const basic_c_string<Ch> & lhs, const basic_c_string<Ch> & rhs)
		{
			return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		template <
			class Ch
		> constexpr bool operator!=(const basic_c_string<Ch> & lhs, const basic_c_string<Ch> & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			class Ch
		> constexpr bool operator<(const basic_c_string<Ch> & lhs, const basic_c_string<Ch> & rhs)
		{
			return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		template <
			class Ch
		> constexpr bool operator>(const basic_c_string<Ch> & lhs, const basic_c_string<Ch> & rhs)
		{
			return !(lhs > rhs);
		}

		template <
			class Ch
		> constexpr bool operator<=(const basic_c_string<Ch> & lhs, const basic_c_string<Ch> & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		template <
			class Ch
		> constexpr bool operator>=(const basic_c_string<Ch> & lhs, const basic_c_string<Ch> & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_C_STRING_HPP_