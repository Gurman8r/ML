#ifndef _ML_DETAIL_HPP_
#define _ML_DETAIL_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> static constexpr int32_t index_of(const T value, const T(&arr)[N])
		{
			for (int32_t i = 0; (&arr[i]) != (&arr[N]); i++)
			{
				if (arr[i] == value)
				{
					return i;
				}
			}
			return -1;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, size_t N
		> static constexpr bool value_at(const int32_t i, T & value, const T(&arr)[N])
		{
			if (i < (int32_t)(sizeof(arr) / sizeof(*arr)))
			{
				value = arr[i];
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}

#endif // !_ML_DETAIL_HPP_