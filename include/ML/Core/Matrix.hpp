#ifndef _ML_MATRIX_HPP_
#define _ML_MATRIX_HPP_

#include <ML/Core/Array.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// fixed size matrix
	template <class T, size_t X, size_t Y> struct Matrix
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < (X * Y), "size negative or zero");
		
		static_assert(std::is_trivial_v<T>, "type must be trivial");

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

		constexpr reference operator[](size_t i)
		{
			return m_data[i];
		}

		constexpr const_reference operator[](size_t i) const
		{ 
			return m_data[i];
		}

		constexpr operator base_type &()
		{
			return m_data;
		}

		constexpr operator base_type const &() const 
		{
			return m_data; 
		}

		template <class U> constexpr operator std::array<U, Size>() const
		{ 
			return (std::array<U, Size>)(base_type)(*this);
		}

		template <
			template <class, size_t, size_t> class M, class U, size_t W, size_t H
		> constexpr operator M<U, W, H>() const
		{
			M<U, W, H> temp { 0 };
			for (size_t i = 0; i < temp.size(); i++)
			{
				const size_t x { i % temp.width() };
				const size_t y { i / temp.width() };
				temp[i] = ((y < Rows && x < Cols)
					? static_cast<U>((*this)[y * Cols + x]) 
					: static_cast<U>(0)
				);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr self_type zero()
		{
			return self_type { 0 };
		}

		static constexpr self_type one()
		{
			self_type temp { 0 };
			for (auto & elem : temp)  { elem = static_cast<value_type>(1); }
			return temp;
		}

		static constexpr self_type fill(value_type value)
		{
			self_type temp { 0 };
			for (auto & elem : temp) { temp = value; }
			return temp;
		}

		static constexpr self_type identity()
		{
			self_type temp { 0 };
			for (size_t i = 0; i < temp.size(); i++)
			{
				temp[i] = (((i / temp.width()) == (i % temp.width())) 
					? static_cast<value_type>(1) 
					: static_cast<value_type>(0)
				);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// MATRIX NxN
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_T_(class T, size_t N) tmatn = Matrix<T, N, N>;
	ML_USING_T_(class T, size_t N) tvecn = Matrix<T, N, 1>;


	// MATRIX 2x2
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tmat2 = tmatn<X, 2>;
	ML_USING	mat2b = tmat2<byte_t>;
	ML_USING	mat2i = tmat2<int32_t>;
	ML_USING	mat2u = tmat2<uint32_t>;
	ML_USING	mat2f = tmat2<float32_t>;
	ML_USING	mat2d = tmat2<float64_t>;
	ML_USING	mat2 = mat2f;


	// MATRIX 3x3
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tmat3 = tmatn<X, 3>;
	ML_USING	mat3b = tmat3<byte_t>;
	ML_USING	mat3i = tmat3<int32_t>;
	ML_USING	mat3u = tmat3<uint32_t>;
	ML_USING	mat3f = tmat3<float32_t>;
	ML_USING	mat3d = tmat3<float64_t>;
	ML_USING	mat3 = mat3f;


	// MATRIX 4x4
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tmat4 = tmatn<X, 4>;
	ML_USING	mat4b = tmat4<byte_t>;
	ML_USING	mat4i = tmat4<int32_t>;
	ML_USING	mat4u = tmat4<uint32_t>;
	ML_USING	mat4f = tmat4<float32_t>;
	ML_USING	mat4d = tmat4<float64_t>;
	ML_USING	mat4 = mat4f;


	// VECTOR 2
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tvec2 = tvecn<X, 2>;
	ML_USING	vec2b = tvec2<byte_t>;
	ML_USING	vec2i = tvec2<int32_t>;
	ML_USING	vec2u = tvec2<uint32_t>;
	ML_USING	vec2f = tvec2<float32_t>;
	ML_USING	vec2d = tvec2<float64_t>;
	ML_USING	vec2 = vec2f;


	// VECTOR 3
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tvec3 = tvecn<X, 3>;
	ML_USING	vec3b = tvec3<byte_t>;
	ML_USING	vec3i = tvec3<int32_t>;
	ML_USING	vec3u = tvec3<uint32_t>;
	ML_USING	vec3f = tvec3<float32_t>;
	ML_USING	vec3d = tvec3<float64_t>;
	ML_USING	vec3 = vec3f;


	// VECTOR 4
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tvec4 = tvecn<X, 4>;
	ML_USING	vec4b = tvec4<byte_t>;
	ML_USING	vec4i = tvec4<int32_t>;
	ML_USING	vec4u = tvec4<uint32_t>;
	ML_USING	vec4f = tvec4<float32_t>;
	ML_USING	vec4d = tvec4<float64_t>;
	ML_USING	vec4 = vec4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T, size_t X, size_t Y
	> inline ML_SERIALIZE(std::ostream & out, Matrix<T, X, Y> const & value)
	{
		for (auto const & elem : value)
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class Tx, class Ty, size_t X, size_t Y
	> constexpr bool operator==(Matrix<Tx, X, Y> const & lhs, Matrix<Ty, X, Y> const & rhs)
	{
		return (lhs.m_data == rhs.m_data);
	}

	template <
		class Tx, class Ty, size_t X, size_t Y
	> constexpr bool operator!=(Matrix<Tx, X, Y> const & lhs, Matrix<Ty, X, Y> const & rhs)
	{
		return (lhs.m_data != rhs.m_data);
	}

	template <
		class Tx, class Ty, size_t X, size_t Y
	> constexpr bool operator<(Matrix<Tx, X, Y> const & lhs, Matrix<Ty, X, Y> const & rhs)
	{
		return (lhs.m_data < rhs.m_data);
	}

	template <
		class Tx, class Ty, size_t X, size_t Y
	> constexpr bool operator<=(Matrix<Tx, X, Y> const & lhs, Matrix<Ty, X, Y> const & rhs)
	{
		return (lhs.m_data <= rhs.m_data);
	}

	template <
		class Tx, class Ty, size_t X, size_t Y
	> constexpr bool operator>(Matrix<Tx, X, Y> const & lhs, Matrix<Ty, X, Y> const & rhs)
	{
		return (lhs.m_data > rhs.m_data);
	}

	template <
		class Tx, class Ty, size_t X, size_t Y
	> constexpr bool operator>=(Matrix<Tx, X, Y> const & lhs, Matrix<Ty, X, Y> const & rhs)
	{
		return (lhs.m_data >= rhs.m_data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator+=(tvecn<Tx, N> & lhs, tvecn<Ty, N> const & rhs)
		-> tvecn<Tx, N> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] + static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator-=(tvecn<Tx, N> & lhs, tvecn<Ty, N> const & rhs)
		-> tvecn<Tx, N> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] - static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator*=(tvecn<Tx, N> & lhs, tvecn<Ty, N> const & rhs)
		-> tvecn<Tx, N> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] * static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator/=(tvecn<Tx, N> & lhs, tvecn<Ty, N> const & rhs)
		-> tvecn<Tx, N> &
	{
		for (size_t i = 0; i < lhs.size(); i++)
		{
			lhs[i] = (lhs[i] / static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator+(tvecn<Tx, N> const & lhs, tvecn<Ty, N> const & rhs)
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp { lhs };
		temp += rhs;
		return temp;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator-(tvecn<Tx, N> const & lhs, tvecn<Ty, N> const & rhs)
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp { lhs };
		temp -= rhs;
		return temp;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator*(tvecn<Tx, N> const & lhs, tvecn<Ty, N> const & rhs)
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp { lhs };
		temp *= rhs;
		return temp;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator/(tvecn<Tx, N> const & lhs, tvecn<Ty, N> const & rhs)
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp { lhs };
		temp /= rhs;
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator+=(tvecn<Tx, N> & lhs, Ty const & rhs)
		-> tvecn<Tx, N> &
	{
		for (auto & elem : lhs)
		{
			elem += static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator-=(tvecn<Tx, N> & lhs, Ty const & rhs)
		-> tvecn<Tx, N> &
	{
		for (auto & elem : lhs)
		{
			elem -= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator*=(tvecn<Tx, N> & lhs, Ty const & rhs)
		-> tvecn<Tx, N> &
	{
		for (auto & elem : lhs)
		{
			elem *= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator/=(tvecn<Tx, N> & lhs, Ty const & rhs)
		-> tvecn<Tx, N> &
	{
		for (auto & elem : lhs)
		{
			elem /= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator+(tvecn<Tx, N> const & lhs, Ty const & rhs)
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp { lhs };
		temp += static_cast<Tx>(rhs);
		return temp;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator-(tvecn<Tx, N> const & lhs, Ty const & rhs)
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp { lhs };
		temp -= static_cast<Tx>(rhs);
		return temp;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator*(tvecn<Tx, N> const & lhs, Ty const & rhs)
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp { lhs };
		temp *= static_cast<Tx>(rhs);
		return temp;
	}

	template <
		class Tx, class Ty, size_t N
	> constexpr auto operator/(tvecn<Tx, N> const & lhs, Ty const & rhs)
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp { lhs };
		temp /= static_cast<Tx>(rhs);
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T,  size_t X, size_t Y
	> constexpr auto operator-(Matrix<T, X, Y> const & lhs)
		-> Matrix<T, X, Y>
	{
		return (lhs * static_cast<T>(-1));
	}

	template <
		class T,  size_t X, size_t Y
	> constexpr auto operator+(Matrix<T, X, Y> const & lhs)
		-> Matrix<T, X, Y>
	{
		return -(-(lhs));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATRIX_HPP_