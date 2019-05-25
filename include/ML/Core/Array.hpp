#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <ML/Core/Algorithm.hpp>
#include <ML/Core/Hash.hpp>

//namespace ml
//{
//	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
//	template <class T, size_t N>
//	struct array_t final
//	{
//		/* * * * * * * * * * * * * * * * * * * * */
//
//		static_assert(N > 0, "size negative or zero");
//
//		/* * * * * * * * * * * * * * * * * * * * */
//
//		using value_type		= typename T;
//		using data_type			= typename value_type[N];
//		using self_type			= typename array_t<value_type, N>;
//		using pointer			= typename value_type *;
//		using reference			= typename value_type &;
//		using const_pointer		= typename const value_type *;
//		using const_reference	= typename const value_type &;
//
//		/* * * * * * * * * * * * * * * * * * * * */
//
//		constexpr	auto data()	const	-> const_pointer	{ return m_data; }
//		constexpr	auto hash()	const	-> hash_t			{ return hash::fnv1a<T>(N, m_data); }
//		constexpr	auto size()	const	-> size_t			{ return N; }
//		inline		auto begin()		-> reference		{ return m_data; }
//		constexpr	auto begin() const	-> const_pointer	{ return m_data; }
//		constexpr	auto cbegin() const	-> const_reference	{ return begin(); }
//		inline		auto end()			-> reference		{ return m_data + size(); }
//		constexpr	auto end() const	-> const_pointer	{ return m_data + size(); }
//		constexpr	auto cend()	const	-> const_reference	{ return end(); }
//
//		/* * * * * * * * * * * * * * * * * * * * */
//
//		constexpr const_reference operator[](size_t i) const
//		{
//			return m_data[i];
//		}
//
//		constexpr reference operator[](size_t i)
//		{
//			return m_data[i];
//		}
//
//		template <class Index, typename U>
//		constexpr void set(Index i, U && value)
//		{
//			m_data[static_cast<size_t>(i)] = std::forward<U>(value);
//		}
//
//		/* * * * * * * * * * * * * * * * * * * * */
//
//		inline friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
//		{
//			for (size_t i = 0; i < rhs.size(); i++)
//			{
//				out << rhs[i] << " ";
//			}
//			return out;
//		}
//
//		inline friend std::istream & operator>>(std::istream & in, self_type & rhs)
//		{
//			for (size_t i = 0; i < rhs.size(); i++)
//			{
//				in >> rhs[i];
//			}
//			return in;
//		}
//
//		/* * * * * * * * * * * * * * * * * * * * */
//
//		data_type m_data;
//
//		/* * * * * * * * * * * * * * * * * * * * */
//	};
//
//	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//
//	template <class T, size_t N>
//	constexpr bool operator==(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
//	{
//		return algorithm::equal_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
//	}
//
//	template <class T, size_t N>
//	constexpr bool operator!=(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
//	{
//		return !(lhs == rhs);
//	}
//
//	template <class T, size_t N>
//	constexpr bool operator<(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
//	{
//		return algorithm::less_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
//	}
//
//	template <class T, size_t N>
//	constexpr bool operator<=(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
//	{
//		return (lhs < rhs) || (lhs == rhs);
//	}
//
//	template <class T, size_t N>
//	constexpr bool operator>(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
//	{
//		return !(lhs < rhs) && (lhs != rhs);
//	}
//
//	template <class T, size_t N>
//	constexpr bool operator>=(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
//	{
//		return (lhs > rhs) || (lhs == rhs);
//	}
//
//	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	struct matrix_t final
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum { Cols = X, Rows = Y, Size = X * Y };

		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using size_type			= typename size_t;
		using hash_type			= typename hash_t;
		using data_type			= typename value_type[Size];
		using self_type			= typename matrix_t<value_type, Cols, Rows>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr	auto cols() const	-> size_type		{ return Cols; }
		constexpr	auto data()	const	-> const_pointer	{ return m_data; }
		constexpr	auto hash()	const	-> hash_type		{ return hash::fnv1a<T>(Size, m_data); }
		constexpr	auto rows() const	-> size_type		{ return Rows; }
		constexpr	auto size()	const	-> size_type		{ return Size; }

		inline		auto begin()		-> reference		{ return m_data; }
		constexpr	auto begin() const	-> const_pointer	{ return m_data; }
		constexpr	auto cbegin() const	-> const_reference	{ return begin(); }
		
		inline		auto end()			-> reference		{ return m_data + size(); }
		constexpr	auto end() const	-> const_pointer	{ return m_data + size(); }
		constexpr	auto cend()	const	-> const_reference	{ return end(); }

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr const_reference operator[](size_t i) const
		{
			return m_data[i];
		}

		constexpr reference operator[](size_t i)
		{
			return m_data[i];
		}

		template <class Index, typename U>
		constexpr void set(Index i, U && value)
		{
			m_data[static_cast<size_t>(i)] = std::forward<U>(value);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
		{
			for (size_t i = 0; i < rhs.size(); i++)
			{
				out << rhs[i] << " ";
			}
			return out;
		}

		inline friend std::istream & operator>>(std::istream & in, self_type & rhs)
		{
			for (size_t i = 0; i < rhs.size(); i++)
			{
				in >> rhs[i];
			}
			return in;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		static constexpr self_type identity()
		{
			self_type temp { 0 };
			for (size_t i = 0; i < temp.size(); i++)
			{
				temp[i] = (((i / temp.cols()) == (i % temp.cols()))
					? static_cast<T>(1)
					: static_cast<T>(0));
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		data_type m_data;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	constexpr bool operator==(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return algorithm::equal_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator!=(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return !(lhs == rhs);
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator<(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return algorithm::less_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator<=(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator>(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator>=(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	constexpr auto & operator+=(matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		for (size_t i = 0; i < (X * Y); i++)
		{
			lhs[i] = (lhs[i] + rhs[i]);
		}
		return lhs;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto & operator-=(matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		for (size_t i = 0; i < (X * Y); i++)
		{
			lhs[i] = (lhs[i] - rhs[i]);
		}
		return lhs;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto & operator*=(matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		for (size_t i = 0; i < (X * Y); i++)
		{
			lhs[i] = (lhs[i] * rhs[i]);
		}
		return lhs;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto & operator/=(matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		for (size_t i = 0; i < (X * Y); i++)
		{
			lhs[i] = (lhs[i] / rhs[i]);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	constexpr auto & operator+=(matrix_t<T, X, Y> & lhs, const T & rhs)
	{
		for (size_t i = 0; i < (X * Y); i++)
		{
			lhs[i] = (lhs[i] + rhs);
		}
		return lhs;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto & operator-=(matrix_t<T, X, Y> & lhs, const T & rhs)
	{
		for (size_t i = 0; i < (X * Y); i++)
		{
			lhs[i] = (lhs[i] - rhs);
		}
		return lhs;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto & operator*=(matrix_t<T, X, Y> & lhs, const T & rhs)
	{
		for (size_t i = 0; i < (X * Y); i++)
		{
			lhs[i] = (lhs[i] * rhs);
		}
		return lhs;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto & operator/=(matrix_t<T, X, Y> & lhs, const T & rhs)
	{
		for (size_t i = 0; i < (X * Y); i++)
		{
			lhs[i] = (lhs[i] / rhs);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	constexpr auto operator+(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		matrix_t<T, X, Y> temp { lhs };
		temp += rhs;
		return temp;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto operator-(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		matrix_t<T, X, Y> temp { lhs };
		temp -= rhs;
		return temp;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto operator*(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		matrix_t<T, X, Y> temp { lhs };
		temp *= rhs;
		return temp;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto operator/(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		matrix_t<T, X, Y> temp { lhs };
		temp /= rhs;
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	constexpr auto operator+(const matrix_t<T, X, Y> & lhs, const T & rhs)
	{
		matrix_t<T, X, Y> temp { lhs };
		temp += rhs;
		return temp;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto operator-(const matrix_t<T, X, Y> & lhs, const T & rhs)
	{
		matrix_t<T, X, Y> temp { lhs };
		temp -= rhs;
		return temp;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto operator*(const matrix_t<T, X, Y> & lhs, const T & rhs)
	{
		matrix_t<T, X, Y> temp { lhs };
		temp *= rhs;
		return temp;
	}

	template <class T, size_t X, size_t Y>
	constexpr auto operator/(const matrix_t<T, X, Y> & lhs, const T & rhs)
	{
		matrix_t<T, X, Y> temp { lhs };
		temp /= rhs;
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	constexpr matrix_t<T, X, Y> operator-(const matrix_t<T, X, Y> & lhs)
	{
		return lhs * static_cast<T>(-1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N>
	using matrix_n = matrix_t<T, N, N>;

	template <class T>
	using matrix_3	= matrix_n<T, 3>;
	using matrix_3b = matrix_3<bool>;
	using matrix_3f = matrix_3<float>;

	template <class T>
	using matrix_4	= matrix_n<T, 4>;
	using matrix_4b = matrix_4<bool>;
	using matrix_4f = matrix_4<float>;

	template <class T, size_t N>
	using vector_t	= matrix_t<T, N, 1>;

	template <class T>
	using vector_2	= vector_t<T, 2>;
	using vector_2b = vector_2<bool>;
	using vector_2f = vector_2<float>;
	using vector_2i = vector_2<int32_t>;
	using vector_2d = vector_2<double>;
	using vector_2u = vector_2<uint32_t>;

	template <class T>
	using vector_3	= vector_t<T, 3>;
	using vector_3b = vector_3<bool>;
	using vector_3f = vector_3<float>;
	using vector_3i = vector_3<int32_t>;
	using vector_3d = vector_3<double>;
	using vector_3u = vector_3<uint32_t>;

	template <class T>
	using vector_4	= vector_t<T, 4>;
	using vector_4b = vector_4<bool>;
	using vector_4f = vector_4<float>;
	using vector_4i = vector_4<int32_t>;
	using vector_4d = vector_4<double>;
	using vector_4u = vector_4<uint32_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ARRAY_HPP_