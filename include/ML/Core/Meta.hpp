#ifndef _ML_META_HPP_
#define _ML_META_HPP_

// Sources:
//	- https://stackoverflow.com/questions/8622256/in-c11-is-sqrt-defined-as-constexpr
//	- https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
//	- https://github.com/Manu343726/ctti
//		- https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/cstring.hpp
//		- https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/meta.hpp
//	- https://github.com/kthohr/gcem
//	- https://stackoverflow.com/questions/3380628/fast-arc-cos-algorithm

#include <ML/Core/Array.hpp>
#include <ML/Core/CString.hpp>

// MATRIX_T
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t X, size_t Y
		> struct matrix_t
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static_assert(0 < (X * Y), "matrix_t : size negative or zero");

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum : size_t { Cols = X, Rows = Y, Size = Cols * Rows };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename T;
			using self_type			= typename matrix_t<value_type, Cols, Rows>;
			using array_type		= typename array_t<value_type, Size>;
			using type				= typename array_type::type;
			using pointer			= typename array_type::pointer;
			using reference			= typename array_type::reference;
			using const_pointer		= typename array_type::const_pointer;
			using const_reference	= typename array_type::const_reference;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			array_type m_data; // aggregate initializer

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto begin()			-> pointer			{ return m_data.begin(); }
			constexpr auto begin()	const	-> const_pointer	{ return m_data.begin(); }
			constexpr auto cbegin() const	-> const_pointer	{ return m_data.cbegin(); }
			constexpr auto cend()	const	-> const_pointer	{ return m_data.cend(); }
			constexpr auto cols()	const	-> size_t			{ return self_type::Cols; }
			constexpr auto data()			-> pointer			{ return m_data.data(); }
			constexpr auto data()	const	-> const_pointer	{ return m_data.data(); }
			constexpr auto end()			-> pointer			{ return m_data.end(); }
			constexpr auto end()	const	-> const_pointer	{ return m_data.end(); }
			constexpr auto hash()	const	-> size_t			{ return m_data.hash(); }
			constexpr auto height()	const	-> size_t			{ return this->rows(); }
			constexpr auto rows()	const	-> size_t			{ return self_type::Rows; }
			constexpr auto size()	const	-> size_t			{ return m_data.size(); }
			constexpr auto width()	const	-> size_t			{ return this->cols(); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr const_reference operator[](size_t i) const { return m_data[i]; }

			constexpr reference operator[](size_t i) { return m_data[i]; }

			constexpr operator array_type() const { return m_data; }

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
						? meta::type_t<U>{ (*this)[y * width() + x] }()
						: meta::type_t<U>::zero
					);
				}
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline friend ML_SERIALIZE(ostream & out, const self_type & rhs)
			{
				for (size_t y = 0; y < rhs.height(); y++)
				{
					for (size_t x = 0; x < rhs.width(); x++)
					{
						out //<< std::left << std::setw(10)
							<< rhs[y * rhs.width() + x] 
							<< ((x < rhs.width() - 1) ? " " : "\n");
					}
				}
				return out;
			}

			inline friend ML_DESERIALIZE(istream & in, self_type & rhs)
			{
				return in >> rhs.m_data;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static constexpr self_type zero()
			{
				return self_type { uninit };
			}

			static constexpr self_type identity()
			{
				self_type temp { self_type::zero() };
				for (size_t i = 0; i < temp.size(); i++)
				{
					temp[i] = (((i / temp.width()) == (i % temp.width()))
						? type::one
						: type::zero
					);
				}
				return temp;
			}

			static constexpr self_type ortho(
				const_reference l, 
				const_reference r, 
				const_reference b, 
				const_reference t)
			{	
				static_assert((X == 4 && Y == 4), "matrix must be 4x4");
				self_type temp { self_type::identity() };
				temp[0 * 4 + 0] = type::two / (r - l);
				temp[1 * 4 + 1] = type::two / (t - b);
				temp[2 * 4 + 2] = type::one;
				temp[3 * 4 + 0] = -(r + l) / (r - l);
				temp[3 * 4 + 1] = -(t + b) / (t - b);
				return temp;
			}

			static constexpr self_type ortho(
				const_reference l,
				const_reference r,
				const_reference b,
				const_reference t,
				const_reference n,
				const_reference f)
			{	
				static_assert((X == 4 && Y == 4), "matrix must be 4x4");
				self_type temp { self_type::identity() };
				temp[0 * 4 + 0] = type::two / (r - l);
				temp[1 * 4 + 1] = type::two / (t - b);
				temp[3 * 4 + 0] = -(r + l) / (r - l);
				temp[3 * 4 + 1] = -(t + b) / (t - b);
				temp[2 * 4 + 2] = -type::two / (f - n);
				temp[3 * 4 + 2] = -(f + n) / (f - n);
				return temp;
			}

			static constexpr self_type persp(
				const_reference fov,
				const_reference aspect,
				const_reference near, 
				const_reference far)
			{	
				static_assert((X == 4 && Y == 4), "matrix must be 4x4");
				self_type temp { self_type::zero() };
				temp[0 * 4 + 0] = type::one / (aspect * alg::tan(fov / type::two));
				temp[1 * 4 + 1] = type::one / alg::tan(fov / type::two);
				temp[2 * 4 + 3] = type::minus_one;
				temp[2 * 4 + 2] = -(far + near) / (far - near);
				temp[3 * 4 + 3] = -(type::two * far * near) / (far - near);
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		// MATRIX NxN
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T, size_t N>
		using tmat_nxn = matrix_t<T, N, N>;

		template <class T, size_t N>
		using tmat_nx1 = matrix_t<T, N, 1>;

		// MATRIX3
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tmat3 = tmat_nxn<T, 3>;
		using mat3b = tmat3<bool>;
		using mat3f = tmat3<float>;
		using mat3i = tmat3<int32_t>;
		using mat3d = tmat3<double>;
		using mat3u = tmat3<uint32_t>;
		using mat3	= mat3f;

		// MATRIX4
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tmat4 = tmat_nxn<T, 4>;
		using mat4b = tmat4<bool>;
		using mat4f = tmat4<float>;
		using mat4i = tmat4<int32_t>;
		using mat4d = tmat4<double>;
		using mat4u = tmat4<uint32_t>;
		using mat4	= mat4f;

		// VECTOR2
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tvec2 = tmat_nx1<T, 2>;
		using vec2b = tvec2<bool>;
		using vec2f = tvec2<float>;
		using vec2i = tvec2<int32_t>;
		using vec2d = tvec2<double>;
		using vec2u = tvec2<uint32_t>;
		using vec2	= vec2f;

		// VECTOR3
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tvec3 = tmat_nx1<T, 3>;
		using vec3b = tvec3<bool>;
		using vec3f = tvec3<float>;
		using vec3i = tvec3<int32_t>;
		using vec3d = tvec3<double>;
		using vec3u = tvec3<uint32_t>;
		using vec3	= vec3f;

		// VECTOR4
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tvec4 = tmat_nx1<T, 4>;
		using vec4b = tvec4<bool>;
		using vec4f = tvec4<float>;
		using vec4i = tvec4<int32_t>;
		using vec4d = tvec4<double>;
		using vec4u = tvec4<uint32_t>;
		using vec4	= vec4f;

		// COLORS
		/* * * * * * * * * * * * * * * * * * * * */

		struct color final
		{
			static constexpr vec4 clear		{ 0.0f, 0.0f, 0.0f, 0.0f };
			static constexpr vec4 white		{ 1.0f, 1.0f, 1.0f, 1.0f };
			static constexpr vec4 black		{ 0.0f, 0.0f, 0.0f, 1.0f };
			static constexpr vec4 gray		{ 0.2f, 0.2f, 0.2f, 1.0f };
			static constexpr vec4 red		{ 1.0f, 0.0f, 0.0f, 1.0f };
			static constexpr vec4 green		{ 0.0f, 1.0f, 0.0f, 1.0f };
			static constexpr vec4 blue		{ 0.0f, 0.0f, 1.0f, 1.0f };
			static constexpr vec4 cyan		{ 0.0f, 1.0f, 1.0f, 1.0f };
			static constexpr vec4 yellow	{ 1.0f, 1.0f, 0.0f, 1.0f };
			static constexpr vec4 magenta	{ 1.0f, 0.0f, 1.0f, 1.0f };
			static constexpr vec4 violet	{ 0.5f, 0.0f, 1.0f, 1.0f };
			static constexpr vec4 lime		{ 0.5f, 1.0f, 0.0f, 1.0f };
			static constexpr vec4 orange	{ 1.0f, 0.5f, 0.0f, 1.0f };
			static constexpr vec4 fuchsia	{ 1.0f, 0.0f, 0.5f, 1.0f };
			static constexpr vec4 aqua		{ 0.0f, 1.0f, 0.5f, 1.0f };
			static constexpr vec4 azure		{ 0.0f, 0.5f, 1.0f, 1.0f };
		};

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// MATRIX ALGORITHMS
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto sqr_magnitude(const A<T, N...> & value)
			-> T
		{
			T temp { meta::type_t<T>::zero };
			for (const auto & elem : value)
			{
				temp += (elem * elem);
			}
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto magnitude(const A<T, N...> & value)
			-> T
		{
			return meta::type_t<T> { sqrt<T> {}(sqr_magnitude(value)) }();
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto normalize(const A<T, N...> & value)
			-> A<T, N...>
		{
			return (value / magnitude(value));
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto dot(const A<T, N...> & lhs, const A<T, N...> & rhs)
			-> T
		{
			T temp { meta::type_t<T>::zero };
			for (size_t i = 0; i < lhs.size(); i++)
			{
				temp += (lhs[i] * rhs[i]);
			}
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto transpose(const A<T, N...> & value)
			-> A<T, N...>
		{
			A<T, N...> temp { meta::uninit };
			for (size_t i = 0; i < value.size(); i++)
			{
				temp[i] = value[
					(i % value.cols()) * value.cols() + (i / value.cols())
				];
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t, size_t> class M, class T
		> static constexpr auto determinant(const M<T, 4, 4> & value)
			-> T
		{
			return
				value[0] * (value[15] * value[5] - value[7] * value[13]) -
				value[1] * (value[15] * value[4] - value[7] * value[12]) +
				value[3] * (value[13] * value[4] - value[5] * value[12]);
		}

		template <
			template <class, size_t, size_t> class M, class T
		> static constexpr auto inverse(const M<T, 4, 4> & value)
			-> M<T, 4, 4>
		{
			const T det { determinant(value) };
			return ((det != meta::type_t<T>::zero)
			? M<T, 4, 4>
			{	+(value[15] * value[5] - value[7] * value[13]) / det,
				-(value[15] * value[4] - value[7] * value[12]) / det,
				+(value[13] * value[4] - value[5] * value[12]) / det,
				-(value[15] * value[1] - value[3] * value[13]) / det,
				+(value[15] * value[0] - value[3] * value[12]) / det,
				-(value[13] * value[0] - value[1] * value[12]) / det,
				+(value[7] * value[1] - value[3] * value[5]) / det,
				-(value[7] * value[0] - value[3] * value[4]) / det,
				+(value[5] * value[0] - value[1] * value[4]) / det
			}
			: M<T, 4, 4>::identity());
		}

		template <
			template <class, size_t, size_t> class M, class T
		> static constexpr auto rebase(const M<T, 3, 3> & v, const M<T, 4, 4> & m)
			-> M<T, 3, 3>
		{
			return M<T, 3, 3>
			{
				m[0] * v[0] + m[4] * v[3] + m[8] * v[6],
					m[1] * v[0] + m[5] * v[3] + m[9] * v[6],
					m[2] * v[0] + m[6] * v[3] + m[10] * v[6],
					m[0] * v[1] + m[4] * v[4] + m[8] * v[7],
					m[1] * v[1] + m[5] * v[4] + m[9] * v[7],
					m[2] * v[1] + m[6] * v[4] + m[10] * v[7],
					m[0] * v[2] + m[4] * v[5] + m[8] * v[8],
					m[1] * v[2] + m[5] * v[5] + m[9] * v[8],
					m[2] * v[2] + m[6] * v[5] + m[10] * v[8]
			};
		}

		template <
			template <class, size_t, size_t> class M, class T
		> static constexpr auto rebase(const M<T, 3, 1> & v, const M<T, 4, 4> & m)
			-> M<T, 3, 1>
		{
			return M<T, 3, 1>
			{
				m[0] * v[0] * m[4] * v[1] * m[8] * v[2] * m[12],
					m[1] * v[0] * m[5] * v[1] * m[9] * v[2] * m[13],
					m[2] * v[0] * m[6] * v[1] * m[10] * v[2] * m[14]
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> static constexpr auto concat(const meta::tmat3<T> & lhs, const meta::tmat3<T> & rhs)
			-> meta::tmat3<T>
		{
			meta::tmat3<T> temp { meta::uninit };
			for (size_t y = 0; y < temp.height(); y++)
			{
				for (size_t x = 0; x < temp.width(); x++)
				{
					for (size_t r = 0; r < temp.height(); r++)
					{
						temp[y * temp.width() + x] +=
							lhs[y * temp.width() + x] *
							rhs[r * temp.width() + x];
					}
				}
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> static constexpr auto rotationX(const T & rad)
			-> meta::tmat3<T>
		{
			using type = meta::type_t<T>;
			meta::tmat3<T> temp
			{
				type::one,	type::zero,		type::zero,
				type::zero, alg::cos(rad),	-alg::sin(rad),
				type::zero, alg::sin(rad),	alg::cos(rad),
			};
			return temp;
		}

		template <
			class T
		> static constexpr auto rotationY(const T & rad)
			-> meta::tmat3<T>
		{
			using type = meta::type_t<T>;
			meta::tmat3<T> temp
			{
				alg::cos(rad),	type::zero, alg::sin(rad),
				type::zero,		type::one,	type::zero,
				-alg::sin(rad), type::zero, alg::cos(rad),
			};
			return temp;
		}

		template <
			class T
		> static constexpr auto rotationZ(const T & rad)
			-> meta::tmat3<T>
		{
			using type = meta::type_t<T>;
			meta::tmat3<T> temp
			{
				alg::cos(rad),	-alg::sin(rad),	type::zero,
				alg::sin(rad),	alg::cos(rad),	type::zero,
				type::zero,		type::zero,		type::one,
			};
			return temp;
		}

		template <
			class T
		> static constexpr auto rotationXYZ(const meta::tvec3<T> & v)
			-> meta::tmat3<T>
		{
			return rotationX(v[0]) * rotationY(v[1]) * rotationZ(v[2]);
		}

		template <
			class T
		> static constexpr auto rotationZYX(const meta::tvec3<T> & v)
			-> meta::tmat3<T>
		{
			return rotationZ(v[2]) * rotationY(v[1]) * rotationX(v[0]);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// VERTEX
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		struct vertex final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum { Size = vec3::Size + vec4::Size + vec2::Size };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename float;
			using self_type			= typename vertex;
			using array_type		= typename array_t<value_type, Size>;
			using pointer			= typename array_type::pointer;
			using reference			= typename array_type::reference;
			using const_pointer		= typename array_type::const_pointer;
			using const_reference	= typename array_type::const_reference;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr explicit vertex(const array_type & value)
				: m_data { value }
			{
			}

			constexpr explicit vertex(const vec3 & pos, const vec4 & col, const vec2 & tex)
				: self_type { {
				pos[0], pos[1], pos[2],
				col[0], col[1], col[2], col[3],
				tex[0], tex[1] } }
			{
			}

			constexpr explicit vertex(const vec3 & pos, const vec4 & col)
				: self_type { pos, col, vec2::zero() }
			{
			}

			constexpr explicit vertex(const vec3 & pos, const vec2 & tex)
				: self_type { pos, color::white, tex }
			{
			}

			constexpr explicit vertex(const vec4 & col)
				: self_type { vec3::zero(), col, vec2::zero() }
			{
			}

			constexpr explicit vertex(const vec3 & pos)
				: self_type { pos, color::white, vec2::zero() }
			{
			}

			constexpr explicit vertex(const vec2 & tex)
				: self_type { vec3::zero(), color::white, tex }
			{
			}

			constexpr vertex(const vertex & copy)
				: self_type { copy.pos(), copy.col(), copy.tex() }
			{
			}

			constexpr vertex()
				: self_type { vec3::zero(), color::white, vec2::zero() }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto begin()			-> pointer			{ return m_data.begin(); }
			constexpr auto begin()	const	-> const_pointer	{ return m_data.begin(); }
			constexpr auto cbegin() const	-> const_pointer	{ return m_data.cbegin(); }
			constexpr auto cend()	const	-> const_pointer	{ return m_data.cend(); }
			constexpr auto data()			-> pointer			{ return m_data.data(); }
			constexpr auto data()	const	-> const_pointer	{ return m_data.data(); }
			constexpr auto end()			-> pointer			{ return m_data.end(); }
			constexpr auto end()	const	-> const_pointer	{ return m_data.end(); }
			constexpr auto size()	const	-> size_t			{ return m_data.size(); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr const_reference operator[](size_t i) const { return m_data[i]; }

			constexpr reference operator[](size_t i) { return m_data[i]; }

			constexpr operator array_type() const { return m_data; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto get(size_t i) const 
				-> const_reference
			{
				return (*this)[i];
			}

			constexpr auto pos() const -> vec3 { return { get(0), get(1), get(2) }; }
			constexpr auto col() const -> vec4 { return { get(3), get(4), get(5), get(6) }; }
			constexpr auto tex() const -> vec2 { return { get(7), get(8) }; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto set(size_t i, const_reference value) 
				-> self_type &
			{
				(*this)[i] = value;
				return (*this);
			}

			constexpr auto pos(const vec3 & v) -> self_type & { return set(0, v[0]).set(1, v[1]).set(2, v[2]); }
			constexpr auto col(const vec4 & v) -> self_type & { return set(3, v[0]).set(4, v[1]).set(5, v[2]).set(6, v[3]); }
			constexpr auto tex(const vec2 & v) -> self_type & { return set(7, v[0]).set(8, v[1]); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline friend ML_SERIALIZE(ostream & out, const self_type & rhs)
			{
				return out << rhs.m_data;
			}

			inline friend ML_DESERIALIZE(istream & in, self_type & rhs)
			{
				return in >> rhs.m_data;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private: array_type m_data;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		namespace geometry
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			
			namespace impl
			{
				template <
					size_t V, size_t I
				> struct static_mesh
				{
					struct num final
					{
						static constexpr size_t vertices	{ V };
						static constexpr size_t indices		{ I };
						static constexpr size_t contiguous	{ vertices * vertex::Size };
					};

					using vertices_t	= typename array_t<vertex,	 num::vertices>;
					using indices_t		= typename array_t<uint32_t, num::indices>;
					using contiguous_t	= typename array_t<float,	 num::contiguous>;

				protected:
					static constexpr contiguous_t Contiguous(const vertices_t & value)
					{
						contiguous_t temp { uninit };
						for (size_t i = 0; i < temp.size(); i++)
						{
							temp[i] = (value[i / vertex::Size])[i % vertex::Size];
						}
						return temp;
					}
				};
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct tri final : impl::static_mesh<3, 3>
			{
				static constexpr vertices_t vertices
				{ 
					vertex { {  0.0f,  0.5f, 0.0f }, color::white, { 0.5f, 1.0f } },
					vertex { {  0.5f, -0.5f, 0.0f }, color::white, { 1.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, 0.0f }, color::white, { 0.0f, 0.0f } },
				};
				static constexpr indices_t indices
				{
					0, 1, 2
				};
				static constexpr contiguous_t contiguous
				{
					static_mesh::Contiguous(vertices)
				};
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct quad final : impl::static_mesh<4, 6>
			{
				static constexpr vertices_t vertices
				{
					vertex { { +1.0f, +1.0f, 0.0f }, color::white, { 1.0f, 1.0f } },
					vertex { { +1.0f, -1.0f, 0.0f }, color::white, { 1.0f, 0.0f } },
					vertex { { -1.0f, -1.0f, 0.0f }, color::white, { 0.0f, 0.0f } },
					vertex { { -1.0f, +1.0f, 0.0f }, color::white, { 0.0f, 1.0f } },
				};
				static constexpr indices_t indices
				{
					0, 1, 3,
					1, 2, 3
				};
				static constexpr contiguous_t contiguous
				{
					 static_mesh::Contiguous(vertices)
				};
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct cube final : impl::static_mesh<24, 36>
			{
				static constexpr vertices_t vertices
				{
					vertex { { +0.5f, +0.5f, -0.5f }, color::white, { 1.0f, 1.0f } },
					vertex { { +0.5f, -0.5f, -0.5f }, color::white, { 1.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
					vertex { { -0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
					vertex { { +0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
					vertex { { +0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, +0.5f }, color::white, { 0.0f, 0.0f } },
					vertex { { -0.5f, +0.5f, +0.5f }, color::white, { 0.0f, 1.0f } },
					vertex { { +0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
					vertex { { +0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
					vertex { { +0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
					vertex { { +0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
					vertex { { -0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
					vertex { { -0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
					vertex { { -0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
					vertex { { -0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
					vertex { { +0.5f, +0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
					vertex { { +0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
					vertex { { -0.5f, +0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
					vertex { { -0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 1.0f } },
					vertex { { +0.5f, -0.5f, +0.5f }, color::white, { 1.0f, 0.0f } },
					vertex { { +0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, -0.5f }, color::white, { 0.0f, 1.0f } },
				};
				static constexpr indices_t indices
				{
					0, 1, 3,
					1, 2, 3,
					7, 5, 4,
					7, 6, 5,
					8, 9, 11,
					9, 10, 11,
					15, 13, 12,
					15, 14, 13,
					17, 18, 16,
					18, 19, 16,
					22, 21, 20,
					20, 23, 22
				};
				static constexpr contiguous_t contiguous
				{
					 static_mesh::Contiguous(vertices)
				};
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_META_HPP_