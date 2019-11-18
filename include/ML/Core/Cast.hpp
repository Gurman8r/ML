#ifndef _ML_CAST_HPP_
#define _ML_CAST_HPP_

#include <ML/Core/Decay.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct cast;

	template <> struct cast<> { cast() = delete; };

	template <class T> struct cast<T> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type = typename detail::decay_t<T>;

		using self_type = typename cast<type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr cast() noexcept = default;

		template <class T> constexpr auto operator()(T value) const noexcept
		{
			return static_cast<type>(value);
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONSTANTS_HPP_