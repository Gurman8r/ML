#ifndef _ML_TYPE_TAG_HPP_
#define _ML_TYPE_TAG_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		enum { uninit };
	}

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct type_t final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type	= typename T;
		using self_type		= typename type_t<value_type>;
		using limits_type	= typename std::numeric_limits<value_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct cast_v final
		{
			template <
				class U
			> constexpr value_type operator()(const U & value) const
			{
				return static_cast<value_type>(value);
			}
		};

		struct epsilon_v final
		{
			constexpr value_type operator()(value_type seed) const
			{
				value_type temp { zero };
				while ((one + seed) != one)
				{
					temp = seed;
					seed /= two;
				}
				return temp;
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr value_type	minus_one		{ cast_v()( -1) };
		static constexpr value_type	zero			{ cast_v()(  0) };
		static constexpr value_type	one				{ cast_v()(  1) };
		static constexpr value_type	two				{ cast_v()(  2) };
		static constexpr value_type	three			{ cast_v()(  3) };
		static constexpr value_type	four			{ cast_v()(  4) };
		static constexpr value_type	five			{ cast_v()(  5) };
		static constexpr value_type	six				{ cast_v()(  6) };
		static constexpr value_type	seven			{ cast_v()(  7) };
		static constexpr value_type	eight			{ cast_v()(  8) };
		static constexpr value_type	nine			{ cast_v()(  9) };
		static constexpr value_type	ten				{ cast_v()( 10) };
		static constexpr value_type	fourty_five		{ cast_v()( 45) };
		static constexpr value_type	sixty			{ cast_v()( 60) };
		static constexpr value_type	ninety			{ cast_v()( 90) };
		static constexpr value_type	one_hundred		{ cast_v()(100) };
		static constexpr value_type	one_eighty		{ cast_v()(180) };
		static constexpr value_type	three_sixty		{ cast_v()(360) };

		static constexpr value_type	half			{ one / two };
		static constexpr value_type	third			{ one / three };
		static constexpr value_type	quarter			{ one / four };
		static constexpr value_type	fifth			{ one / five };
		static constexpr value_type	sixth			{ one / six };
		static constexpr value_type	seventh			{ one / seven };
		static constexpr value_type	eighth			{ one / eight };
		static constexpr value_type	ninth			{ one / nine };
		static constexpr value_type	tenth			{ one / ten };
		static constexpr value_type	two_thirds		{ two / three };
		static constexpr value_type	three_quarters	{ three / four };

		static constexpr value_type	infinity		{ limits_type::infinity() };
		static constexpr value_type	nan				{ limits_type::quiet_NaN() };
		static constexpr value_type	min				{ limits_type::min() };
		static constexpr value_type	max				{ limits_type::max() };
		static constexpr value_type	epsilon			{ epsilon_v()(half) };
		static constexpr value_type	half_epsilon	{ epsilon * half };

		static constexpr value_type	pi				{ cast_v()(3.14159265358979323846264338327L) };
		static constexpr value_type	two_pi			{ pi * two };
		static constexpr value_type	half_pi			{ pi * half };
		static constexpr value_type	quarter_pi		{ pi * quarter };
		static constexpr value_type	third_pi		{ pi * third };
		static constexpr value_type	deg2rad			{ pi / one_eighty };
		static constexpr value_type	rad2deg			{ one_eighty / pi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class U
		> constexpr explicit type_t(const U & value)
			: m_value { cast_v()(value) }
		{
		}
			
		constexpr value_type operator()() const
		{
			return m_value;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: const value_type m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> constexpr bool operator==(const type_t<T> & lhs, const type_t<T> & rhs)
	{
		return (lhs() == rhs());
	}

	template <
		class T
	> constexpr bool operator!=(const type_t<T> & lhs, const type_t<T> & rhs)
	{
		return !(lhs == rhs);
	}

	template <
		class T
	> constexpr bool operator<(const type_t<T> & lhs, const type_t<T> & rhs)
	{
		return (lhs() < rhs());
	}

	template <
		class T
	> constexpr bool operator>(const type_t<T> & lhs, const type_t<T> & rhs)
	{
		return !(lhs < rhs);
	}

	template <
		class T
	> constexpr bool operator<=(const type_t<T> & lhs, const type_t<T> & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <
		class T
	> constexpr bool operator>=(const type_t<T> & lhs, const type_t<T> & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_TAG_HPP_