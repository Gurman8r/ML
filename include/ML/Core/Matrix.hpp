#ifndef _ML_MATRIX_HPP_
#define _ML_MATRIX_HPP_

#include <ML/Core/Array.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> struct Matrix
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < (X * Y), "Matrix : size negative or zero");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Cols = X, Rows = Y, Size = Cols * Rows };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Matrix<value_type, Cols, Rows>;
		using base_type			= typename Array<value_type, Size>;
		using cast_type			= typename static_value<value_type>;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		base_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto begin()			-> iterator			{ return m_data.begin(); }
		constexpr auto begin()	const	-> const_iterator	{ return m_data.begin(); }
		constexpr auto cbegin() const	-> const_iterator	{ return m_data.cbegin(); }
		constexpr auto cend()	const	-> const_iterator	{ return m_data.cend(); }
		constexpr auto cols()	const	-> size_t			{ return self_type::Cols; }
		constexpr auto data()			-> pointer			{ return m_data.data(); }
		constexpr auto data()	const	-> const_pointer	{ return m_data.data(); }
		constexpr auto end()			-> iterator			{ return m_data.end(); }
		constexpr auto end()	const	-> const_iterator	{ return m_data.end(); }
		constexpr auto hash()	const	-> hash_t			{ return m_data.hash(); }
		constexpr auto height()	const	-> size_t			{ return this->rows(); }
		constexpr auto rows()	const	-> size_t			{ return self_type::Rows; }
		constexpr auto size()	const	-> size_t			{ return m_data.size(); }
		constexpr auto width()	const	-> size_t			{ return this->cols(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr const_reference operator[](size_t i) const { return m_data[i]; }

		constexpr reference operator[](size_t i) { return m_data[i]; }

		constexpr operator base_type() const { return m_data; }

		template <
			template <class, size_t, size_t> class M, class U, size_t W, size_t H
		> constexpr operator M<U, W, H>() const
		{
			M<U, W, H> temp { uninit };
			for (size_t i = 0; i < temp.size(); i++)
			{
				const size_t x = i % temp.width();
				const size_t y = i / temp.width();
				temp[i] = ((y < height() && x < width())
					? static_value<U>{ (*this)[y * width() + x] }
					: static_value<U>::zero
				);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr self_type one()
		{
			self_type temp { uninit };
			for (auto & elem : temp)
			{
				elem = cast_type::one;
			}
			return temp;
		}

		static constexpr self_type identity()
		{
			self_type temp { uninit };
			for (size_t i = 0; i < temp.size(); i++)
			{
				temp[i] = (((i / temp.width()) == (i % temp.width()))
					? cast_type::one
					: cast_type::zero
				);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr self_type ortho(
			const_reference left,
			const_reference right,
			const_reference bottom,
			const_reference top)
		{
			static_assert((X == 4 && Y == 4), "matrix must be 4x4");
			self_type temp { self_type::identity() };
			temp[0 * 4 + 0] = cast_type::two / (right - left);
			temp[1 * 4 + 1] = cast_type::two / (top - bottom);
			temp[2 * 4 + 2] = cast_type::minus_one;
			temp[3 * 4 + 0] = -(right + left) / (right - left);
			temp[3 * 4 + 1] = -(top + bottom) / (top - bottom);
			return temp;
		}

		static constexpr self_type ortho(
			const_reference left,
			const_reference right,
			const_reference bottom,
			const_reference top,
			const_reference near,
			const_reference far)
		{
			static_assert((X == 4 && Y == 4), "matrix must be 4x4");
			self_type temp { self_type::identity() };
			temp[0 * 4 + 0] = cast_type::two / (right - left);
			temp[1 * 4 + 1] = cast_type::two / (top - bottom);
			temp[3 * 4 + 0] = -(right + left) / (right - left);
			temp[3 * 4 + 1] = -(top + bottom) / (top - bottom);
			temp[2 * 4 + 2] = -cast_type::two / (far - near);
			temp[3 * 4 + 2] = -(far + near) / (far - near);
			return temp;
		}

		static constexpr self_type persp(
			const_reference fov,
			const_reference aspect,
			const_reference near,
			const_reference far)
		{
			static_assert((X == 4 && Y == 4), "matrix must be 4x4");
			self_type temp { uninit };
			temp[0 * 4 + 0] = cast_type::one / (aspect * alg::tan(fov / cast_type::two));
			temp[1 * 4 + 1] = cast_type::one / alg::tan(fov / cast_type::two);
			temp[2 * 4 + 3] = cast_type::minus_one;
			temp[2 * 4 + 2] = -(far + near) / (far - near);
			temp[3 * 4 + 3] = -(cast_type::two * far * near) / (far - near);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_NO_NEW_ARRAYS
		private:
			inline void * operator	new		 (size_t size) { return nullptr; }
			inline void * operator	new[]	 (size_t size) { return nullptr; }
			inline void	  operator	delete	 (void * ptr)  { return;  }
			inline void	  operator	delete[] (void * ptr)  { return;  }
#endif // !ML_NO_NEW_ARRAYS

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
	> inline ML_SERIALIZE(Ostream & out, const Matrix<T, X, Y> & value)
	{
		for (const auto & elem : value)
		{
			out << elem << ' ';
		}
		return out;
	}

	template <
		class T, size_t X, size_t Y
	> inline ML_DESERIALIZE(Istream & in, Matrix<T, X, Y> & value)
	{
		for (auto & elem : value)
		{
			in >> elem;
		}
		return in;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator==(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return alg::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator!=(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return !(lhs == rhs);
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator<(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return alg::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator<=(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator>(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <
		class T, size_t X, size_t Y
	> constexpr bool operator>=(const Matrix<T, X, Y> & lhs, const Matrix<T, X, Y> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
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
		return (lhs * static_value<T>::minus_one);
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

#endif // !_ML_MATRIX_HPP_