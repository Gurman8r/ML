#ifndef _ML_CAST_HPP_
#define _ML_CAST_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// Used to store a static_cast within a reusable type.
		template <
			class T
		> struct cast_static
		{
			constexpr cast_static() = delete;

			template <
				class U
			> constexpr explicit cast_static(const U & value) noexcept
				: m_value { static_cast<T>(value) }
			{
			}

			constexpr operator T() const noexcept
			{ 
				return m_value;
			}

		private: const T m_value;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */

	// Combines impl::cast_static<T> with default numerical constants.
	template <
		class T
	> struct static_value final : public impl::cast_static<T>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using cast		= typename impl::cast_static<T>;
		using limits	= typename std::numeric_limits<T>;
		using type		= typename std::decay<T>::type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class U
		> constexpr explicit static_value(const U & value) noexcept
			: cast { value }
		{
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

		static constexpr type half			{ one	/ two };
		static constexpr type third			{ one	/ three };
		static constexpr type quarter		{ one	/ four };
		static constexpr type fifth			{ one	/ five };
		static constexpr type sixth			{ one	/ six };
		static constexpr type seventh		{ one	/ seven };
		static constexpr type eighth		{ one	/ eight };
		static constexpr type ninth			{ one	/ nine };
		static constexpr type tenth			{ one	/ ten };
		static constexpr type two_thirds	{ two	/ three };
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
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> constexpr bool operator==(const static_value<T> & lhs, const static_value<T> & rhs)
	{
		return ((T)lhs == (T)rhs);
	}

	template <
		class T
	> constexpr bool operator!=(const static_value<T> & lhs, const static_value<T> & rhs)
	{
		return !(lhs == rhs);
	}

	template <
		class T
	> constexpr bool operator<(const static_value<T> & lhs, const static_value<T> & rhs)
	{
		return ((T)lhs < (T)rhs);
	}

	template <
		class T
	> constexpr bool operator>(const static_value<T> & lhs, const static_value<T> & rhs)
	{
		return !(lhs < rhs);
	}

	template <
		class T
	> constexpr bool operator<=(const static_value<T> & lhs, const static_value<T> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class T
	> constexpr bool operator>=(const static_value<T> & lhs, const static_value<T> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAST_HPP_