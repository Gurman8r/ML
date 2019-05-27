#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

// Sources:
// https://stackoverflow.com/questions/8622256/in-c11-is-sqrt_t-defined-as-constexpr
// https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/cstring.hpp

#include <ML/Core/GLM.hpp>
#include <ML/Core/StandardLib.hpp>

#define ML_META ::ml::meta::

// VALUE_T
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum { uninitialize };

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> struct value_t final
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
				> constexpr type operator()(const U & value) const noexcept
				{
					return static_cast<type>(value);
				}
			};

			struct epsilon_v final
			{
				constexpr type operator()(type seed) const noexcept
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

			static constexpr type	infinity		{ limits_t::infinity() };
			static constexpr type	nan				{ limits_t::quiet_NaN() };
			static constexpr type	min				{ limits_t::min() };
			static constexpr type	max				{ limits_t::max() };
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
			static constexpr type	pi				{ cast_v()(3.14159265358979323846264338327L) };
			static constexpr type	two_pi			{ pi * two };
			static constexpr type	half_pi			{ pi * half };
			static constexpr type	quarter_pi		{ pi * quarter };
			static constexpr type	third_pi		{ pi * third };
			static constexpr type	deg2rad			{ pi / one_eighty };
			static constexpr type	rad2deg			{ one_eighty / pi };
			static constexpr type	epsilon			{ epsilon_v()(half) };
			static constexpr type	half_epsilon	{ epsilon * half };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class U
			> constexpr explicit value_t(const U & value) noexcept
				: m_value { cast_v()(value) }
			{
			}

			constexpr value_t() noexcept : self_type { zero } {}
			
			constexpr type operator()() const noexcept { return m_value; }

			constexpr operator type() const noexcept { return (*this)(); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private: type m_value;
		};
		
		/* * * * * * * * * * * * * * * * * * * * */
		
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ALGORITHMS
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		namespace alg
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct hash final
			{
				using type = typename hash_t;

				struct fnv1a
				{
					static constexpr type basis { 14695981039346656037ULL };
					static constexpr type prime { 1099511628211 };
				};

				template <
					class T
				> constexpr type operator()(size_t size, const T * arr, type seed)
				{
					return ((size > 0)
						? operator()(size - 1, arr + 1, (seed ^ *arr) * fnv1a::prime)
						: seed);
				}

				template <
					class T
				> constexpr type operator()(size_t size, const T * arr)
				{
					return operator()(size, arr, fnv1a::basis);
				}

				template <
					class T, size_t N
				> constexpr type operator()(const T(&value)[N])
				{
					return operator()(value);
				}
			};

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

			namespace impl
			{
				/* * * * * * * * * * * * * * * * * * * * */

				template <
					class T
				> struct sqrt_t;

				/* * * * * * * * * * * * * * * * * * * * */

				template <> struct sqrt_t<size_t>
				{
					using type_v = value_t<size_t>;

					constexpr explicit sqrt_t(size_t value)
						: m_value { sqrt_t { type_v::one, type_v::three, value }() }
					{
					}

					constexpr explicit sqrt_t(size_t value, size_t curr, size_t prev)
						: m_value { ((value <= prev)
						? sqrt_t { value + curr, curr + type_v::two, prev }()
						: ((curr >> type_v::one) - type_v::one)) }
					{
					}

					constexpr size_t operator()() const { return m_value; }

				private: size_t m_value;
				};

				/* * * * * * * * * * * * * * * * * * * * */

				template <> struct sqrt_t<double>
				{
					using type_v = value_t<double>;

					constexpr explicit sqrt_t(double value)
						: m_value { ((value >= type_v::zero && value < type_v::infinity)
						? sqrt_t { value, value, type_v::zero }()
						: type_v::nan) }
					{
					}

					constexpr explicit sqrt_t(double value, double curr, double prev)
						: m_value { ((curr == prev)
						? curr
						: sqrt_t { value, type_v::half * (curr + value / curr), curr }()) }
					{
					}

					constexpr double operator()() const { return m_value; }

				private: double m_value;
				};

				/* * * * * * * * * * * * * * * * * * * * */

				template <
					class To, class From
				> struct sqrt_cast
				{
					template <
						class U
					> constexpr explicit sqrt_cast(U value)
						: m_value { value_t<To>{ sqrt_t<From>{ value_t<From>{ value } }() } }
					{
					}

					constexpr To operator()() const { return m_value; }

				private: To m_value;
				};

				/* * * * * * * * * * * * * * * * * * * * */
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class T
			> struct sqrt_t;

			template <> struct sqrt_t<size_t>
			{
				constexpr size_t operator()(size_t value) const
				{
					return impl::sqrt_t<size_t>{ value }();
				}
			};

			template <> struct sqrt_t<intmax_t>
			{
				constexpr intmax_t operator()(intmax_t value) const
				{
					return impl::sqrt_cast<intmax_t, size_t>{ value }();
				}
			};

			template <> struct sqrt_t<double>
			{
				constexpr double operator()(double value) const
				{
					return impl::sqrt_t<double>{ value }();
				}
			};

			template <> struct sqrt_t<float>
			{
				constexpr float operator()(float value) const
				{
					return  impl::sqrt_cast<float, double>{ value }();
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
				class T, class Exp
			> static constexpr auto power(const T & base, const Exp & exp)
				-> T
			{
				return ((exp < value_t<Exp>::zero)
					? ((base == value_t<T>::zero)
						? value_t<T>::nan
						: value_t<T>::one / (base * power(base, (-exp) - value_t<Exp>::one)))
					: ((exp == value_t<Exp>::zero)
						? value_t<T>::one
						: ((exp == value_t<Exp>::one)
							? base
							: base * power(base, exp - value_t<Exp>::one))));
			}

			template <
				class T
			> static constexpr auto factorial(const T & value)
				-> T
			{
				return ((value > value_t<T>::one)
					? value * factorial(value - value_t<T>::one)
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

			namespace trig
			{
				template <
					class T
				> static constexpr auto sin(const T & value)
					-> T
				{
					static_assert(0, "sin NYI");
					T temp { value_t<T>::zero };
					return temp;
				}

				template <
					class T
				> static constexpr auto cos(const T & value)
					-> T
				{
					static_assert(0, "cos NYI");
					T temp { value_t<T>::zero };
					return temp;
				}

				template <
					class T
				> static constexpr auto tan(const T & value)
					-> T
				{
					static_assert(0, "tan NYI");
					T temp { value_t<T>::zero };
					return temp;
				}
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			namespace range
			{
				/* * * * * * * * * * * * * * * * * * * * */

				template <
					class T, size_t N
				> static constexpr auto begin(const T(&arr)[N])
					-> const T *
				{
					return (&arr[0]);
				}

				template <
					class T, size_t N
				> static constexpr auto end(const T(& arr)[N])
					-> const T *
				{
					return (&arr[N]);
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
					return equals(begin(lhs), end(lhs), begin(rhs), end(rhs));
				}

				/* * * * * * * * * * * * * * * * * * * * */

				template <
					class Left, class Right
				> static constexpr bool less(Left lBegin, Left lEnd, Right rBegin, Right rEnd)
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
					return less(begin(lhs), end(lhs), begin(rhs), end(rhs));
				}

				/* * * * * * * * * * * * * * * * * * * * */

				template <
					template <class, size_t ...> class A, class T, size_t ... N
				> static constexpr void copy(A<T, N...> & lhs, const A<T, N...> & rhs)
				{
					for (size_t i = 0; i < lhs.size(); i++)
					{
						lhs[i] = rhs[i];
					}
				}

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
				return value_t<T> { sqrt_t<T>{}(sqr_magnitude(value)) };
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
				A<T, N...> temp { uninitialize };
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

// XSTRING
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		struct xstring final
		{
			/* * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename char;
			using self_type			= typename xstring;
			using hash_type			= typename alg::hash::type;
			using size_type			= typename size_t;
			using pointer			= typename value_type *;
			using reference			= typename value_type &;
			using const_pointer		= typename const value_type *;
			using const_reference	= typename const value_type &;

			template <
				size_type N
			> constexpr xstring(const value_type(& str)[N])
				: self_type { &str[0], (N - 1) }
			{
			}

			constexpr xstring(const_pointer begin, size_type size)
				: m_str { begin }
				, m_size { size }
			{
			}

			constexpr xstring(const_pointer begin, const_pointer end)
				: self_type { begin, value_t<size_type>{ end - begin } }
			{
			}

			constexpr xstring(const_pointer begin)
				: self_type { begin, length(begin) }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * */

			constexpr auto begin()	const -> const_pointer	{ return m_str; }
			constexpr auto cbegin() const -> const_pointer	{ return begin(); }
			constexpr auto cend()	const -> const_pointer	{ return end(); }
			constexpr auto c_str()	const -> const_pointer	{ return begin(); }
			constexpr auto end()	const -> const_pointer	{ return begin() + size(); }
			constexpr auto hash()	const -> hash_type		{ return alg::hash()(size(), begin()); }
			constexpr auto size()	const -> size_type		{ return m_size; }

			/* * * * * * * * * * * * * * * * * * * * */

			static constexpr auto length(const_pointer value)
				-> size_type
			{
				return (*value) ? 1 + length(value + 1) : 0;
			}

			/* * * * * * * * * * * * * * * * * * * * */

			constexpr operator const_pointer() const 
			{
				return c_str(); 
			}

			constexpr auto operator[](size_type i) const
				-> value_type 
			{ 
				return m_str[i]; 
			}
			
			constexpr auto operator()(size_type i) const
				-> const_pointer 
			{
				return (m_str + i); 
			}

			constexpr auto operator()(size_type begin, size_type end) const
				-> self_type
			{
				return self_type { m_str + begin, m_str + end };
			}

			constexpr auto pad(size_type begin_off, size_type end_off) const
				-> self_type
			{
				return self_type::operator()(begin_off, size() - end_off);
			}

			friend std::ostream & operator<<(std::ostream & out, const self_type & value)
			{
				for (const auto & elem : value)
				{
					out << elem;
				}
				return out;
			}

			/* * * * * * * * * * * * * * * * * * * * */

		private:
			const_pointer	m_str;
			const size_type m_size;
		};

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ARRAY
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> struct array_t final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static_assert((0 < N), "size negative or zero");

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum { Size = N };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename T;
			using base_type			= typename value_type[Size];
			using self_type			= typename array_t<value_type, Size>;
			using hash_type			= typename alg::hash::type;
			using size_type			= typename size_t;
			using pointer			= typename value_type *;
			using reference			= typename value_type &;
			using const_pointer		= typename const value_type *;
			using const_reference	= typename const value_type &;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			base_type m_data;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto begin()			-> pointer			{ return data(); }
			constexpr auto begin()	const	-> const_pointer	{ return data(); }
			constexpr auto cbegin() const	-> const_reference	{ return begin(); }
			constexpr auto cend()	const	-> const_reference	{ return end(); }
			constexpr auto data()			-> pointer			{ return m_data; }
			constexpr auto data()	const	-> const_pointer	{ return m_data; }
			constexpr auto end()			-> pointer			{ return data() + size(); }
			constexpr auto end()	const	-> const_pointer	{ return data() + size(); }
			constexpr auto hash()	const	-> hash_type		{ return alg::hash()(size(), data()); }
			constexpr auto size()	const	-> size_type		{ return Size; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr const_reference operator[](size_type i) const { return m_data[i]; };

			constexpr reference operator[](size_type i) { return m_data[i]; };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
			{
				for (const auto & elem : rhs)
				{
					out << elem << " ";
				}
				return out;
			}

			inline friend std::istream & operator>>(std::istream & in, self_type & rhs)
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

// MATRIX
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t X, size_t Y
		> struct matrix_t final
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static_assert((0 < (X * Y)), "size negative or zero");

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			enum { Cols = X, Rows = Y, Size = Cols * Rows };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using value_type		= typename T;
			using base_type			= typename array_t<value_type, Size>;
			using self_type			= typename matrix_t<value_type, Cols, Rows>;
			using hash_type			= typename base_type::hash_type;
			using size_type			= typename base_type::size_type;
			using pointer			= typename base_type::pointer;
			using reference			= typename base_type::reference;
			using const_pointer		= typename base_type::const_pointer;
			using const_reference	= typename base_type::const_reference;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			base_type m_data;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto begin()			-> pointer			{ return m_data.begin(); }
			constexpr auto begin()	const	-> const_pointer	{ return m_data.begin(); }
			constexpr auto cbegin() const	-> const_reference	{ return m_data.cbegin(); }
			constexpr auto cend()	const	-> const_reference	{ return m_data.cend(); }
			constexpr auto cols()	const	-> size_type		{ return self_type::Cols; }
			constexpr auto data()			-> pointer			{ return m_data.data(); }
			constexpr auto data()	const	-> const_pointer	{ return m_data.data(); }
			constexpr auto end()			-> pointer			{ return m_data.end(); }
			constexpr auto end()	const	-> const_pointer	{ return m_data.end(); }
			constexpr auto hash()	const	-> hash_type		{ return m_data.hash(); }
			constexpr auto height()	const	-> size_type		{ return rows(); }
			constexpr auto rows()	const	-> size_type		{ return self_type::Rows; }
			constexpr auto size()	const	-> size_type		{ return m_data.size(); }
			constexpr auto width()	const	-> size_type		{ return cols(); }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr const_reference operator[](size_type i) const { return m_data[i]; }

			constexpr reference operator[](size_type i) { return m_data[i]; }

			constexpr operator base_type() const { return m_data; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline friend std::ostream & operator<<(std::ostream & out, const self_type & rhs)
			{
				return out << rhs.m_data;
			}

			inline friend std::istream & operator>>(std::istream & in, self_type & rhs)
			{
				return in >> rhs.m_data;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static constexpr self_type fill(const_reference value)
			{
				self_type temp { uninitialize };
				for (auto & elem : temp)
				{
					elem = value;
				}
				return temp;
			}

			static constexpr self_type copy(const self_type & value)
			{
				self_type temp { uninitialize };
				alg::range::copy(temp, value);
				return temp;
			}

			static constexpr self_type identity()
			{
				self_type temp { uninitialize };
				for (size_t i = 0; i < temp.size(); i++)
				{
					temp[i] = (((i / temp.cols()) == (i % temp.cols()))
						? value_t<T>::one
						: value_t<T>::zero);
				}
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(GLM_VERSION)

			inline operator glm::tvec2<value_type, glm::defaultp>() const noexcept
			{
				static_assert((X == 2 && Y == 1), "unable to convert to glm::tvec2");
				return glm::tvec2<value_type, glm::defaultp>(
					(*this)[0], (*this)[1]
				);
			}

			inline operator glm::tvec3<value_type, glm::defaultp>() const noexcept
			{
				static_assert((X == 3 && Y == 1), "unable to convert to glm::tvec3");
				return glm::tvec3<value_type, glm::defaultp>(
					(*this)[0], (*this)[1], (*this)[2]
				);
			}

			inline operator glm::tvec4<value_type, glm::defaultp>() const noexcept
			{
				static_assert((X == 4 && Y == 1), "unable to convert to glm::tvec4");
				return glm::tvec4<value_type, glm::defaultp>(
					(*this)[0], (*this)[1], (*this)[2], (*this)[3]
				);
			}

			inline operator glm::tquat<value_type, glm::defaultp>() const noexcept
			{
				static_assert((X == 4 && Y == 1), "unable to convert to glm::tquat");
				return glm::tquat<value_type, glm::defaultp>(
					(*this)[0], (*this)[1], (*this)[2], (*this)[3]
				);
			}

			inline operator glm::tmat3x3<value_type, glm::defaultp>() const noexcept
			{
				static_assert((X == 3 && Y == 3), "unable to convert to glm::tmat3x3");
				return glm::tmat3x3<value_type, glm::defaultp>(
					(*this)[0], (*this)[1], (*this)[2],
					(*this)[3], (*this)[4], (*this)[5],
					(*this)[6], (*this)[7], (*this)[8]
				);
			}

			inline operator glm::tmat4x4<value_type, glm::defaultp>() const noexcept
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

		template <class T, size_t N>
		using tmat_nxn = matrix_t<T, N, N>;

		template <class T, size_t N>
		using tmat_nx1 = matrix_t<T, N, 1>;

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tmat3 = tmat_nxn<T, 3>;
		using mat3b = tmat3<bool>;
		using mat3f = tmat3<float>;

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tmat4 = tmat_nxn<T, 4>;
		using mat4b = tmat4<bool>;
		using mat4f = tmat4<float>;

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tvec2 = tmat_nx1<T, 2>;
		using vec2b = tvec2<bool>;
		using vec2f = tvec2<float>;
		using vec2i = tvec2<int32_t>;
		using vec2d = tvec2<double>;
		using vec2u = tvec2<uint32_t>;

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tvec3 = tmat_nx1<T, 3>;
		using vec3b = tvec3<bool>;
		using vec3f = tvec3<float>;
		using vec3i = tvec3<int32_t>;
		using vec3d = tvec3<double>;
		using vec3u = tvec3<uint32_t>;

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		using tvec4 = tmat_nx1<T, 4>;
		using vec4b = tvec4<bool>;
		using vec4f = tvec4<float>;
		using vec4i = tvec4<int32_t>;
		using vec4d = tvec4<double>;
		using vec4u = tvec4<uint32_t>;

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

			enum { Size = 9 };

			using base_type = typename array_t<float, Size>;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr vertex()
				: vertex { { uninitialize }, { uninitialize }, { uninitialize } }
			{
			}

			constexpr explicit vertex(const vec3f & pos)
				: vertex { pos, { uninitialize }, { uninitialize } }
			{
			}

			constexpr explicit vertex(const vec3f & pos, const vec4f & col)
				: vertex { pos, col, { uninitialize } }
			{
			}

			constexpr explicit vertex(const vec3f & pos, const vec4f & col, const vec2f & tex)
				: m_pos { pos[0], pos[1], pos[2] }
				, m_col { col[0], col[1], col[2], col[3] }
				, m_tex { tex[0], tex[1] }
			{
			}

			constexpr vertex(const vertex & copy)
				: vertex { copy.pos(), copy.col(), copy.tex() }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr auto pos() const -> const vec3f & { return m_pos; }
			constexpr auto col() const -> const vec4f & { return m_col; }
			constexpr auto tex() const -> const vec2f & { return m_tex; }

			constexpr auto pos(const vec3f & value) -> vec3f & { return m_pos = value; }
			constexpr auto col(const vec4f & value) -> vec4f & { return m_col = value; }
			constexpr auto tex(const vec2f & value) -> vec2f & { return m_tex = value; }

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			constexpr float operator[](size_t i) const 
			{ 
				return ((base_type)(*this))[i]; 
			}

			constexpr operator base_type() const
			{
				return { 
					m_pos[0], m_pos[1], m_pos[2],
					m_col[0], m_col[1], m_col[2], m_col[3],
					m_tex[0], m_tex[1]
				};
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			inline friend std::ostream & operator<<(std::ostream & out, const vertex & value)
			{
				return out
					<< "{ " << value.pos() << "} "
					<< "{ " << value.col() << "} "
					<< "{ " << value.tex() << "} ";
			}
			
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private:
			vec3f m_pos;
			vec4f m_col;
			vec2f m_tex;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			size_t N
		> static constexpr auto contiguous(const array_t<vertex, N> & v)
			-> array_t<float, N * vertex::Size>
		{
			array_t<float, N * vertex::Size> temp { uninitialize };
			for (size_t i = 0; i < N * vertex::Size; i++)
			{
				temp[i] = (v[i / vertex::Size][i % vertex::Size]);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		namespace geometry
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			
			template <
				size_t V, size_t I
			> struct shape
			{
				struct num final
				{
					enum { Vertices = V, Indices = I, Size = Vertices * vertex::Size };
				};
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct triangle final : public shape<3, 3>
			{
				static constexpr array_t<vertex, num::Vertices> vertices 
				{ 
					vertex { {  0.0f,  0.5f, 0.0f }, { 1, 1, 1, 1 }, { 0.5f, 1.0f } },
					vertex { {  0.5f, -0.5f, 0.0f }, { 1, 1, 1, 1 }, { 1.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, 0.0f }, { 1, 1, 1, 1 }, { 0.0f, 0.0f } },
				};
				static constexpr array_t<size_t, num::Indices> indices 
				{
					0, 1, 2
				};
				static constexpr array_t<float, num::Size> contiguous
				{
					ML_META contiguous(vertices)
				};
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct quad final : public shape<4, 6>
			{
				static constexpr array_t<vertex, num::Vertices> vertices 
				{
					vertex { { +1.0f, +1.0f, 0.0f }, { 1, 1, 1, 1 }, { 1.0f, 1.0f } },
					vertex { { +1.0f, -1.0f, 0.0f }, { 1, 1, 1, 1 }, { 1.0f, 0.0f } },
					vertex { { -1.0f, -1.0f, 0.0f }, { 1, 1, 1, 1 }, { 0.0f, 0.0f } },
					vertex { { -1.0f, +1.0f, 0.0f }, { 1, 1, 1, 1 }, { 0.0f, 1.0f } },
				};
				static constexpr array_t<size_t, num::Indices> indices 
				{
					0, 1, 3,
					1, 2, 3
				};
				static constexpr array_t<float, num::Size> contiguous
				{
					 ML_META contiguous(vertices)
				};
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			struct cube final : public shape<24, 36>
			{
				static constexpr array_t<vertex, num::Vertices> vertices 
				{
					vertex { { +0.5f, +0.5f, -0.5f }, { 1, 1, 1, 1 }, { 1.0f, 1.0f } },
					vertex { { +0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, { 1.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 0.0f } },
					vertex { { -0.5f, +0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 1.0f } },
					vertex { { +0.5f, +0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 1.0f } },
					vertex { { +0.5f, -0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, +0.5f }, { 1, 1, 1, 1 }, { 0.0f, 0.0f } },
					vertex { { -0.5f, +0.5f, +0.5f }, { 1, 1, 1, 1 }, { 0.0f, 1.0f } },
					vertex { { +0.5f, +0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 1.0f } },
					vertex { { +0.5f, -0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 0.0f } },
					vertex { { +0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 0.0f } },
					vertex { { +0.5f, +0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 1.0f } },
					vertex { { -0.5f, +0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 1.0f } },
					vertex { { -0.5f, -0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 0.0f } },
					vertex { { -0.5f, +0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 1.0f } },
					vertex { { -0.5f, +0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 1.0f } },
					vertex { { +0.5f, +0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 0.0f } },
					vertex { { +0.5f, +0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 0.0f } },
					vertex { { -0.5f, +0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 1.0f } },
					vertex { { -0.5f, -0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 1.0f } },
					vertex { { +0.5f, -0.5f, +0.5f }, { 1, 1, 1, 1 }, { 1.0f, 0.0f } },
					vertex { { +0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 0.0f } },
					vertex { { -0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, { 0.0f, 1.0f } },
				};
				static constexpr array_t<size_t, num::Indices> indices 
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
				static constexpr array_t<float, num::Size> contiguous
				{
					 ML_META contiguous(vertices)
				};
			};

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// OPERATORS
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class> class V, class T
		> constexpr bool operator==(const V<T> & lhs, const V<T> & rhs)
		{
			return (lhs() == rhs());
		}

		template <
			template <class> class V, class T
		> constexpr bool operator!=(const V<T> & lhs, const V<T> & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			template <class> class V, class T
		> constexpr bool operator<(const V<T> & lhs, const V<T> & rhs)
		{
			return (lhs() < rhs());
		}

		template <
			template <class> class V, class T
		> constexpr bool operator>(const V<T> & lhs, const V<T> & rhs)
		{
			return !(lhs < rhs);
		}

		template <
			template <class> class V, class T
		> constexpr bool operator<=(const V<T> & lhs, const V<T> & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		template <
			template <class> class V, class T
		> constexpr bool operator>=(const V<T> & lhs, const V<T> & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator==(const xstring & lhs, const xstring & rhs)
		{
			return alg::range::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		constexpr bool operator!=(const xstring & lhs, const xstring & rhs)
		{
			return !(lhs == rhs);
		}

		constexpr bool operator<(const xstring & lhs, const xstring & rhs)
		{
			return alg::range::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}

		constexpr bool operator>(const xstring & lhs, const xstring & rhs)
		{
			return !(lhs > rhs);
		}

		constexpr bool operator<=(const xstring & lhs, const xstring & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		constexpr bool operator>=(const xstring & lhs, const xstring & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr bool operator==(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			return alg::range::equals(lhs, rhs);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr bool operator!=(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			return !(lhs == rhs);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr bool operator<(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			return alg::range::less(lhs, rhs);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr bool operator<=(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr bool operator>(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			return !(lhs < rhs) && (lhs != rhs);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr bool operator>=(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator+=(A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] + rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator-=(A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] - rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator*=(A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] * rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator/=(A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...> &
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] / rhs[i]);
			}
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator+(const A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...>
		{
			A<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator-(const A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...>
		{
			A<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator*(const A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...>
		{
			A<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator/(const A<T, N...> & lhs, const A<T, N...> & rhs)
			-> A<T, N...>
		{
			A<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator+=(A<T, N...> & lhs, const T & rhs)
			-> A<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem += rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator-=(A<T, N...> & lhs, const T & rhs)
			-> A<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem -= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator*=(A<T, N...> & lhs, const T & rhs)
			-> A<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem *= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator/=(A<T, N...> & lhs, const T & rhs)
			-> A<T, N...> &
		{
			for (auto & elem : lhs)
			{
				elem /= rhs;
			}
			return lhs;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator+(const A<T, N...> & lhs, const T & rhs)
			-> A<T, N...>
		{
			A<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator-(const A<T, N...> & lhs, const T & rhs)
			-> A<T, N...>
		{
			A<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator*(const A<T, N...> & lhs, const T & rhs)
			-> A<T, N...>
		{
			A<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator/(const A<T, N...> & lhs, const T & rhs)
			-> A<T, N...>
		{
			A<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator-(const A<T, N...> & lhs)
			-> A<T, N...>
		{
			return (lhs * value_t<T>::minus_one);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator+(const A<T, N...> & lhs)
			-> A<T, N...>
		{
			return -(-(lhs));
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ARRAY_HPP_