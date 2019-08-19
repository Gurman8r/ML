#ifndef _ML_ALG_HPP_
#define _ML_ALG_HPP_

#include <ML/Core/Numeric.hpp>
#include <gcem/gcem.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_ALG _ML alg::

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_MIN(x, y) ((x <= y) ? x : y)

#define ML_MAX(x, y) ((x >= y) ? x : y)

#define ML_CLAMP(VAL, MIN, MAX) (ML_MIN(ML_MAX(VAL, MIN), MAX))

#define ML_ASPECT(w, h) ((w != 0 && h != 0) ? ((float_t)w / (float_t)(h)) : 0.0f)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define bitRead(value, bit) ((value >> bit) & 1)

#define bitSet(value, bit) (value |= (1 << bit))

#define bitClear(value, bit) (value &= ~(1 << bit))

#define bitWrite(value, bit, bitValue) ((bitValue) ? bitSet(value, bit) : bitClear(value, bit)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Trigonometry
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> static constexpr T cos(const T & value)
		{
			return gcem::cos<T>(value);
		}

		template <
			class T
		> static constexpr T sin(const T & value)
		{
			return gcem::sin<T>(value);
		}

		template <
			class T
		> static constexpr T tan(const T value)
		{
			return gcem::tan<T>(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> static constexpr T acos(const T & value)
		{
			return gcem::acos<T>(value);
		}

		template <
			class T
		> static constexpr T asin(const T & value)
		{
			return gcem::asin<T>(value);
		}

		template <
			class T
		> static constexpr auto atan(const T & value)
		{
			return gcem::atan<T>(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class Tx, class Ty
		> static constexpr T acos2(const Tx & x, const Ty & y)
		{
			return numeric<T> { gcem::acos2<Tx, Ty>(x, y) };
		}

		template <
			class T, class Tx, class Ty
		> static constexpr T asin2(const Tx & x, const Ty & y)
		{
			return numeric<T> { gcem::asin2<Tx, Ty>(x, y) };
		}

		template <
			class T, class Tx, class Ty
		> static constexpr T atan2(const Tx & x, const Ty & y)
		{
			return numeric<T>{ gcem::atan2<Tx, Ty>(x, y) };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Square Root
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		namespace impl
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class ... Ts
			> struct sqrt;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<uintmax_t>
			{
				using cast = typename numeric<uintmax_t>;

				constexpr uintmax_t operator()(uintmax_t value) const
				{
					return sqrt {}(cast::one, cast::three, value);
				}

				constexpr uintmax_t operator()(uintmax_t value, uintmax_t curr, uintmax_t prev) const
				{
					return ((value <= prev)
						? sqrt {}(value + curr, curr + cast::two, prev)
						: ((curr >> cast::one) - cast::one)
					);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<float64_t>
			{
				using cast = typename numeric<float64_t>;

				constexpr float64_t operator()(float64_t value, float64_t curr, float64_t prev) const
				{
					return ((curr == prev)
						? curr
						: sqrt {}(value, cast::half * (curr + value / curr), curr)
					);
				}
					
				constexpr float64_t operator()(float64_t value) const
				{
					return ((value >= cast::zero && value < cast::infinity)
						? sqrt {}(value, value, cast::zero)
						: cast::nan
					);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class To, class From
			> struct sqrt<To, From> : public sqrt<From>
			{
				template <
					class U
				> constexpr To operator()(const U & value) const
				{
					return numeric<To>(sqrt<From>{}(numeric<From>(value)));
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<intmax_t>
			{
				constexpr intmax_t operator()(intmax_t value) const
				{
					return sqrt<intmax_t, uintmax_t>{}(value);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<float32_t>
			{
				constexpr float32_t operator()(float32_t value) const
				{
					return sqrt<float32_t, float64_t>{}(value);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		template <
			class T
		> struct sqrt : public ML_ALG impl::sqrt<T>
		{
			constexpr T operator()(T value) const
			{
				return ML_ALG impl::sqrt<T>{}(value);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// General
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> static constexpr auto sign(const T & value)
			-> T
		{
			return ((value == numeric<T>::zero)
				? numeric<T>::zero
				: ((value < numeric<T>::zero)
					? numeric<T>::minus_one
					: numeric<T>::one
			));
		}

		template <
			class T
		> static constexpr auto abs(const T & value)
			-> T
		{
			return ((ML_ALG sign(value) < numeric<T>::zero)
				? (value * numeric<T>::minus_one)
				: value
			);
		}

		template <
			class B, class E
		> static constexpr auto pow(const B & base, const E & exp)
			-> B
		{
			using TB = numeric<B>;
			using TE = numeric<E>;
			return ((exp < TE::zero)
				? ((base == TB::zero)
					? TB::nan
					: TB::one / (base * ML_ALG pow(base, (-exp) - TE::one)))
				: ((exp == TE::zero)
					? TB::one
					: ((exp == TE::one)
						? base
						: base * ML_ALG pow(base, exp - TE::one)
						)));
		}

		template <
			class T
		> static constexpr auto fact(const T & value)
			-> T
		{
			return ((value > numeric<T>::one)
				? value * ML_ALG fact(value - numeric<T>::one)
				: numeric<T>::one
			);
		}
			
		template <
			class T
		> static constexpr auto max(const T & lhs, const T & rhs)
			-> const T &
		{
			return (lhs >= rhs) ? lhs : rhs;
		}

		template <
			class T
		> static constexpr auto min(const T & lhs, const T & rhs)
			-> const T &
		{
			return (lhs <= rhs) ? lhs : rhs;
		}

		template <
			class T
		> static constexpr auto clamp(const T & value, const T & mn, const T & mx)
			-> const T
		{
			return ML_ALG min(ML_ALG max(value, mn), mx);
		}

		template <
			class T
		> static constexpr void swap(T & lhs, T & rhs)
		{
			auto temp { lhs };
			lhs = rhs;
			rhs = temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class C
		> static constexpr T lerp(const T & a, const T & b, const C & coeff)
		{
			return (a * coeff + b * (numeric<C>::one - coeff));
		}

		template <
			class T
		> static constexpr T map(
			const T & value, 
			const T & a0, const T & a1,
			const T & b0, const T & b1)
		{
			return (b0 + (value - a0) * (b1 - b0) / (a1 - a0));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class Num, class Den = typename Num
		> static constexpr T delta_cast(const Num numerator, const Den denominator)
		{
			using cast = numeric<T>;
			const T num { cast(numerator) };
			const T den { cast(denominator) };
			return (((den > cast::zero) && (num < den)) ? (num / den) : cast::zero);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ranges
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> static constexpr auto begin(const T(&value)[N])
			-> const T *
		{
			return (&value[0]);
		}

		template <
			class T, size_t N
		> static constexpr auto end(const T(&value)[N])
			-> const T *
		{
			return (&value[N]);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class LI, class RI
		> static constexpr bool equals(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
		{
			return ((lBegin != lEnd && rBegin != rEnd)
				? ((*lBegin) == (*rBegin))
					&& ML_ALG equals((lBegin + 1), lEnd, (rBegin + 1), rEnd)
				: ((lBegin == lEnd) && (rBegin == rEnd))
			);
		}

		template <
			class LI, class RI
		> static constexpr bool less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
		{
			return ((lBegin != lEnd && rBegin != rEnd)
				? ((*lBegin) < (*rBegin))
					&& ML_ALG less((lBegin + 1), lEnd, (rBegin + 1), rEnd)
				: ((lBegin == lEnd) && (rBegin == rEnd))
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class InIt, class OutIt
		> static constexpr auto copy_unchecked(InIt first, InIt last, OutIt dest)
			-> OutIt
		{
			while (first != last)
			{
				*(first++) = *(dest++);
			}
			return first;
		}

		template <
			class InIt, class OutIt
		> static constexpr auto copy(InIt first, InIt last, OutIt dest)
			-> OutIt
		{
			return ML_ALG copy_unchecked(first, last, dest);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto copy(A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...> &
		{
			ML_ALG copy(ML_ALG begin(lhs), ML_ALG end(lhs), ML_ALG begin(rhs));
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto copy(const A<T, N...> & value)
			-> A<T, N...>
		{
			A<T, N...> temp { uninit };
			ML_ALG copy(temp, value);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class InIt, class T
		> static constexpr auto fill_unchecked(InIt first, InIt last, T value)
			-> InIt
		{
			while (first != last)
			{
				*(first++) = value;
			}
			return first;
		}

		template <
			class InIt, class T
		> static constexpr auto fill(InIt first, InIt last, T value)
			-> InIt
		{
			return ML_ALG fill_unchecked(first, last, value);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto fill(A<T, N...> & arr, const T & value)
			-> A<T, N...> &
		{
			ML_ALG fill(ML_ALG begin(arr), ML_ALG end(arr), value);
			return arr;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto fill(const T & value)
			-> A<T, N...>
		{
			A<T, N...> temp { uninit };
			ML_ALG fill(temp, value);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> static constexpr size_t size_of(const T(&value)[N])
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
				value = ML_ALG value_at(index, arr);
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
			T temp { uninit };
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
			T temp { numeric<T>::zero };
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
			return numeric<T> { sqrt<T> {}(ML_ALG sqr_magnitude(value)) };
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
			return (value / ML_ALG magnitude(value));
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr A<T, N...> transpose(const A<T, N...> & value)
		{
			A<T, N...> temp { uninit };
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
		> static constexpr M<T, 4, 4> inverse(const M<T, 4, 4> & v)
		{
			const T det { ML_ALG determinant(v) };
			return ((det != numeric<T>::zero)
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
			return (a[0] * b[1]) - (a[1] * b[0]);
		}

		template <
			template <class, size_t, size_t> class M,
			class T
		> static constexpr M<T, 3, 1> cross(const M<T, 3, 1> & a, const M<T, 3, 1> & b)
		{
			return M<T, 3, 1> {
				(a[1] * b[2]) - (a[2] * b[1]),
				(a[2] * b[0]) - (a[0] * b[2]),
				(a[0] * b[1]) - (a[1] * b[0])
			};
		}

		// this needs to be made constexpr, normalize functions causing problems?
		template <
			template <class, size_t, size_t> class M,
			class T
		> static inline M<T, 4, 4> lookAt(
			const M<T, 3, 1> & eye, 
			const M<T, 3, 1> & center,
			const M<T, 3, 1> & up
		)
		{
			using cast = numeric<T>;
			const M<T, 3, 1> f = ML_ALG normalize(center - eye);
			const M<T, 3, 1> s = ML_ALG normalize(ML_ALG cross(f, up));
			const M<T, 3, 1> u = ML_ALG cross(s, f);
			return M<T, 4, 4> { 
				s[0], u[0], -f[0], cast::one, 
				s[1], u[1], -f[1], cast::one, 
				s[2], u[2], -f[2], cast::one, 
				-ML_ALG dot(s, eye), -ML_ALG dot(u, eye), ML_ALG dot(f, eye), cast::one
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ALG_HPP_