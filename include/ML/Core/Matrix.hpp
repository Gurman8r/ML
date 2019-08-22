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
		constexpr auto cols()		const	-> size_t			{ return self_type::Cols; }
		constexpr auto data()				-> pointer			{ return m_data.data(); }
		constexpr auto data()		const	-> const_pointer	{ return m_data.data(); }
		constexpr auto end()				-> iterator			{ return m_data.end(); }
		constexpr auto end()		const	-> const_iterator	{ return m_data.end(); }
		constexpr auto front()				-> reference		{ return m_data.front(); }
		constexpr auto front()		const	-> const_reference	{ return m_data.front(); }
		constexpr auto hash()		const	-> hash_t			{ return m_data.hash(); }
		constexpr auto height()		const	-> size_t			{ return rows(); }
		constexpr auto rows()		const	-> size_t			{ return self_type::Rows; }
		constexpr auto size()		const	-> size_t			{ return m_data.size(); }
		constexpr auto width()		const	-> size_t			{ return cols(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr reference operator[](size_t i) { return m_data[i]; }

		constexpr const_reference operator[](size_t i) const { return m_data[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator base_type &() { return m_data; }

		constexpr operator const base_type &() const { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t, size_t> class M, class U, size_t W, size_t H
		> constexpr operator M<U, W, H>() const
		{
			M<U, W, H> temp { uninit };
			for (size_t i = 0; i < temp.size(); i++)
			{
				const size_t x { i % temp.width() };
				const size_t y { i / temp.width() };
				const size_t w { this->width() };
				const size_t h { this->height() };

				using UU = numeric<U>;
				temp[i] = ((y < h && x < w) ? UU { (*this)[y * w + x] } : UU::zero);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr self_type zero()
		{
			return self_type { uninit };
		}

		static constexpr self_type one()
		{
			self_type temp { uninit };
			for (auto & elem : temp) 
			{
				elem = numeric<T>::one; 
			}
			return temp;
		}

		static constexpr self_type identity()
		{
			self_type temp { uninit };
			for (size_t i = 0; i < temp.size(); i++)
			{
				temp[i] = (((i / temp.width()) == (i % temp.width())) 
					? numeric<T>::one 
					: numeric<T>::zero
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
			temp[0 * 4 + 0] = numeric<T>::two / (right - left);
			temp[1 * 4 + 1] = numeric<T>::two / (top - bottom);
			temp[2 * 4 + 2] = numeric<T>::minus_one;
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
			temp[0 * 4 + 0] = numeric<T>::two / (right - left);
			temp[1 * 4 + 1] = numeric<T>::two / (top - bottom);
			temp[3 * 4 + 0] = -(right + left) / (right - left);
			temp[3 * 4 + 1] = -(top + bottom) / (top - bottom);
			temp[2 * 4 + 2] = -numeric<T>::two / (far - near);
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
			temp[0 * 4 + 0] = numeric<T>::one / (aspect * alg::tan(fov / numeric<T>::two));
			temp[1 * 4 + 1] = numeric<T>::one / alg::tan(fov / numeric<T>::two);
			temp[2 * 4 + 3] = numeric<T>::minus_one;
			temp[2 * 4 + 2] = -(far + near) / (far - near);
			temp[3 * 4 + 3] = -(numeric<T>::two * far * near) / (far - near);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// preventing dynamic allocation because matrices lack C/D-tors
		private:
			inline void * operator	new		 (size_t)	{ return nullptr; }
			inline void * operator	new[]	 (size_t)	{ return nullptr; }
			inline void	  operator	delete	 (void *)	{ return;  }
			inline void	  operator	delete[] (void *)	{ return;  }

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
			out << elem << " ";
		}
		return out;
	}

	template <
		class T, size_t X, size_t Y
	> inline ML_DESERIALIZE(Istream & in, Matrix<T, X, Y> & value)
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
		return (lhs * numeric<T>::minus_one);
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

namespace ml
{
	namespace alg
	{
		static inline mat4 look_at(const vec3 & eye, const vec3 & center, const vec3 & up)
		{
			vec3 f = alg::normalize(center - eye);
			vec3 s = alg::normalize(alg::cross(f, up));
			vec3 u = alg::cross(s, f);
			mat4 m = mat4::one();
			m[0 * 4 + 0] = s[0];
			m[1 * 4 + 0] = s[1];
			m[2 * 4 + 0] = s[2];
			m[0 * 4 + 1] = u[0];
			m[1 * 4 + 1] = u[1];
			m[2 * 4 + 1] = u[2];
			m[0 * 4 + 2] = -f[0];
			m[1 * 4 + 2] = -f[1];
			m[2 * 4 + 2] = -f[2];
			m[3 * 4 + 0] = -alg::dot(s, eye);
			m[3 * 4 + 1] = -alg::dot(u, eye);
			m[3 * 4 + 2] = alg::dot(f, eye);
			return m;
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace std
{
	template <
		class T, size_t X, size_t Y
	> struct hash<_ML Matrix<T, X, Y>>
	{
		using argument_type = _ML Matrix<T, X, Y>;

		inline _ML hash_t operator()(const argument_type & value) const noexcept
		{
			return _Hash_array_representation(value.data(), value.size());
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_MATRIX_HPP_