#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

// Sources:
// https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/hash.hpp
// http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace meta
	{
		struct hash
		{
			struct fnv1a
			{
				static constexpr hash_t basis { 14695981039346656037ULL };
				static constexpr hash_t prime { 1099511628211ULL };
			};

			template <class T>
			constexpr hash_t operator()(size_t size, const T * arr, hash_t seed)
			{
				return ((size > 0)
					? operator()(size - 1, arr + 1, (seed ^ *arr) * fnv1a::prime)
					: seed);
			}

			template <class T>
			constexpr hash_t operator()(size_t size, const T * arr)
			{
				return operator()(size, arr, fnv1a::basis);
			}

			template <class T, size_t N>
			constexpr hash_t operator()(const T(&value)[N])
			{
				return operator()(value);
			}
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_