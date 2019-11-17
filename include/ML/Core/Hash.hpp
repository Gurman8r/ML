#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// fnv1a hash implementation
	struct Hash final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto basis { static_cast<hash_t>(14695981039346656037ULL) };

		static constexpr auto prime { static_cast<hash_t>(1099511628211ULL) };

		constexpr Hash() noexcept = default;

		template <
			class T
		> constexpr hash_t operator()(const T * arr, hash_t size, hash_t seed) noexcept
		{
			return ((size > 0)
				? (*this)((arr + 1), (size - 1), (seed ^ static_cast<hash_t>(*arr)) * prime)
				: seed
			);
		}

		template <
			class T
		> constexpr hash_t operator()(const T * arr, hash_t size) noexcept
		{
			return (*this)(arr, size, basis);
		}

		template <
			class T, hash_t N
		> constexpr hash_t operator()(const T(&value)[N]) noexcept
		{
			return (*this)(value, (N - 1));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_
