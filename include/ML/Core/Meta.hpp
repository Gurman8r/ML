#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

// Sources:
// https://stackoverflow.com/questions/8622256/in-c11-is-sqrt_t-defined-as-constexpr
// https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h

#include <ML/Core/Algorithm.hpp>
#include <ML/Core/Hash.hpp>
#include <ML/Core/Maths.hpp>

#define _ML_META ::ml::meta::

// VALUE TYPE
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> struct value_t
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			using type = typename T;

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static constexpr type calculate_epsilon(type eps) noexcept
			{
				auto temp { zero };
				while ((one + eps) != one)
				{
					temp = eps;
					eps /= two;
				}
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			static constexpr type infinity		{ std::numeric_limits<type>::infinity() };
			static constexpr type max			{ std::numeric_limits<type>::max() };
			static constexpr type min			{ std::numeric_limits<type>::min() };
			static constexpr type NaN			{ std::numeric_limits<type>::quiet_NaN() };
			static constexpr type minus_one		{ static_cast<type>( -1) };
			static constexpr type zero			{ static_cast<type>(  0) };
			static constexpr type one			{ static_cast<type>(  1) };
			static constexpr type two			{ static_cast<type>(  2) };
			static constexpr type three			{ static_cast<type>(  3) };
			static constexpr type four			{ static_cast<type>(  4) };
			static constexpr type five			{ static_cast<type>(  5) };
			static constexpr type six			{ static_cast<type>(  6) };
			static constexpr type seven			{ static_cast<type>(  7) };
			static constexpr type eight			{ static_cast<type>(  8) };
			static constexpr type nine			{ static_cast<type>(  9) };
			static constexpr type ten			{ static_cast<type>( 10) };
			static constexpr type fourtyfive	{ static_cast<type>( 45) };
			static constexpr type sixty			{ static_cast<type>( 60) };
			static constexpr type ninety		{ static_cast<type>( 90) };
			static constexpr type onehundred	{ static_cast<type>(100) };
			static constexpr type oneeighty		{ static_cast<type>(180) };
			static constexpr type threesixty	{ static_cast<type>(360) };
			static constexpr type half			{ one / two };
			static constexpr type third			{ one / three };
			static constexpr type quarter		{ one / four };
			static constexpr type fifth			{ one / five };
			static constexpr type sixth			{ one / six };
			static constexpr type seventh		{ one / seven };
			static constexpr type eighth		{ one / eight };
			static constexpr type ninth			{ one / nine };
			static constexpr type tenth			{ one / ten };
			static constexpr type twothirds		{ two / three };
			static constexpr type threequarters	{ three / four };
			static constexpr type pi			{ static_cast<type>(3.1415926535897932384650288L) };
			static constexpr type twopi			{ pi * two };
			static constexpr type halfpi		{ pi * half };
			static constexpr type deg2rad		{ pi / oneeighty };
			static constexpr type rad2deg		{ oneeighty / pi };
			static constexpr type epsilon		{ calculate_epsilon(half) };
			static constexpr type halfepsilon	{ epsilon * half };

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class U
			> constexpr explicit value_t(const U & value) noexcept
				: m_value { static_cast<type>(value) }
			{
			}

			constexpr value_t() : value_t { zero } {}
			
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
					constexpr explicit sqrt_t(size_t x)
						: m_value { sqrt_t { 1, 3, x }() }
					{
					}

					constexpr explicit sqrt_t(size_t x, size_t curr, size_t prev)
						: m_value { (x <= prev)
						? sqrt_t { x + curr, curr + 2, prev }()
						: ((curr >> 1) - 1) }
					{
					}

					constexpr size_t operator()() const { return m_value; }

				private: size_t m_value;
				};

				/* * * * * * * * * * * * * * * * * * * * */

				template <> struct sqrt_t<double>
				{
					constexpr explicit sqrt_t(double x)
						: m_value { x >= 0 && x < value_t<double>::infinity
						? sqrt_t { x, x, 0 }()
						: value_t<double>::NaN }
					{
					}

					constexpr explicit sqrt_t(double x, double curr, double prev)
						: m_value { curr == prev
						? curr
						: sqrt_t { x, value_t<double>::half * (curr + x / curr), curr }() }
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
				class Left, class Right
			> constexpr bool equal_range(Left lBegin, Left lEnd, Right rBegin, Right rEnd)
			{
				return ((lBegin != lEnd && rBegin != rEnd)
					? ((*lBegin) == (*rBegin))
						&& equal_range((lBegin + 1), lEnd, (rBegin + 1), rEnd)
					: ((lBegin == lEnd) && (rBegin == rEnd))
				);
			}

			template <
				class Left, class Right
			> constexpr bool less_range(Left lBegin, Left lEnd, Right rBegin, Right rEnd)
			{
				return ((lBegin != lEnd && rBegin != rEnd)
					? ((*lBegin) < (*rBegin))
						&& less_range((lBegin + 1), lEnd, (rBegin + 1), rEnd)
					: ((lBegin == lEnd) && (rBegin == rEnd))
				);
			}

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
			> static constexpr auto power(const T & base, const E & exp)
				-> T
			{
				return ((exp < value_t<E>::zero)
					? ((base == value_t<T>::zero)
						? value_t<T>::NaN
						: value_t<T>::one / (base * power(base, (-exp) - value_t<E>::one)))
					: ((exp == value_t<E>::zero)
						? value_t<T>::one
						: ((exp == value_t<E>::one)
							? base
							: base * power(base, exp - value_t<E>::one))));
			}

			template <
				class T
			> static constexpr auto factorial(const T & value)
				-> T
			{
				return ((value < value_t<T>::zero)
					? value_t<T>::one
					: value * factorial(value - value_t<T>::one));
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

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class T
			> static constexpr auto sin(const T & value)
				-> T
			{
				static_assert(0, "NYI");
				auto temp { value_t<T>::zero };
				return temp;
			}

			template <
				class T
			> static constexpr auto cos(const T & value)
				-> T
			{
				static_assert(0, "NYI");
				auto temp { value_t<T>::zero };
				return temp;
			}

			template <
				class T
			> static constexpr auto tan(const T & value)
				-> T
			{
				static_assert(0, "NYI");
				auto temp { value_t<T>::zero };
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			template <
				class T, class Coeff
			> static constexpr auto lerp(const T & a, const T & b, Coeff c)
				-> T
			{
				return (a * c + b * (value_t<Coeff>::one - c));
			}

			template <
				class T
			> static constexpr auto map_range(
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
				T out { value_t<T>::zero };
				for (const auto & elem : value)
				{
					out += (elem * elem);
				}
				return out;
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
				T value { value_t<T>::zero };
				for (size_t i = 0; i < lhs.size(); i++)
				{
					value += (lhs[i] * rhs[i]);
				}
				return value;
			}

			template <
				template <class, size_t ...> class A, class T, size_t ... N
			> static constexpr auto transpose(const A<T, N...> & value)
				-> A<T, N...>
			{
				A<T, N...> temp { value_t<T>::zero };
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
				template <class, size_t, size_t> class M, class T, size_t X, size_t Y,
				class = typename std::enable_if<X == 4 && Y == 4>::type
			> static constexpr auto determinant(const M<T, X, Y> & value)
				-> T
			{
				return
					value[0] * (value[15] * value[5] - value[7] * value[13]) -
					value[1] * (value[15] * value[4] - value[7] * value[12]) +
					value[3] * (value[13] * value[4] - value[5] * value[12]);
			}

			template <
				template <class, size_t, size_t> class M, class T, size_t X, size_t Y,
				class = typename std::enable_if<X == 4 && Y == 4>::type
			> static constexpr auto inverse(const M<T, X, Y> & value)
				-> M<T, X, Y>
			{
				const T det { determinant(value) };
				return ((det != value_t<T>::zero)
				? M<T, X, Y>
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
				: M<T, X, Y>::identity());
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}

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
			using hash_type			= typename hash_t;
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
			constexpr auto hash()	const	-> hash_type		{ return _ML_META hash()(size(), data()); }
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

			static constexpr self_type identity()
			{
				self_type temp { value_t<value_type>::zero };
				for (size_t i = 0; i < temp.size(); i++)
				{
					temp[i] = (((i / temp.cols()) == (i % temp.cols()))
						? value_t<T>::one
						: value_t<T>::zero);
				}
				return temp;
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T, size_t N>
		using tmatrix_n = matrix_t<T, N, N>;

		template <class T>
		using tmatrix_3 = tmatrix_n<T, 3>;
		using matrix_3b = tmatrix_3<bool>;
		using matrix_3f = tmatrix_3<float>;

		template <class T>
		using tmatrix_4 = tmatrix_n<T, 4>;
		using matrix_4b = tmatrix_4<bool>;
		using matrix_4f = tmatrix_4<float>;

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T, size_t N>
		using tvector_n = matrix_t<T, N, 1>;

		template <class T>
		using tvector_2 = tvector_n<T, 2>;
		using vector_2b = tvector_2<bool>;
		using vector_2f = tvector_2<float>;
		using vector_2i = tvector_2<int32_t>;
		using vector_2d = tvector_2<double>;
		using vector_2u = tvector_2<uint32_t>;

		template <class T>
		using tvector_3 = tvector_n<T, 3>;
		using vector_3b = tvector_3<bool>;
		using vector_3f = tvector_3<float>;
		using vector_3i = tvector_3<int32_t>;
		using vector_3d = tvector_3<double>;
		using vector_3u = tvector_3<uint32_t>;

		template <class T>
		using tvector_4 = tvector_n<T, 4>;
		using vector_4b = tvector_4<bool>;
		using vector_4f = tvector_4<float>;
		using vector_4i = tvector_4<int32_t>;
		using vector_4d = tvector_4<double>;
		using vector_4u = tvector_4<uint32_t>;

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
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr bool operator==(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			return alg::equal_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
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
			return alg::less_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
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
		> constexpr auto & operator+=(A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] + rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto & operator-=(A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] - rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto & operator*=(A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			for (size_t i = 0; i < lhs.size(); i++)
			{
				lhs[i] = (lhs[i] * rhs[i]);
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto & operator/=(A<T, N...> & lhs, const A<T, N...> & rhs)
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
		{
			A<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator-(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			A<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator*(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			A<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator/(const A<T, N...> & lhs, const A<T, N...> & rhs)
		{
			A<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto & operator+=(A<T, N...> & lhs, const T & rhs)
		{
			for (auto & elem : lhs)
			{
				elem += rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto & operator-=(A<T, N...> & lhs, const T & rhs)
		{
			for (auto & elem : lhs)
			{
				elem -= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto & operator*=(A<T, N...> & lhs, const T & rhs)
		{
			for (auto & elem : lhs)
			{
				elem *= rhs;
			}
			return lhs;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto & operator/=(A<T, N...> & lhs, const T & rhs)
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
		{
			A<T, N...> temp { lhs };
			temp += rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator-(const A<T, N...> & lhs, const T & rhs)
		{
			A<T, N...> temp { lhs };
			temp -= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator*(const A<T, N...> & lhs, const T & rhs)
		{
			A<T, N...> temp { lhs };
			temp *= rhs;
			return temp;
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator/(const A<T, N...> & lhs, const T & rhs)
		{
			A<T, N...> temp { lhs };
			temp /= rhs;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator-(const A<T, N...> & lhs)
		{
			return (lhs * value_t<T>::minus_one);
		}

		template <
			template <class, size_t ...> class A, class T, size_t ... N
		> constexpr auto operator+(const A<T, N...> & lhs)
		{
			return -(-(lhs));
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ARRAY_HPP_