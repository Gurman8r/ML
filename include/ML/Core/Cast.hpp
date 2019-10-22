#ifndef _ML_CAST_HPP_
#define _ML_CAST_HPP_

#include <ML/Core/Decay.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct cast_t final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type				= typename detail::decay_t<T>;
		using limits			= typename std::numeric_limits<type>;
		using pointer			= typename type *;
		using reference			= typename type &;
		using const_pointer		= typename const type *;
		using const_reference	= typename const type &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr cast_t() = default;

		template <class T> constexpr auto operator()(T && value) const
		{
			return static_cast<type>(std::forward<T>(value));
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		static constexpr type fourty_five	{ static_cast<type>( 45) };
		static constexpr type sixty			{ static_cast<type>( 60) };
		static constexpr type ninety		{ static_cast<type>( 90) };
		static constexpr type one_hundred	{ static_cast<type>(100) };
		static constexpr type one_eighty	{ static_cast<type>(180) };
		static constexpr type three_sixty	{ static_cast<type>(360) };

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

		static constexpr type pi			{ static_cast<type>(3.14159265358979323846264338327L) };
		static constexpr type two_pi		{ pi * two };
		static constexpr type half_pi		{ pi * half };
		static constexpr type quarter_pi	{ pi * quarter };
		static constexpr type third_pi		{ pi * third };
		static constexpr type deg2rad		{ pi / one_eighty };
		static constexpr type rad2deg		{ one_eighty / pi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONSTANTS_HPP_