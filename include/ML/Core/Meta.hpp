#ifndef _ML_META_HPP
#define _ML_META_HPP

// Sources:
//	- https://stackoverflow.com/questions/8622256/in-c11-is-sqrt-defined-as-constexpr
//	- https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
//	- https://github.com/Manu343726/ctti
//		- https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/cstring.hpp
//		- https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/meta.hpp
//	- https://github.com/kthohr/gcem
//	- https://stackoverflow.com/questions/3380628/fast-arc-cos-algorithm

#include <ML/Core/GLM.hpp>
#include <ML/Core/String.hpp>
#include <gcem/gcem.hpp>

#define ML_META ::ml::meta::
#define ML_INLINE inline

namespace ml
{
	enum { uninit };
}

// VALUE_T
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		// STRUCT VALUE_T
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> struct value_t
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using type		= typename T;
			using self_type = typename value_t<type>;
			using limits_t	= typename std::numeric_limits<type>;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct cast_v final
			{
				template <
					class U
				> constexpr type operator()(const U & value) const
				{
					return static_cast<type>(value);
				}
			};

			struct epsilon_v final
			{
				constexpr type operator()(type seed) const
				{
					auto temp { zero };
					while ((one + seed) != one)
					{
						temp = seed;
						seed /= two;
					}
					return temp;
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static constexpr type	minus_one		{ cast_v()( -1) };
			static constexpr type	zero			{ cast_v()(  0) };
			static constexpr type	one				{ cast_v()(  1) };
			static constexpr type	two				{ cast_v()(  2) };
			static constexpr type	three			{ cast_v()(  3) };
			static constexpr type	four			{ cast_v()(  4) };
			static constexpr type	five			{ cast_v()(  5) };
			static constexpr type	six				{ cast_v()(  6) };
			static constexpr type	seven			{ cast_v()(  7) };
			static constexpr type	eight			{ cast_v()(  8) };
			static constexpr type	nine			{ cast_v()(  9) };
			static constexpr type	ten				{ cast_v()( 10) };
			static constexpr type	fourty_five		{ cast_v()( 45) };
			static constexpr type	sixty			{ cast_v()( 60) };
			static constexpr type	ninety			{ cast_v()( 90) };
			static constexpr type	one_hundred		{ cast_v()(100) };
			static constexpr type	one_eighty		{ cast_v()(180) };
			static constexpr type	three_sixty		{ cast_v()(360) };

			static constexpr type	half			{ one / two };
			static constexpr type	third			{ one / three };
			static constexpr type	quarter			{ one / four };
			static constexpr type	fifth			{ one / five };
			static constexpr type	sixth			{ one / six };
			static constexpr type	seventh			{ one / seven };
			static constexpr type	eighth			{ one / eight };
			static constexpr type	ninth			{ one / nine };
			static constexpr type	tenth			{ one / ten };
			static constexpr type	two_thirds		{ two / three };
			static constexpr type	three_quarters	{ three / four };

			static constexpr type	infinity		{ limits_t::infinity() };
			static constexpr type	nan				{ limits_t::quiet_NaN() };
			static constexpr type	min				{ limits_t::min() };
			static constexpr type	max				{ limits_t::max() };
			static constexpr type	epsilon			{ epsilon_v()(half) };
			static constexpr type	half_epsilon	{ epsilon * half };

			static constexpr type	pi				{ cast_v()(3.14159265358979323846264338327L) };
			static constexpr type	two_pi			{ pi * two };
			static constexpr type	half_pi			{ pi * half };
			static constexpr type	quarter_pi		{ pi * quarter };
			static constexpr type	third_pi		{ pi * third };
			static constexpr type	deg2rad			{ pi / one_eighty };
			static constexpr type	rad2deg			{ one_eighty / pi };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class U
			> constexpr explicit value_t(const U & value)
				: m_value { cast_v()(value) }
			{
			}
			
			constexpr type operator()() const { return m_value; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private: const type m_value;
		};

		// OPERATORS
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class> class Val, class T
		> constexpr bool operator==(const Val<T> & lhs, const Val<T> & rhs)
		{
			return (lhs() == rhs());
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator!=(const Val<T> & lhs, const Val<T> & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator<(const Val<T> & lhs, const Val<T> & rhs)
		{
			return (lhs() < rhs());
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator>(const Val<T> & lhs, const Val<T> & rhs)
		{
			return !(lhs < rhs);
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator<=(const Val<T> & lhs, const Val<T> & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		template <
			template <class> class Val, class T
		> constexpr bool operator>=(const Val<T> & lhs, const Val<T> & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ALG
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		namespace alg
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static constexpr size_t strlen(CString value)
			{
				return ((*value) ? (1 + alg::strlen(value + 1)) : 0);
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class T
			> constexpr auto bit_read(T value, T bit)
				-> T
			{
				return ((value >> bit) & value_t<T>::one);
			}

			template <
				class T
			> constexpr auto bit_set(T & value, T bit)
				-> T &
			{
				return (value |= (value_t<T>::one << bit));
			}

			template <
				class T
			> constexpr auto bit_clear(T & value, T bit)
				-> T &
			{
				return (value &= ~(value_t<T>::one << bit));
			}

			template <
				class T
			> constexpr auto bit_write(T & value, T bit, T bitValue)
				-> T &
			{
				return (bitValue ? bit_set(value, bit) : bit_clear(value, bit));
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct hash
			{
				struct fnv1a final
				{
					static constexpr hash_t basis { 14695981039346656037ULL };
					static constexpr hash_t prime { 1099511628211ULL };
				};

				template <
					class T
				> constexpr hash_t operator()(size_t size, const T * value, hash_t seed)
				{
					return ((size > 0)
						? operator()(size - 1, value + 1, (seed ^ (*value)) * fnv1a::prime)
						: seed);
				}

				template <
					class T
				> constexpr hash_t operator()(size_t size, const T * arr)
				{
					return operator()(size, arr, fnv1a::basis);
				}

				template <
					class T, size_t N
				> constexpr hash_t operator()(const T(&value)[N])
				{
					return operator()(N - 1, value);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			namespace impl
			{
				/* * * * * * * * * * * * * * * * * * * * */

				template <
					class ... Args
				> struct sqrt;

				/* * * * * * * * * * * * * * * * * * * * */

				template <> struct sqrt<size_t>
				{
					using type = typename value_t<size_t>;

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
					using type = typename value_t<double>;

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
						return value_t<To>{ sqrt<From>{}(value_t<From>{ value }()) }();
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
				using type = typename value_t<T>;
				constexpr auto operator()(T value) const
				{
					return alg::impl::sqrt<T>{}(value);
				}
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class T
			> static constexpr auto sign(const T & value)
				-> T
			{
				return ((value == value_t<T>::zero)
					? value_t<T>::zero
					: ((value < value_t<T>::zero)
						? value_t<T>::minus_one
						: value_t<T>::one));
			}

			template <
				class T
			> static constexpr auto abs(const T & value)
				-> T
			{
				return ((sign(value) < value_t<T>::zero)
					? (value * value_t<T>::minus_one)
					: value);
			}

			template <
				class T, class E
			> static constexpr auto pow(const T & base, const E & exp)
				-> T
			{

				return ((exp < value_t<E>::zero)
					? ((base == value_t<T>::zero)
						? value_t<T>::nan
						: value_t<T>::one / (base * alg::pow(base, (-exp) - value_t<E>::one)))
					: ((exp == value_t<E>::zero)
						? value_t<T>::one
						: ((exp == value_t<E>::one)
							? base
							: base * alg::pow(base, exp - value_t<E>::one))));
			}

			template <
				class T
			> static constexpr auto fact(const T & value)
				-> T
			{
				return ((value > value_t<T>::one)
					? value * alg::fact(value - value_t<T>::one)
					: value_t<T>::one);
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
				-> const T &
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

			template <
				class Tx
			> static constexpr auto cos(const Tx & x)
			{
				return gcem::cos(x);
			}

			/* * * * * * * * * * * * * * * * * * * * */

			template <
				class Tx
			> static constexpr auto asin(const Tx & x)
			{
				return gcem::asin(x);
			}

			template <
				class Tx
			> static constexpr auto acos(const Tx & x)
			{
				return gcem::acos(x);
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
			> static constexpr auto atan2(const Tx & x, const Ty & y)
			{
				return gcem::atan2(x, y);
			}


			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			namespace range
			{
				/* * * * * * * * * * * * * * * * * * * * */

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

				/* * * * * * * * * * * * * * * * * * * * */

				template <
					class Lhs, class Rhs
				> static constexpr bool equals(Lhs lBegin, Lhs lEnd, Rhs rBegin, Rhs rEnd)
				{
					return ((lBegin != lEnd && rBegin != rEnd)
						? ((*lBegin) == (*rBegin))
							&& equals((lBegin + 1), lEnd, (rBegin + 1), rEnd)
						: ((lBegin == lEnd) && (rBegin == rEnd))
					);
				}

				template <
					template <class, size_t ...> class A, class T, size_t ... N
				> static constexpr bool equals(const A<T, N...> & lhs, const A<T, N...> & rhs)
				{
					using namespace meta::alg::range;
					return equals(begin(lhs), end(lhs), begin(rhs), end(rhs));
				}

				/* * * * * * * * * * * * * * * * * * * * */

				template <
					class Lhs, class Rhs
				> static constexpr bool less(Lhs lBegin, Lhs lEnd, Rhs rBegin, Rhs rEnd)
				{
					return ((lBegin != lEnd && rBegin != rEnd)
						? ((*lBegin) < (*rBegin))
							&& less((lBegin + 1), lEnd, (rBegin + 1), rEnd)
						: ((lBegin == lEnd) && (rBegin == rEnd))
					);
				}

				template <
					template <class, size_t ...> class A, class T, size_t ... N
				> static constexpr bool less(const A<T, N...> & lhs, const A<T, N...> & rhs)
				{
					using namespace meta::alg::range;
					return less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
				}

				/* * * * * * * * * * * * * * * * * * * * */

				template <
					class OutIt, class InIt
				> static constexpr auto copy(OutIt first, OutIt last, InIt d_first)
				{
					while (first != last)
					{
						*(first++) = *(d_first++);
					}
					return d_first;
				}

				template <
					template <class, size_t ...> class A, class T, size_t ... N
				> static constexpr auto copy(A<T, N...> & lhs, const A<T, N...> & rhs)
					-> A<T, N...> &
				{
					alg::range::copy(lhs.begin(), lhs.end(), rhs.begin());
					return lhs;
				}

				template <
					template <class, size_t ...> class A, class T, size_t ... N
				> static constexpr auto copy(const A<T, N...> & value)
					-> A<T, N...>
				{
					A<T, N...> temp { uninit };
					alg::range::copy(temp, value);
					return temp;
				}

				/* * * * * * * * * * * * * * * * * * * * */

				template <
					class OutIt, class T
				> static constexpr auto fill(OutIt first, OutIt last, T value)
					-> OutIt
				{
					while (first != last)
					{
						*(first++) = value;
					}
					return first;
				}

				template <
					template <class, size_t ...> class A, class T, size_t ... N
				> static constexpr auto fill(A<T, N...> & arr, const T & value)
					-> A<T, N...> &
				{
					alg::range::fill(arr.begin(), arr.end(), value);
					return arr;
				}

				template <
					template <class, size_t ...> class A, class T, size_t ... N
				> static constexpr auto fill(const T & value)
					-> A<T, N...>
				{
					A<T, N...> temp { uninit };
					alg::range::fill(temp, value);
					return temp;
				}

				/* * * * * * * * * * * * * * * * * * * * */

				template <
					template <class, size_t ...> class A, class T, size_t ... N
				> static constexpr void swap(A<T, N...> & lhs, A<T, N...> & rhs)
				{
					for (size_t i = 0; i < lhs.size(); i++)
					{
						alg::swap(lhs[i], rhs[i]);
					}
				}

				/* * * * * * * * * * * * * * * * * * * * */
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class T, class C
			> static constexpr auto lerp(const T & a, const T & b, const C & coeff)
				-> T
			{
				return (a * coeff + b * (value_t<C>::one - coeff));
			}

			template <
				class T
			> static constexpr auto map(
				const T & value,
				const T & aMin, const T & aMax,
				const T & bMin, const T & bMax
			) -> T
			{
				return (bMin + (value - aMin) * (bMax - bMin) / (aMax - aMin));
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				template <class, size_t ...> class A, class T, size_t ... N
			> static constexpr auto sqr_magnitude(const A<T, N...> & value)
				-> T
			{
				T temp { value_t<T>::zero };
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
				return value_t<T> { sqrt<T> {}(sqr_magnitude(value)) }();
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
				T temp { value_t<T>::zero };
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
				return ((det != value_t<T>::zero)
				? M<T, 4, 4>
				{	+(value[15] * value[5] - value[7] * value[13]) / det,
					-(value[15] * value[4] - value[7] * value[12]) / det,
					+(value[13] * value[4] - value[5] * value[12]) / det,
					-(value[15] * value[1] - value[3] * value[13]) / det,
					+(value[15] * value[0] - value[3] * value[12]) / det,
					-(value[13] * value[0] - value[1] * value[12]) / det,
					+(value[ 7] * value[1] - value[3] * value[ 5]) / det,
					-(value[ 7] * value[0] - value[3] * value[ 4]) / det,
					+(value[ 5] * value[0] - value[1] * value[ 4]) / det
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
			> static constexpr auto rebase(const M<T, 3, 1> & v, const M<T, 4, 4> & m)
				-> M<T, 3, 1>
			{
				return M<T, 3, 1>
				{
					m[0] * v[0] * m[4] * v[1] * m[ 8] * v[2] * m[12],
					m[1] * v[0] * m[5] * v[1] * m[ 9] * v[2] * m[13], 
					m[2] * v[0] * m[6] * v[1] * m[10] * v[2] * m[14]
				};
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// C_STRING
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		// STRUCT C_STRING
		/* * * * * * * * * * * * * * * * * * * * */

		struct c_string final
		{
			/* * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename char;
			using self_type			= typename c_string;
			using pointer			= typename value_type *;
			using reference			= typename value_type &;
			using const_pointer		= typename const value_type *;
			using const_reference	= typename const value_type &;

			template <
				size_t N
			> constexpr c_string(const value_type(& value)[N])
				: self_type { &value[0], (N - 1) }
			{
			}

			constexpr c_string(const_pointer begin, const_pointer end)
				: self_type { begin, value_t<size_t>{ end - begin }() }
			{
			}

			constexpr c_string(const_pointer value)
				: self_type { value, alg::strlen(value) }
			{
			}

			constexpr c_string(const_pointer value, size_t size)
				: m_data { value }
				, m_size { size }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * */

			constexpr auto begin()	const -> const_pointer	{ return m_data; }
			constexpr auto cbegin() const -> const_pointer	{ return begin(); }
			constexpr auto cend()	const -> const_pointer	{ return end(); }
			constexpr auto c_str()	const -> const_pointer	{ return begin(); }
			constexpr auto end()	const -> const_pointer	{ return begin() + size(); }
			constexpr auto hash()	const -> hash_t			{ return alg::hash()(size(), begin()); }
			constexpr auto size()	const -> size_t			{ return m_size; }
			ML_INLINE auto str()	const -> String			{ return c_str(); }

			/* * * * * * * * * * * * * * * * * * * * */

			constexpr operator const_pointer() const { return c_str();  }

			constexpr value_type operator[](size_t i) const { return m_data[i]; }
			
			constexpr const_pointer operator()(size_t i) const { return (m_data + i); }

			constexpr self_type operator()(size_t begin, size_t end) const
			{
				return self_type { m_data + begin, m_data + end };
			}

			constexpr self_type pad(size_t begin_off, size_t end_off) const
			{
				return self_type::operator()(begin_off, size() - end_off);
			}

			ML_INLINE friend std::ostream & operator<<(std::ostream & out, const self_type & value)
			{
				for (const auto & elem : value)
				{
					out << elem;
				}
				return out;
			}

			/* * * * * * * * * * * * * * * * * * * * */

		private:
			const_pointer	m_data;
			const size_t	m_size;
		};

		// OPERATORS
		/* * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator==(const c_string & lhs, const c_string & rhs)
		{
			return alg::range::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		constexpr bool operator!=(const c_string & lhs, const c_string & rhs)
		{
			return !(lhs == rhs);
		}

		constexpr bool operator<(const c_string & lhs, const c_string & rhs)
		{
			return alg::range::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		constexpr bool operator>(const c_string & lhs, const c_string & rhs)
		{
			return !(lhs > rhs);
		}

		constexpr bool operator<=(const c_string & lhs, const c_string & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		constexpr bool operator>=(const c_string & lhs, const c_string & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ARRAY_T
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		// STRUCT ARRAY_T
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> struct array_t
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static_assert(0 < N, "array_t : size negative or zero");

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum { Size = N };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename T;
			using type				= typename value_t<value_type>;
			using data_type			= typename value_type[Size];
			using self_type			= typename array_t<value_type, Size>;
			using pointer			= typename value_type *;
			using reference			= typename value_type &;
			using const_pointer		= typename const value_type *;
			using const_reference	= typename const value_type &;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			data_type m_data; // aggregate initializer

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto begin()			-> pointer			{ return data(); }
			constexpr auto begin()	const	-> const_pointer	{ return data(); }
			constexpr auto cbegin() const	-> const_pointer	{ return begin(); }
			constexpr auto cend()	const	-> const_pointer	{ return end(); }
			constexpr auto data()			-> pointer			{ return m_data; }
			constexpr auto data()	const	-> const_pointer	{ return m_data; }
			constexpr auto end()			-> pointer			{ return data() + size(); }
			constexpr auto end()	const	-> const_pointer	{ return data() + size(); }
			constexpr auto hash()	const	-> hash_t			{ return alg::hash()(size(), data()); }
			constexpr auto size()	const	-> size_t			{ return Size; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr const_reference operator[](size_t i) const { return m_data[i]; };

			constexpr reference operator[](size_t i) { return m_data[i]; };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static constexpr auto fill(const_reference value)
			{
				self_type temp { uninit };
				alg::range::fill(temp, value);
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_INLINE friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
			{
				for (const auto & elem : rhs)
				{
					out << elem << " ";
				}
				return out;
			}

			ML_INLINE friend std::istream & operator>>(std::istream & in, self_type & rhs)
			{
				for (auto & elem : rhs)
				{
					in >> elem;
				}
				return in;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};
		
		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// MATRIX_T
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		// STRUCT MATRIX_T
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t X, size_t Y
		> struct matrix_t
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static_assert(0 < (X * Y), "matrix_t : size negative or zero");

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum { Cols = X, Rows = Y, Size = Cols * Rows };

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
			constexpr auto hash()	const	-> hash_t			{ return m_data.hash(); }
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
				for (size_t i = 0; i < self_type::Size; i++)
				{
					temp[i] = ((i < temp.size())
						? value_t<U>{ (*this)[i] }()
						: value_t<U>::zero
					);
				}
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			ML_INLINE friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
			{
				for (size_t y = 0; y < rhs.height(); y++)
				{
					for (size_t x = 0; x < rhs.width(); x++)
					{
						out << rhs[y * rhs.width() + x] 
							<< ((x < rhs.width() - 1) ? " " : "\n");
					}
				}
				return out;
			}

			ML_INLINE friend std::istream & operator>>(std::istream & in, self_type & rhs)
			{
				return in >> rhs.m_data;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static constexpr self_type identity()
			{
				self_type temp { uninit };
				for (size_t i = 0; i < temp.size(); i++)
				{
					temp[i] = (((i / temp.cols()) == (i % temp.cols()))
						? type::one
						: type::zero);
				}
				return temp;
			}

			static constexpr self_type ortho(
				const_reference l, 
				const_reference r, 
				const_reference b, 
				const_reference t
			)
			{	static_assert((X == 4 && Y == 4), "matrix must be 4x4");
				self_type temp { identity() };
				temp[ 0] = type::two / (r - l);
				temp[ 5] = type::two / (t - b);
				temp[10] = type::one;
				temp[12] = -(r + l) / (r - l);
				temp[13] = -(t + b) / (t - b);
				return temp;
			}

			static constexpr self_type persp(
				const_reference fov,
				const_reference aspect,
				const_reference near, 
				const_reference far
			)
			{	static_assert((X == 4 && Y == 4), "matrix must be 4x4");
				self_type temp { uninit };
				temp[ 0] = type::one / (aspect * alg::tan(fov / type::two));
				temp[ 5] = type::one / alg::tan(fov / type::two);
				temp[11] = type::minus_one;
				temp[10] = -(far + near) / (far - near);
				temp[14] = -(type::two * far * near) / (far - near);
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(GLM_VERSION)

			ML_INLINE operator glm::tvec2<value_type, glm::defaultp>() const
			{
				static_assert((X == 2 && Y == 1), "unable to convert to glm::tvec2");
				return glm::tvec2<value_type, glm::defaultp>(
					(*this)[0], (*this)[1]
				);
			}

			ML_INLINE operator glm::tvec3<value_type, glm::defaultp>() const
			{
				static_assert((X == 3 && Y == 1), "unable to convert to glm::tvec3");
				return glm::tvec3<value_type, glm::defaultp>(
					(*this)[0], (*this)[1], (*this)[2]
				);
			}

			ML_INLINE operator glm::tvec4<value_type, glm::defaultp>() const
			{
				static_assert((X == 4 && Y == 1), "unable to convert to glm::tvec4");
				return glm::tvec4<value_type, glm::defaultp>(
					(*this)[0], (*this)[1], (*this)[2], (*this)[3]
				);
			}

			ML_INLINE operator glm::tquat<value_type, glm::defaultp>() const
			{
				static_assert((X == 4 && Y == 1), "unable to convert to glm::tquat");
				return glm::tquat<value_type, glm::defaultp>(
					(*this)[0], (*this)[1], (*this)[2], (*this)[3]
				);
			}

			ML_INLINE operator glm::tmat3x3<value_type, glm::defaultp>() const
			{
				static_assert((X == 3 && Y == 3), "unable to convert to glm::tmat3x3");
				return glm::tmat3x3<value_type, glm::defaultp>(
					(*this)[0], (*this)[1], (*this)[2],
					(*this)[3], (*this)[4], (*this)[5],
					(*this)[6], (*this)[7], (*this)[8]
				);
			}

			ML_INLINE operator glm::tmat4x4<value_type, glm::defaultp>() const
			{
				static_assert((X == 4 && Y == 4), "unable to convert to glm::tmat4x4");
				return glm::tmat4x4<value_type, glm::defaultp>(
					(*this)[ 0], (*this)[ 1], (*this)[ 2], (*this)[ 3],
					(*this)[ 4], (*this)[ 5], (*this)[ 6], (*this)[ 7],
					(*this)[ 8], (*this)[ 9], (*this)[10], (*this)[11],
					(*this)[12], (*this)[13], (*this)[14], (*this)[15]
				);
			}

# endif // GLM_VERSION

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		// MATRIX NxN
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T, size_t N>
		using tmat_nxn = matrix_t<T, N, N>;

		template <class T, size_t N>
		using tvec_n = matrix_t<T, N, 1>;

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
		using tvec2 = tvec_n<T, 2>;
		using vec2b = tvec2<bool>;
		using vec2f = tvec2<float>;
		using vec2i = tvec2<int32_t>;
		using vec2d = tvec2<double>;
		using vec2u = tvec2<uint32_t>;
		using vec2	= vec2f;

		// VECTOR3
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tvec3 = tvec_n<T, 3>;
		using vec3b = tvec3<bool>;
		using vec3f = tvec3<float>;
		using vec3i = tvec3<int32_t>;
		using vec3d = tvec3<double>;
		using vec3u = tvec3<uint32_t>;
		using vec3	= vec3f;

		// VECTOR4
		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tvec4 = tvec_n<T, 4>;
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

		// OPERATORS
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator==(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return alg::range::equals(lhs, rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator!=(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator<(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return alg::range::less(lhs, rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator<=(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator>(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return !(lhs < rhs) && (lhs != rhs);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr bool operator>=(const M<T, N...> & lhs, const M<T, N...> & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+=(M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] + rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-=(M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] - rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator*=(M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] * rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator/=(M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] / rhs[i]);
			}
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+(const M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-(const M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator*(const M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator/(const M<T, N...> & lhs, const M<T, N...> & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+=(M<T, N...> & lhs, const T & rhs)
			-> M<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem += rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-=(M<T, N...> & lhs, const T & rhs)
			-> M<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem -= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator*=(M<T, N...> & lhs, const T & rhs)
			-> M<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem *= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator/=(M<T, N...> & lhs, const T & rhs)
			-> M<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem /= rhs;
			}
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+(const M<T, N...> & lhs, const T & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-(const M<T, N...> & lhs, const T & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator*(const M<T, N...> & lhs, const T & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator/(const M<T, N...> & lhs, const T & rhs)
			-> M<T, N...>
		{
			M<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator-(const M<T, N...> & lhs)
			-> M<T, N...>
		{
			return (lhs * value_t<T>::minus_one);
		}

		template <
			template <class, size_t ...> class M, class T, size_t ... N
		> constexpr auto operator+(const M<T, N...> & lhs)
			-> M<T, N...>
		{
			return -(-(lhs));
		}

		/* * * * * * * * * * * * * * * * * * * * */
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
				: self_type { pos, col, { uninit } }
			{
			}

			constexpr explicit vertex(const vec3 & pos, const vec2 & tex)
				: self_type { pos, { uninit }, tex }
			{
			}

			constexpr explicit vertex(const vec4 & col)
				: self_type { { uninit }, col, { uninit } }
			{
			}

			constexpr explicit vertex(const vec3 & pos)
				: self_type { pos, { uninit }, { uninit } }
			{
			}

			constexpr explicit vertex(const vec2 & tex)
				: self_type { { uninit }, { uninit }, tex }
			{
			}

			constexpr vertex(const vertex & copy)
				: self_type { copy.pos(), copy.col(), copy.tex() }
			{
			}

			constexpr vertex()
				: self_type { { uninit }, { uninit }, { uninit } }
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

			ML_INLINE friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
			{
				return out << rhs.m_data;
			}

			ML_INLINE friend std::istream & operator>>(std::istream & in, self_type & rhs)
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

					using vertices_t	= typename array_t<vertex,		num::vertices>;
					using indices_t		= typename array_t<uint32_t,	num::indices>;
					using contiguous_t	= typename array_t<float,		num::contiguous>;

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