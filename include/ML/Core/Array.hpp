#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <ML/Core/Algorithm.hpp>
#include <ML/Core/Hash.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N>
	struct array_t final
	{
		/* * * * * * * * * * * * * * * * * * * * */

		static_assert(N > 0, "size negative or zero");

		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using size_type			= typename size_t;
		using hash_type			= typename hash_t;
		using data_type			= typename value_type[N];
		using self_type			= typename array_t<value_type, N>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr	auto data()	const	-> const_pointer	{ return m_data; }
		constexpr	auto hash()	const	-> hash_type		{ return hash::fnv1a<T>(N, m_data); }
		constexpr	auto size()	const	-> size_type		{ return N; }
		inline		auto begin()		-> reference		{ return m_data; }
		constexpr	auto begin() const	-> const_pointer	{ return m_data; }
		constexpr	auto cbegin() const	-> const_reference	{ return begin(); }
		inline		auto end()			-> reference		{ return m_data + size(); }
		constexpr	auto end() const	-> const_pointer	{ return m_data + size(); }
		constexpr	auto cend()	const	-> const_reference	{ return end(); }

		/* * * * * * * * * * * * * * * * * * * * */

		inline reference operator[](size_type i)
		{
			return m_data[i];
		}

		constexpr const_reference operator[](size_type i) const
		{
			return m_data[i];
		}

		inline friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
		{
			for (size_type i = 0; i < N; i++)
			{
				out << rhs[i] << " ";
			}
			return out;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		data_type m_data;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N>
	constexpr bool operator==(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return algorithm::equal_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, size_t N>
	constexpr bool operator!=(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return !(lhs == rhs);
	}

	template <class T, size_t N>
	constexpr bool operator<(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return algorithm::less_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, size_t N>
	constexpr bool operator<=(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <class T, size_t N>
	constexpr bool operator>(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <class T, size_t N>
	constexpr bool operator>=(const array_t<T, N> & lhs, const array_t<T, N> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	struct matrix_t
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum { Cols = X, Rows = Y, Size = X * Y };

		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using size_type			= typename size_t;
		using hash_type			= typename hash_t;
		using data_type			= typename array_t<T, Size>;
		using self_type			= typename matrix_t<T, Cols, Rows>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr	auto data()	const	-> const_pointer	{ return m_data.data(); }
		constexpr	auto hash()	const	-> hash_type		{ return m_data.hash(); }
		constexpr	auto size()	const	-> size_type		{ return m_data.size(); }
		inline		auto begin()		-> reference		{ return m_data.begin(); }
		constexpr	auto begin() const	-> const_pointer	{ return m_data.begin(); }
		constexpr	auto cbegin() const	-> const_reference	{ return m_data.cbegin(); }
		inline		auto end()			-> reference		{ return m_data.end(); }
		constexpr	auto end() const	-> const_pointer	{ return m_data.end(); }
		constexpr	auto cend()	const	-> const_reference	{ return m_data.cend(); }

		/* * * * * * * * * * * * * * * * * * * * */

		inline reference operator[](size_type i)
		{
			return m_data[i];
		}

		constexpr const_reference operator[](size_type i) const
		{
			return m_data[i];
		}

		inline friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
		{
			return out << rhs.m_data;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr matrix_t()
			: m_data { 0 }
		{
		}

		constexpr matrix_t(const data_type & value)
			: m_data { value }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		data_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y>
	constexpr bool operator==(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return (lhs.data() == rhs.data());
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator!=(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return (lhs.data() != rhs.data());
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator<(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return (lhs.data() < rhs.data());
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator<=(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return (lhs.data() <= rhs.data());
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator>(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return (lhs.data() > rhs.data());
	}

	template <class T, size_t X, size_t Y>
	constexpr bool operator>=(const matrix_t<T, X, Y> & lhs, const matrix_t<T, X, Y> & rhs)
	{
		return (lhs.data() >= rhs.data());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N>
	using matrix_n = matrix_t<T, N, N>;

	template <class T>
	using matrix_3 = matrix_n<T, 3>;
	using matrix_3f = matrix_3<float>;

	template <class T>
	using matrix_4 = matrix_n<T, 4>;
	using matrix_4f = matrix_4<float>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N>
	struct vector_t
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using size_type			= typename size_t;
		using hash_type			= typename hash_t;
		using data_type			= typename array_t<T, N>;
		using self_type			= typename vector_t<value_type, N>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr	auto data()	const	-> const_pointer	{ return m_data.data(); }
		constexpr	auto hash()	const	-> hash_type		{ return m_data.hash(); }
		constexpr	auto size()	const	-> size_type		{ return m_data.size(); }
		inline		auto begin()		-> reference		{ return m_data.begin(); }
		constexpr	auto begin() const	-> const_pointer	{ return m_data.begin(); }
		constexpr	auto cbegin() const	-> const_reference	{ return m_data.cbegin(); }
		inline		auto end()			-> reference		{ return m_data.end(); }
		constexpr	auto end() const	-> const_pointer	{ return m_data.end(); }
		constexpr	auto cend()	const	-> const_reference	{ return m_data.cend(); }

		/* * * * * * * * * * * * * * * * * * * * */

		inline reference operator[](size_type i)
		{
			return m_data[i];
		}

		constexpr const_reference operator[](size_type i) const
		{
			return m_data[i];
		}

		inline friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
		{
			return out << rhs.m_data;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr vector_t()
			: m_data { 0 }
		{
		}

		constexpr vector_t(const data_type & value)
			: m_data { value }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		data_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N>
	constexpr bool operator==(const vector_t<T, N> & lhs, const vector_t<T, N> & rhs)
	{
		return (lhs.data() == rhs.data());
	}

	template <class T, size_t N>
	constexpr bool operator!=(const vector_t<T, N> & lhs, const vector_t<T, N> & rhs)
	{
		return (lhs.data() != rhs.data());
	}

	template <class T, size_t N>
	constexpr bool operator<(const vector_t<T, N> & lhs, const vector_t<T, N> & rhs)
	{
		return (lhs.data() < rhs.data());
	}

	template <class T, size_t N>
	constexpr bool operator<=(const vector_t<T, N> & lhs, const vector_t<T, N> & rhs)
	{
		return (lhs.data() <= rhs.data());
	}

	template <class T, size_t N>
	constexpr bool operator>(const vector_t<T, N> & lhs, const vector_t<T, N> & rhs)
	{
		return (lhs.data() > rhs.data());
	}

	template <class T, size_t N>
	constexpr bool operator>=(const vector_t<T, N> & lhs, const vector_t<T, N> & rhs)
	{
		return (lhs.data() >= rhs.data());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	//template <class T>
	//using vector_2 = vector_t<T, 2>;
	//using vector_2f = vector_2<float>;
	//using vector_2i = vector_2<int32_t>;
	//using vector_2d = vector_2<double>;
	//using vector_2b = vector_2<uint8_t>;
	//using vector_2u = vector_2<uint32_t>;

	template <class T>
	using vector_3 = vector_t<T, 3>;
	using vector_3f = vector_3<float>;
	using vector_3i = vector_3<int32_t>;
	using vector_3d = vector_3<double>;
	using vector_3b = vector_3<uint8_t>;
	using vector_3u = vector_3<uint32_t>;
	
	template <class T>
	using vector_4 = vector_t<T, 4>;
	using vector_4f = vector_4<float>;
	using vector_4i = vector_4<int32_t>;
	using vector_4d = vector_4<double>;
	using vector_4b = vector_4<uint8_t>;
	using vector_4u = vector_4<uint32_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T>
	struct vector_2
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum { N = 2 };

		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using size_type			= typename size_t;
		using hash_type			= typename hash_t;
		using data_type			= typename array_t<T, N>;
		using self_type			= typename vector_2<value_type>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr	auto data()	const	-> const_pointer	{ return m_data.data(); }
		constexpr	auto hash()	const	-> hash_type		{ return m_data.hash(); }
		constexpr	auto size()	const	-> size_type		{ return m_data.size(); }
		inline		auto begin()		-> reference		{ return m_data.begin(); }
		constexpr	auto begin() const	-> const_pointer	{ return m_data.begin(); }
		constexpr	auto cbegin() const	-> const_reference	{ return m_data.cbegin(); }
		inline		auto end()			-> reference		{ return m_data.end(); }
		constexpr	auto end() const	-> const_pointer	{ return m_data.end(); }
		constexpr	auto cend()	const	-> const_reference	{ return m_data.cend(); }

		/* * * * * * * * * * * * * * * * * * * * */

		inline reference operator[](size_type i)
		{
			return m_data[i];
		}

		constexpr const_reference operator[](size_type i) const
		{
			return m_data[i];
		}

		inline friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
		{
			return out << rhs.m_data;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr vector_2()
			: vector_2 { data_type { 0, 0 } }
		{
		}

		constexpr vector_2(const_reference x, const_reference y)
			: vector_2 { data_type { x, y } }
		{
		}

		constexpr vector_2(const data_type & value)
			: m_data { value }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		data_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T>
	constexpr bool operator==(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs.data() == rhs.data());
	}

	template <class T>
	constexpr bool operator!=(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs.data() != rhs.data());
	}

	template <class T>
	constexpr bool operator<(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs.data() < rhs.data());
	}

	template <class T>
	constexpr bool operator<=(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs.data() <= rhs.data());
	}

	template <class T>
	constexpr bool operator>(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs.data() > rhs.data());
	}

	template <class T>
	constexpr bool operator>=(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs.data() >= rhs.data());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T>
	constexpr vector_2<T> operator+(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return vector_2<T>({
			lhs[0] + rhs[0],
			lhs[1] + rhs[1],
		});
	}

	template <class T>
	constexpr vector_2<T> operator-(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return vector_2<T>({
			lhs[0] - rhs[0],
			lhs[1] - rhs[1],
		});
	}

	template <class T>
	constexpr vector_2<T> operator*(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return vector_2<T>({
			lhs[0] * rhs[0],
			lhs[1] * rhs[1],
		});
	}

	template <class T>
	constexpr vector_2<T> operator/(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return vector_2<T>({
			lhs[0] / rhs[0],
			lhs[1] / rhs[1],
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T>
	constexpr vector_2<T> operator+(const vector_2<T> & lhs, const T & rhs)
	{
		return vector_2<T>({
			lhs[0] + rhs,
			lhs[1] + rhs,
		});
	}

	template <class T>
	constexpr vector_2<T> operator-(const vector_2<T> & lhs, const T & rhs)
	{
		return vector_2<T>({
			lhs[0] - rhs,
			lhs[1] - rhs,
		});
	}

	template <class T>
	constexpr vector_2<T> operator*(const vector_2<T> & lhs, const T & rhs)
	{
		return {
			lhs[0] * rhs,
			lhs[1] * rhs,
		};
	}

	template <class T>
	constexpr vector_2<T> operator/(const vector_2<T> & lhs, const T & rhs)
	{
		return vector_2<T>({
			lhs[0] / rhs,
			lhs[1] / rhs,
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T>
	inline vector_2<T> & operator+=(vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs = lhs + rhs);
	}

	template <class T>
	inline vector_2<T> & operator-(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs = lhs * rhs);
	}

	template <class T>
	inline vector_2<T> & operator*(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs = lhs * rhs);
	}

	template <class T>
	inline vector_2<T> & operator/(const vector_2<T> & lhs, const vector_2<T> & rhs)
	{
		return (lhs = lhs / rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T>
	inline vector_2<T> & operator+=(vector_2<T> & lhs, const T & rhs)
	{
		return (lhs = lhs + rhs);
	}

	template <class T>
	inline vector_2<T> & operator-(const vector_2<T> & lhs, const T & rhs)
	{
		return (lhs = lhs * rhs);
	}

	template <class T>
	inline vector_2<T> & operator*(const vector_2<T> & lhs, const T & rhs)
	{
		return (lhs = lhs * rhs);
	}

	template <class T>
	inline vector_2<T> & operator/(const vector_2<T> & lhs, const T & rhs)
	{
		return (lhs = lhs / rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T>
	constexpr vector_2<T> operator-(const vector_2<T> & lhs)
	{
		return lhs * static_cast<T>(-1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_ARRAY_HPP_