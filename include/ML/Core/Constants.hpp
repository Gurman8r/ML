#ifndef _ML_CONSTANTS_HPP_
#define _ML_CONSTANTS_HPP_

#include <ML/Core/Decay.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct constant_t final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type				= typename detail::decay_t<T>;
		using limits			= typename std::numeric_limits<type>;
		using pointer			= typename type *;
		using reference			= typename type &;
		using const_pointer		= typename const type *;
		using const_reference	= typename const type &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constant_t() = delete;

		template <
			class U
		> constexpr explicit constant_t(const U & value) noexcept
			: m_value { cast(value) }
		{
		}

		constexpr operator const_reference() const noexcept
		{
			return m_value;
		}

		template <
			class U
		> static constexpr type cast(const U & value) noexcept
		{
			return static_cast<type>(value);
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr type minus_one		{ cast( -1) };
		static constexpr type zero			{ cast(  0) };
		static constexpr type one			{ cast(  1) };
		static constexpr type two			{ cast(  2) };
		static constexpr type three			{ cast(  3) };
		static constexpr type four			{ cast(  4) };
		static constexpr type five			{ cast(  5) };
		static constexpr type six			{ cast(  6) };
		static constexpr type seven			{ cast(  7) };
		static constexpr type eight			{ cast(  8) };
		static constexpr type nine			{ cast(  9) };
		static constexpr type ten			{ cast( 10) };
		static constexpr type fourty_five	{ cast( 45) };
		static constexpr type sixty			{ cast( 60) };
		static constexpr type ninety		{ cast( 90) };
		static constexpr type one_hundred	{ cast(100) };
		static constexpr type one_eighty	{ cast(180) };
		static constexpr type three_sixty	{ cast(360) };

		static constexpr type half			{ one / two };
		static constexpr type third			{ one / three };
		static constexpr type quarter		{ one / four };
		static constexpr type fifth			{ one / five };
		static constexpr type sixth			{ one / six };
		static constexpr type seventh		{ one / seven };
		static constexpr type eighth		{ one / eight };
		static constexpr type ninth			{ one / nine };
		static constexpr type tenth			{ one / ten };
		static constexpr type two_thirds	{ two / three };
		static constexpr type three_fourths	{ three / four };

		static constexpr type infinity		{ limits::infinity() };
		static constexpr type nan			{ limits::quiet_NaN() };
		static constexpr type min			{ limits::min() };
		static constexpr type max			{ limits::max() };
		static constexpr type epsilon		{ limits::epsilon() };
		static constexpr type half_epsilon	{ epsilon * half };

		static constexpr type pi			{ cast(3.14159265358979323846264338327L) };
		static constexpr type two_pi		{ pi * two };
		static constexpr type half_pi		{ pi * half };
		static constexpr type quarter_pi	{ pi * quarter };
		static constexpr type third_pi		{ pi * third };
		static constexpr type deg2rad		{ pi / one_eighty };
		static constexpr type rad2deg		{ one_eighty / pi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: type m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> constexpr bool operator==(const constant_t<T> & lhs, const constant_t<T> & rhs)
	{
		return ((T)lhs == (T)rhs);
	}

	template <
		class T
	> constexpr bool operator!=(const constant_t<T> & lhs, const constant_t<T> & rhs)
	{
		return !(lhs == rhs);
	}

	template <
		class T
	> constexpr bool operator<(const constant_t<T> & lhs, const constant_t<T> & rhs)
	{
		return ((T)lhs < (T)rhs);
	}

	template <
		class T
	> constexpr bool operator>(const constant_t<T> & lhs, const constant_t<T> & rhs)
	{
		return !(lhs < rhs);
	}

	template <
		class T
	> constexpr bool operator<=(const constant_t<T> & lhs, const constant_t<T> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class T
	> constexpr bool operator>=(const constant_t<T> & lhs, const constant_t<T> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONSTANTS_HPP_