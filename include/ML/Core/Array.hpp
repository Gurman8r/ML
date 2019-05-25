#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

// Source:
// https://stackoverflow.com/questions/8622256/in-c11-is-sqrt-defined-as-constexpr

#include <ML/Core/Algorithm.hpp>
#include <ML/Core/Hash.hpp>
#include <ML/Core/Maths.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	struct matrix_t final
	{
		/* * * * * * * * * * * * * * * * * * * * */

		static_assert((X * Y) > 0, "size negative or zero");

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
		constexpr	auto data()	const	-> const_pointer	{ return begin(); }
		constexpr	auto hash()	const	-> hash_type		{ return hash::fnv1a<T>(size(), data()); }
		constexpr	auto rows() const	-> size_type		{ return Rows; }
		constexpr	auto size()	const	-> size_type		{ return Size; }

		constexpr	auto begin()		-> pointer			{ return m_data; }
		constexpr	auto begin() const	-> const_pointer	{ return m_data; }
		constexpr	auto cbegin() const	-> const_reference	{ return begin(); }
		
		constexpr	auto end()			-> pointer			{ return m_data + size(); }
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

		/* * * * * * * * * * * * * * * * * * * * */

		inline friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
		{
			for (const auto & e : rhs)
			{
				out << e << " ";
			}
			return out;
		}

		inline friend std::istream & operator>>(std::istream & in, self_type & rhs)
		{
			for (auto & e : rhs)
			{
				in >> e;
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
	constexpr auto operator-(const matrix_t<T, X, Y> & lhs)
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <typename T>
		struct sqrt_impl;

		template <> struct sqrt_impl<double>
		{
			constexpr sqrt_impl(double x)
				: m_value { x >= 0 && x < std::numeric_limits<double>::infinity()
				? sqrt_impl(x, x, 0)()
				: std::numeric_limits<double>::quiet_NaN() }
			{
			}

			constexpr sqrt_impl(double x, double curr, double prev)
				: m_value { curr == prev
				? curr
				: sqrt_impl(x, 0.5 * (curr + x / curr), curr)() }
			{
			}

			constexpr double operator()() const { return m_value; }

		private:
			double m_value;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <> struct sqrt_impl<size_t>
		{
			constexpr sqrt_impl(size_t x)
				: m_value { sqrt_impl(1, 3, x)()
			}
			{
			}

			constexpr sqrt_impl(size_t x, size_t curr, size_t prev)
				: m_value { (x <= prev)
				? sqrt_impl(x + curr, curr + 2, prev)()
				: ((curr >> 1) - 1)
			}
			{
			}

			constexpr size_t operator()() const { return m_value; }

		private:
			size_t m_value;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <typename T>
		struct sqrt
		{
			constexpr T operator()(const T & value) const
			{
				return sqrt_impl<T>(value)();
			}
		};

		template <> struct sqrt<double>
		{
			constexpr double operator()(const double & value) const
			{
				return sqrt_impl<double>(value)();
			}
		};

		template <> struct sqrt<size_t>
		{
			constexpr size_t operator()(const size_t & value) const
			{
				return sqrt_impl<size_t>(value)();
			}
		};

		template <> struct sqrt<float>
		{
			constexpr double operator()(const float & value) const
			{
				return static_cast<float>(sqrt_impl<double>(static_cast<double>(value))());
			}
		};

		/* * * * * * * * * * * * * * * * * * * * */
	}

	namespace algorithm
	{
		template <
			typename Value,
			typename Coeff
		> constexpr Value lerp(Value a, Value b, Coeff c)
		{
			return (a * c + b * (1 - c));
		}

		template <typename T, size_t X, size_t Y>
		constexpr T sqrMagnitude(const matrix_t<T, X, Y> & value)
		{
			T out { 0 };
			for (const auto & e : value)
				out += e * e;
			return out;
		}

		template <typename T, size_t X, size_t Y>
		constexpr T magnitude(const matrix_t<T, X, Y> & value)
		{
			return static_cast<T>(detail::sqrt<T>()(sqrMagnitude(value)));
		}

		template <typename T, size_t X, size_t Y>
		constexpr auto normalize(const matrix_t<T, X, Y> & value)
		{
			return (value / magnitude(value));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ARRAY_HPP_