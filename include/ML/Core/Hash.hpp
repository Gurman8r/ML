#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/hash.hpp
// http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
// https://github.com/foonathan/string_id

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		constexpr hash_t fnv_basis = 14695981039346656037ULL;
		constexpr hash_t fnv_prime = 1099511628211ULL;

		// FNV-1a 64 bit hash
		constexpr hash_t fnv1a_hash(size_t n, const char *str, hash_t hash = fnv_basis)
		{
			return n > 0 ? fnv1a_hash(n - 1, str + 1, (hash ^ *str) * fnv_prime) : hash;
		}

		template<size_t N>
		constexpr hash_t fnv1a_hash(const char(&array)[N])
		{
			return fnv1a_hash(N - 1, &array[0]);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_