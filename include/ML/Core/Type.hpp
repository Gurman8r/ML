#ifndef _ML_TYPE_HPP_
#define _ML_TYPE_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	enum : size_t 
	{ 
		uninit // Used to initialize empty arrays.
	};

	/* * * * * * * * * * * * * * * * * * * * */

	// Basically a wrapper around static_cast<> with some built in constants.
	template <
		class T
	> struct type_t final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type	= typename T;
		using self_type		= typename type_t<value_type>;
		using limits_type	= typename std::numeric_limits<value_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// Shorthand for static_cast<>
		struct cast_t final
		{
			template <
				class U
			> constexpr value_type operator()(const U & value) const
			{
				return static_cast<value_type>(value);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// Used to calculate (T) epsilon
		struct epsilon_t final
		{
			constexpr value_type operator()(value_type seed) const
			{
				value_type temp { zero };
				while ((one + seed) != one)
				{
					temp = seed;
					seed = seed / two;
				}
				return temp;
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto minus_one			{ cast_t()( -1) };
		static constexpr auto zero				{ cast_t()(  0) };
		static constexpr auto one				{ cast_t()(  1) };
		static constexpr auto two				{ cast_t()(  2) };
		static constexpr auto three				{ cast_t()(  3) };
		static constexpr auto four				{ cast_t()(  4) };
		static constexpr auto five				{ cast_t()(  5) };
		static constexpr auto six				{ cast_t()(  6) };
		static constexpr auto seven				{ cast_t()(  7) };
		static constexpr auto eight				{ cast_t()(  8) };
		static constexpr auto nine				{ cast_t()(  9) };
		static constexpr auto ten				{ cast_t()( 10) };
		static constexpr auto fourty_five		{ cast_t()( 45) };
		static constexpr auto sixty				{ cast_t()( 60) };
		static constexpr auto ninety			{ cast_t()( 90) };
		static constexpr auto one_hundred		{ cast_t()(100) };
		static constexpr auto one_eighty		{ cast_t()(180) };
		static constexpr auto three_sixty		{ cast_t()(360) };

		static constexpr auto half				{ one / two };
		static constexpr auto third				{ one / three };
		static constexpr auto quarter			{ one / four };
		static constexpr auto fifth				{ one / five };
		static constexpr auto sixth				{ one / six };
		static constexpr auto seventh			{ one / seven };
		static constexpr auto eighth			{ one / eight };
		static constexpr auto ninth				{ one / nine };
		static constexpr auto tenth				{ one / ten };
		static constexpr auto two_thirds		{ two / three };
		static constexpr auto three_quarters	{ three / four };

		static constexpr auto infinity			{ limits_type::infinity() };
		static constexpr auto nan				{ limits_type::quiet_NaN() };
		static constexpr auto min				{ limits_type::min() };
		static constexpr auto max				{ limits_type::max() };
		static constexpr auto epsilon			{ epsilon_t()(half) };
		static constexpr auto half_epsilon		{ epsilon * half };

		static constexpr auto pi				{ cast_t()(3.14159265358979323846264338327L) };
		static constexpr auto two_pi			{ pi * two };
		static constexpr auto half_pi			{ pi * half };
		static constexpr auto quarter_pi		{ pi * quarter };
		static constexpr auto third_pi			{ pi * third };
		static constexpr auto deg2rad			{ pi / one_eighty };
		static constexpr auto rad2deg			{ one_eighty / pi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class U
		> constexpr explicit type_t(const U & value)
			: m_value { cast_t()(value) }
		{
		}

		constexpr value_type operator()() const
		{
			return m_value;
		}

		constexpr operator value_type() const
		{
			return (*this)();
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

#endif // !_ML_TYPE_HPP_