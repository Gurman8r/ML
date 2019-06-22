#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct hash final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t fnv1a_basis { 14695981039346656037ULL };
		static constexpr size_t fnv1a_prime { 1099511628211ULL };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> constexpr size_t operator()(size_t size, const T * arr, size_t seed)
		{
			return ((size > 0)
				? (*this)(
					(size - 1), (arr + 1), (seed ^ static_cast<size_t>(*arr)) * fnv1a_prime)
				: seed
			);
		}

		template <
			class T
		> constexpr size_t operator()(size_t size, const T * arr)
		{
			return (*this)(size, arr, fnv1a_basis);
		}

		template <
			class T, size_t N
		> constexpr size_t operator()(const T(&value)[N])
		{
			return (*this)(N - 1, value);
		}

		template <
			class T
		> inline size_t operator()(const T & value)
		{
			return std::hash<T>()(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_
