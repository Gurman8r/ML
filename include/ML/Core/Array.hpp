#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <ML/Core/Alg.hpp>
#include <ML/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> struct array_t
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < N, "array_t : size negative or zero");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Size = N };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using type				= typename type_t<value_type>;
		using data_type			= typename value_type[Size];
		using self_type			= typename array_t<value_type, Size>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		data_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto begin()			-> pointer			{ return data(); }
		constexpr auto begin()	const	-> const_pointer	{ return data(); }
		constexpr auto cbegin() const	-> const_pointer	{ return begin(); }
		constexpr auto cend()	const	-> const_pointer	{ return end(); }
		constexpr auto data()			-> pointer			{ return m_data; }
		constexpr auto data()	const	-> const_pointer	{ return m_data; }
		constexpr auto end()			-> pointer			{ return data() + size(); }
		constexpr auto end()	const	-> const_pointer	{ return data() + size(); }
		constexpr auto hash()	const	-> size_t			{ return ml::hash()(size(), data()); }
		constexpr auto size()	const	-> size_t			{ return Size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr const_reference operator[](size_t i) const 
		{ 
			return m_data[i]; 
		}

		constexpr reference operator[](size_t i)
		{ 
			return m_data[i]; 
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto fill(const_reference value)
		{
			self_type temp { meta::uninit };
			alg::fill(temp, value);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> inline ML_SERIALIZE(ostream & out, const array_t<T, N> & value)
	{
		for (const auto & elem : value)
		{
			out << elem << ' ';
		}
		return out;
	}

	template <
		class T, size_t N
	> inline ML_DESERIALIZE(istream & in, array_t<T, N> & value)
	{
		for (auto & elem : value)
		{
			in >> elem;
		}
		return in;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> constexpr bool operator==(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <
		class T, size_t N
	> constexpr bool operator!=(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return !(lhs == rhs);
	}

	template <
		class T, size_t N
	> constexpr bool operator<(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <
		class T, size_t N
	> constexpr bool operator<=(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class T, size_t N
	> constexpr bool operator>(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <
		class T, size_t N
	> constexpr bool operator>=(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> constexpr auto operator+=(array_t<T, N> & lhs, const array_t<T, N> & rhs)
		-> array_t<T, N> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] + rhs[i]);
		}
		return lhs;
	}

	template <
		class T, size_t N
	> constexpr auto operator-=(array_t<T, N> & lhs, const array_t<T, N> & rhs)
		-> array_t<T, N> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] - rhs[i]);
		}
		return lhs;
	}

	template <
		class T, size_t N
	> constexpr auto operator*=(array_t<T, N> & lhs, const array_t<T, N> & rhs)
		-> array_t<T, N> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] * rhs[i]);
		}
		return lhs;
	}

	template <
		class T, size_t N
	> constexpr auto operator/=(array_t<T, N> & lhs, const array_t<T, N> & rhs)
		-> array_t<T, N> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] / rhs[i]);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> constexpr auto operator+(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
		-> array_t<T, N>
	{
		array_t<T, N> temp { lhs };
		temp += rhs;
		return temp;
	}

	template <
		class T, size_t N
	> constexpr auto operator-(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
		-> array_t<T, N>
	{
		array_t<T, N> temp { lhs };
		temp -= rhs;
		return temp;
	}

	template <
		class T, size_t N
	> constexpr auto operator*(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
		-> array_t<T, N>
	{
		array_t<T, N> temp { lhs };
		temp *= rhs;
		return temp;
	}

	template <
		class T, size_t N
	> constexpr auto operator/(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
		-> array_t<T, N>
	{
		array_t<T, N> temp { lhs };
		temp /= rhs;
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> constexpr auto operator+=(array_t<T, N> & lhs, const T & rhs)
		-> array_t<T, N> &
	{
		for (auto & elem : lhs)
		{
			elem += rhs;
		}
		return lhs;
	}

	template <
		class T, size_t N
	> constexpr auto operator-=(array_t<T, N> & lhs, const T & rhs)
		-> array_t<T, N> &
	{
		for (auto & elem : lhs)
		{
			elem -= rhs;
		}
		return lhs;
	}

	template <
		class T, size_t N
	> constexpr auto operator*=(array_t<T, N> & lhs, const T & rhs)
		-> array_t<T, N> &
	{
		for (auto & elem : lhs)
		{
			elem *= rhs;
		}
		return lhs;
	}

	template <
		class T, size_t N
	> constexpr auto operator/=(array_t<T, N> & lhs, const T & rhs)
		-> array_t<T, N> &
	{
		for (auto & elem : lhs)
		{
			elem /= rhs;
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> constexpr auto operator+(const array_t<T, N> & lhs, const T & rhs)
		-> array_t<T, N>
	{
		array_t<T, N> temp { lhs };
		temp += rhs;
		return temp;
	}

	template <
		class T, size_t N
	> constexpr auto operator-(const array_t<T, N> & lhs, const T & rhs)
		-> array_t<T, N>
	{
		array_t<T, N> temp { lhs };
		temp -= rhs;
		return temp;
	}

	template <
		class T, size_t N
	> constexpr auto operator*(const array_t<T, N> & lhs, const T & rhs)
		-> array_t<T, N>
	{
		array_t<T, N> temp { lhs };
		temp *= rhs;
		return temp;
	}

	template <
		class T, size_t N
	> constexpr auto operator/(const array_t<T, N> & lhs, const T & rhs)
		-> array_t<T, N>
	{
		array_t<T, N> temp { lhs };
		temp /= rhs;
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t N
	> constexpr auto operator-(const array_t<T, N> & lhs)
		-> array_t<T, N>
	{
		return (lhs * type_t<T>::minus_one);
	}

	template <
		class T, size_t N
	> constexpr auto operator+(const array_t<T, N> & lhs)
		-> array_t<T, N>
	{
		return -(-(lhs));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ARRAY_HPP_