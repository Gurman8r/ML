#ifndef _ML_MATRIX_HPP_
#define _ML_MATRIX_HPP_

#include <ML/Core/Array.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y> struct Matrix
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < (X * Y), "Matrix : size negative or zero");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Cols = X, Rows = Y, Size = Cols * Rows };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Matrix<value_type, Cols, Rows>;
		using base_type			= typename Array<value_type, Size>;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		base_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto at(size_t i)			-> reference		{ return m_data.at(i); }
		constexpr auto at(size_t i) const	-> const_reference	{ return m_data.at(i); }
		constexpr auto back()				-> reference		{ return m_data.back(); }
		constexpr auto back()		const	-> const_reference	{ return m_data.back(); }
		constexpr auto begin()				-> iterator			{ return m_data.begin(); }
		constexpr auto begin()		const	-> const_iterator	{ return m_data.begin(); }
		constexpr auto cbegin()		const	-> const_iterator	{ return m_data.cbegin(); }
		constexpr auto cend()		const	-> const_iterator	{ return m_data.cend(); }
		constexpr auto data()				-> pointer			{ return m_data.data(); }
		constexpr auto data()		const	-> const_pointer	{ return m_data.data(); }
		constexpr auto end()				-> iterator			{ return m_data.end(); }
		constexpr auto end()		const	-> const_iterator	{ return m_data.end(); }
		constexpr auto front()				-> reference		{ return m_data.front(); }
		constexpr auto front()		const	-> const_reference	{ return m_data.front(); }
		constexpr auto hash()		const	-> hash_t			{ return m_data.hash(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t cols()		{ return self_type::Cols; }
		static constexpr size_t height()	{ return rows(); }
		static constexpr size_t rows()		{ return self_type::Rows; }
		static constexpr size_t size()		{ return self_type::Size; }
		static constexpr size_t width()		{ return cols(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr reference operator[](size_t i) { return m_data[i]; }

		constexpr const_reference operator[](size_t i) const { return m_data[i]; }

		constexpr operator base_type &() { return m_data; }

		constexpr operator const base_type &() const { return m_data; }

		template <class U> constexpr operator std::array<U, Size>() const
		{ 
			return (std::array<U, Size>)(base_type)(*this);
		}

		template <
			template <class, size_t, size_t> class M, class U, size_t W, size_t H
		> constexpr operator M<U, W, H>() const
		{
			M<U, W, H> temp { NULL };
			for (size_t i = 0; i < temp.size(); i++)
			{
				const size_t x { i % temp.width() };
				const size_t y { i / temp.width() };
				const size_t w { this->width() };
				const size_t h { this->height() };
				using UU = constant_t<U>;
				temp[i] = ((y < h && x < w) ? UU {}((*this)[y * w + x]) : UU::zero);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr self_type zero()
		{
			return self_type { NULL };
		}

		static constexpr self_type one()
		{
			self_type temp { NULL };
			for (auto & elem : temp) 
			{
				elem = constant_t<T>::one; 
			}
			return temp;
		}

		static constexpr self_type identity()
		{
			self_type temp { NULL };
			for (size_t i = 0; i < temp.size(); i++)
			{
				temp[i] = (((i / temp.width()) == (i % temp.width())) 
					? constant_t<T>::one 
					: constant_t<T>::zero
				);
			}
			return temp;
		}

		inline self_type & update(const_pointer value)
		{
			for (size_t i = 0; i < Size; i++)
			{
				m_data[i] = value[i];
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// MATRIX NxN
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T, size_t N>
	using tmat_nxn = Matrix<T, N, N>;

	template <class T, size_t N>
	using tmat_nx1 = Matrix<T, N, 1>;

	// MATRIX 2x2
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T>
	using tmat2 = tmat_nxn<T, 2>;
	using mat2b = tmat2<uint8_t>;
	using mat2i = tmat2<int32_t>;
	using mat2u = tmat2<uint32_t>;
	using mat2f = tmat2<float32_t>;
	using mat2d = tmat2<float64_t>;
	using mat2  = mat2f;

	// MATRIX 3x3
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T>
	using tmat3 = tmat_nxn<T, 3>;
	using mat3b = tmat3<uint8_t>;
	using mat3i = tmat3<int32_t>;
	using mat3u = tmat3<uint32_t>;
	using mat3f = tmat3<float32_t>;
	using mat3d = tmat3<float64_t>;
	using mat3	= mat3f;

	// MATRIX 4x4
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T>
	using tmat4 = tmat_nxn<T, 4>;
	using mat4b = tmat4<uint8_t>;
	using mat4i = tmat4<int32_t>;
	using mat4u = tmat4<uint32_t>;
	using mat4f = tmat4<float32_t>;
	using mat4d = tmat4<float64_t>;
	using mat4	= mat4f;

	// VECTOR 2
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T>
	using tvec2 = tmat_nx1<T, 2>;
	using vec2b = tvec2<uint8_t>;
	using vec2i = tvec2<int32_t>;
	using vec2u = tvec2<uint32_t>;
	using vec2f = tvec2<float32_t>;
	using vec2d = tvec2<float64_t>;
	using vec2	= vec2f;

	// VECTOR 3
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T>
	using tvec3 = tmat_nx1<T, 3>;
	using vec3b = tvec3<uint8_t>;
	using vec3i = tvec3<int32_t>;
	using vec3u = tvec3<uint32_t>;
	using vec3f = tvec3<float32_t>;
	using vec3d = tvec3<float64_t>;
	using vec3	= vec3f;

	// VECTOR 4
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T>
	using tvec4 = tmat_nx1<T, 4>;
	using vec4b = tvec4<uint8_t>;
	using vec4i = tvec4<int32_t>;
	using vec4u = tvec4<uint32_t>;
	using vec4f = tvec4<float32_t>;
	using vec4d = tvec4<float64_t>;
	using vec4	= vec4f;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> inline ML_SERIALIZE(std::ostream & out, const Matrix<T, X, Y> & value)
	{
		for (const auto & elem : value)
		{
			out << elem << " ";
		}
		return out;
	}

	template <
		class T, size_t X, size_t Y
	> inline ML_DESERIALIZE(std::istream & in, Matrix<T, X, Y> & value)
	{
		for (auto & elem : value)
		{
			if (in.good()) { in >> elem; }
		}
		return in;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator==(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return (lhs.m_data == rhs.m_data);
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator!=(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return (lhs.m_data != rhs.m_data);
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator<(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return (lhs.m_data < rhs.m_data);
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator<=(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return (lhs.m_data <= rhs.m_data);
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator>(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return (lhs.m_data > rhs.m_data);
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator>=(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return (lhs.m_data >= rhs.m_data);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator+=(Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
		-> Matrix<T, X, Y> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] + rhs[i]);
		}
		return lhs;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator-=(Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
		-> Matrix<T, X, Y> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] - rhs[i]);
		}
		return lhs;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator*=(Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
		-> Matrix<T, X, Y> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] * rhs[i]);
		}
		return lhs;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator/=(Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
		-> Matrix<T, X, Y> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] / rhs[i]);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator+(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
		-> Matrix<T, X, Y>
	{
		Matrix<T, X, Y> temp { lhs };
		temp += rhs;
		return temp;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator-(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
		-> Matrix<T, X, Y>
	{
		Matrix<T, X, Y> temp { lhs };
		temp -= rhs;
		return temp;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator*(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
		-> Matrix<T, X, Y>
	{
		Matrix<T, X, Y> temp { lhs };
		temp *= rhs;
		return temp;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator/(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
		-> Matrix<T, X, Y>
	{
		Matrix<T, X, Y> temp { lhs };
		temp /= rhs;
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator+=(Matrix<T, X, Y> & lhs, const T & rhs)
		-> Matrix<T, X, Y> &
	{
		for (auto & elem : lhs)
		{
			elem += rhs;
		}
		return lhs;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator-=(Matrix<T, X, Y> & lhs, const T & rhs)
		-> Matrix<T, X, Y> &
	{
		for (auto & elem : lhs)
		{
			elem -= rhs;
		}
		return lhs;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator*=(Matrix<T, X, Y> & lhs, const T & rhs)
		-> Matrix<T, X, Y> &
	{
		for (auto & elem : lhs)
		{
			elem *= rhs;
		}
		return lhs;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator/=(Matrix<T, X, Y> & lhs, const T & rhs)
		-> Matrix<T, X, Y> &
	{
		for (auto & elem : lhs)
		{
			elem /= rhs;
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator+(const Matrix<T, X, Y> & lhs, const T & rhs)
		-> Matrix<T, X, Y>
	{
		Matrix<T, X, Y> temp { lhs };
		temp += rhs;
		return temp;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator-(const Matrix<T, X, Y> & lhs, const T & rhs)
		-> Matrix<T, X, Y>
	{
		Matrix<T, X, Y> temp { lhs };
		temp -= rhs;
		return temp;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator*(const Matrix<T, X, Y> & lhs, const T & rhs)
		-> Matrix<T, X, Y>
	{
		Matrix<T, X, Y> temp { lhs };
		temp *= rhs;
		return temp;
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator/(const Matrix<T, X, Y> & lhs, const T & rhs)
		-> Matrix<T, X, Y>
	{
		Matrix<T, X, Y> temp { lhs };
		temp /= rhs;
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator-(const Matrix<T, X, Y> & lhs)
		-> Matrix<T, X, Y>
	{
		return (lhs * constant_t<T>::minus_one);
	}

	template <
		class T, size_t X, size_t Y
	> constexpr auto operator+(const Matrix<T, X, Y> & lhs)
		-> Matrix<T, X, Y>
	{
		return -(-(lhs));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace std
{
	template <
		class T, size_t X, size_t Y
	> struct hash<::ml::Matrix<T, X, Y>>
	{
		using argument_type = ::ml::Matrix<T, X, Y>;

		inline ::ml::hash_t operator()(const argument_type & value) const noexcept
		{
			return _Hash_array_representation(value.data(), value.size());
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_MATRIX_HPP_