#ifndef _ML_ALGORITHM_HPP_
#define _ML_ALGORITHM_HPP_

namespace ml
{
	namespace detail
	{
		template<typename LhsIt, typename RhsIt>
		constexpr bool equal_range(LhsIt lb, LhsIt le, RhsIt rb, RhsIt re)
		{
			return ((lb != le && rb != re)
				? *lb == *rb && equal_range(lb + 1, le, rb + 1, re)
				: (lb == le && rb == re)
			);
		}
	}
}

#endif // !_ML_ALGORITHM_HPP_