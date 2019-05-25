#ifndef _ML_STATIC_VALUE_HPP_
#define _ML_STATIC_VALUE_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/static_value.hpp

#define ML_STATIC_VALUE(x) ::ml::static_value<decltype(x), (x)>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template<typename T, T Value>
	struct static_value
	{
		/* * * * * * * * * * * * * * * * * * * * */

		constexpr static_value() = default;
		
		using value_type = T;
		
		static constexpr value_type value = Value;

		/* * * * * * * * * * * * * * * * * * * * */

		constexpr operator value_type() const
		{
			return Value;
		}

		constexpr value_type get() const
		{
			return Value;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		friend constexpr bool operator==(const static_value & lhs, const value_type rhs)
		{
			return lhs.get() == rhs;
		}

		friend constexpr bool operator==(const value_type lhs, const static_value & rhs)
		{
			return rhs == lhs;
		}

		friend constexpr bool operator!=(const static_value & lhs, const value_type rhs)
		{
			return !(lhs == rhs);
		}

		friend constexpr bool operator!=(const value_type lhs, const static_value & rhs)
		{
			return !(lhs == rhs);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template<typename T, T Value>
	constexpr T static_value<T, Value>::value;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STATIC_VALUE_HPP_