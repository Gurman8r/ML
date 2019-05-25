#ifndef _ML_ALGORITHM_HPP_
#define _ML_ALGORITHM_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/algorithm.hpp

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template<typename Left, typename Right>
		constexpr bool equal_range(Left lBegin, Left lEnd, Right rBegin, Right rEnd)
		{
			return ((lBegin != lEnd && rBegin != rEnd)
				? ((*lBegin) == (*rBegin)) && equal_range(lBegin + 1, lEnd, rBegin + 1, rEnd)
				: ((lBegin == lEnd) && (rBegin == rEnd))
			);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <typename T>
		constexpr const T & max(const T & lhs, const T & rhs)
		{
			return (lhs >= rhs) ? lhs : rhs;
		}

		template <typename T>
		constexpr const T & min(const T & lhs, const T & rhs)
		{
			return (lhs <= rhs) ? lhs : rhs;
		}

		template <typename T>
		constexpr const T & clamp(const T & value, const T & minVal, const T & maxVal)
		{
			return min(max(value, minVal), maxVal);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ALGORITHM_HPP_