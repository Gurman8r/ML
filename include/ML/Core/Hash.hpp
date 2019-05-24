#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/seed.hpp
// http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace hash
	{
		namespace fnv
		{
			static constexpr hash_t basis = 14695981039346656037ULL;
			static constexpr hash_t prime = 1099511628211ULL;

			constexpr hash_t do_hash(size_t n, const char * str, hash_t seed = basis)
			{
				return ((n > 0) ? do_hash(n - 1, str + 1, (seed ^ *str) * prime) : (seed));
			}

			template<size_t N>
			constexpr hash_t do_hash(const char(&array)[N])
			{
				return do_hash(N - 1, &array[0]);
			}
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_