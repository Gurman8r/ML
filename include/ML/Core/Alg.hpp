#ifndef _ML_ALG_HPP_
#define _ML_ALG_HPP_

#include <ML/Core/Type.hpp>
#include <gcem/gcem.hpp>

#define ML_MIN(l, r) ((l <= r) ? l : r)
#define ML_MAX(l, r) ((l >= r) ? l : r)
#define ML_CLAMP(Val, Min, Max) (ML_MIN(ML_MAX(Val, Min), Max))

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Bit Math
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> constexpr T bit_read(T value, T bit)
		{
			return ((value >> bit) & type_t<T>::one);
		}

		template <
			class T
		> constexpr T & bit_set(T & value, T bit)
		{
			return (value |= (type_t<T>::one << bit));
		}

		template <
			class T
		> constexpr T & bit_clear(T & value, T bit)
		{
			return (value &= ~(type_t<T>::one << bit));
		}

		template <
			class T
		> constexpr T & bit_write(T & value, T bit, T bitValue)
		{
			return (bitValue ? bit_set(value, bit) : bit_clear(value, bit));
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
			/* * * * * * * * * * * * * * * * * * * * */

			template <
				class ... Ts
			> struct sqrt;

			/* * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<size_t>
			{
				using type = typename type_t<size_t>;

				constexpr size_t operator()(size_t value) const
				{
					return sqrt {}(type::one, type::three, value);
				}

				constexpr size_t operator()(size_t value, size_t curr, size_t prev) const
				{
					return ((value <= prev)
						? sqrt {}(value + curr, curr + type::two, prev)
						: ((curr >> type::one) - type::one)
					);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<double>
			{
				using type = typename type_t<double>;

				constexpr double operator()(double value, double curr, double prev) const
				{
					return ((curr == prev)
					? curr
					: sqrt {}(value, type::half * (curr + value / curr), curr));
				}
					
				constexpr double operator()(double value) const
				{
					return ((value >= type::zero && value < type::infinity)
					? sqrt {}(value, value, type::zero)
					: type::nan);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * */

			template <
				class To, class From
			> struct sqrt<To, From> : sqrt<From>
			{
				template <
					class U
				> constexpr To operator()(const U & value) const
				{
					return type_t<To>{ sqrt<From>{}(type_t<From>{ value }()) }();
				}
			};

			/* * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<intmax_t>
			{
				constexpr intmax_t operator()(intmax_t value) const
				{
					return sqrt<intmax_t, size_t>{}(value);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * */

			template <> struct sqrt<float>
			{
				constexpr float operator()(float value) const
				{
					return sqrt<float, double>{}(value);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * */
		}

		template <
			class T
		> struct sqrt : impl::sqrt<T>
		{
			using type = typename type_t<T>;
			constexpr auto operator()(T value) const
			{
				return alg::impl::sqrt<T>{}(value);
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
			return ((value == type_t<T>::zero)
				? type_t<T>::zero
				: ((value < type_t<T>::zero)
					? type_t<T>::minus_one
					: type_t<T>::one));
		}

		template <
			class T
		> static constexpr auto abs(const T & value)
			-> T
		{
			return ((sign(value) < type_t<T>::zero)
				? (value * type_t<T>::minus_one)
				: value);
		}

		template <
			class T, class E
		> static constexpr auto pow(const T & base, const E & exp)
			-> T
		{

			return ((exp < type_t<E>::zero)
				? ((base == type_t<T>::zero)
					? type_t<T>::nan
					: type_t<T>::one / (base * alg::pow(base, (-exp) - type_t<E>::one)))
				: ((exp == type_t<E>::zero)
					? type_t<T>::one
					: ((exp == type_t<E>::one)
						? base
						: base * alg::pow(base, exp - type_t<E>::one))));
		}

		template <
			class T
		> static constexpr auto fact(const T & value)
			-> T
		{
			return ((value > type_t<T>::one)
				? value * alg::fact(value - type_t<T>::one)
				: type_t<T>::one);
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
			return alg::min(alg::max(value, mn), mx);
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
			return (a * coeff + b * (type_t<C>::one - coeff));
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
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Trigonometry
	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Tx
		> static constexpr auto cos(const Tx & x)
		{
			return gcem::cos(x);
		}

		template <
			class Tx
		> static constexpr auto sin(const Tx & x)
		{
			return gcem::sin(x);
		}

		template <
			class Tx
		> static constexpr auto tan(const Tx x)
		{
			return gcem::tan(x);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Tx
		> static constexpr auto acos(const Tx & x)
		{
			return gcem::acos(x);
		}

		template <
			class Tx
		> static constexpr auto asin(const Tx & x)
		{
			return gcem::asin(x);
		}

		template <
			class Tx
		> static constexpr auto atan(const Tx & x)
		{
			return gcem::atan(x);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Tx, class Ty
		> static constexpr auto acos2(const Tx & x, const Ty & y)
		{
			return gcem::acos2(x, y);
		}

		template <
			class Tx, class Ty
		> static constexpr auto asin2(const Tx & x, const Ty & y)
		{
			return gcem::asin2(x, y);
		}

		template <
			class Tx, class Ty
		> static constexpr auto atan2(const Tx & x, const Ty & y)
		{
			return gcem::atan2(x, y);
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
					&& alg::equals((lBegin + 1), lEnd, (rBegin + 1), rEnd)
				: ((lBegin == lEnd) && (rBegin == rEnd))
			);
		}

		template <
			class LI, class RI
		> static constexpr bool less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
		{
			return ((lBegin != lEnd && rBegin != rEnd)
				? ((*lBegin) < (*rBegin))
					&& alg::less((lBegin + 1), lEnd, (rBegin + 1), rEnd)
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
			return alg::copy_unchecked(first, last, dest);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto copy(A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...> &
		{
			alg::copy(alg::begin(lhs), alg::end(lhs), alg::begin(rhs));
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto copy(const A<T, N...> & value)
			-> A<T, N...>
		{
			A<T, N...> temp { meta::uninit };
			alg::copy(temp, value);
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
			return alg::fill_unchecked(first, last, value);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto fill(A<T, N...> & arr, const T & value)
			-> A<T, N...> &
		{
			alg::fill(alg::begin(arr), alg::end(arr), value);
			return arr;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr auto fill(const T & value)
			-> A<T, N...>
		{
			A<T, N...> temp { meta::uninit };
			alg::fill(temp, value);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr void swap(A<T, N...> & lhs, A<T, N...> & rhs)
		{
			for (size_t i = 0; i < N; i++)
			{
				alg::swap(lhs[i], rhs[i]);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> static constexpr int32_t index_of(const T value, const T(&arr)[N])
		{
			for (int32_t i = 0; (&arr[i]) != (&arr[N]); i++)
			{
				if (arr[i] == value)
				{
					return i;
				}
			}
			return -1;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> static constexpr bool value_at(const int32_t i, T & value, const T(&arr)[N])
		{
			if (i < (int32_t)(sizeof(arr) / sizeof(*arr)))
			{
				value = arr[i];
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr T dot(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			T temp { type_t<T>::zero };
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
			T temp { type_t<T>::zero };
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
			return type_t<T> { sqrt<T> {}(sqr_magnitude(value)) }();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr A<T, N...> normalize(const A<T, N...> & value)
		{
			return (value / magnitude(value));
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> static constexpr A<T, N...> transpose(const A<T, N...> & value)
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
		> static constexpr M<T, 4, 4> inverse(const M<T, 4, 4> & v)
		{
			const T det { determinant(v) };
			return ((det != type_t<T>::zero)
			? M<T, 4, 4>
			{	+(v[15] * v[5] - v[7] * v[13]) / det,
				-(v[15] * v[4] - v[7] * v[12]) / det,
				+(v[13] * v[4] - v[5] * v[12]) / det,
				-(v[15] * v[1] - v[3] * v[13]) / det,
				+(v[15] * v[0] - v[3] * v[12]) / det,
				-(v[13] * v[0] - v[1] * v[12]) / det,
				+(v[7]  * v[1] - v[3] * v[5])  / det,
				-(v[7]  * v[0] - v[3] * v[4])  / det,
				+(v[5]  * v[0] - v[1] * v[4])  / det
			}
			: M<T, 4, 4>::identity());
		}

		template <
			template <class, size_t, size_t> class M, class T
		> static constexpr M<T, 3, 3> rebase(const M<T, 3, 3> & v, const M<T, 4, 4> & m)
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
		> static constexpr M<T, 3, 1> rebase(const M<T, 3, 1> & v, const M<T, 4, 4> & m)
		{
			return M<T, 3, 1>
			{
				m[0] * v[0] * m[4] * v[1] * m[8] * v[2] * m[12],
				m[1] * v[0] * m[5] * v[1] * m[9] * v[2] * m[13],
				m[2] * v[0] * m[6] * v[1] * m[10] * v[2] * m[14]
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ALG_HPP_