#ifndef _ML_ALGORITHM_HPP_
#define _ML_ALGORITHM_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace algorithm
	{
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
			return algorithm::min(algorithm::max(value, minVal), maxVal);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace algorithm
	{
		template<typename Left, typename Right>
		constexpr bool equal_range(Left lBegin, Left lEnd, Right rBegin, Right rEnd)
		{
			return ((lBegin != lEnd && rBegin != rEnd)
				? ((*lBegin) == (*rBegin))
					&& equal_range((lBegin + 1), lEnd, (rBegin + 1), rEnd)
				: ((lBegin == lEnd) && (rBegin == rEnd))
			);
		}

		template<typename Left, typename Right>
		constexpr bool less_range(Left lBegin, Left lEnd, Right rBegin, Right rEnd)
		{
			return ((lBegin != lEnd && rBegin != rEnd)
				? ((*lBegin) < (*rBegin))
					&& less_range((lBegin + 1), lEnd, (rBegin + 1), rEnd)
				: ((lBegin == lEnd) && (rBegin == rEnd))
			);
		}


	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ALGORITHM_HPP_