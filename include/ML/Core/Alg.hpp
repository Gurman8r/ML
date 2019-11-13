#ifndef _ML_ALG_HPP_
#define _ML_ALG_HPP_

#include <ML/Core/Pi.hpp>
#include <ML/Core/Sqrt.hpp>
#include <gcem.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_MIN(lhs, rhs) ((lhs <= rhs) ? lhs : rhs)
#define ML_MAX(lhs, rhs) ((lhs >= rhs) ? lhs : rhs)
#define ML_CLAMP(value, minimum, maximum) (ML_MIN(ML_MAX(value, minimum), maximum))

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Strings
	namespace alg
	{
		template <class Ch> static constexpr size_t strlen(const Ch * value)
		{
			return ((*value) ? (1 + alg::strlen(value + 1)) : 0);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// General
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static constexpr int32_t sign(const T & value)
		{
			return ((value == (T)0)
				? (T)0
				: ((value < (T)0)
					? (T)-1
					: (T)1
			));
		}

		template <class T> static constexpr T abs(const T & value)
		{
			return ((_ML alg::sign(value) < (T)0)
				? (value * (T)-1)
				: value
			);
		}

		template <
			class Base, class Exp
		> static constexpr Base pow(const Base & base, const Exp & exp)
		{
			using B = cast<Base>;
			using E = cast<Exp>;
			return ((exp < E::zero)
				? ((base == B::zero)
					? limits<Base>::nan
					: B::one / (base * _ML alg::pow(base, (-exp) - E::one)))
				: ((exp == E::zero)
					? B::one
					: ((exp == E::one)
						? base
						: base * _ML alg::pow(base, exp - E::one)
						)));
		}

		template <class T> static constexpr T fact(const T & value)
		{
			return ((value > (T)1) ? value * _ML alg::fact(value - (T)1) : (T)1);
		}

		template <class T> static constexpr const T & min(const T & lhs, const T & rhs)
		{
			return (lhs <= rhs) ? lhs : rhs;
		}
			
		template <class T> static constexpr const T & max(const T & lhs, const T & rhs)
		{
			return (lhs >= rhs) ? lhs : rhs;
		}

		template <class T> static constexpr auto clamp(const T & value, const T & mn, const T & mx)
		{
			return _ML alg::min(_ML alg::max(value, mn), mx);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class C
		> static constexpr T lerp(const T & a, const T & b, const C & coeff)
		{
			return (a * coeff + b * ((C)1 - coeff));
		}

		template <class T> static constexpr T map(
			const T & value, 
			const T & a0, const T & a1,
			const T & b0, const T & b1
		)
		{
			return (b0 + (value - a0) * (b1 - b0) / (a1 - a0));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ranges
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, size_t N> static constexpr const_ptr_t<T> begin(const T(&value)[N])
		{
			return (&value[0]);
		}

		template <class T, size_t N> static constexpr const_ptr_t<T> end(const T(&value)[N])
		{
			return (&value[N]);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class LI, class RI
		> static constexpr bool equals(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
		{
			return ((lBegin != lEnd && rBegin != rEnd)
				? (((*lBegin) == (*rBegin))
					&& _ML alg::equals((lBegin + 1), lEnd, (rBegin + 1), rEnd))
				: ((lBegin == lEnd) && (rBegin == rEnd))
			);
		}

		template <
			class LI, class RI
		> static constexpr bool less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
		{
			return ((lBegin != lEnd && rBegin != rEnd)
				? (((*lBegin) < (*rBegin))
					&& _ML alg::less((lBegin + 1), lEnd, (rBegin + 1), rEnd))
				: ((lBegin == lEnd) && (rBegin == rEnd))
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class InIt, class OutIt
		> static constexpr OutIt copy_unchecked(InIt first, InIt last, OutIt dest)
		{
			while (first != last)
			{
				*(first++) = *(dest++);
			}
			return first;
		}

		template <
			class InIt, class OutIt
		> static constexpr OutIt copy(InIt first, InIt last, OutIt dest)
		{
			return _ML alg::copy_unchecked(first, last, dest);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr A<T, N...> & copy(A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			_ML alg::copy(_ML alg::begin(lhs), _ML alg::end(lhs), _ML alg::begin(rhs));
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class InIt, class T
		> static constexpr InIt fill_unchecked(InIt first, InIt last, T value)
		{
			while (first != last)
			{
				*(first++) = value;
			}
			return first;
		}

		template <
			class InIt, class T
		> static constexpr InIt fill(InIt first, InIt last, T value)
		{
			return _ML alg::fill_unchecked(first, last, value);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr A<T, N...> & fill(A<T, N...> & arr, const T & value)
		{
			_ML alg::fill(_ML alg::begin(arr), _ML alg::end(arr), value);
			return arr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> static constexpr size_t array_size(const T(&value)[N])
		{
			return static_cast<size_t>(ML_ARRAYSIZE(value));
		}

		template <
			class T, size_t N
		> static constexpr int32_t index_of(const T & value, const T(&arr)[N])
		{
			for (int32_t i = 0; (&arr[i]) != (&arr[N]); i++)
			{
				if (arr[i] == value) { return i; }
			}
			return -1;
		}

		template <
			class T, size_t N
		> static constexpr const T & value_at(int32_t index, const T(&arr)[N])
		{
			return arr[index];
		}

		template <
			class T, size_t N
		> static constexpr bool value_at(int32_t index, T & value, const T(&arr)[N])
		{
			if (index < N) 
			{ 
				value = _ML alg::value_at(index, arr);
				return true;
			}
			return false;
		}

		template <
			class T, size_t N
		> static constexpr auto at_index(int32_t index, const T(&arr)[N])
		{
			return ((index >= 0 && index < N) ? arr[index] : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrices
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr T dot(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			T temp { 0 };
			for (size_t i = 0; i < lhs.size(); i++)
			{
				temp += (lhs[i] * rhs[i]);
			}
			return temp;
		}

		template <
			template <class, size_t, size_t> class M, class T
		> static constexpr T determinant(const M<T, 4, 4> & v)
		{
			return
				v[0] * (v[15] * v[5] - v[7] * v[13]) -
				v[1] * (v[15] * v[4] - v[7] * v[12]) +
				v[3] * (v[13] * v[4] - v[5] * v[12]);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr T sqr_magnitude(const A<T, N...> & value)
		{
			T temp { (T)0 };
			for (const auto & elem : value)
			{
				temp += (elem * elem);
			}
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr T magnitude(const A<T, N...> & value)
		{
			return static_cast<T>(sqrt<T> {}(_ML alg::sqr_magnitude(value)));
		}

		template <
			template <class, size_t, size_t> class M,
			class T
		> static constexpr M<T, 2, 1> scale_to_fit(const M<T, 2, 1> & l, const M<T, 2, 1> & r)
		{
			const M<T, 2, 1>
				h = { (r[0] / l[0]), (r[0] / l[0]) },
				v = { (r[1] / l[1]), (r[1] / l[1]) };
			return (l * (((h) < (v)) ? (h) : (v)));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr A<T, N...> normalize(const A<T, N...> & value)
		{
			return (value / _ML alg::magnitude(value));
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr A<T, N...> transpose(const A<T, N...> & value)
		{
			A<T, N...> temp { 0 };
			for (size_t i = 0; i < value.size(); i++)
			{
				const size_t y { i % value.width() };
				const size_t x { i / value.width() };
				temp[i] = value[y * value.width() + x];
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t, size_t> class M, class T
		> static constexpr M<T, 4, 4> inverse(const M<T, 4, 4> & v)
		{
			const T det { _ML alg::determinant(v) };
			return ((det != (T)0)
				? M<T, 4, 4> {	
					+(v[15] * v[5] - v[7] * v[13]) / det,
					-(v[15] * v[4] - v[7] * v[12]) / det,
					+(v[13] * v[4] - v[5] * v[12]) / det,
					-(v[15] * v[1] - v[3] * v[13]) / det,
					+(v[15] * v[0] - v[3] * v[12]) / det,
					-(v[13] * v[0] - v[1] * v[12]) / det,
					+(v[ 7] * v[1] - v[3] * v[ 5]) / det,
					-(v[ 7] * v[0] - v[3] * v[ 4]) / det,
					+(v[ 5] * v[0] - v[1] * v[ 4]) / det
				}
				: M<T, 4, 4>::identity()
			);
		}

		template <
			template <class, size_t, size_t> class M, class T
		> static constexpr M<T, 3, 3> rebase(const M<T, 3, 3> & v, const M<T, 4, 4> & m)
		{
			return M<T, 3, 3>
			{
				m[0] * v[0] + m[4] * v[3] + m[ 8] * v[6],
				m[1] * v[0] + m[5] * v[3] + m[ 9] * v[6],
				m[2] * v[0] + m[6] * v[3] + m[10] * v[6],
				m[0] * v[1] + m[4] * v[4] + m[ 8] * v[7],
				m[1] * v[1] + m[5] * v[4] + m[ 9] * v[7],
				m[2] * v[1] + m[6] * v[4] + m[10] * v[7],
				m[0] * v[2] + m[4] * v[5] + m[ 8] * v[8],
				m[1] * v[2] + m[5] * v[5] + m[ 9] * v[8],
				m[2] * v[2] + m[6] * v[5] + m[10] * v[8]
			};
		}

		template <
			template <class, size_t, size_t> class M, class T
		> static constexpr M<T, 3, 1> rebase(const M<T, 3, 1> & v, const M<T, 4, 4> & m)
		{
			return M<T, 3, 1>
			{
				m[0] * v[0] * m[4] * v[1] * m[ 8] * v[2] * m[12],
				m[1] * v[0] * m[5] * v[1] * m[ 9] * v[2] * m[13],
				m[2] * v[0] * m[6] * v[1] * m[10] * v[2] * m[14]
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t, size_t> class M,
			class T
		> static constexpr T cross(const M<T, 2, 1> & a, const M<T, 2, 1> & b)
		{
			return a[0] * b[1] - b[0] * a[1];
		}

		template <
			template <class, size_t, size_t> class M,
			class T
		> static constexpr M<T, 3, 1> cross(const M<T, 3, 1> & a, const M<T, 3, 1> & b)
		{
			return M<T, 3, 1> {
				a[1] * b[2] - b[1] * a[2],
				a[2] * b[0] - b[2] * a[0],
				a[0] * b[1] - b[0] * a[1]
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ALG_HPP_